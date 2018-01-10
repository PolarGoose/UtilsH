#include "utils_h/circular_buffer.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace testing;

class circular_buffer_test : public Test {
  protected:
    void fill_buffer()
    {
        for (size_t i{0}; i < m_capacity; i++) {
            m_buffer.put(i);
        }
    }

    const size_t m_capacity{100};
    circular_buffer<size_t> m_buffer{m_capacity};
};

TEST_F(circular_buffer_test, Capacity_returns_correct_value)
{
    ASSERT_EQ(m_buffer.capacity(), m_capacity);
}

TEST_F(circular_buffer_test, Get_method_throws_an_exception_when_the_buffer_is_empty)
{
    ASSERT_TRUE(m_buffer.is_empty());
    ASSERT_THROW(m_buffer.get(), circular_buffer_is_empty_exception);
}

TEST_F(circular_buffer_test, Buffer_can_containt_the_amount_of_elements_equal_to_its_capacity)
{
    fill_buffer();

    ASSERT_EQ(m_buffer.size(), m_capacity);
    ASSERT_TRUE(m_buffer.is_full());

    for (size_t i{0}; i < m_capacity; i++) {
        ASSERT_EQ(m_buffer.get(), i);
    }

    ASSERT_EQ(m_buffer.size(), 0);
    ASSERT_TRUE(m_buffer.is_empty());
}

TEST_F(circular_buffer_test, Adding_one_element_to_full_buffer_removes_its_last_added_element)
{
    fill_buffer();

    m_buffer.put(m_capacity);

    for (size_t i{0}; i < m_capacity; i++) {
        ASSERT_EQ(m_buffer.get(), i + 1);
    }
}
