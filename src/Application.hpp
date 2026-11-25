#pragma once
/// ==== Allosker 2026-2027 ===================
// Holds the Application's states
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>

class Application
{
public:


	static Application& get() noexcept
	{
		static Application instance;
		return instance;
	}


	std::atomic<bool> stop_program{};

	/// <summary>
	/// Thread Safe
	/// </summary>
	/// <returns></returns>
	filepath get_root_path() const noexcept
	{
		std::shared_lock lock{ mutex };

		return root_path;
	}

	bool save_apps() noexcept;

	std::string get_save_file_content() noexcept;


private:

	Application();

	/// <summary>
	/// Paths for all apps that can be launched directly with the CMD
	/// </summary>
	std::unordered_map<std::string, filepath> apps{};

	filepath root_path{};

	mutable std::shared_mutex mutex;

	inline static const std::string g_data_file{ "\\data.txt" };

};