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
