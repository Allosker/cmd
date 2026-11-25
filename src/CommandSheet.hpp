#pragma once
/// ==== Allosker 2026-2027 ===================
// All functions that can be used as Commands within this CMD
// Note, all functions DO return a string
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"
#include <SFML/Window/Window.hpp>
#include <functional>
#include <unordered_map>

namespace cmd
{
	using Args = const std::vector<std::string>&;

	struct Output
	{
		std::string msg;
		enum Type
		{
			None,
			Warning,
			Error
		} type;
	};


	Output exit(sf::Window& window, Args args) noexcept;

	[[nodiscard]] Output cd(sf::Window& window, Args args) noexcept;

	Output ls(sf::Window& window, Args args) noexcept;


	Output power(sf::Window& window, Args args) noexcept;

	Output terminate(sf::Window& window, Args args) noexcept;

	Output help(sf::Window& window, Args args) noexcept;

	Output save(sf::Window& window, Args args) noexcept;


	/// <summary>
	/// The normal set of commands
	/// </summary>
	static const std::unordered_map<std::string, std::function<Output(sf::Window&, Args)>> g_commands
	{
		std::pair<std::string, std::function<Output(sf::Window&, Args)>>
		{"exit", cmd::exit},
		{"cd", cmd::cd},
		{"ls", cmd::ls},
		{"power", cmd::power},
		{"terminate", cmd::terminate},
		{"help", cmd::help},
		{"save", cmd::save}

	};


}