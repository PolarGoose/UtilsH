#include "utils_h/logging/scoped_log.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>

using namespace utils_h;
using namespace logging;
using namespace testing;

class scoped_log_test : public Test {
  protected:
    void log_function(const std::string &message)
    {
        _log_message = message;
    }

    std::string _log_message;
};

TEST_F(scoped_log_test, Writes_correct_information_to_the_log)
{
    {
        scoped_log scopedLog{"LineInfo ", [&](const std::string &msg) { log_function(msg); }};
        ASSERT_EQ(_log_message, "LineInfo Enter");
    }
    ASSERT_TRUE(std::regex_match(_log_message, std::regex{"LineInfo Exit\\(\\d+ms\\)"}));
}

TEST_F(scoped_log_test, Detects_exception)
{
    try {
        scoped_log scopedLog{"LineInfo ", [&](const std::string &msg) { log_function(msg); }};
        ASSERT_EQ(_log_message, "LineInfo Enter");
        throw std::runtime_error("error");
    }
    catch (const std::runtime_error &) {
        ASSERT_TRUE(std::regex_match(_log_message, std::regex{"LineInfo Exception\\(\\d+ms\\)"}));
    }
}
