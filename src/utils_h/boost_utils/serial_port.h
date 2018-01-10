#pragma once
#include "utils_h/non_copyable_and_non_movable.h"
#include <boost/asio.hpp>
#include <queue>
#include <thread>
#include <vector>

namespace utils_h { namespace boost_utils {

class serial_port_events_handler {
  public:
    virtual void on_bytes_received(const char *data, size_t size) = 0;
    virtual void on_receive_error(const boost::system::error_code &ec) = 0;
    virtual void on_send_error(const boost::system::error_code &ec) = 0;

    virtual ~serial_port_events_handler() = default;
};

class serial_port final : non_copyable_and_non_movable {
  public:
    explicit serial_port(const std::string &serial_port_name, serial_port_events_handler &event_handler)
        : _event_handler{event_handler}, _serial_port{_ioService, serial_port_name}
    {
        using namespace boost::asio;
        _serial_port.set_option(serial_port_base::baud_rate{4 * 1000 * 1000});
        _serial_port.set_option(serial_port_base::flow_control{serial_port_base::flow_control::none});
        _serial_port.set_option(serial_port_base::parity{serial_port_base::parity::none});
        _serial_port.set_option(serial_port_base::stop_bits{serial_port_base::stop_bits::one});
        _serial_port.set_option(serial_port_base::character_size{8});
    }

    void start_receiving()
    {
        _ioService.post([this] { do_recv(); });
    }

    void async_send(const char *data, const size_t size)
    {
        _ioService.post([this, bytes_to_send = std::vector<char>(data, data + size)] {
            const auto write_in_progress = !_send_queue.empty();
            _send_queue.emplace(bytes_to_send);
            if (!write_in_progress) {
                do_send();
            }
        });
    }

    ~serial_port()
    {
        _ioService.post([this] {
            _serial_port.close();
            _work.reset();
        });
        _io_service_thread.join();
    }

  private:
    void do_recv()
    {
        _serial_port.async_read_some(boost::asio::buffer(_receive_buffer),
                                     [this](boost::system::error_code ec, std::size_t size) {
                                         if (ec) {
                                             _event_handler.on_receive_error(ec);
                                             return;
                                         }

                                         _event_handler.on_bytes_received(_receive_buffer.data(), size);
                                         do_recv();
                                     });
    }

    void do_send()
    {
        boost::asio::async_write(_serial_port, boost::asio::buffer(_send_queue.front()),
                                 [this](boost::system::error_code ec, std::size_t /*size*/) {
                                     if (ec) {
                                         _event_handler.on_send_error(ec);
                                         return;
                                     }

                                     _send_queue.pop();
                                     if (!_send_queue.empty()) {
                                         do_send();
                                     }
                                 });
    }

    serial_port_events_handler &_event_handler;
    boost::asio::io_service _ioService;
    std::optional<boost::asio::io_service::work> _work{_ioService};
    boost::asio::serial_port _serial_port;
    std::queue<std::vector<char>> _send_queue;
    std::array<char, 8192> _receive_buffer{};
    std::thread _io_service_thread{[&] { _ioService.run(); }};
};

}} // namespace Montana
