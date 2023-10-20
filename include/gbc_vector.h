#ifndef _GBC_VECTOR_H  // generic buffered collection
#define _GBC_VECTOR_H
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_VEC_CAP 8

/// @brief The Vector collection
typedef struct _vec {
  size_t size;
  size_t cap;
  size_t obj_size;
  char *buf;
} vec_t;

/// @brief create a new vector
/// @param obj_size the size of each element
/// @return
vec_t *vec_new(size_t obj_size);

/// @brief creata a new vector with capacity
/// @param obj_size
/// @param cap
/// @return
vec_t *vec_new_with_cap(size_t obj_size, size_t cap);

/// @brief drop the vector out of memory
/// @param vec
bool vec_drop(vec_t *);

/// @brief push one element into the vector
/// @param
/// @param
bool vec_push(vec_t *, const void *);

/// Check the top element in the vector
const void *vec_top(const vec_t *);

/// @brief deep copy a vector
/// @param
/// @return
vec_t *vec_clone(vec_t *);

/// @brief copy an array an create a new vector
/// @param _arr
/// @param array_size
/// @param obj_size
/// @return
vec_t *vec_from_array(const void *_arr, size_t array_size, size_t obj_size);

/// @brief Insert an element into the vector with an index
/// @param vec
/// @param data
/// @param idx
bool vec_insert(vec_t *vec, size_t idx, const void *data);

/// @brief delete an item at the input index
/// @param vec
/// @param idx
/// @return
bool vec_del_at(vec_t *vec, const size_t idx);

/// @brief pop out the last element on the heap memory!!
/// @param
/// @return
bool vec_del_top(vec_t *);

/// @brief delete a target value in the vector, return false if the value was
/// not in the vector
/// @param
/// @param target_value
/// @param cmp_fn
/// @return
bool vec_del(vec_t *, const void *target_value,
             int (*cmp_fn)(const void *, const void *));

/// @brief get the element in the vector at the input index
/// @param vec
/// @param idx
/// @return
const void *vec_at(const vec_t *vec, const size_t idx);

/// @brief get the mutable pointer at the input index
/// @param vec
/// @param idx
/// @return
void *vec_at_mut(vec_t *vec, size_t idx);

/// @brief update the value given an index
bool vec_update(vec_t *q, size_t idx, const void *value);

/// @brief foreach function of vector
/// @param
/// @param
void vec_foreach(const vec_t *, void (*)(const void *));

/// @brief
///         comparison function which returns ​a negative integer
///         value if the first argument is less than the second,
///         a positive integer value if the first argument is greater
///         than the second and zero if the arguments are equivalent.
/// @param vec
/// @param cmp_fn
bool vec_sort(vec_t *vec, int (*cmp_fn)(const void *, const void *));

/// @brief reverse the vec_t
/// @param vec
/// @return
bool vec_reverse(vec_t *vec);

vec_t *vec_new(size_t obj_size) {
  char *buf = (char *)malloc(obj_size * DEFAULT_VEC_CAP);
  if (!buf) return NULL;
  vec_t *v = (vec_t *)malloc(sizeof(vec_t));
  if (!v) {
    free(buf);
    buf = NULL;
    return NULL;
  }
  v->buf = buf;
  v->size = 0;
  v->obj_size = obj_size;
  v->cap = DEFAULT_VEC_CAP;
  return v;
}

vec_t *vec_new_with_cap(size_t obj_size, size_t cap) {
  char *buf = (char *)malloc(obj_size * cap);
  if (!buf) return NULL;
  vec_t *v = (vec_t *)malloc(sizeof(vec_t));
  if (!v) {
    free(buf);
    buf = NULL;
    return NULL;
  }
  v->buf = buf;
  v->size = 0;
  v->obj_size = obj_size;
  v->cap = cap;
  return v;
}

bool vec_drop(vec_t *vec) {
  if (!vec) return false;
  if (!vec->buf) {
    free(vec);
    vec = NULL;
    return true;
  } else {
    free(vec->buf);
    vec->buf = NULL;
    free(vec);
    vec = NULL;
    return true;
  }
}

static bool vec_enlarge(vec_t *vec, size_t new_cap) {
  char *new_buf = (char *)malloc(vec->obj_size * new_cap);
  if (!new_buf) {
    return false;
  }
  vec->cap = new_cap;
  memcpy(new_buf, vec->buf, (vec->size * vec->obj_size));
  free(vec->buf);
  vec->buf = new_buf;
  return true;
}

