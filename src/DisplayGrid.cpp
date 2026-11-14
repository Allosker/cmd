#include "DisplayGrid.hpp"

#include "AssetsManager.hpp"

DisplayGrid::DisplayGrid(const TextBuffer* text_buffer) noexcept
	: m_text_buffer{ text_buffer }, m_text_view { AssetsManager::get().font }
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
}

void DisplayGrid::update() noexcept
{
	if (m_text_buffer->get_change_count() != m_change)
	{
		m_change = m_text_buffer->get_change_count();

		m_text_view.setString(m_text_buffer->get_format());
	}
}
