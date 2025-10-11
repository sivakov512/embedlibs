#pragma once

#include <embedlibs/ringbuf.h>

#define BUFFER_SIZE 8 // Power of 2 NOLINT

extern uint8_t g_test_buffer[BUFFER_SIZE * sizeof(uint32_t)];
extern ringbuf_t rb; // NOLINT

extern uint32_t g_sample[BUFFER_SIZE];

void run_basic_tests(void);
void run_iterator_tests(void);
void run_commit_tests(void);
