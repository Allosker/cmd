#pragma once
/// ==== Allosker 2026-2027 ===================
// Defines a lot of helper functions/types that can help code faster and better
/// ==== https://github.com/Allosker/cmd ======

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <filesystem>

using i8 = std::int8_t;
using u8 = std::uint8_t;

using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;


using f32 = float;
using f64 = double;

using filepath = std::filesystem::path;


using v2i8 = sf::Vector2<i8>;
using v2u8 = sf::Vector2<u8>;

using v2i16 = sf::Vector2<i16>;
using v2u16 = sf::Vector2<u16>;

using v2i32 = sf::Vector2<i32>;
using v2u32 = sf::Vector2<u32>;

using v2i64 = sf::Vector2<i64>;
using v2u64 = sf::Vector2<u64>;


using v2f32 = sf::Vector2<f32>;
using v2f64 = sf::Vector2<f64>;


#define DELETE_COPY_INIT(Type) Type(const Type&) noexcept = delete; Type& operator=(Type&) = delete;
#define DEFAULT_COPY_INIT(Type) Type(const Type&) noexcept = default; Type& operator=(Type&) = default;

#define DEFAULT_MOVE_INIT(Type) Type(Type&&) noexcept = default; Type& operator=(Type&&) = default;