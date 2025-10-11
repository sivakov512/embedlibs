// NOLINTBEGIN(readability-magic-numbers)
#include "ringbuf_tests.h"
#include <embedlibs/ringbuf.h>
#include <stdint.h>
#include <unity.h>

// Initialization tests
static void test_initializes_with_correct_parameters() {
  ringbuf_init(&rb, g_test_buffer, 4, sizeof(int));

  TEST_ASSERT_EQUAL(4, rb.cap);
  TEST_ASSERT_EQUAL(sizeof(int), rb.elem_size);
  TEST_ASSERT_EQUAL(0, rb.head);
  TEST_ASSERT_EQUAL(0, rb.tail);
  TEST_ASSERT_EQUAL_PTR(g_test_buffer, rb.buf);
}

static void test_no_slots_to_read_by_default() {
  TEST_ASSERT_EQUAL(0, ringbuf_available(&rb));
}

static void test_all_slots_free_by_default() {
  TEST_ASSERT_EQUAL(BUFFER_SIZE, ringbuf_free(&rb));
}

static void test_reset_clears_slots() {
  ringbuf_push(&rb, g_sample);

  ringbuf_reset(&rb);

  TEST_ASSERT_EQUAL(0, ringbuf_available(&rb));
  TEST_ASSERT_EQUAL(BUFFER_SIZE, ringbuf_free(&rb));
}

static void test_reset_doesnt_clear_actual_data() {
  ringbuf_push(&rb, g_sample);

  ringbuf_reset(&rb);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {1};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

// Write operations
static void test_push_single_element_increases_available() {
  ringbuf_push(&rb, g_sample);

  TEST_ASSERT_EQUAL(1, ringbuf_available(&rb));
}

static void test_push_single_element_stores_correct_data() {
  ringbuf_push(&rb, g_sample);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {1};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

static void test_write_multiple_elements_updates_counters() {
  size_t written = ringbuf_write(&rb, g_sample, 3);

  TEST_ASSERT_EQUAL(3, written);
  TEST_ASSERT_EQUAL(3, ringbuf_available(&rb));
}

static void test_write_multiple_elements_stores_in_order() {
  ringbuf_write(&rb, g_sample, 3);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {1, 2, 3};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

static void test_fill_to_capacity_leaves_no_free_slots() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);

  TEST_ASSERT_EQUAL(BUFFER_SIZE, ringbuf_available(&rb));
  TEST_ASSERT_EQUAL(0, ringbuf_free(&rb));
}

static void test_fill_to_capacity_stores_all_elements() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  TEST_ASSERT_EQUAL_UINT32_ARRAY(g_sample, buffer_data, BUFFER_SIZE);
}

// Overwrite behavior tests
static void test_overwrite_oldest_when_exceeding_capacity() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  size_t old_tail = rb.tail;
  uint32_t new_data = 99;

  ringbuf_push(&rb, &new_data);

  TEST_ASSERT_EQUAL(BUFFER_SIZE, ringbuf_available(&rb));
  TEST_ASSERT_EQUAL(old_tail + 1, rb.tail);
}

static void test_overwrite_replaces_oldest_data() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  uint32_t new_data = 99;

  ringbuf_push(&rb, &new_data);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {new_data, 2, 3, 4, 5, 6, 7, 8};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

static void test_overwrite_multiple_oldest_elements() {
  uint32_t overflow[] = {10, 11, 12, 13, 14, 15};

  ringbuf_write(&rb, g_sample, 4);
  ringbuf_write(&rb, overflow, 6);

  TEST_ASSERT_EQUAL(BUFFER_SIZE, ringbuf_available(&rb));
  TEST_ASSERT_EQUAL(2, rb.tail);
}

static void test_overwrite_multiple_preserves_newer_data() {
  uint32_t overflow[] = {10, 11, 12, 13, 14, 15};

  ringbuf_write(&rb, g_sample, 4);
  ringbuf_write(&rb, overflow, 6);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {14, 15, 3, 4, 10, 11, 12, 13};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

static void test_write_wraps_around_physical_buffer() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  ringbuf_commit_count(&rb, 4);
  uint32_t data[] = {9, 10, 11, 12};

  ringbuf_write(&rb, data, 4);

  TEST_ASSERT_EQUAL(8, ringbuf_available(&rb));
}

static void test_wraparound_data_stored_correctly() {
  ringbuf_write(&rb, g_sample, BUFFER_SIZE);
  ringbuf_commit_count(&rb, 4);
  uint32_t data[] = {9, 10, 11, 12};

  ringbuf_write(&rb, data, 4);

  uint32_t *buffer_data = (uint32_t *) g_test_buffer;
  uint32_t expected[BUFFER_SIZE] = {9, 10, 11, 12, 5, 6, 7, 8};
  TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, buffer_data, BUFFER_SIZE);
}

void run_basic_tests(void) {
  // Initialization
  RUN_TEST(test_initializes_with_correct_parameters);
  RUN_TEST(test_no_slots_to_read_by_default);
  RUN_TEST(test_all_slots_free_by_default);
  RUN_TEST(test_reset_clears_slots);
  RUN_TEST(test_reset_doesnt_clear_actual_data);

  // Write operations
  RUN_TEST(test_push_single_element_increases_available);
  RUN_TEST(test_push_single_element_stores_correct_data);
  RUN_TEST(test_write_multiple_elements_updates_counters);
  RUN_TEST(test_write_multiple_elements_stores_in_order);
  RUN_TEST(test_fill_to_capacity_leaves_no_free_slots);
  RUN_TEST(test_fill_to_capacity_stores_all_elements);

  // Overwrite behavior
  RUN_TEST(test_overwrite_oldest_when_exceeding_capacity);
  RUN_TEST(test_overwrite_replaces_oldest_data);
  RUN_TEST(test_overwrite_multiple_oldest_elements);
  RUN_TEST(test_overwrite_multiple_preserves_newer_data);
  RUN_TEST(test_write_wraps_around_physical_buffer);
  RUN_TEST(test_wraparound_data_stored_correctly);
}
// NOLINTEND(readability-magic-numbers)
