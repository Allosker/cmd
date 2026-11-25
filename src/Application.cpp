#include "Application.hpp"

#include "sys/utils.hpp"
#include <Windows.h>

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

Application::Application()
{
	root_path = fetch_root_path();
}
