#ifndef _GBC_DQUEUE_H
#define _GBC_DQUEUE_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbc_iterator.h"

#define DEFAULT_DQ_SIZE 8

/// @brief vector double-ended queue
typedef struct _vdq_t {
  size_t obj_size;
  size_t front;
  size_t rear;
  size_t cap;
  size_t size;
  char *buf;
} vdq_t;

/// @brief The vector deque iterator
typedef struct _vdq_iter_t {
  iter_t base;
  size_t cur_idx;
  vdq_t *dq;
} vdq_iter_t;

/// @brief create a new vdq_t
/// @param element_size: the size of each element
vdq_t *vdq_new(size_t element_size);

/// @brief drop the vdq_t out of memory
/// @param q
/// @return return false if the drop failed
bool vdq_drop(vdq_t *q);

/// @brief check if vdq_t is empty
/// @param q
/// @return
bool vdq_is_empty(const vdq_t *q);

/// @brief check if vdq_t is full
/// @param
/// @return
bool vdq_is_full(const vdq_t *);

/// @brief get the length of vdq_t
/// @param q
/// @return
size_t vdq_length(const vdq_t *q);

/// @brief push an element at the back of vdq_t
/// @param q
/// @param vlaue
/// @return
bool vdq_push_back(vdq_t *q, const void *value);

/// @brief push an element at the front of vdq_t
/// @param q
/// @param value
/// @return
bool vdq_push_front(vdq_t *q, const void *value);

/// @brief insert an element in the vdq_t given an index
/// @param q
/// @param idx
/// @param value
/// @return
bool vdq_insert(vdq_t *q, size_t idx, const void *value);

/// @brief check the front/first element in the vdq_t
/// @param q
/// @return
const void *vdq_front(const vdq_t *q);

/// @brief check the back/last element in the vdq_t
/// @param q
/// @return
const void *vdq_back(const vdq_t *q);

/// @brief check the element with a certain index
/// @param q
/// @param idx
/// @return
const void *vdq_at(const vdq_t *q, size_t idx);

/// @brief Get the mutable pointer with an index
/// @param q
/// @param idx
/// @return
void *vdq_at_mut(vdq_t *q, size_t idx);

/// @brief update the value in the vdq_t with an index
/// @param q
/// @param idx
/// @param value
/// @return
bool vdq_update(vdq_t *q, size_t idx, const void *value);

/// @brief for each element in the vdq_t and process it
/// @param q
/// @param foreach_fn
void vdq_foreach(const vdq_t *q, void (*foreach_fn)(const void *));

/// @brief delete the first element
/// @param q
/// @return return false if the deletion failed
bool vdq_del_front(vdq_t *q);

/// @brief delete the last element
/// @param q
/// @return return false if the deletion failed
bool vdq_del_back(vdq_t *q);

/// @brief delete an element in the vdq_t with an index
/// @param q
/// @param idx
/// @return
bool vdq_del_at(vdq_t *q, size_t idx);

/// @brief delete an element out of the vdq_t
/// @param q
/// @param target_value
/// @param cmp_fn
/// @return
bool vdq_del(vdq_t *q, const void *target_value,
             int (*cmp_fn)(const void *, const void *));

/// @brief create a new vdq_t with specified capacity size
/// @param element_size
/// @param cap
/// @return
vdq_t *vdq_new_with_cap(size_t element_size, size_t cap);

/// @brief deep clone a vdq_t
/// @param q
/// @return
vdq_t *vdq_clone(vdq_t *q);

/// reverse the vdq_t
bool vdq_reverse(vdq_t *q);

/// sort the vdq_t
bool vdq_sort(vdq_t *q, int (*cmp_fn)(const void *, const void *));

/// @brief create a vdq_t from an array
/// @param _arr
/// @param array_size
/// @param obj_size
/// @return
vdq_t *vdq_from_array(const void *_arr, size_t array_size, size_t obj_size);

/// @brief create a new vdq_iter_t
/// @param dq
/// @return
vdq_iter_t *vdq_iter_new(vdq_t *dq);

/// @brief drop a vdq_iter_t
/// @param iter
/// @return
bool vdq_iter_drop(vdq_iter_t *iter);

/// @brief check if the vdq_iter_t has next element
/// @param iter
/// @return
bool vdq_iter_has_next(const vdq_iter_t *iter);

/// @brief get the next element
/// @param iter
/// @return
void *vdq_iter_next(vdq_iter_t *iter);

/// @brief create a vdq_t from an iterator
/// @param iter
/// @return
vdq_t *vdq_from_iter(iter_t *iter);

vdq_t *vdq_new(size_t element_size) {
  char *buf = (char *)malloc(DEFAULT_DQ_SIZE * element_size);
  if (!buf) return NULL;
  vdq_t *q = (vdq_t *)malloc(sizeof(vdq_t));
  if (!q) {
    free(buf);
    buf = NULL;
    return NULL;
  }
  q->obj_size = element_size;
  q->front = 0;
  q->rear = 0;
  q->cap = DEFAULT_DQ_SIZE;
  q->size = 0;
  q->buf = buf;
  return q;
}

