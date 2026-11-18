#pragma once


#include <Windows.h>
#include <PowrProf.h>
#include <codecvt>
#include <locale>
#include <shlobj_core.h>
#include <strsafe.h>
#include <tchar.h>


/// <returns>Last Windows API error</returns>
inline std::string get_last_error() noexcept
{
	DWORD error = GetLastError();

	std::error_code ec{ static_cast<int>(error), std::system_category() };

	return ec.message();
}

/// <summary>
/// Get the cwd from the Windows API
/// </summary>
/// <param name="The path if successful the error otherwise"></param>
/// <returns>True if successful false otherwise</returns>
inline bool get_cwd(std::string& in) noexcept
{
	std::basic_string<TCHAR> temp{};
	if (auto size = GetCurrentDirectory(0, nullptr)/*get required size*/; size != 0)
	{
		temp.resize(size);
		if (!GetCurrentDirectory(size, temp.data()))
		{
			in = get_last_error();
			return false;
		}
	}
	else
	{
		in = get_last_error();
		return false;
	}
	temp.erase(temp.end() - 1); // last null character
	in = temp;
	return true;
}

/// <summary>
/// Get the cwd from the Windows API
/// </summary>
/// <param name="The path if successful the error otherwise"></param>
/// <returns>True if successful false otherwise</returns>
inline bool get_cwd(std::wstring& in) noexcept
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	std::basic_string<TCHAR> temp{};
	if (auto size = GetCurrentDirectory(0, nullptr)/*get required size*/; size != 0)
	{
		temp.resize(size);
		if (!GetCurrentDirectory(size, temp.data()))
		{
			in = converter.from_bytes(get_last_error());
			return false;
		}
	}
	else
	{
		in = converter.from_bytes(get_last_error());
		return false;
	}
	temp.erase(temp.end() - 1); // last null character
	in = converter.from_bytes(temp);
	return true;
}