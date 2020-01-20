#pragma once

#include "Profiler/IManager.h"

#include "RuntimeReflection/Macro.h"

#include "Profiler/Manager.Generated.h"

namespace SteelEngine { namespace Profiler {

    SE_CLASS()
    class Manager : public IManager
    {
        GENERATED_BODY
    public:
        struct TimeData
        {
            float m_Time;
            std::string m_Name;
        };

    private:
        std::vector<TimeData> m_Timings;

    public:
        Manager();
        ~Manager();

        SE_METHOD()
        void AddTime(TimeData time);

        SE_METHOD()
        std::vector<Manager::TimeData>* GetTime();
    };

}}