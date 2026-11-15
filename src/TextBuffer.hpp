#pragma once
/// ==== Allosker 2026-2027 ===================
// A text buffer manages a dynamic array of strings (with each representing one line).
// The goal of the class is to accept entry text to format it according to various customisable parameters.
//
// Only the current command may be altered, those that have alreadby been setn don't need that
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
	/// <summary>
	/// A command is any chunk of text (either entered by the user or program)
	/// </summary>
	using CommandBuffer = std::vector<std::string>;
	/// <summary>
	/// A terminal buffer takes responsibility over several commands in order to
	/// keep track of what is the newest command that needs to be parsed
	/// </summary>
	using TerminalBuffer = std::vector<CommandBuffer>;

public:

	DELETE_COPY_INIT(TextBuffer);
	DEFAULT_MOVE_INIT(TextBuffer);

	TextBuffer();

	const std::string get_terminal_buffer() const noexcept;
	const std::string& get_UserInput_buffer() const noexcept { return m_buffer; }


	v2i32 get_grid_size() const noexcept { return m_grid_size; }

	u8 get_change_count() const noexcept { return m_change; }

	v2i32 get_cursor() const noexcept { return m_cursor; }

	void move_cursor(v2i32 offset) noexcept;

	
	/// <summary>
	/// Enter the current command
	/// </summary>
	void enter() noexcept;


	template<typename... Types>
	void add(std::format_string<Types...> fmt, Types&&... args) noexcept;

	void add(i8 c) noexcept;

	void carriage_return() noexcept;

	void erase() noexcept;

	void erase(v2i32 pos) noexcept;



private:

	TerminalBuffer m_commands{};
	CommandBuffer* m_format{};

	std::string m_buffer{};

	/// <summary>
	/// Size of the grid in characters
	/// </summary>
	v2i32 m_grid_size{ 120, 28 };

	mutable v2i32 m_cursor{};

	u8 m_change{};
	/// <summary>
	/// False represents Program mode
	/// True represents User mode
	/// </summary>
	bool m_entry_mode{};

	bool m_writing{ true };

};


template<typename... Types>
inline void TextBuffer::add(std::format_string<Types...> fmt, Types&&... args) noexcept
{	
	for (const auto i : std::format(fmt, std::forward(args)...))
	{
		add(i);
		if (m_entry_mode)
			m_buffer += i;
	}
	m_change++;
}
