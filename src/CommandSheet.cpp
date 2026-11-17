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
#include <strsafe.h>
#include <tchar.h>


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
#pragma region Special Funcs

	Output exit(sf::Window& window) noexcept
	{
		window.close();
		return { .msg{"Closing Current Session"}, .type{Output::None} };

	}

	Output cd(Args args) noexcept
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wstr{ converter.from_bytes(args[0]) };

		const wchar_t* drive = wstr.c_str();

		if (SetCurrentDirectoryW(drive))
		{
			return { .msg{args[0]}, .type{Output::None} };
		}

		return { .err{"Directory " + args[0] + " does not exist."}, .type{Output::Error} };
	}

	Output ls(const filepath& cd, Args args) noexcept
	{
		LARGE_INTEGER filesize{};
		
		// convert filepath to TCHAR
		std::basic_string<TCHAR> tstr = cd.string();

		size_t length_of_arg{};
		StringCchLengthA(tstr.c_str(), MAX_PATH, &length_of_arg);

		// We remove 3 from the Max path size for the "\*" and the NULL appended below
		if (length_of_arg > (MAX_PATH - 3))
			return { .err{"<## Error: File Path is too long ##>"}, .type{ Output::Error} };

		TCHAR szDir[MAX_PATH];
		// Prepare sting for use with FindFile functions
		StringCchCopy(szDir, MAX_PATH, tstr.c_str());
		StringCchCat(szDir, MAX_PATH, TEXT("\\*")); // append "\*" to the directory name

		WIN32_FIND_DATA ffd{};
		HANDLE hFind = INVALID_HANDLE_VALUE;
		hFind = FindFirstFile(szDir, &ffd); // find the first file in the directory

		if (INVALID_HANDLE_VALUE == hFind)
		{
			DWORD error = GetLastError(); 

			std::error_code ec{ static_cast<int>(error), std::system_category() };

			return { .err{std::format("<## Windows Error: {} ##>", ec.message())}, .type{Output::Error}};
		}

		std::string output{};
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				output.append(ffd.cFileName);
			}
			else
			{
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;

				output.append(ffd.cFileName);
				output.append(std::to_string(filesize.QuadPart));
			}

			output += '\n';
		} while (FindNextFile(hFind, &ffd) != 0);

		DWORD error = GetLastError();
		if (error != ERROR_NO_MORE_FILES)
		{
			std::error_code ec{ static_cast<int>(error), std::system_category() };

			return { .err{std::format("<## Windows Error: {} ##>", ec.message())}, .type{ Output::Error} };
		}

		FindClose(hFind);
		return { .msg{output}, .type{Output::None} };
	}

#pragma endregion


#pragma region Funcs

	Output power(Args args) noexcept
	{
		std::string ret;
		const auto& mode = args[0];

		if (mode == "sleep")
		{
			SetSuspendState(false, false, false);
			ret = "sleep";
		}
		else if (mode == "hibernate")
		{
			SetSuspendState(true, false, false);
			ret = "hibernate";
		}
		else if (mode == "off")
		{
			ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			ret = "power off";
		}
		else if (mode == "restart")
		{
			ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			ret = "restart";
		}
		else if (mode == "restart_wapps")
		{
			ExitWindowsEx(EWX_RESTARTAPPS, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			ret = "restart";
		}
		else
			return { .err{"\n<## Error: power does not take argument " + mode + " ##>"}, .type{Output::Error} };

		return { .msg{ret}, .type{Output::None} };
	}

	

	Output terminate(Args args) noexcept
	{
		Application::get().stop_program = true;
		return { .msg{"Terminating Program"}, .type{Output::None} };
	}

	Output help(Args args) noexcept
	{
		return { .msg{"unfinished"}, .type{Output::Warning}};
	}

#pragma endregion

}