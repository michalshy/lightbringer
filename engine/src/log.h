#pragma once
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#define LOG_DEBUG spdlog::debug
#define LOG_INFO spdlog::info
#define LOG_WARN spdlog::warn
#define LOG_ERROR spdlog::error

#define LOG_LEVEL_DEBUG spdlog::level::debug
#define LOG_LEVEL_INFO spdlog::level::info