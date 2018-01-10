#include "utils/time_passed_method.h"
#include "utils_h/concurrency/waiting_queue.h"
#include <gtest/gtest.h>
#include <thread>

using namespace utils_h;
using namespace std::chrono_literals;
using namespace testing;

class waiting_queue_test : public Test, protected time_passed_method {
  protected:
    void fill_waiting_queue()
    {
        for (size_t i = 0; i < m_boundary; i++) {
            m_bounded_waiting_queue.blocking_push(i);
        }
    }

    const size_t m_boundary{10};
    concurrency::waiting_queue<size_t> m_bounded_waiting_queue{m_boundary};
    concurrency::waiting_queue<size_t> m_unbounded_waiting_queue;
};

TEST_F(waiting_queue_test, Max_size_of_unbounded_queue_is_equal_to_size_t_max)
{
    ASSERT_EQ(m_unbounded_waiting_queue.max_size(), std::numeric_limits<size_t>::max());
}

TEST_F(waiting_queue_test, Queue_is_empty_by_default)
{
    ASSERT_EQ(m_unbounded_waiting_queue.empty(), true);
}

TEST_F(waiting_queue_test, User_can_put_an_unlimited_amount_of_elements_into_an_unbounded_queue)
{
    const size_t numberOfElements{10 * 1000};

    for (size_t i = 0; i < numberOfElements; i++) {
        m_unbounded_waiting_queue.blocking_push(i);
    }
    ASSERT_EQ(m_unbounded_waiting_queue.size(), numberOfElements);

    for (size_t i = 0; i < numberOfElements; i++) {
        ASSERT_EQ(m_unbounded_waiting_queue.blocking_pop(), i);
    }
    ASSERT_EQ(m_unbounded_waiting_queue.size(), 0);
}

TEST_F(waiting_queue_test, Reading_from_an_empty_queue_throws_an_exception_after_time_out)
{
    const auto timeout{100ms};
    ASSERT_THROW(m_unbounded_waiting_queue.blocking_pop(timeout), concurrency::timed_out_exception);
    ASSERT_GE(time_passed(), timeout);
}

TEST_F(waiting_queue_test, Reading_from_an_empty_queue_is_unblocked_when_an_element_is_added_to_the_queue)
{
    const auto timeout{1s};
    const auto time_to_wait_before_send{timeout / 2};
    const auto data{1};

    std::thread sender([&] {
        std::this_thread::sleep_for(time_to_wait_before_send);
        m_unbounded_waiting_queue.blocking_push(data);
    });

    ASSERT_EQ(m_unbounded_waiting_queue.blocking_pop(timeout), data);
    ASSERT_GE(time_passed(), time_to_wait_before_send);
    sender.join();
}

TEST_F(waiting_queue_test, Send_function_blocks_if_queue_is_full)
{
    fill_waiting_queue();

    ASSERT_THROW(m_bounded_waiting_queue.blocking_push(1, 100ms), concurrency::timed_out_exception);
    ASSERT_GE(time_passed(), 100ms);
}

TEST_F(waiting_queue_test, Send_method_is_unblocked_when_queue_has_free_space)
{
    fill_waiting_queue();
    const auto timeout{1s};
    const auto time_to_wait_before_read{timeout / 2};

    std::thread receiver([&] {
        std::this_thread::sleep_for(time_to_wait_before_read);
        m_unbounded_waiting_queue.blocking_pop();
    });

    m_unbounded_waiting_queue.blocking_push(1, timeout);
    ASSERT_GE(time_passed(), time_to_wait_before_read);
    receiver.join();
}

TEST_F(waiting_queue_test, Receive_operations_can_be_interrupted)
{
    bool receiver1_interrupted{false};
    bool receiver2_interrupted{false};

    auto receiveCall{[&] {
        try {
            m_unbounded_waiting_queue.blocking_pop();
        }
        catch (const concurrency::interrupted_exception &) {
            return true;
        }
        return false;
    }};

    std::thread receiver1{[&] { receiver1_interrupted = receiveCall(); }};
    std::thread receiver2{[&] { receiver2_interrupted = receiveCall(); }};

    std::this_thread::sleep_for(100ms);
    m_unbounded_waiting_queue.interrupt();
    receiver1.join();
    receiver2.join();

    ASSERT_TRUE(receiver1_interrupted);
    ASSERT_TRUE(receiver2_interrupted);
}

TEST_F(waiting_queue_test, Send_operations_can_be_interrupted)
{
    fill_waiting_queue();

    auto sender1_interrupted{false};
    auto sender2_interrupted{false};

    auto sendCall{[&] {
        try {
            m_bounded_waiting_queue.blocking_push(1);
        }
        catch (const concurrency::interrupted_exception &) {
            return true;
        }
        return false;
    }};

    std::thread sender1{[&] { sender1_interrupted = sendCall(); }};
    std::thread sender2{[&] { sender2_interrupted = sendCall(); }};

    std::this_thread::sleep_for(200ms);
    m_bounded_waiting_queue.interrupt();
    sender1.join();
    sender2.join();

    ASSERT_TRUE(sender1_interrupted);
    ASSERT_TRUE(sender2_interrupted);
}

TEST_F(waiting_queue_test, After_waiting_queue_is_interrupted_all_Send_and_Receive_calls_fail)
{
    m_unbounded_waiting_queue.interrupt();

    ASSERT_THROW(m_unbounded_waiting_queue.blocking_push(1), concurrency::interrupted_exception);
    ASSERT_THROW(m_unbounded_waiting_queue.blocking_pop(), concurrency::interrupted_exception);
}
