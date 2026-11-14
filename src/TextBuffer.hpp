#pragma once
/// ==== Allosker 2026-2027 ===================
// A text buffer manages a dynamic array of strings (with each representing one line).
// The goal of the class is to accept entry text to format it according to various customisable parameters.
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"
#include <format>
#include <string>
#include <vector>

static inline const std::string get_extra_text(const std::string& text_line, const v2i32& grid_size) noexcept
{
	auto delta{ (int)std::abs((double)std::min(0ll, grid_size.x - (long long int)text_line.size())) };

	if (delta == 0) return "";

	return { text_line.end() - delta, text_line.end() };
}

class TextBuffer
{
public:

	DELETE_COPY_INIT(TextBuffer);
	DEFAULT_MOVE_INIT(TextBuffer);

	TextBuffer();

	const std::string get_format() const noexcept;
	v2i32 get_grid_size() const noexcept { return m_grid_size; }
	size_t size() const noexcept { return m_format.size(); }

	const std::string get_line(size_t i) const { return i >= m_format.size() ? "" : m_format[i]; }

	u8 get_change_count() const noexcept { return m_change; }


	template<typename... Types>
	void add(std::format_string<Types...> fmt, Types&&... args) noexcept;

	void add(i8 c) noexcept;

	void carriage_return() noexcept;

	void erase() noexcept;



private:

	std::vector<std::string> m_format{};
	std::string m_buffer{};
	/// <summary>
	/// Indicates the line at which to change the text memory
	/// </summary>
	size_t m_line{};

	/// <summary>
	/// Size of the grid in characters
	/// </summary>
	v2i32 m_grid_size{ 120, 30 };

	u8 m_change{};
};


template<typename ...Types>
inline void TextBuffer::add(std::format_string<Types...> fmt, Types&&... args) noexcept
{	
	for (const auto i : std::format(fmt, std::forward(args)...))
		add(i);
	m_change++;
}
