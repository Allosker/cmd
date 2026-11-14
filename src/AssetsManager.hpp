#pragma once
/// ==== Allosker 2026-2027 ===================
// Allocates/Deallocates every single asset that can be passed around 
/// ==== https://github.com/Allosker/cmd ======

#include "sys/utils.hpp"

class AssetsManager
{
public:

	static AssetsManager& get() noexcept
	{
		static AssetsManager instance{};
		return instance;
	}

	sf::Font font;


private:

	AssetsManager() noexcept
	{
		font.openFromFile(ASSET_PATH"SpaceMono-Regular.ttf");
	}


};