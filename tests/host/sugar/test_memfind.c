#include <embedlibs/sugar.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unity.h>

#define WRAP(...) __VA_ARGS__

void setUp() {}
void tearDown() {}

static void test_returns_expected_pointer_if_found(
    const void *haystack, size_t hlen, const void *needle,
    size_t nlen, // NOLINT(bugprone-easily-swappable-parameters)
    int expected_pos, size_t el_size) {
  const void *ptr = memfind(haystack, hlen, needle, nlen);

  TEST_ASSERT_NOT_NULL(ptr);
  size_t pos = ((const char *) ptr - (const char *) haystack) / el_size;
  TEST_ASSERT_EQUAL(expected_pos, pos);
}
#define SUCCESS_CASES(X)                                                                           \
  X(uint8_t, WRAP({1, 2, 3, 4, 5}), WRAP({3, 4}), 2)                                               \
  X(uint16_t, WRAP({1, 2, 3, 4, 5}), WRAP({4}), 3)                                                 \
  X(char, "hellohello", WRAP({'e', 'l', 'l'}), 1)                                                  \
  X(int, WRAP({10, 20, 30}), WRAP({20, 30}), 1)                                                    \
  X(bool, WRAP({true, false, true, true, false}), WRAP({true, true}), 2)

#define DEF_TEST(type, hay, nee, expected)                                                         \
  static void test_returns_expected_pointer_if_found__##type() {                                   \
    type haystack[] = hay;                                                                         \
    type needle[] = nee;                                                                           \
    test_returns_expected_pointer_if_found(haystack, sizeof(haystack), needle, sizeof(needle),     \
                                           expected, sizeof(type));                                \
  }
SUCCESS_CASES(DEF_TEST)
#undef DEF_TEST

static void test_returns_null_if_not_found(const void *haystack, size_t hlen, const void *needle,
                                           size_t nlen) {
  const void *ptr = memfind(haystack, hlen, needle, nlen);

  TEST_ASSERT_NULL(ptr);
}
#define UNSUCCESS_CASES(X)                                                                         \
  X(uint8_t, WRAP({1, 2, 3, 4, 5}), WRAP({2, 4}))                                                  \
  X(char, "hellohello", WRAP({'e', 'h', 'o'}))                                                     \
  X(int, WRAP({10, 20, 30}), WRAP({30, 40}))                                                       \
  X(bool, WRAP({true, false, true, true, false}), WRAP({true, true, true}))

#define DEF_TEST(type, hay, nee)                                                                   \
  static void test_returns_null_if_not_found__##type() {                                           \
    type haystack[] = hay;                                                                         \
    type needle[] = nee;                                                                           \
    test_returns_null_if_not_found(haystack, sizeof(haystack), needle, sizeof(needle));            \
  }
UNSUCCESS_CASES(DEF_TEST)
#undef DEF_TEST

int main() {
  UNITY_BEGIN();

#define RUN(type, hay, nee, expected) RUN_TEST(test_returns_expected_pointer_if_found__##type);
  SUCCESS_CASES(RUN)
#undef RUN

#define RUN(type, hay, nee) RUN_TEST(test_returns_null_if_not_found__##type);
  UNSUCCESS_CASES(RUN)
#undef RUN

  return UNITY_END();
}
