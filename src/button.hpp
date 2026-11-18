#pragma once
/// ==== Allosker 2026-2027 ===================
// Implementation of a Graphical Button
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"

class Button
	: public sf::RectangleShape
{
public:

	bool intersects(v2f32 pos) noexcept
	{
		return getGlobalBounds().contains(pos);
	}

	bool is_pressed() const noexcept { return m_pressed; }

	void set_pressed(bool pressed) noexcept
	{
		m_pressed = pressed;
	}


private:

	bool m_pressed{};

};