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
#include <functional>

namespace cmd
{
	using Args = const std::vector<std::string>&;

	struct Output
	{
		std::string err;
		std::string msg;
		enum Type
		{
			None,
			Warning,
			Error
		} type;
	};

	/// <summary>
	/// Special Commands are commands that do not take the "base" set of Arguments, that need to have a special caller
	/// </summary>
	static const std::unordered_set<std::string> g_special_commands
	{
		"exit",
		"ls",
		"cd"
	};


	/// <summary>
	/// Cannot throw an error
	/// </summary>
	/// <param name="Window">Current Terminal window</param>
	/// <returns></returns>
	Output exit(sf::Window& window) noexcept;

	/// <summary>
	/// The first argument of this function must always be the current Directory
	/// This function's output must not be discarded for the program's sakes
	/// </summary>
	/// <param name="args"></param>
	/// <returns></returns>
	[[nodiscard]] Output cd(const filepath& cd, Args args) noexcept;

	/// <summary>
	/// The first argument of this function must always be the current Directory
	/// </summary>
	/// <param name="args"></param>
	/// <returns></returns>
	Output ls(const filepath& cd, Args args) noexcept;


	Output power(Args args) noexcept;

	Output terminate(Args args) noexcept;

	Output help(Args args) noexcept;


	

	/// <summary>
	/// The normal set of commands
	/// </summary>
	static const std::unordered_map<std::string, std::function<Output(Args)>> g_commands
	{
		std::pair<std::string, std::function<Output(Args)>>
		{"power", cmd::power},
		{"terminate", cmd::terminate},
		{"help", cmd::help}

	};


}