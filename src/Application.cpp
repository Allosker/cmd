#include "Application.hpp"

#include "sys/utils.hpp"
#include <Windows.h>
#include <fstream>

/// <summary>
/// Return the root path of this app
/// </summary>
static filepath fetch_root_path() noexcept
{
	char buff[MAX_PATH];
	auto size = GetModuleFileNameA(nullptr, buff, MAX_PATH); // path to the executable which is running this program

	if (size == 0 || size >= MAX_PATH)
		throw std::runtime_error("Failed to retrieve executable path");

	return filepath{ buff }.parent_path();
}

bool Application::save_apps() noexcept
{
	std::unique_lock lock{ mutex };

	std::ofstream out{};

	out.open(root_path.string() + g_data_file, std::ios::out /*| std::ios::app*/);

	if (out.is_open())
	{
		out << "This is a test\n1234\n";
	}
	else
		return false;
	out.close();

	return true;
}

std::string Application::get_save_file_content() noexcept
{
	std::unique_lock lock{ mutex };

	std::ifstream in{};

	in.open(root_path.string() + g_data_file, std::ios::in);

	std::string ret;
	if (in.is_open())
	{
		std::string s;
		while (std::getline(in, s))
			ret.append(s + '\n');
	}
	else
		return "";
	in.close();

	return ret;
}

Application::Application()
{
	//root_path = fetch_root_path(); reaplce that when done doing the final version
	root_path = L"C:\\development\\cmd\\data";
}
