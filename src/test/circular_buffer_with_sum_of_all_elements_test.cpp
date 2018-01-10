#include "utils_h/circular_buffer_with_sum_of_all_elements.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace testing;

class circular_buffer_with_sum_of_all_elements_test : public Test {
  protected:
    circular_buffer_with_sum_of_all_elements m_buffer{100};
};

TEST_F(circular_buffer_with_sum_of_all_elements_test, Empty_buffer_has_sum_equal_to_zero)
{
    ASSERT_EQ(m_buffer.get_sum_of_all_elements(), 0);
}

TEST_F(circular_buffer_with_sum_of_all_elements_test, When_adding_elements_The_sum_is_increased)
{
    m_buffer.put(10);
    m_buffer.put(10);
    ASSERT_EQ(m_buffer.get_sum_of_all_elements(), 20);
}

TEST_F(circular_buffer_with_sum_of_all_elements_test, When_removing_elements_the_sum_is_decreased)
{
    m_buffer.put(10);
    m_buffer.put(10);

    m_buffer.get();

    ASSERT_EQ(m_buffer.get_sum_of_all_elements(), 10);
}
