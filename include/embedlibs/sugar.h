#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }

#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END

#endif

EXTERN_C_BEGIN

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif /* MAX */

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif /* ARRAY_SIZE */

/**
 * @brief Find a subarray within a larger array of bytes
 *
 * Searches for the first occurrence of the needle subarray within the haystack array.
 *
 * @param haystack Pointer to the array to search within
 * @param hlen Length of the haystack array in bytes
 * @param needle Pointer to the subarray to search for
 * @param nlen Length of the needle subarray in bytes
 *
 * @return Pointer to the first occurrence of needle in haystack, or NULL if not found
 */
const void *memfind(const void *haystack, size_t hlen, const void *needle, size_t nlen);

EXTERN_C_END
