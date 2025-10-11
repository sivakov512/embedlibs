// NOLINTBEGIN(readability-magic-numbers)
#include "ringbuf_tests.h"
#include <embedlibs/ringbuf.h>
#include <stdint.h>
#include <unity.h>

// Commit count tests
static void test_commit_count_does_nothing_when_empty() {
  ringbuf_commit_count(&rb, 5);

  TEST_ASSERT_EQUAL(0, ringbuf_available(&rb));
}

static void test_commit_count_reduces_available_elements() {
  ringbuf_write(&rb, g_sample, 5);

  ringbuf_commit_count(&rb, 2);

  TEST_ASSERT_EQUAL(3, ringbuf_available(&rb));
}

static void test_commit_count_empties_buffer_completely() {
  ringbuf_write(&rb, g_sample, 3);

  ringbuf_commit_count(&rb, 3);

  TEST_ASSERT_EQUAL(0, ringbuf_available(&rb));
}

static void test_commit_count_clamps_to_available() {
  ringbuf_write(&rb, g_sample, 3);

  ringbuf_commit_count(&rb, 10);

  TEST_ASSERT_EQUAL(0, ringbuf_available(&rb));
}

// Commit to pointer tests
static void test_commit_to_ptr_succeeds_for_valid_pointer() {
  ringbuf_write(&rb, g_sample, 3);

  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *first_ptr;
  ringbuf_iter_next_ptr(&iter, &first_ptr);

  bool success = ringbuf_commit_to_ptr(&rb, first_ptr);

  TEST_ASSERT_TRUE(success);
}

static void test_commit_to_ptr_removes_multiple_elements() {
  ringbuf_write(&rb, g_sample, 5);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *first_ptr;
  ringbuf_iter_next_ptr(&iter, &first_ptr);
  const void *second_ptr;
  ringbuf_iter_next_ptr(&iter, &second_ptr);
  const void *third_ptr;
  ringbuf_iter_next_ptr(&iter, &third_ptr);

  bool success = ringbuf_commit_to_ptr(&rb, third_ptr);

  TEST_ASSERT_EQUAL(2, ringbuf_available(&rb));
}

static void test_commit_to_ptr_fails_for_invalid_pointer() {
  ringbuf_write(&rb, g_sample, 3);
  uint32_t external_data = 999;

  bool success = ringbuf_commit_to_ptr(&rb, &external_data);

  TEST_ASSERT_FALSE(success);
}

static void test_commit_works_after_overwrite_scenario() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  uint32_t data[] = {10, 11, 12};
  ringbuf_write(&rb, data, 3);

  ringbuf_commit_count(&rb, 4);

  TEST_ASSERT_EQUAL(4, ringbuf_available(&rb));
}

static void test_commit_works_with_wraparound_data() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  ringbuf_commit_count(&rb, 4);
  uint32_t data[] = {9, 10, 11, 12};
  ringbuf_write(&rb, data, 4);

  ringbuf_commit_count(&rb, 2);

  TEST_ASSERT_EQUAL(6, ringbuf_available(&rb));
}

static void test_commit_advances_to_correct_data() {
  ringbuf_write(&rb, g_sample, 5);

  ringbuf_commit_count(&rb, 2);

  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;
  ringbuf_iter_next_ptr(&iter, &ptr);
  TEST_ASSERT_EQUAL(3, *(const uint32_t *) ptr);
}

void run_commit_tests(void) {
  // Commit count operations
  RUN_TEST(test_commit_count_does_nothing_when_empty);
  RUN_TEST(test_commit_count_reduces_available_elements);
  RUN_TEST(test_commit_count_empties_buffer_completely);
  RUN_TEST(test_commit_count_clamps_to_available);

  // Commit to pointer operations
  RUN_TEST(test_commit_to_ptr_succeeds_for_valid_pointer);
  RUN_TEST(test_commit_to_ptr_removes_multiple_elements);
  RUN_TEST(test_commit_to_ptr_fails_for_invalid_pointer);

  // Data verification
  RUN_TEST(test_commit_advances_to_correct_data);

  // Complex scenarios
  RUN_TEST(test_commit_works_after_overwrite_scenario);
  RUN_TEST(test_commit_works_with_wraparound_data);
}
// NOLINTEND(readability-magic-numbers)
