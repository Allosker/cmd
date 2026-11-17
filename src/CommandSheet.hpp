#pragma once
/// ==== Allosker 2026-2027 ===================
// All functions that can be used as Commands within this CMD
// Note, all functions DO return a string
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"
#include <SFML/Window/Window.hpp>
#include <unordered_map>
#include <unordered_set>
#include <expected>

namespace cmd
{
	/// <summary>
	/// Name of command, parameters
	/// </summary>
	static const std::unordered_map<std::string, std::unordered_set<std::string>> g_commands
	{
		{ "help", {} },
		{ "exit", {} },
		{ "terminate", {} },
		{ "cd", {"__str__"} },
		{ "power", {"sleep", "off", "restart", "hibernate", "restart_wapps"}}
	};

	using Args = const std::vector<std::string>&;

	std::string power(Args args) noexcept;

	/// <summary>
	/// </summary>
	/// <param name="Window">Current Terminal window</param>
	/// <returns></returns>
	std::string exit(sf::Window& window) noexcept;

	std::string terminate() noexcept;

	std::expected<std::string, std::string> cd(Args args) noexcept;

	std::string help(Args args) noexcept;


}