vdq_t *vdq_new_with_cap(size_t element_size, size_t cap) {
  char *buf = (char *)malloc(cap * element_size);
  if (!buf) return NULL;
  vdq_t *q = (vdq_t *)malloc(sizeof(vdq_t));
  if (!q) {
    free(buf);
    buf = NULL;
    return NULL;
  }
  q->obj_size = element_size;
  q->front = 0;
  q->rear = 0;
  q->cap = cap;
  q->size = 0;
  q->buf = buf;
  return q;
}

vdq_t *vdq_clone(vdq_t *q) {
  if (!q) return NULL;
  vdq_t *out = vdq_new_with_cap(q->obj_size, q->cap);
  if (!out) return NULL;
  memcpy(out->buf, q->buf, q->obj_size * q->size);
  out->front = q->front;
  out->rear = q->rear;
  out->size = q->size;
  return out;
}

bool vdq_drop(vdq_t *q) {
  if (!q) return false;
  if (q->buf) {
    free(q->buf);
    q->buf = NULL;
  }
  free(q);
  q = NULL;
  return true;
}

bool vdq_is_empty(const vdq_t *q) {
  assert(q);
  return q->size == 0;
}

bool vdq_is_full(const vdq_t *q) {
  assert(q);
  return q->size == q->cap;
}

size_t vdq_length(const vdq_t *q) {
  assert(q);
  return q->size;
}

static bool vdq_enlarge(vdq_t *q, size_t new_cap) {
  char *new_buf = (char *)malloc(q->obj_size * new_cap);
  if (!new_buf) return false;
  if (q->rear > q->front) {
    memcpy(new_buf, q->buf, q->obj_size * q->size);
    q->front = 0;
    q->rear = q->size;
    q->cap = new_cap;
    if (q->buf) {
      free(q->buf);
      q->buf = new_buf;
    } else
      q->buf = new_buf;
    return true;
  } else {
    size_t front_parts = (q->cap - q->front) * q->obj_size;
    size_t rear_parts = q->rear * q->obj_size;
    memcpy(new_buf, q->buf + (q->front * q->obj_size), front_parts);
    memcpy(new_buf + front_parts, q->buf, rear_parts);
    q->front = 0;
    q->rear = q->size;
    q->cap = new_cap;
    if (q->buf) {
      free(q->buf);
      q->buf = new_buf;
    } else
      q->buf = new_buf;
    return true;
  }
}

bool vdq_push_back(vdq_t *q, const void *value) {
  assert(q && value);
  if (vdq_is_full(q)) {
    if (!vdq_enlarge(q, q->cap * 2)) return false;
  }
  memcpy(q->buf + (q->rear * q->obj_size), value, q->obj_size);
  q->rear = (q->rear + 1) % q->cap;
  q->size++;
  return true;
}

bool vdq_push_front(vdq_t *q, const void *value) {
  assert(q && value);
  if (vdq_is_full(q)) {
    if (!vdq_enlarge(q, q->cap * 2)) return false;
  }
  q->front = (q->front + q->cap - 1) % q->cap;
  memcpy(q->buf + (q->front * q->obj_size), value, q->obj_size);
  q->size++;
  return true;
}

bool vdq_insert(vdq_t *q, size_t idx, const void *value) {
  assert(q && value && q->size > idx);
  if (vdq_is_full(q)) {
    if (!vdq_enlarge(q, q->cap * 2)) return false;
  } else {
    if (!vdq_enlarge(q, q->cap)) return false;
  }
  if (idx == 0)
    return vdq_push_front(q, value);
  else if (idx == q->size)
    return vdq_push_back(q, value);
  else {
    size_t pos0 = (q->front + idx) % q->cap;
    size_t pos1 = (q->front + idx + 1) % q->cap;
    memmove(q->buf + pos1 * q->obj_size, q->buf + pos0 * q->obj_size,
            (q->size - idx) * q->obj_size);
    memcpy(q->buf + pos0 * q->obj_size, value, q->obj_size);
    q->size++;
    q->rear = (q->rear + 1) % q->cap;
    return true;
  }
}

const void *vdq_front(const vdq_t *q) {
  assert(q);
  if (q->size == 0) return NULL;
  char *ptr = q->buf + q->front * q->obj_size;
  return ptr;
}

const void *vdq_back(const vdq_t *q) {
  assert(q);
  if (q->size == 0) return NULL;
  char *ptr = q->buf + ((q->rear + q->cap - 1) % q->cap) * q->obj_size;
  return ptr;
}

const void *vdq_at(const vdq_t *q, size_t idx) {
  assert(q && q->size > idx);
  char *ptr = q->buf + ((q->front + idx) % q->cap) * q->obj_size;
  return ptr;
}

void *vdq_at_mut(vdq_t *q, size_t idx) {
  assert(q && q->size > idx);
  char *ptr = q->buf + ((q->front + idx) % q->cap) * q->obj_size;
  return ptr;
}

