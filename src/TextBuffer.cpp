#include "TextBuffer.hpp"
#include "sys/utils.hpp"
#include <string>



TextBuffer::TextBuffer()
	: m_format{ {} }
{
}

const std::string TextBuffer::get_format() const noexcept
{
	if (!m_format.size()) return "";

	std::string str{};
	str.reserve((m_format.size() - 1) * m_grid_size.x);

	for (const auto& i : m_format)
		str.append(i);

	return str;
}

void TextBuffer::move_cursor(v2i32 offset) noexcept
{
	m_cursor += offset;

	if (offset.x != 0)
	{
		if (m_cursor.y > 0 && m_cursor.x < 0)
		{
			m_cursor.y--;
			m_cursor.x = m_grid_size.x - 1;
		}
		else if (m_cursor.y == 0 && m_cursor.x < 0)
			m_cursor.x = 0;

		if (m_cursor.x >= m_grid_size.x)
		{
			move_cursor({ m_cursor.x - m_grid_size.x, 0 });
			m_cursor.y++;
			m_cursor.x = 0;
		}
	}
	else if (offset.y != 0)
	{
		if (m_cursor.y < 0)
			m_cursor.y = 0;
	}

	m_change++;
}

void TextBuffer::add(i8 c) noexcept
{
	if (!m_format.size())
		m_format.push_back("");
	else if (m_format.back().size() == m_grid_size.x)
	{
		m_format.back() += '\n';
		m_format.push_back("");
	}
	
	m_format.back() += c;
	m_change++;
}

void TextBuffer::carriage_return() noexcept
{
	add('\n');
	m_format.push_back("");
	m_change++;
}

void TextBuffer::erase() noexcept
{
	if (!m_format.size()) return;

	if (!m_format.back().size())
		m_format.pop_back();
	else
		m_format.back().pop_back();

	if (m_format.size() && m_format.back().size() && m_format.back().back() == '\n')
	{
		erase();
		erase();
	}

	m_change++;
}
