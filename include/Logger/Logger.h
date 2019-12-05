#pragma once

#include "Logger/ILogger.h"

#include "RuntimeReflection/Macro.h"

#include "Logger/Logger.Generated.h"

#include "vector"
#include "fstream"
#include "cstdarg"

#undef FATAL
#undef ERROR

namespace SteelEngine {

    SE_CLASS()
    class Logger : public ILogger
    {
        GENERATED_BODY
    public:
        enum Verbosity
        {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

    private:
        std::vector<std::string> m_PendingLogs;
        std::ofstream m_OutFile;
        const char* m_LogFilePath;
        bool m_LogToFile;

    public:
        Logger(const char* file);
        ~Logger();

        SE_METHOD()
        Result Init() override;

        SE_METHOD()
        void Log(const std::string& message, int verbosity, Type::uint32 line, const std::string& file, va_list args) override;

        SE_METHOD()
        void Log(const std::string& message, int verbosity, va_list args) override;

        void Update() override;

        void Info(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::INFO, line, file, args);

            va_end(args);
        }

        void Info(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::INFO, args);

            va_end(args);
        }

        void Warning(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::WARNING, line, file, args);

            va_end(args);
        }

        void Warning(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::WARNING, args);

            va_end(args);
        }

        void Error(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::ERROR, line, file, args);

            va_end(args);
        }

        void Error(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::ERROR, args);

            va_end(args);
        }

        void Fatal(Type::uint32 line, const std::string& file, const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::FATAL, line, file, args);

            va_end(args);
        }

        void Fatal(const char* message, ...)
        {
            va_list args;

            va_start(args, message);

            Log(message, Verbosity::FATAL, args);

            va_end(args);
        }
    };

// #define SE_INFO(message, ...) ((Logger*)m_Logger)->Info(__LINE__, __FILE__, message, __VA_ARGS__)
// #define SE_WARNING(message, ...) ((Logger*)m_Logger)->Warning(__LINE__, __FILE__, message, __VA_ARGS__)
// #define SE_ERROR(message, ...) ((Logger*)m_Logger)->Error(__LINE__, __FILE__, message, __VA_ARGS__)
// #define SE_FATAL(message, ...) ((Logger*)m_Logger)->Fatal(__LINE__, __FILE__, message, __VA_ARGS__)

#define SE_INFO(message, ...) (*Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<Logger**>())->Info(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_WARNING(message, ...) (*Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<Logger**>())->Warning(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_ERROR(message, ...) (*Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<Logger**>())->Error(__LINE__, __FILE__, message, __VA_ARGS__)
#define SE_FATAL(message, ...) (*Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::LOGGER)->Convert<Logger**>())->Fatal(__LINE__, __FILE__, message, __VA_ARGS__)

}