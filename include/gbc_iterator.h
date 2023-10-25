#ifndef _GBC_ITER_H
#define _GBC_ITER_H
#include <stdbool.h>
#include <stdlib.h>

/// @brief The base struct of iterator
typedef struct _iter iter_t;

typedef struct _iter {
  size_t obj_size;
  bool (*has_next)(const iter_t *);
  void *(*next)(iter_t *iter);
} iter_t;

#endif