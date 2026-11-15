#pragma once
/// ==== Allosker 2026-2027 ===================
// Holds the Application's states
/// ==== https://github.com/Allosker/cmd ======

#include <atomic>

class Application
{
public:


	static Application& get() noexcept
	{
		static Application instance;
		return instance;
	}


	std::atomic<bool> stop_program{};

private:

	Application()
	{
	}


};