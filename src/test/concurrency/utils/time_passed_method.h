#pragma once
#include <chrono>

class time_passed_method {
  protected:
    [[nodiscard]] auto time_passed() const
    {
        return std::chrono::steady_clock::now() - m_startTime;
    }

    const std::chrono::steady_clock::time_point m_startTime{std::chrono::steady_clock::now()};
};
