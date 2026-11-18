#include "TerminalWindow.hpp"

#include "Application.hpp"
#include "AssetsManager.hpp"
#include <Windows.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <memory>
#include <stop_token>
#include <thread>
#include <vector>

static inline void run_window(std::stop_token stop) noexcept
{
	TerminalWindow window{};

	while (window.is_open() && !stop.stop_requested())
		window.update();
}



DWORD main_thread_id{};
/// <summary>
/// Indicates whether this Windows session is closing
/// </summary>
/// <param name="event"></param>
/// <returns></returns>
BOOL WINAPI Handler(DWORD event)
{
	switch (event)
	{
		case CTRL_SHUTDOWN_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_CLOSE_EVENT:
			Application::get().stop_program = true;
			PostThreadMessage(main_thread_id, WM_QUIT, 0, 0); // stops windows from sending inputs to the program
			return TRUE;
	}

	return FALSE;
}

struct Worker
{
	std::jthread thread;
	std::atomic<bool> finished{ false };
};

int main()
{
	using namespace std::chrono_literals;

	main_thread_id = GetCurrentThreadId();

	if (!RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_ALT, 'T'))
	{
		std::printf("Failed to register hotkey\n");
		return 1;
	}

	MSG msg = {};

	SetConsoleCtrlHandler(Handler, TRUE);

	std::vector<std::unique_ptr<Worker>> threads{};
	AssetsManager::get();

	while (!Application::get().stop_program)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			if (msg.message == WM_HOTKEY && msg.wParam == 1)
			{
				// Allocate new window

				std::unique_ptr<Worker> worker = std::make_unique<Worker>();

				worker->thread = std::jthread{ [w = worker.get()](std::stop_token stop)
					{
						run_window(stop);
						w->finished.store(true);
					} 
				};

				threads.emplace_back(std::move(worker));
			}
		}

		for (auto it = threads.begin(); it != threads.end();)
		{
			if (it->get()->finished.load())
			{
				it = threads.erase(it);
			}
			else
			{
				it++;
			}
		}

		std::this_thread::sleep_for(16ms);
	}

	// Clears all the resources and shuts down the application
	for (auto& worker : threads)
		worker->thread.request_stop();
	threads.clear();

	UnregisterHotKey(nullptr, 1);

	return 0;
}
