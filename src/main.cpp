#include <print>

#include <SFML/Graphics.hpp>

#include "TerminalWindow.hpp"

int main()
{
	TerminalWindow window{};

	while (window.is_open())
		window.update();
	

	return 0;
}
