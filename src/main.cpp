#include <print>

#include <SFML/Graphics.hpp>

#include "TextBuffer.hpp"
#include "DisplayGrid.hpp"

int main()
{

	sf::Vector2i grid_size{ 120, 30 };

	const sf::Font font(ASSET_PATH"SpaceMono-Regular.ttf");

	sf::Vector2f cell_size
	{
		font.getGlyph(U' ', 16, false).advance,
		font.getLineSpacing(16)
	};
	sf::Vector2f terminal_size
	{
		cell_size.x * grid_size.x,
		cell_size.y * grid_size.y,
		
	};

	sf::RenderWindow window{ sf::VideoMode{static_cast<sf::Vector2u>(sf::Vector2f{terminal_size.x, terminal_size.y})}, "Terminal" };


	sf::RectangleShape rectangle{ {cell_size.x,  cell_size.y} };
	rectangle.setFillColor(sf::Color::Blue);

	using norm_pos = sf::Vector2i;

	norm_pos cursor{};
	int graphical_y_cursor{};

	bool control_mode_on{};

	// Represent each end of the render-terminal 
	int begin{};
	int end{ grid_size.y };

	TextBuffer buffer{};
	DisplayGrid display_grid{ &buffer };

	sf::Text debug{ font };
	debug.setPosition({ 800, 200 });

	while (window.isOpen())
	{

		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			if (auto key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->scancode == sf::Keyboard::Scancode::Escape)
					window.close();

				if (key->scancode == sf::Keyboard::Scancode::LControl ||
					key->scancode == sf::Keyboard::Scancode::RControl)
					control_mode_on = true;

				if (control_mode_on)
				{
					if (key->scancode == sf::Keyboard::Scancode::Enter)
					{
						buffer.carriage_return();
					}
				}

				if (key->scancode == sf::Keyboard::Scancode::Delete ||
					key->scancode == sf::Keyboard::Scancode::Backspace
					)
				{
					buffer.erase();
				}

				if (key->scancode == sf::Keyboard::Scancode::Right)
					cursor.x++;

				if (key->scancode == sf::Keyboard::Scancode::Left)
					cursor.x--;

				if (key->scancode == sf::Keyboard::Scancode::Up)
				{
					if (cursor.y > 0)
					{
						cursor.y--;
						graphical_y_cursor--;

						if (graphical_y_cursor < 1)
							graphical_y_cursor = 0;
					}

					if (cursor.y < begin && begin > 0)
					{
						begin--;
					}
				}

				if (key->scancode == sf::Keyboard::Scancode::Down)
				{
					cursor.y++;
					graphical_y_cursor++;
					if (graphical_y_cursor >= grid_size.y)
						graphical_y_cursor = grid_size.y - 1;

					if (cursor.y >= begin + grid_size.y - 1)
						begin++;
					
				}


				if (cursor.x >= grid_size.x)
				{
					cursor.x = 0;
					cursor.y++;
				}
				else if (cursor.x < 0)
				{
					if (cursor.y > 0)
					{
						cursor.x = grid_size.x - 1;
						cursor.y--;
					}
					else
						cursor.x = 0;
				}

			}
			
			if (auto key = event->getIf<sf::Event::KeyReleased>())
			{
				if (key->scancode == sf::Keyboard::Scancode::LControl ||
					key->scancode == sf::Keyboard::Scancode::RControl)
					control_mode_on = false;
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
						buffer.add("    ");
					else
						buffer.add(static_cast<u8>(key->unicode));
				}
			}
		}

		debug.setString(std::format("\n\nC: {}\nGC: {}\nB: {}\nE: {}", cursor.y, graphical_y_cursor, begin, begin + grid_size.y));


		display_grid.update();

		rectangle.setPosition(sf::Vector2f{ cursor.x * cell_size.x, graphical_y_cursor * cell_size.y });


		window.clear();

		window.draw(rectangle);
		window.draw(display_grid);
		window.draw(debug);

		window.display();
	}

	return 0;
}
