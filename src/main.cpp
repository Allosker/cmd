#include <print>

#include <SFML/Graphics.hpp>


std::string get_extra_text(const std::string& text_line, const sf::Vector2i& grid_size)
{
	auto delta{ (int)std::abs((double)std::min(0ll, grid_size.x - (long long int)text_line.size())) };

	if (delta == 0) return "";

	return { text_line.end() - delta, text_line.end() };
}

int main()
{

	sf::Vector2u w_size{ 1100, 620 };
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

	sf::RenderWindow window{ sf::VideoMode{static_cast<sf::Vector2u>(sf::Vector2f{terminal_size.x, terminal_size.y})}, "Test" };

	/// What is displayed in the terminal
	sf::Text view_buffer(font, "");
	view_buffer.setCharacterSize(16);

	/// Each string represents a line of the terminal
	std::vector<std::string> text_buffer{ {} };

	sf::RectangleShape rectangle{ {cell_size.x,  cell_size.y} };
	rectangle.setFillColor(sf::Color::Blue);

	using norm_pos = sf::Vector2i;

	norm_pos cursor{};
	int graphical_y_cursor{};
	size_t buffer_cursor{};

	bool control_mode_on{};

	bool text_entered{};
	bool append_buffer{};

	// Represent each end of the render-terminal 
	int begin{};
	int end{ grid_size.y };

	sf::Text debug{ font };
	debug.setPosition({ 500, 200 });

	while (window.isOpen())
	{
		std::string temp{ text_buffer[buffer_cursor] };

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
						temp += '\n';
						text_entered = true;
						append_buffer = true;
					}
				}

				if (key->scancode == sf::Keyboard::Scancode::Delete ||
					key->scancode == sf::Keyboard::Scancode::Backspace
					)
				{
					if (text_buffer[buffer_cursor].size())
						text_buffer[buffer_cursor].pop_back();
					else
					{
						if (buffer_cursor)
							buffer_cursor--;

						if (text_buffer[buffer_cursor].size())
							text_buffer[buffer_cursor].pop_back();
					}
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
						temp += "    ";
					else
						temp += static_cast<char>(key->unicode);
					text_entered = true;
				}
			}

			
		}

		debug.setString(std::format("\n\nC: {}\nGC: {}\nB: {}\nE: {}", cursor.y, graphical_y_cursor, begin, begin + grid_size.y));

		if (text_entered)
		{
			if (const auto extra = get_extra_text(temp, grid_size); extra != "")
			{
				if (buffer_cursor == text_buffer.size() - 1)
				{
					text_buffer.push_back(extra);
					buffer_cursor++;
				}
				else
					text_buffer[++buffer_cursor] = extra;
			}
			else
				text_buffer[buffer_cursor] = temp;

			if (append_buffer)
			{
				text_buffer.push_back("");
				buffer_cursor++;
				append_buffer = false;
			}

			text_entered = false;
		}


		std::string view_buffer_data{};
		for (size_t i = begin; i < begin + grid_size.y; i++)
		{
			if (i < text_buffer.size())
			{
				view_buffer_data.append(text_buffer[i]);

				if (!text_buffer[i].empty() && text_buffer[i].back() != '\n')
					view_buffer_data.push_back('\n');
			}
		}
		view_buffer.setString(view_buffer_data);

		rectangle.setPosition(sf::Vector2f{ cursor.x * cell_size.x, graphical_y_cursor * cell_size.y });


		window.clear();

		window.draw(rectangle);
		window.draw(view_buffer);
		window.draw(debug);

		window.display();
	}

	return 0;
}
