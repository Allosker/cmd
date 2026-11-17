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


	// Init message
	m_text_buffer.set_usr_msg(m_commands_executioner.get_cd());
	m_text_buffer.add("New instance of the Terminal window.\nType: help for a list of all available commands");
	m_text_buffer.enter();

	/// Round the edges of the window

	m_window_handle = HWND(m_window.getNativeHandle());

	HRGN region = CreateRoundRectRgn(0, 0, m_display_grid.get_grid_size().x, m_display_grid.get_grid_size().y + g_size_button.y, 15, 15 );

	SetWindowRgn(m_window_handle, region, true);

	m_window.setFramerateLimit(40);
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

			{
				if (m_window.hasFocus())
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
						// Compute user commands
					{
						m_text_buffer.enter();

						if (auto err = m_commands_executioner.parse(m_text_buffer.get_UserInput_buffer()); err != "")
						{
							m_text_buffer.add(err);
							m_text_buffer.enter();
						}
						else
						{
							std::string ret = m_commands_executioner.run_commands(m_window);
							m_text_buffer.set_usr_msg(m_commands_executioner.get_cd());
							m_text_buffer.add(ret);
							m_text_buffer.enter();
						}

						
					}
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

			if (mb->button == sf::Mouse::Button::Left)
			{
				if (m_border.getGlobalBounds().contains(m_window_mouse_pos))
					m_drag_window = true;

				if (m_close_button.getGlobalBounds().contains(m_window_mouse_pos))
				{
					m_window.close();
				}
			}


		}

		if (auto mb = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mb->button == sf::Mouse::Button::Left)
				m_drag_window = false;
		}

		if (m_window.hasFocus())
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

	if (m_window.hasFocus())
	{
		m_window_mouse_pos = static_cast<v2f32>(sf::Mouse::getPosition(m_window));

		if (m_close_button.getGlobalBounds().contains(m_window_mouse_pos))
			m_close_button.setFillColor(sf::Color{ 140, 7, 7, 255 });
		else
			m_close_button.setFillColor(sf::Color::Transparent);

		if (m_drag_window)
		{
			auto mouse_pos = sf::Mouse::getPosition();
			m_window.setPosition(mouse_pos - m_window_offset);
		}
		else
			m_window_offset = sf::Mouse::getPosition() - m_window.getPosition();

		m_display_grid.update();
	}

	draw();
}
