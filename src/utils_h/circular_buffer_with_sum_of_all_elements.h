#pragma once
#include "utils_h/circular_buffer.h"

namespace utils_h {

class circular_buffer_with_sum_of_all_elements final {
  public:
    explicit circular_buffer_with_sum_of_all_elements(const size_t bufferSize) : m_buffer{bufferSize}
    {
    }

    void put(const int item)
    {
        m_sumOfAllElements += item;
        m_buffer.put(item);
    }

    int get()
    {
        const auto item{m_buffer.get()};
        m_sumOfAllElements -= item;
        return item;
    }

    [[nodiscard]] int get_sum_of_all_elements() const
    {
        return m_sumOfAllElements;
    }

    [[nodiscard]] bool is_empty() const
    {
        return m_buffer.is_empty();
    }

    [[nodiscard]] bool is_full() const
    {
        return m_buffer.is_full();
    }

    [[nodiscard]] size_t capacity() const
    {
        return m_buffer.capacity();
    }

    [[nodiscard]] size_t size() const
    {
        return m_buffer.size();
    }

  private:
    circular_buffer<int> m_buffer;
    int m_sumOfAllElements{0};
};

}
