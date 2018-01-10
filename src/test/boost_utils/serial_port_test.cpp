#include "utils/socat_loopback_serial_port.h"
#include "utils_h/boost_utils/serial_port.h"
#include "utils_h/concurrency/waiting_queue.h"
#include <boost/range/irange.hpp>
#include <gtest/gtest.h>

using namespace utils_h;
using namespace concurrency;
using namespace boost_utils;
using namespace testing;
using namespace std::chrono_literals;

static auto generate_message(size_t size)
{
    std::vector<char> message;
    message.reserve(size);
    for (const auto i : boost::irange(static_cast<size_t>(0), size)) {
        message.emplace_back(static_cast<char>(i));
    }
    return message;
}

class serial_port_test : public Test, public serial_port_events_handler {
    void on_bytes_received(const char *data, const size_t size) override
    {
        _receive_queue.blocking_push({data, data + size});
    }

    void on_receive_error(const boost::system::error_code & /*ec*/) override
    {
    }

    void on_send_error(const boost::system::error_code & /*ec*/) override
    {
    }

  protected:
    serial_port_test()
    {
        _serial_port.start_receiving();
    }

    auto wait_for_message()
    {
        return _receive_queue.blocking_pop(3s);
    }

    std::optional<socat_loopback_serial_port> _loopback_port{std::make_optional<socat_loopback_serial_port>()};
    waiting_queue<std::vector<char>> _receive_queue;
    serial_port _serial_port{_loopback_port->port_name(), *this};
};

TEST_F(serial_port_test, Throws_exception_if_port_doesnt_exist)
{
    auto constructor = [&] { serial_port{"non_existing_port_name", *this}; };
    ASSERT_THROW(constructor(), boost::system::system_error);
}

TEST_F(serial_port_test, Message_can_be_sent_and_received)
{
    const auto message = generate_message(100);

    _serial_port.async_send(message.data(), message.size());
    const auto received_message = wait_for_message();

    ASSERT_EQ(message.size(), received_message.size());
    ASSERT_EQ(message, received_message);
    ASSERT_EQ(_receive_queue.size(), 0);
}

TEST_F(serial_port_test, Works_if_message_is_being_sent_while_port_is_destructing)
{
    const auto message = generate_message(10 * 1000 * 1000);
    _serial_port.async_send(message.data(), message.size());
}

TEST_F(serial_port_test, Stress_test)
{
    const auto message = generate_message(10 * 1000 * 1000);
    std::vector<char> received_message;

    for (size_t i{0}; i < message.size(); i += 1000) {
        _serial_port.async_send(message.data() + i, 1000);
    }

    while (received_message.size() != message.size()) {
        const auto m = wait_for_message();
        received_message.insert(std::end(received_message), std::begin(m), std::end(m));
    }

    ASSERT_EQ(message.size(), received_message.size());
    ASSERT_EQ(message, received_message);
}
