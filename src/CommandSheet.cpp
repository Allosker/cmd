#include "CommandSheet.hpp"

#include "sys/utils.hpp"

#include "Application.hpp"
#include <locale>
#include <codecvt>

/// <summary>
/// The Window API
/// </summary>
#include <Windows.h>
#include <PowrProf.h>
#include <shlobj_core.h>


static inline bool get_shutdown_privileges() noexcept
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process

	if (!OpenProcessToken(GetCurrentProcess(),
							   TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	// Get the LUID for the shutdown privilege

	LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1; // one privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process

	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)nullptr, 0);
}

namespace cmd
{
	std::string power(Args args) noexcept
	{
		const auto& mode = args[0];

		if (mode == "sleep")
		{
			SetSuspendState(false, false, false);
			return "sleep";
		}
		else if (mode == "hibernate")
		{
			SetSuspendState(true, false, false);
			return "hibernate";
		}
		else if (mode == "off")
		{
			ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			return "power off";
		} 
		else if (mode == "restart")
		{
			ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			return "restart";
		}
		else if (mode == "restart_wapps")
		{
			ExitWindowsEx(EWX_RESTARTAPPS, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
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

	std::expected<std::string, std::string> cd(Args args) noexcept
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wstr{ converter.from_bytes(args[0]) };

		const wchar_t* drive = wstr.c_str();

		if (SetCurrentDirectoryW(drive))
		{
			return args[0];
		}

		return std::unexpected{ "Directory " + args[0] + " does not exist." };
	}

	std::string help(Args args) noexcept
	{
		return "Not yet done";
	}



}