#pragma once
/// ==== Allosker 2026-2027 ===================
// A display grid takes in a TextBuffer and sets everything that is needed in order to properly display it on screen
// As such, it depends on an instance of a Terminal Window to properly display the cursor
//
// Note: don't forget to implement syntax highlighting
/// ==== https://github.com/Allosker/cmd ======

#include "TextBuffer.hpp"
#include "sys/utils.hpp"

class DisplayGrid
	: public sf::Drawable
{
public:

	DELETE_COPY_INIT(DisplayGrid);
	DEFAULT_MOVE_INIT(DisplayGrid);

	DisplayGrid(const TextBuffer* text_buffer) noexcept;

	void update() noexcept;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_text_view, states);
	}


/// <summary>
/// Everything in this section is considered to be pixel-wise
/// </summary>
private:

	sf::Text m_text_view;
	v2f32 m_cell_size{};
	v2f32 m_grid_size{};

	const TextBuffer* m_text_buffer{ nullptr };

	u8 m_change{};

};