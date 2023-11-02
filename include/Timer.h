#pragma once

#ifndef TIMER_H
#define TIMER_H
#include <iostream>
#include <chrono>
#include <string>
#endif // TIMER_H

class Timer
{
public:
    Timer()
    {
        timeStarted = std::chrono::high_resolution_clock::now();
    }

    int endTimer()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - timeStarted);

        return static_cast<int>(duration.count());
    }

private:
    std::chrono::steady_clock::time_point timeStarted;
};

namespace TimerFuncs
{
    std::string convertTime(int microsecondTime)
    {
        constexpr long long kMicrosecondsInMillisecond = 1000;
        constexpr long long kMicrosecondsInSecond = kMicrosecondsInMillisecond * 1000;

        double timeValue;
        std::string unit;

        if (microsecondTime < kMicrosecondsInMillisecond)
        {
            timeValue = microsecondTime;
            unit = "µs";
        }
        else if (microsecondTime < kMicrosecondsInSecond)
        {
            timeValue = static_cast<double>(microsecondTime) / kMicrosecondsInMillisecond;
            unit = "ms";
        }
        else
        {
            timeValue = static_cast<double>(microsecondTime) / kMicrosecondsInSecond;
            unit = "s";
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << timeValue << " " << unit;
        return oss.str();
    }
}