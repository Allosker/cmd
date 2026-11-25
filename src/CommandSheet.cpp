#include "CommandSheet.hpp"

#include "sys/utils.hpp"

#include "Application.hpp"


#include "sys/winAPI.hpp"

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

	Output exit(sf::Window& window, Args args) noexcept
	{
		window.close();
		return { .msg{"Closing Current Session"}, .type{Output::None} };

	}

	Output cd(sf::Window& window, Args args) noexcept
	{
		if (args.size() > 1)
			return { .msg{"\n\n<##Error: Command cd does not take " + std::to_string(args.size()) + " arguments##>"}, .type{Output::Error} };

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

		std::wstring cwd{};
		if (!get_cwd(cwd))
			return { .msg{"<## Windows Error: " + converter.to_bytes(cwd) + " ##>"}, .type{ Output::Error} };

		std::wstring path{};

		if (args[0].substr(1, 2) == ":\\") // A drive name letter is always 1 character
		{
			path = converter.from_bytes(args[0]);
		}
		else
		{
			path = cwd;
			if (path.back() != L'\\')
				path.append(L"\\" + converter.from_bytes(args[0])); // append the path with \ if it doesn't contain it
			else
				path.append(converter.from_bytes(args[0]));
		}

		if (SetCurrentDirectoryW(path.c_str()))
		{
			return { .msg{""}, .type{Output::None}}; // make it so that the whole path is returned
		}
		
		return { .msg{"Directory " + converter.to_bytes(path) + " does not exist."}, .type{Output::Error} };
	}

	Output ls(sf::Window& window, Args args) noexcept
		// No arg limit, gonna add some args to it soon
	{
		LARGE_INTEGER filesize{};
		
		// convert filepath to TCHAR
		std::basic_string<TCHAR> szDir{};

		if (!get_cwd(szDir))
			return { .msg{"<## Windows Error: " + szDir + " ##>"}, .type{ Output::Error} };

		// We remove 3 from the Max path size for the "\*" and the NULL appended below
		if (szDir.length() > (MAX_PATH - 3))
			return { .msg{"<## Error: File Path is too long ##>"}, .type{ Output::Error} };

		// Prepare sting for use with FindFile functions
		szDir.append("\\*"); // append "\*" to the directory name

		WIN32_FIND_DATA ffd{};
		HANDLE hFind = INVALID_HANDLE_VALUE;
		hFind = FindFirstFile(szDir.c_str(), &ffd); // find the first file in the directory

		if (INVALID_HANDLE_VALUE == hFind)
			return { .msg{std::format("<## Windows Error: {} ##>", get_last_error())}, .type{Output::Error}};

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

			return { .msg{std::format("<## Windows Error: {} ##>", ec.message())}, .type{ Output::Error} };
		}

		FindClose(hFind);
		return { .msg{output}, .type{Output::None} };
	}


	Output power(sf::Window& window, Args args) noexcept
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
			get_shutdown_privileges();
			ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			Application::get().stop_program = true;
			ret = "power off";
		}
		else if (mode == "restart")
		{
			ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			Application::get().stop_program = true;
			ret = "restart";
		}
		else if (mode == "restart_wapps")
		{
			ExitWindowsEx(EWX_RESTARTAPPS, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED);
			ret = "restart";
		}
		else
			return { .msg{"\n<## Error: power does not take argument " + mode + " ##>"}, .type{Output::Error} };

		return { .msg{ret}, .type{Output::None} };
	}

	

	Output terminate(sf::Window& window, Args args) noexcept
	{
		Application::get().stop_program = true;
		return { .msg{"Terminating Program"}, .type{Output::None} };
	}

	Output help(sf::Window& window, Args args) noexcept
	{
		return { .msg{"unfinished"}, .type{Output::Warning}};
	}

	Output save(sf::Window& window, Args args) noexcept
	{
		Application::get().save_apps();
		return { .msg{ Application::get().get_save_file_content() }, .type{Output::None}};
		//return { .msg{ Application::get().get_root_path().string() }, .type{Output::None} };
	}

	Output launch(sf::Window& window, Args args) noexcept
	{
		std::string path = "C:\\Users\\Allosker\\AppData\\Local\\Discord\\Discord.exe";

		STARTUPINFO info{ sizeof(info) };
		PROCESS_INFORMATION process_info{};

		if (CreateProcess(path.c_str(), nullptr, nullptr, nullptr, true, 0, nullptr, nullptr, &info, &process_info))
		{
			WaitForSingleObject(process_info.hProcess, INFINITE);
			CloseHandle(process_info.hProcess);
			CloseHandle(process_info.hThread);
		}
		else
			return { .msg{ "<# Error: Couldn't launch app correctly #>" }, .type{Output::Error} };


		return { .msg{ "Successfully Launched Discord"}, .type{Output::None}};
	}


}