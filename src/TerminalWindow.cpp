#include "TerminalWindow.hpp"
#include "AssetsManager.hpp"

TerminalWindow::TerminalWindow() noexcept
	: m_display_grid{ &m_text_buffer }, m_window
	{ 
		sf::VideoMode{static_cast<sf::Vector2u>(sf::Vector2f{m_display_grid.get_grid_size().x, m_display_grid.get_grid_size().y + g_size_button.y})}, 
		"Terminal",
		sf::Style::None
	}
{
	m_display_grid.move({ 0, g_size_button.y });

	m_border.setFillColor(sf::Color{ 26, 5, 43, 255 }); 
	m_border.setSize({ m_display_grid.get_grid_size().x, g_size_button.y });
	m_border.setOutlineColor(sf::Color{ 43, 9, 71, 255 });
	m_border.setOutlineThickness(-2.f);

	m_close_button.setSize({ g_size_button.x, g_size_button.y });
	m_close_button.setPosition({ m_display_grid.get_grid_size().x - g_size_button.x, 0 });
	
	m_close_button_tex.setSize({ g_size_button.y / 2.f, g_size_button.y / 2.f });
	m_close_button_tex.setPosition({
		m_close_button.getPosition().x + m_close_button.getSize().x / 2.f - m_close_button_tex.getSize().x / 2.f,
		m_close_button.getPosition().y + m_close_button.getSize().y / 2.f - m_close_button_tex.getSize().y / 2.f });
	m_close_button_tex.setTexture(&AssetsManager::get().close_cross);

}

void TerminalWindow::manage_events() noexcept
{
	while (const auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();

		if (auto key = event->getIf<sf::Event::KeyPressed>())
		{
			if (key->scancode == sf::Keyboard::Scancode::Escape)
				m_window.close();

			if (key->scancode == sf::Keyboard::Scancode::LControl ||
				key->scancode == sf::Keyboard::Scancode::RControl)
				m_kb_mode |= KeyboardMode::Control;

			if (key->scancode == sf::Keyboard::Scancode::LShift ||
				key->scancode == sf::Keyboard::Scancode::RShift)
				m_kb_mode |= KeyboardMode::Shift;


			if (key->scancode == sf::Keyboard::Scancode::Enter)
			{
				if (m_kb_mode & KeyboardMode::Control)
					m_text_buffer.carriage_return();
				else
					m_text_buffer.enter();
			}

			if (key->scancode == sf::Keyboard::Scancode::Delete ||
				key->scancode == sf::Keyboard::Scancode::Backspace
				)
			{
				m_text_buffer.erase();
			}


			if (key->scancode == sf::Keyboard::Scancode::Right)
				m_text_buffer.move_cursor({ 1, 0 });

			if (key->scancode == sf::Keyboard::Scancode::Left)
				m_text_buffer.move_cursor({ -1, 0 });

			if (key->scancode == sf::Keyboard::Scancode::Up)
				m_text_buffer.move_cursor({ 0, -1 });

			if (key->scancode == sf::Keyboard::Scancode::Down)
				m_text_buffer.move_cursor({ 0, 1 });

		}

		if (auto key = event->getIf<sf::Event::KeyReleased>())
		{
			if (key->scancode == sf::Keyboard::Scancode::LControl ||
				key->scancode == sf::Keyboard::Scancode::RControl)
				m_kb_mode &= ~KeyboardMode::Control;

			if (key->scancode == sf::Keyboard::Scancode::LShift ||
				key->scancode == sf::Keyboard::Scancode::RShift)
				m_kb_mode &= ~KeyboardMode::Shift;
		}

		if (auto mb = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (m_close_button.getGlobalBounds().contains(m_window_mouse_pos))
			{
				m_window.close();
			}
		}


		if (auto key = event->getIf<sf::Event::TextEntered>())
		{
			if ((key->unicode > 0x001F /*escape characters sequence*/ || (key->unicode < 0x007E && key->unicode > 0x00A0)) ||
				// exceptions  
				key->unicode == 0x0009 // horizontal tab
				)
				// printable characters range
			{
				if (key->unicode == 0x0009)
					m_text_buffer.add("    ");
				else
					m_text_buffer.add(static_cast<u8>(key->unicode));
			}
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{

	}
	else
		m_original_window_pos = m_window.getPosition();

}

void TerminalWindow::draw() noexcept
{
	m_window.clear();

	m_window.draw(m_border);
	m_window.draw(m_display_grid);
	m_window.draw(m_close_button);
	m_window.draw(m_close_button_tex);

	m_window.display();
}

void TerminalWindow::update() noexcept
{
	manage_events();

	m_window_mouse_pos = static_cast<v2f32>(sf::Mouse::getPosition(m_window));

	if (m_close_button.getGlobalBounds().contains(m_window_mouse_pos))
		m_close_button.setFillColor(sf::Color{ 140, 7, 7, 255 });
	else
		m_close_button.setFillColor(sf::Color::Transparent);


	m_window.setPosition(m_window.getPosition() + m_move_offset);

	m_display_grid.update();

	draw();
}