bool vdq_update(vdq_t *q, size_t idx, const void *value) {
  assert(q && value && q->size > idx);
  char *ptr = q->buf + ((q->front + idx) % q->cap) * q->obj_size;
  memcpy(ptr, value, q->obj_size);
  return true;
}

void vdq_foreach(const vdq_t *q, void (*foreach_fn)(const void *)) {
  for (int i = 0; i < q->size; ++i) foreach_fn(vdq_at(q, i));
}

bool vdq_del_front(vdq_t *q) {
  assert(q);
  if (q->size == 0) return false;
  q->front = (q->front + 1) % q->cap;
  q->size--;
  return true;
}

bool vdq_del_back(vdq_t *q) {
  assert(q);
  if (q->size == 0) return false;
  q->rear = (q->rear + q->cap - 1) % q->cap;
  q->size--;
  return true;
}

bool vdq_del_at(vdq_t *q, size_t idx) {
  assert(q && q->size > idx);
  if (idx == 0)
    return vdq_del_front(q);
  else if (idx == q->size)
    return vdq_del_back(q);
  else {
    if (q->rear < q->front) {
      if (!vdq_enlarge(q, q->cap)) return false;
    }
    memmove(q->buf + ((idx + q->front) % q->cap) * q->obj_size,
            q->buf + ((idx + 1 + q->front) % q->cap) * q->obj_size,
            (q->size - idx - 1) * q->obj_size);
    q->size--;
    q->rear = (q->rear + q->cap - 1) % q->cap;
    return true;
  }
}

bool vdq_del(vdq_t *q, const void *target_value,
             int (*cmp_fn)(const void *, const void *)) {
  size_t idx;
  bool flag = false;
  for (int i = 0; i < q->size; ++i) {
    const void *ele = vdq_at(q, i);
    int order = cmp_fn(ele, target_value);
    if (order == 0) {
      flag = true;
      idx = i;
      break;
    }
  }
  if (flag) {
    return vdq_del_at(q, idx);
  }

  return flag;
}

bool vdq_reverse(vdq_t *q) {
  assert(q);
  if (q->size == 0) return false;
  if (!vdq_enlarge(q, q->cap)) return false;
  size_t start = q->front;
  size_t end = (q->rear + q->cap - 1) % q->cap;
  char tmp[q->obj_size];
  while (start < end) {
    memcpy(tmp, q->buf + start * q->obj_size, q->obj_size);
    memcpy(q->buf + start * q->obj_size, q->buf + end * q->obj_size,
           q->obj_size);
    memcpy(q->buf + end * q->obj_size, tmp, q->obj_size);
    start = (start + 1) % q->cap;
    end = (end + q->cap - 1) % q->cap;
  }
  return true;
}

bool vdq_sort(vdq_t *q, int (*cmp_fn)(const void *, const void *)) {
  assert(q);
  if (q->size == 0) return false;
  if (!vdq_enlarge(q, q->cap)) return false;
  qsort(q->buf, q->size, q->obj_size, cmp_fn);
  return true;
}

vdq_t *vdq_from_array(const void *_arr, size_t array_size, size_t obj_size) {
  size_t cap = (array_size % 2 == 0) ? array_size : array_size + 1;
  vdq_t *q = vdq_new_with_cap(obj_size, cap);
  if (!q) return NULL;
  q->size = array_size;
  memcpy(q->buf, _arr, obj_size * array_size);
  q->front = 0;
  q->rear = array_size;
  return q;
}

bool _vdq_iter_has_next(const iter_t *_iter) {
  const vdq_iter_t *iter = (vdq_iter_t *)_iter;
  return iter->cur_idx < iter->dq->size;
}

void *_vdq_iter_next(iter_t *_iter) {
  vdq_iter_t *iter = (vdq_iter_t *)_iter;
  if (!_vdq_iter_has_next(_iter)) {
    return NULL;
  } else {
    void *out = vdq_at_mut(iter->dq, iter->cur_idx);
    iter->cur_idx++;
    return out;
  }
}

vdq_iter_t *vdq_iter_new(vdq_t *dq) {
  assert(dq);
  vdq_iter_t *iter = (vdq_iter_t *)malloc(sizeof(vdq_iter_t));
  if (!iter) {
    return NULL;
  }
  iter_t base = {.obj_size = dq->obj_size,
                 .has_next = _vdq_iter_has_next,
                 .next = _vdq_iter_next};
  iter->base = base;
  iter->cur_idx = 0;
  iter->dq = dq;
  return iter;
}

bool vdq_iter_drop(vdq_iter_t *iter) {
  if (!iter) return false;
  free(iter);
  return true;
}

bool vdq_iter_has_next(const vdq_iter_t *iter) {
  return iter->base.has_next((iter_t *)iter);
}

void *vdq_iter_next(vdq_iter_t *iter) {
  return iter->base.next((iter_t *)iter);
}

vdq_t *vdq_from_iter(iter_t *iter) {
  assert(iter);
  vdq_t *dq = vdq_new(iter->obj_size);
  while (iter->has_next(iter)) {
    const void *out = iter->next(iter);
    vdq_push_back(dq, out);
  }
  return dq;
}

#endif