bool vec_push(vec_t *vec, const void *data) {
  assert(vec && data);
  if (vec->cap == vec->size) {
    if (!vec_enlarge(vec, vec->cap * 2)) return false;
  }
  memcpy(vec->buf + vec->obj_size * vec->size, data, vec->obj_size);
  vec->size++;
  return true;
}

bool vec_del_top(vec_t *vec) {
  // assert(vec->obj_size == len(out_buf))
  assert(vec);
  if (vec->size == 0) {
    return false;
  }
  vec->size--;
  return true;
}

vec_t *vec_clone(vec_t *vec) {
  assert(vec);
  vec_t *v = vec_new_with_cap(vec->obj_size, vec->cap);
  if (!v) return NULL;
  v->size = vec->size;
  v->cap = vec->cap;
  v->obj_size = vec->obj_size;
  memcpy(v->buf, vec->buf, vec->size * vec->obj_size);
  return v;
}

vec_t *vec_from_array(const void *_arr, size_t array_size, size_t obj_size) {
  size_t cap = (array_size % 2 == 0) ? array_size : array_size + 1;
  vec_t *v = vec_new_with_cap(obj_size, cap);
  if (!v) return NULL;
  v->size = array_size;
  memcpy(v->buf, _arr, obj_size * array_size);
  return v;
}

bool vec_insert(vec_t *vec, size_t idx, const void *_data) {
  assert(vec && _data && vec->size > idx);
  if (vec->cap == vec->size) {
    if (!vec_enlarge(vec, vec->cap * 2)) return false;
  }
  memmove(vec->buf + vec->obj_size * (idx + 1), vec->buf + vec->obj_size * idx,
          vec->size - idx);
  memcpy(vec->buf + vec->obj_size * idx, _data, vec->obj_size);
  vec->size++;
  return true;
}

bool vec_sort(vec_t *vec, int (*cmp_fn)(const void *, const void *)) {
  assert(vec);
  if (vec->size == 0) return false;
  qsort(vec->buf, vec->size, vec->obj_size, cmp_fn);
  return true;
}

bool vec_del_at(vec_t *vec, const size_t idx) {
  assert(vec && vec->size > idx);
  if (idx == vec->size - 1) {
    return vec_del_top(vec);
  }
  char *buf = vec->buf;
  memmove(buf + vec->obj_size * idx, buf + vec->obj_size * (idx + 1),
          vec->obj_size * (vec->size - idx - 1));
  vec->size--;
  return true;
}

bool vec_del(vec_t *vec, const void *target_value,
             int (*cmp_fn)(const void *, const void *)) {
  size_t idx;
  bool flag = false;
  for (int i = 0; i < vec->size; ++i) {
    const void *ele = vec_at(vec, i);
    int order = cmp_fn(ele, target_value);
    if (order == 0) {
      flag = true;
      idx = i;
      break;
    }
  }
  if (flag) {
    return vec_del_at(vec, idx);
  }
  return flag;
}

const void *vec_at(const vec_t *vec, const size_t idx) {
  assert(vec && vec->size > idx);
  char *ptr = vec->buf + vec->obj_size * idx;
  return (void *)ptr;
}

void *vec_at_mut(vec_t *vec, size_t idx) {
  assert(vec && vec->size > idx);
  char *ptr = vec->buf + vec->obj_size * idx;
  return (void *)ptr;
}

bool vec_update(vec_t *vec, size_t idx, const void *value) {
  void *ptr = vec_at_mut(vec, idx);
  memcpy(ptr, value, vec->obj_size);
  return true;
}

void vec_foreach(const vec_t *vec, void (*foreach_fn)(const void *)) {
  assert(vec);
  for (int i = 0; i < vec->size; ++i) {
    foreach_fn(vec_at(vec, i));
  }
}

const void *vec_top(const vec_t *vec) { return vec_at(vec, vec->size - 1); }

bool vec_reverse(vec_t *v) {
  assert(v);
  if (v->size == 0) return false;
  size_t start = 0;
  size_t end = v->size - 1;
  char tmp[v->obj_size];
  while (start < end) {
    memcpy(tmp, v->buf + start * v->obj_size, v->obj_size);
    memcpy(v->buf + start * v->obj_size, v->buf + end * v->obj_size,
           v->obj_size);
    memcpy(v->buf + end * v->obj_size, tmp, v->obj_size);
    start++;
    end--;
  }
  return true;
}

#endif