#include <cstdint>

#pragma once

// Беззнаковые целочисленные типы
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

// Проверка, что все типы занимают ожидаемое количество байт
static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

static_assert(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// Определение целевой платформы (ОС)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define POVLY_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define POVLY_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define POVLY_PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Другие Unix-системы
#define POVLY_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// POSIX
#define POVLY_PLATFORM_POSIX 1
#elif __APPLE__
// Платформы Apple
#define POVLY_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// Симулятор iOS
#define POVLY_PLATFORM_IOS 1
#define POVLY_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define POVLY_PLATFORM_IOS 1
// Устройство iOS
#elif TARGET_OS_MAC
// Другие виды macOS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

// Макросы экспорта/импорта для динамических библиотек (.dll / .so)
#ifdef POVLY_EXPORT
// Экспорт (когда компилируется сам движок)
#ifdef _MSC_VER
#define POVLY_API __declspec(dllexport)
#else
#define POVLY_API __attribute__((visibility("default")))
#endif
#else
// Импорт (когда движок подключается к игре)
#ifdef _MSC_VER
#define POVLY_API __declspec(dllimport)
#else
#define POVLY_API
#endif
#endif
