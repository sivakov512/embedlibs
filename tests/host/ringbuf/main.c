// NOLINTBEGIN(readability-magic-numbers)
#include "ringbuf_tests.h"
#include <embedlibs/ringbuf.h>
#include <string.h>
#include <unity.h>

uint8_t g_test_buffer[BUFFER_SIZE * sizeof(uint32_t)];
ringbuf_t rb; // NOLINT

uint32_t g_sample[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};

void setUp() {
  memset(g_test_buffer, 0, sizeof(g_test_buffer));
  ringbuf_init(&rb, g_test_buffer, BUFFER_SIZE, sizeof(uint32_t));
}
void tearDown() {}

int main() {
  UNITY_BEGIN();

  run_basic_tests();
  run_iterator_tests();
  run_commit_tests();

  return UNITY_END();
}
// NOLINTEND(readability-magic-numbers)
