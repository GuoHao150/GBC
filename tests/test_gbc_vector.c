#include "../include/gbc_vector.h"

void print_int(const void *i) {
  const int *v = (int *)i;
  printf("%d\n", *v);
}

void test_vector_new(void) {
  vec_t *ll_v = vec_new(sizeof(long long));
  assert(ll_v->cap == 8 && ll_v->size == 0);
  int n = 10;
  for (int i = 0; i < n; ++i) {
    vec_push(ll_v, &i);
  }
  vec_t *ll_v2 = vec_clone(ll_v);
  assert(ll_v->size == ll_v2->size && ll_v->cap == ll_v2->cap);
  vec_drop(ll_v);
  vec_drop(ll_v2);
}

void test_vector_del(void) {
  vec_t *int_v = vec_new(sizeof(int));
  int n = 10;
  for (int i = 0; i < n; ++i) {
    vec_push(int_v, &i);
  }
  // vec_foreach(int_v, print_int);
  const int *t0 = vec_top(int_v);
  assert(9 == *t0);
  assert(int_v->size == n);
  vec_del_top(int_v);
  assert(int_v->size == n - 1);
  const int *top1 = vec_top(int_v);
  assert(*top1 == 8);
  vec_del_at(int_v, 1);
  assert(int_v->size == n - 2);
  const int *demo = vec_at(int_v, 1);
  assert(*demo == 2);
  vec_del_at(int_v, 0);
  const int *demo2 = vec_at(int_v, 0);
  // printf("demo: %p and demo2 %p\n", demo, demo2);
  assert(*demo2 == 2);
  vec_drop(int_v);
}

void test_vector_reverse(void) {
  vec_t *int_v = vec_new(sizeof(int));
  int n = 10;
  for (int i = 0; i < n; ++i) {
    vec_push(int_v, &i);
  }
  assert(int_v->size == n && ((int *)int_v->buf)[0] == 0);
  // vec_foreach(int_v, print_int);
  vec_reverse(int_v);
  // vec_foreach(int_v, print_int);
  assert(int_v->size == n && ((int *)int_v->buf)[n - 1] == 0);
  vec_drop(int_v);
}

void test_vector_push(void) {
  vec_t *int_v = vec_new(sizeof(int));
  int n = 5;
  for (int i = 0; i < n; ++i) {
    vec_push(int_v, &i);
  }
  assert(int_v->size == n && ((int *)int_v->buf)[0] == 0);
  int i6 = 6;
  vec_insert(int_v, 0, &i6);
  assert(((int *)int_v->buf)[0] == 6 && int_v->size == n + 1);
  vec_drop(int_v);
}

int int_cmp(const void *a, const void *b) {
  const int *_a = (int *)a;
  const int *_b = (int *)b;
  if (*_a == *_b)
    return 0;
  else if (*_a < *_b)
    return -1;
  else
    return 1;
}

void test_vector_sort(void) {
  vec_t *int_v = vec_new(sizeof(int));
  int i1 = 30;
  int i2 = 51;
  int i3 = 21;
  int i4 = 24;
  int i5 = 26;
  int i6 = 10;
  vec_push(int_v, &i1);
  vec_push(int_v, &i2);
  vec_push(int_v, &i3);
  vec_push(int_v, &i4);
  vec_push(int_v, &i5);
  vec_push(int_v, &i6);
  assert(int_v->size == 6 && ((int *)int_v->buf)[0] == 30);
  vec_sort(int_v, int_cmp);
  assert(int_v->size == 6 && ((int *)int_v->buf)[0] == 10);
  assert(int_v->size == 6 && ((int *)int_v->buf)[5] == 51);
  // vec_foreach(int_v, print_int);
}

int main() {
  test_vector_new();
  test_vector_del();
  test_vector_reverse();
  test_vector_push();
  test_vector_sort();
  return 0;
}