#include "TextBuffer.hpp"
#include "sys/utils.hpp"
#include <string>



TextBuffer::TextBuffer()
	: m_commands{ {} }, m_format{ &m_commands.back() }
{
}

const std::string TextBuffer::get_terminal_buffer() const noexcept
{
	std::string ret{};

	size_t total_size{};
	size_t y_pos{};
	for (const auto& i : m_commands)
	{
		if (!i.size()) continue;

		std::string str{};
		str.reserve((i.size() - 1) * m_grid_size.x);

		for (const auto& j : i)
		{
			size_t line_x_count{};
			for (const auto& k : j)
			{
				total_size++;
				if (k == '\n')
				{
					y_pos++;
					total_size = 0;
				}
				str.push_back(k);
			}
		}

		
		ret.append(str);
	}

	if (m_writing)
		m_cursor = { total_size == 0 ? 0 : (i32)(total_size) - 1 /*To start at 0*/, (i32)y_pos};
	return ret;
}

void TextBuffer::move_cursor(v2i32 offset) noexcept
{
	m_writing = false;

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

void TextBuffer::cursor_to_writing_position() noexcept
{
}

void TextBuffer::enter() noexcept
{
	if (!m_entry_mode)
	{
		add("\n{}", m_user_msg);
		m_buffer = ""; // clear user input buffer
	}
	else
		add("\n");

	m_entry_mode = !m_entry_mode;

	m_commands.push_back({});
	m_format = &m_commands.back();
}

void TextBuffer::add(i8 c) noexcept
{
	if (!m_format->size())
		m_format->push_back("");
	else if (m_format->back().size() == m_grid_size.x)
	{
		m_format->back() += '\n';

		move_cursor({ 0, 1 });
		m_cursor.x = 0;

		m_format->push_back("");
	}
	
	m_format->back() += c;

	if (m_entry_mode)
		m_buffer += c;

	m_writing = true;
	m_change++;
}

void TextBuffer::add(const std::string& str) noexcept
{
	for (const auto i : str)
	{
		add(i);
		if (m_entry_mode)
			m_buffer += i;
	}
	m_change++;
}

void TextBuffer::carriage_return() noexcept
{
	add('\n');
	m_format->push_back("");

	m_cursor.x = 0;
	move_cursor({ 0, 1 });

	m_change++;
}

void TextBuffer::erase() noexcept
{
	if (!m_format->size()) return;

	if (!m_format->back().size())
		m_format->pop_back();
	else
		m_format->back().pop_back();

	if (m_format->size() && m_format->back().size() && m_format->back().back() == '\n')
	{
		if (!m_format->back().size())
			m_format->pop_back();
		else
			m_format->back().pop_back();

		if (!m_format->back().size())
			m_format->pop_back();
		else
			m_format->back().pop_back();
		// Have to do that to erase the line and the \n character at the start of the previous one
	}

	if (m_entry_mode)
		if (!m_buffer.empty())
			m_buffer.pop_back();

	m_change++;
}

void TextBuffer::erase(v2i32 pos) noexcept
{

}
