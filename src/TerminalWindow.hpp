#pragma once
/// ==== Allosker 2026-2027 ===================
// Window for displaying a DisplayGrid as well as managing the events affiliating with it
/// ==== https://github.com/Allosker/cmd ======

#include "DisplayGrid.hpp"
#include "TextBuffer.hpp"
#include "sys/utils.hpp"

class TerminalWindow
{
public:

	DELETE_COPY_INIT(TerminalWindow);
	DEFAULT_MOVE_INIT(TerminalWindow);

	TerminalWindow() noexcept;

	bool is_open() noexcept { return m_window.isOpen(); }

	void manage_events() noexcept;

	void draw() noexcept;

	void update() noexcept;


private:

	TextBuffer m_text_buffer{};
	DisplayGrid m_display_grid;
	sf::RenderWindow m_window;


	enum KeyboardMode
		: u8
	{
		Shift = 0x1,
		Control = 0x2
	};

	u8 m_kb_mode{};

};