#include "CommandSheet.hpp"

#include "sys/utils.hpp"

#include "Application.hpp"

/// <summary>
/// The Window API
/// </summary>
namespace win
{
#	include <Windows.h>
#	include <PowrProf.h>
} // win

static inline bool get_shutdown_privileges() noexcept
{
	win::HANDLE hToken;
	win::TOKEN_PRIVILEGES tkp;

	// Get a token for this process

	if (!win::OpenProcessToken(win::GetCurrentProcess(),
							   TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	// Get the LUID for the shutdown privilege

	LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1; // one privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process

	win::AdjustTokenPrivileges(hToken, false, &tkp, 0, (win::PTOKEN_PRIVILEGES)nullptr, 0);
}

namespace cmd
{
	std::string power(Args args) noexcept
	{
		const auto& mode = args[0];

		if (mode == "sleep")
		{
			win::SetSuspendState(false, false, false);
			return "sleep";
		}
		else if (mode == "hibernate")
		{
			win::SetSuspendState(true, false, false);
			return "hibernate";
		}
		else if (mode == "off")
		{
			win::ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			return "power off";
		} 
		else if (mode == "restart")
		{
			win::ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			return "restart";
		}
		else if (mode == "restart_wapps")
		{
			win::ExitWindowsEx(EWX_RESTARTAPPS, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			return "restart";
		}

		return "";
	}

	std::string exit(sf::Window& window) noexcept
	{
		window.close();
		return "Closing Current Session";
	}

	std::string terminate() noexcept
	{
		Application::get().stop_program = true;
		return "Terminating Program";
	}

	std::string cd(Args args) noexcept
	{
		return "Not yet done";
	}

	std::string help(Args args) noexcept
	{
		return "Not yet done";
	}



}