#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// A sample function to be tested
int add(int a, int b)
{
  return a + b;
}

// Test case for the add function
void test_add(void **state)
{
  (void)state; // unused
  assert_int_equal(add(2, 3), 5);
  assert_int_equal(add(-1, -1), -2);
  assert_int_equal(add(0, 0), 0);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_add),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
