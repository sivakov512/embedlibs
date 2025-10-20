#include <embedlibs/sugar.h>
#include <stddef.h>
#include <string.h>

const void *memfind(const void *haystack, size_t hlen, const void *needle, size_t nlen) {
  if (nlen == 0 || hlen < nlen) {
    return NULL;
  }

  const unsigned char *haystack_ptr = (const unsigned char *) haystack;
  const unsigned char *needle_ptr = (const unsigned char *) needle;

  for (size_t i = 0; i <= hlen - nlen; i++) {
    if (memcmp(haystack_ptr + i, needle_ptr, nlen) == 0) {
      return haystack_ptr + i;
    }
  }

  return NULL;
}
