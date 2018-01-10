#include "utils_h/logging/log_stream.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace logging;
using namespace testing;

struct log_stream_test : public Test {
    void log_function(const std::string &message)
    {
        m_logMessage = message;
    }

    std::string m_logMessage;
};

TEST_F(log_stream_test, works_correctly)
{
    log_stream{[&](const std::string &str) { log_function(str); }}.get_stream() << "a"
                                                                                << "b" << 10;

    ASSERT_EQ(m_logMessage, "ab10");
}
