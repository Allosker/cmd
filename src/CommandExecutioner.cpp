#include "CommandExecutioner.hpp"

#include <unordered_map>

std::string CommandExecutioner::parse(const std::string& buffer) noexcept
{
	std::string cmd{};
	bool fetch_args{};
	for (const auto& i : buffer)
	{
		if (i != ' ' && i != '\n' && i != '\0')
			cmd += i;
		else
		{
			if (!cmd.empty())
			{
				if (!fetch_args)
				{
					if (cmd::g_commands.contains(cmd))
					{
						m_command.name = cmd;
						fetch_args = true;
						cmd = "";
					}
					else
						return "Error: command " + cmd + " doesn't exist.";
				}
				else
				{
					if (cmd::g_commands.at(m_command.name).contains(cmd))
					{
						m_command.args.push_back(cmd);
						fetch_args = false;
						cmd = "";
					}
					else
						return std::format("Error: command {} doesn't accept argument {}", m_command.name, cmd);
				}

			}
		}
	}

	return "";
}

std::string CommandExecutioner::run_commands(sf::Window& window) noexcept
{
	std::string ret{};

	if (m_command.name == "exit") ret = cmd::exit(window);
	else if (m_command.name == "power") ret = cmd::power(m_command.args);
	else if (m_command.name == "terminate") ret = cmd::terminate();

	m_command = {};
	return ret;
}
