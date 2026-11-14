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

	v2f32 get_grid_size() const noexcept { return m_grid_size; }

	void update() noexcept;

	void render() const noexcept
	{
		m_render_tex.setView(m_view);

		m_render_tex.clear(sf::Color::Blue);

		m_render_tex.draw(m_cursor);
		m_render_tex.draw(m_text_view);

		m_render_tex.display();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		render();

		sf::Sprite sprite{ m_render_tex.getTexture() };
		target.draw(sprite, states);
	}


/// <summary>
/// Everything in this section is considered to be pixel-wise
/// </summary>
private:

	mutable sf::RenderTexture m_render_tex{};
	sf::Text m_text_view;
	sf::RectangleShape m_cursor{}; 
	sf::View m_view;
	
	v2f32 m_cell_size{};
	v2f32 m_grid_size{};

	const TextBuffer* m_text_buffer{ nullptr };

	u8 m_change{};

};