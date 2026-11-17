#pragma once
/// ==== Allosker 2026-2027 ===================
// Parses a Terminal Buffer and extracts the corresponding commands that are put into a Command List
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"

#include <queue>
#include <string>
#include "CommandSheet.hpp"

struct Command
{
	std::string name{};
	std::vector<std::string> args{};
};

class CommandExecutioner
{
public:

	CommandExecutioner() = default;

	DELETE_COPY_INIT(CommandExecutioner);
	DEFAULT_MOVE_INIT(CommandExecutioner);


	/// <summary>
	/// Parses the user input buffer to determine the commands
	/// </summary>
	/// <param name="buffer"></param>
	/// <returns>Empty string is no error, the error otherwise</returns>
	std::string parse(const std::string& buffer) noexcept;

	/// <summary>
	/// Runs every command that was put into the queue
	/// </summary>
	/// <returns>The graphical output of all the commands that were run</returns>
	std::string run_commands(sf::Window& window) noexcept;

	/// <summary>
	/// Get the current Windows' directory as a user-friendly string
	/// </summary>
	std::string get_cd() const noexcept { return m_cd.string() + "> "; };


private:

	Command m_command{};

	filepath m_cd{ "C:\\" };

};