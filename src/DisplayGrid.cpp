#include "DisplayGrid.hpp"

#include "AssetsManager.hpp"

DisplayGrid::DisplayGrid(const TextBuffer* text_buffer) noexcept
	: m_text_buffer { text_buffer }, m_text_view{ AssetsManager::get().font }
{
	m_cell_size = v2f32{
		m_text_view.getFont().getGlyph(U' ', 16, false).advance,
		m_text_view.getFont().getLineSpacing(16)
	};

	m_grid_size = {
		m_cell_size.x * text_buffer->get_grid_size().x,
		m_cell_size.y * text_buffer->get_grid_size().y,
	};

	m_text_view.setCharacterSize(16);
	const f32 g_offset{ 5.f };
	m_text_view.move({ 0.f, g_offset });

	m_cursor.setSize({m_cell_size.x / 10.f, m_cell_size.y});
	m_cursor.setOrigin({ -(m_cell_size.x / 10.f) * 9.f, 0 });

	m_view.setSize(m_grid_size);
	m_view.setCenter(m_view.getSize() / 2.f);

	m_render_tex.resize(static_cast<v2u32>(m_grid_size));
	m_render_tex.setView(m_view);
}

void DisplayGrid::update() noexcept
{
	if (m_text_buffer->get_change_count() != m_change)
	{
		m_change = m_text_buffer->get_change_count();

		m_text_view.setString(m_text_buffer->get_terminal_buffer());

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

void DisplayGrid::render() noexcept
{
	m_render_tex.setView(m_view);

	m_render_tex.clear(sf::Color{ 30,32,35, 255 });


	constexpr float blinking_cycle{ 1.f }; // in seconds

	const auto blinking = std::fmod(m_time.getElapsedTime().asSeconds(), blinking_cycle);
	if (blinking < blinking_cycle / 2.f)
		m_render_tex.draw(m_cursor);

	m_render_tex.draw(m_text_view);

	m_render_tex.display();
}

void DisplayGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite sprite{ m_render_tex.getTexture() };
	sprite.setPosition(m_pos);
	target.draw(sprite, states);
}
