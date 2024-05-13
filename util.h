#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
// Dynamic allocation of memory
#define append(v, d)                                                           \
  do {                                                                         \
    if (v.count >= v.capacity) {                                               \
      if (v.capacity == 0)                                                     \
        v.capacity = 256;                                                      \
      else                                                                     \
        v.capacity *= 2;                                                       \
      v.items = realloc(v.items, v.capacity * sizeof(*v.items));               \
    }                                                                          \
    v.items[v.count++] = d;                                                    \
  } while (0)

// Dynamic allocation of memory
#define append_argument(v, d)                                                  \
  do {                                                                         \
    if (v.count >= v.capacity) {                                               \
      if (v.capacity == 0)                                                     \
        v.capacity = 256;                                                      \
      else                                                                     \
        v.capacity *= 2;                                                       \
      v.items = realloc(v.items, v.capacity * sizeof(**v.items));              \
    }                                                                          \
    v.items[v.count++] = d;                                                    \
  } while (0)

#endif // !UTIL_H
