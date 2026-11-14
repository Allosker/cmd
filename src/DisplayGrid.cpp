#include "DisplayGrid.hpp"

#include "AssetsManager.hpp"

DisplayGrid::DisplayGrid(const TextBuffer* text_buffer) noexcept
	: m_text_buffer { text_buffer }, m_text_view{ AssetsManager::get().font }
{
	m_text_view.setCharacterSize(16);

	m_cell_size = v2f32{
		m_text_view.getFont().getGlyph(U' ', 16, false).advance,
		m_text_view.getFont().getLineSpacing(16)
	};

	m_grid_size = {
		m_cell_size.x * text_buffer->get_grid_size().x,
		m_cell_size.y * text_buffer->get_grid_size().y,
	};

	m_render_tex.resize(static_cast<v2u32>(m_grid_size));

	m_cursor.setSize(m_cell_size);
	m_view.setSize(m_grid_size);
	m_view.setCenter(m_view.getSize() / 2.f);

	m_render_tex.setView(m_view);
}

void DisplayGrid::update() noexcept
{
	if (m_text_buffer->get_change_count() != m_change)
	{
		m_change = m_text_buffer->get_change_count();

		m_text_view.setString(m_text_buffer->get_format());

		m_cursor.setPosition({ m_text_buffer->get_cursor().x * m_cell_size.x, m_text_buffer->get_cursor().y * m_cell_size.y });


		f32 top = m_view.getCenter().y - m_view.getSize().y / 2.f;
		f32 bottom = m_view.getCenter().y + m_view.getSize().y / 2.f;

		if (m_cursor.getPosition().y < top)
			m_view.move({ 0, m_cursor.getPosition().y - top });
		else if (m_cursor.getPosition().y > bottom)
			m_view.move({ 0, m_cursor.getPosition().y - bottom });
		else if (m_cursor.getPosition().y == bottom)
			m_view.move({ 0, m_cell_size.y });

	}
}
