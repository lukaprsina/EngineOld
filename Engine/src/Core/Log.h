#pragma once
#include "pch.h"
#include "Core/Base.h"

#include "spdlog/spdlog.h"

namespace eng
{
    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define ENG_CORE_TRACE(...) ::eng::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENG_CORE_INFO(...) ::eng::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENG_CORE_WARN(...) ::eng::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENG_CORE_ERROR(...) ::eng::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENG_CORE_CRITICAL(...) ::eng::Log::GetCoreLogger()->critical(__VA_ARGS__);

// Client log macros
#define ENG_TRACE(...) ::eng::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENG_INFO(...) ::eng::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENG_WARN(...) ::eng::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENG_ERROR(...) ::eng::Log::GetClientLogger()->error(__VA_ARGS__)
#define ENG_CRITICAL(...) ::eng::Log::GetClientLogger()->critical(__VA_ARGS__);
