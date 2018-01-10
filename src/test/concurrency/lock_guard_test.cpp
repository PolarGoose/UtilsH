#include "utils_h/concurrency/lock_guard.h"
#include <gtest/gtest.h>

using namespace testing;

struct lock_guard_test : public Test {
    auto is_mutex_locked()
    {
        const auto locked_successfully = _m.try_lock();
        if (locked_successfully) {
            _m.unlock();
        }
        return !locked_successfully;
    }

    std::mutex _m;
};

TEST_F(lock_guard_test, Locks_mutex)
{
    UTILS_H_LOCK_GUARD(_m);
    ASSERT_TRUE(is_mutex_locked());
}

TEST_F(lock_guard_test, Unlocks_mutex_at_the_end_of_a_scope)
{
    {
        UTILS_H_LOCK_GUARD(_m);
    }
    ASSERT_FALSE(is_mutex_locked());
}
