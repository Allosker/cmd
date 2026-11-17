#include "CommandExecutioner.hpp"

#include "CommandSheet.hpp"
#include <format>
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
		if (buffer[cur] != ' ' && buffer[cur] != '\n' && buffer[cur] != '\0')
			cmd += buffer[cur];
		else
		{
			if (!cmd.empty())
			{
				if (toa == None && m_command.name == "")
				{
					m_command.name = cmd;
					cmd = "";
				}
				else
				{
					m_command.args.push_back(cmd);
					cmd = "";
				}

			}
		}
	}

	return "";
}

std::string get_output_ret(const cmd::Output& output)
{
	std::string ret;
	switch (output.type)
	{
		case cmd::Output::None:
			return output.msg;
			break;

		case cmd::Output::Warning:
			ret = output.msg;
			ret.append(output.err);
			return ret;
			break;

		case cmd::Output::Error:
			return output.err;
			break;
	}
}

std::string CommandExecutioner::run_commands(sf::Window& window) noexcept
{
	std::string ret{};

	if (m_command.name == "exit")
	{
		ret = get_output_ret(cmd::exit(window));
	}
	else if (m_command.name == "cd")
	{
		auto output = cmd::cd(m_cd, m_command.args);
		switch (output.type)
		{
			case cmd::Output::None:
				m_cd = output.msg;
				ret = "";
				break;

			case cmd::Output::Error:
				ret = output.err;
				break;
		}
	}
	else if (m_command.name == "ls")
	{
		ret = get_output_ret(cmd::ls(m_cd, m_command.args));
	}
	else
	{
		const auto fn = cmd::g_commands.find(m_command.name);

		if (fn != cmd::g_commands.end())
			ret = get_output_ret(fn->second(m_command.args));
		else
			ret = "<## Error: No such command exists ##>";
	}

	m_command = {};
	return ret;
}
