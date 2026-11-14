#include "TerminalWindow.hpp"


TerminalWindow::TerminalWindow() noexcept
	: m_display_grid{ &m_text_buffer }, m_window{ sf::VideoMode{static_cast<sf::Vector2u>(sf::Vector2f{m_display_grid.get_grid_size().x, m_display_grid.get_grid_size().y})}, "Terminal"}
{
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

			if (m_kb_mode & KeyboardMode::Control)
			{
				if (key->scancode == sf::Keyboard::Scancode::Enter)
				{
					m_text_buffer.carriage_return();
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

		if (auto key = event->getIf<sf::Event::KeyReleased>())
		{
			if (key->scancode == sf::Keyboard::Scancode::LControl ||
				key->scancode == sf::Keyboard::Scancode::RControl)
				m_kb_mode &= ~KeyboardMode::Control;

			if (key->scancode == sf::Keyboard::Scancode::LShift ||
				key->scancode == sf::Keyboard::Scancode::RShift)
				m_kb_mode &= ~KeyboardMode::Shift;
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
}

void TerminalWindow::draw() noexcept
{
	m_window.clear();

	m_window.draw(m_display_grid);

	m_window.display();
}

void TerminalWindow::update() noexcept
{
	manage_events();

	m_display_grid.update();

	draw();
}
