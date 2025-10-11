// NOLINTBEGIN(readability-magic-numbers)
#include "ringbuf_tests.h"
#include <embedlibs/ringbuf.h>
#include <stdint.h>
#include <unity.h>

// Iterator initialization tests
static void test_iterator_shows_no_remaining_when_empty() {
  ringbuf_iter_t iter;

  ringbuf_iter_begin(&rb, &iter);

  TEST_ASSERT_EQUAL(0, ringbuf_iter_remaining(&iter));
}

static void test_iterator_counts_available_elements() {
  ringbuf_write(&rb, g_sample, 3);
  ringbuf_iter_t iter;

  ringbuf_iter_begin(&rb, &iter);

  TEST_ASSERT_EQUAL(3, ringbuf_iter_remaining(&iter));
}

// Single element navigation
static void test_next_ptr_returns_null_when_empty() {
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;

  ringbuf_iter_next_ptr(&iter, &ptr);

  TEST_ASSERT_NULL(ptr);
}

static void test_next_ptr_returns_correct_element() {
  uint32_t data = 42;
  ringbuf_push(&rb, &data);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;

  ringbuf_iter_next_ptr(&iter, &ptr);

  TEST_ASSERT_EQUAL(42, *(const uint32_t *) ptr);
}

static void test_next_ptr_returns_first_element() {
  ringbuf_write(&rb, g_sample, 3);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;

  ringbuf_iter_next_ptr(&iter, &ptr);

  TEST_ASSERT_EQUAL(1, *(const uint32_t *) ptr);
}

static void test_next_ptr_advances_to_second_element() {
  ringbuf_write(&rb, g_sample, 3);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;

  ringbuf_iter_next_ptr(&iter, &ptr); // Skip first
  ringbuf_iter_next_ptr(&iter, &ptr); // Get second

  TEST_ASSERT_EQUAL(2, *(const uint32_t *) ptr);
}

// Cursor operations
static void test_cursor_ptr_returns_null_when_empty() {
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);

  const void *cursor = ringbuf_iter_cursor_ptr(&iter);

  TEST_ASSERT_NULL(cursor);
}

static void test_cursor_ptr_points_to_current_element() {
  ringbuf_write(&rb, g_sample, 3);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);

  const void *cursor = ringbuf_iter_cursor_ptr(&iter);

  TEST_ASSERT_EQUAL(1, *(const uint32_t *) cursor);
}

// Advance operations
static void test_advance_count_skips_elements() {
  ringbuf_write(&rb, g_sample, 5);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);

  ringbuf_iter_advance_count(&iter, 2);

  TEST_ASSERT_EQUAL(3, ringbuf_iter_remaining(&iter));
}

static void test_advance_count_clamps_to_available() {
  ringbuf_write(&rb, g_sample, 2);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);

  ringbuf_iter_advance_count(&iter, 10);

  TEST_ASSERT_EQUAL(0, ringbuf_iter_remaining(&iter));
}

static void test_advance_to_ptr_succeeds_for_valid_pointer() {
  ringbuf_write(&rb, g_sample, 4);

  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *first_ptr;
  ringbuf_iter_next_ptr(&iter, &first_ptr);
  ringbuf_iter_begin(&rb, &iter);

  bool success = ringbuf_iter_advance_to_ptr(&iter, first_ptr);

  TEST_ASSERT_TRUE(success);
}

// Span operations
static void test_next_span_returns_null_when_empty() {
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;
  size_t len;

  ringbuf_iter_next_span(&iter, &ptr, &len);

  TEST_ASSERT_NULL(ptr);
}

static void test_next_span_returns_contiguous_elements() {
  ringbuf_write(&rb, g_sample, 3);

  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;
  size_t len;

  ringbuf_iter_next_span(&iter, &ptr, &len);

  TEST_ASSERT_EQUAL(3, len);
}

// Snapshot isolation test
static void test_iterator_reads_correct_data_after_write() {
  ringbuf_write(&rb, g_sample, 3);

  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  const void *ptr;

  ringbuf_iter_next_ptr(&iter, &ptr);

  TEST_ASSERT_EQUAL(1, *(const uint32_t *) ptr);
}

static void test_iterator_snapshot_isolates_from_new_writes() {
  ringbuf_write(&rb, g_sample, 3);
  ringbuf_iter_t iter;
  ringbuf_iter_begin(&rb, &iter);
  uint32_t new_data = 4;

  ringbuf_push(&rb, &new_data);

  TEST_ASSERT_EQUAL(3, ringbuf_iter_remaining(&iter));
}

void run_iterator_tests(void) {
  // Iterator initialization
  RUN_TEST(test_iterator_shows_no_remaining_when_empty);
  RUN_TEST(test_iterator_counts_available_elements);

  // Single element navigation
  RUN_TEST(test_next_ptr_returns_null_when_empty);
  RUN_TEST(test_next_ptr_returns_correct_element);
  RUN_TEST(test_next_ptr_returns_first_element);
  RUN_TEST(test_next_ptr_advances_to_second_element);

  // Cursor operations
  RUN_TEST(test_cursor_ptr_returns_null_when_empty);
  RUN_TEST(test_cursor_ptr_points_to_current_element);

  // Advance operations
  RUN_TEST(test_advance_count_skips_elements);
  RUN_TEST(test_advance_count_clamps_to_available);
  RUN_TEST(test_advance_to_ptr_succeeds_for_valid_pointer);

  // Span operations
  RUN_TEST(test_next_span_returns_null_when_empty);
  RUN_TEST(test_next_span_returns_contiguous_elements);

  // Data verification
  RUN_TEST(test_iterator_reads_correct_data_after_write);

  // Advanced features
  RUN_TEST(test_iterator_snapshot_isolates_from_new_writes);
}
// NOLINTEND(readability-magic-numbers)
