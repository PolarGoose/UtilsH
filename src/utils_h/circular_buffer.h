#pragma once
#include <stdexcept>
#include <vector>

namespace utils_h {

struct circular_buffer_is_empty_exception final : public std::runtime_error {
    explicit circular_buffer_is_empty_exception(char const *const msg) : std::runtime_error(msg)
    {
    }
};

template <typename T> class circular_buffer {
  public:
    explicit circular_buffer(const size_t bufferSize) : _buffer(bufferSize)
    {
    }

    void put(T item)
    {
        _buffer[_head] = std::move(item);

        if (_isFull) {
            _tail = (_tail + 1) % capacity();
        }

        _head = (_head + 1) % capacity();
        _isFull = _head == _tail;
    }

    T get()
    {
        if (is_empty()) {
            throw circular_buffer_is_empty_exception("circular_buffer is empty");
        }

        auto val{_buffer[_tail]};
        _isFull = false;
        _tail = (_tail + 1) % capacity();

        return val;
    }

    [[nodiscard]] bool is_empty() const
    {
        return !_isFull && _head == _tail;
    }

    [[nodiscard]] bool is_full() const
    {
        return _isFull;
    }

    [[nodiscard]] size_t capacity() const
    {
        return _buffer.size();
    }

    [[nodiscard]] size_t size() const
    {
        if (_isFull) {
            return capacity();
        }

        if (_head >= _tail) {
            return _head - _tail;
        }

        return capacity() + _head - _tail;
    }

  private:
    size_t _head{0};
    size_t _tail{0};
    bool _isFull{false};
    std::vector<T> _buffer;
};

}
