#pragma once
#include "utils_h/non_copyable_and_non_movable.h"
#include <boost/process.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <thread>

class socat_loopback_serial_port final : utils_h::non_copyable_and_non_movable {
  public:
    socat_loopback_serial_port()
    {
        // give socat some time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    const std::string &port_name()
    {
        return _port_name;
    }

    ~socat_loopback_serial_port()
    {
        try {
            _socat_process.terminate();
            std::remove(_port_name.c_str());
        }
        catch (const std::exception &ex) {
            // ignore
        }
    }

  private:
    std::string _port_name{std::string("/tmp/socat_serial_port_") +
                           boost::uuids::to_string(boost::uuids::random_generator()())};
    // socat pty,rawer,link=/tmp/serial_port_unique_name SYSTEM:/bin/cat
    boost::process::child _socat_process{
        boost::process::search_path("socat"), std::string("pty,rawer,link='") + _port_name + "'", "SYSTEM:/bin/cat",
        boost::process::std_out > boost::process::null, boost::process::std_err > boost::process::null};
};
