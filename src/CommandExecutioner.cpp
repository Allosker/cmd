#include "CommandExecutioner.hpp"

#include <unordered_map>

enum TypeOfArg
{
	None,
	PreDef,
	Str
};

std::string CommandExecutioner::parse(const std::string& buffer) noexcept
{
	std::string cmd{};
	TypeOfArg toa{};
	for (size_t cur{}; cur < buffer.size(); cur++)
	{
		size_t pre = cur == 0 ? cur : cur - 1;
		size_t fol = cur == buffer.size() - 1 ? cur : cur + 1;

		if (buffer[cur] != ' ' && buffer[cur] != '\n' && buffer[cur] != '\0')
			cmd += buffer[cur];
		else
		{
			if (!cmd.empty())
			{
				if (toa == None && m_command.name == "")
				{
					if (cmd::g_commands.contains(cmd))
					{
						m_command.name = cmd;

						if (cmd::g_commands.at(cmd).size() > 1) // later fix that, only one argument defined if it accepts a sqc (logical, if there is only one arg, it can't be predefined)
							toa = PreDef;
						else if (cmd::g_commands.at(cmd).contains("__str__"))
							toa = Str;

						cmd = "";
					}
					else
						return "Error: command " + cmd + " doesn't exist.";
				}
				else
				{
					switch (toa)
					{
						case PreDef:
						{
							if (cmd::g_commands.at(m_command.name).contains(cmd))
							{
								m_command.args.push_back(cmd);
								return "";
							}
							else
								return std::format("Error: command {} doesn't accept argument {}", m_command.name, cmd);
							break;
						}
						case Str:
						{
							while (buffer[fol] != ' ' && buffer[fol] != '\n')
							{
								pre = cur == 0 ? cur : cur - 1;
								fol = cur == buffer.size() - 1 ? cur : cur + 1;
								cur++;

								cmd += buffer[cur];
							}

							m_command.args.push_back(cmd);

							break;
						}
					}
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
	else if (m_command.name == "cd") ret = m_command.args[0];

	m_command = {};
	return ret;
}
