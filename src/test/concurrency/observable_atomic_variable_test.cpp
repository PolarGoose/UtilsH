#include "utils/time_passed_method.h"
#include "utils_h/concurrency/observable_atomic_variable.h"
#include <future>
#include <gtest/gtest.h>

using namespace utils_h;
using namespace testing;

class observable_atomic_variable_test : public Test, protected time_passed_method {
  protected:
    void on_value_changed(const size_t &value)
    {
        m_historyOfEvents.push_back(value);
    }

    const size_t _initialValue{1};
    concurrency::observable_atomic_variable<size_t> m_observableAtomicVariable{
        _initialValue, [&](const auto &val) { on_value_changed(val); }};
    std::vector<size_t> m_historyOfEvents;
};

TEST_F(observable_atomic_variable_test, Initialized_correctly)
{
    ASSERT_EQ(m_observableAtomicVariable.get(), _initialValue);
}

TEST_F(observable_atomic_variable_test, When_value_is_set_It_is_saved_and_a_client_is_notified_with_an_event)
{
    const size_t numberOfElementsToSet{100};

    for (size_t i = 0; i < numberOfElementsToSet; i++) {
        m_observableAtomicVariable.set(i);
        ASSERT_EQ(m_observableAtomicVariable.get(), i);
    }

    ASSERT_EQ(m_historyOfEvents.size(), numberOfElementsToSet);
    for (size_t i = 0; i < numberOfElementsToSet; i++) {
        ASSERT_EQ(m_historyOfEvents[i], i);
    }
}

TEST_F(observable_atomic_variable_test, Setting_the_same_value_doesnt_cause_an_event_to_be_sent)
{
    m_observableAtomicVariable.set(_initialValue);
    ASSERT_EQ(m_historyOfEvents.size(), 0);
}

TEST_F(observable_atomic_variable_test, Several_clients_can_wait_for_a_specific_value)
{
    const std::chrono::seconds timeout{1};
    const size_t valueToWaitFor{_initialValue + 1};

    const auto client1{
        std::async(std::launch::async, [&] { m_observableAtomicVariable.wait_for_value(valueToWaitFor, timeout); })};

    const auto client2{
        std::async(std::launch::async, [&] { m_observableAtomicVariable.wait_for_value(valueToWaitFor, timeout); })};

    m_observableAtomicVariable.set(valueToWaitFor);
    ASSERT_NE(client1.wait_for(timeout), std::future_status::timeout);
    ASSERT_NE(client2.wait_for(timeout), std::future_status::timeout);
}

TEST_F(observable_atomic_variable_test, Client_gets_an_exception_if_waiting_for_a_value_is_timed_out)
{
    const std::chrono::milliseconds timeout{300};
    const size_t valueToWaitFor{_initialValue + 1};

    ASSERT_THROW(m_observableAtomicVariable.wait_for_value(valueToWaitFor, timeout), concurrency::timed_out_exception);
    ASSERT_GE(time_passed(), timeout);
}

TEST_F(observable_atomic_variable_test, Wait_method_can_be_interrupted)
{
    const std::chrono::seconds timeout{2};

    bool client1IsInterrupted{false};
    bool client2IsInterrupted{false};

    auto waitCall{[&] {
        try {
            m_observableAtomicVariable.wait_for_value(_initialValue + 1, timeout);
        }
        catch (const concurrency::interrupted_exception &) {
            return true;
        }
        return false;
    }};

    std::thread client1{[&] { client1IsInterrupted = waitCall(); }};
    std::thread client2{[&] { client2IsInterrupted = waitCall(); }};

    std::this_thread::sleep_for(std::chrono::milliseconds{200});
    m_observableAtomicVariable.interrupt();
    client1.join();
    client2.join();

    ASSERT_TRUE(client1IsInterrupted);
    ASSERT_TRUE(client2IsInterrupted);
}

TEST_F(observable_atomic_variable_test, After_interrupted_All_Wait_calls_fail)
{
    m_observableAtomicVariable.interrupt();

    ASSERT_THROW(m_observableAtomicVariable.wait_for_value(_initialValue), concurrency::interrupted_exception);
    ASSERT_THROW(m_observableAtomicVariable.wait_for_value(_initialValue), concurrency::interrupted_exception);
}
