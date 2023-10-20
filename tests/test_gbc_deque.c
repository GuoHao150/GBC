#include "../include/gbc_deque.h"

void print_int(const void *i) {
  const int *v = (int *)i;
  printf("%d\n", *v);
}

void test_deque_new(void) {
  vdq_t *ll_vdq = vdq_new(sizeof(long long));
  assert(ll_vdq->cap == 8 && ll_vdq->size == 0);
  int n = 10;
  for (int i = 0; i < n; ++i) {
    vdq_push_back(ll_vdq, &i);
  }
  vdq_t *ll_vdq2 = vdq_clone(ll_vdq);
  assert(ll_vdq->size == ll_vdq2->size && ll_vdq->cap == ll_vdq2->cap &&
         ll_vdq->front == ll_vdq2->front && ll_vdq->rear == ll_vdq2->rear);
  vdq_drop(ll_vdq);
  vdq_drop(ll_vdq2);
}

void test_deque_del(void) {
  vdq_t *q_int = vdq_new(sizeof(int));
  int n = 5;
  for (int i = 0; i < n; ++i) {
    vdq_push_back(q_int, &i);
    int i2 = i + 10;
    vdq_push_front(q_int, &i2);
  }
  for (int i = 0; i < 5; ++i) {
    int i2 = i + 5;
    vdq_push_back(q_int, &i2);
  }
  printf("before del\n");
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("test del\n");
  vdq_del_back(q_int);
  vdq_del_front(q_int);
  vdq_del_at(q_int, 1);
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("test del\n");
  vdq_drop(q_int);
}

void test_deque_reverse(void) {
  vdq_t *q_int = vdq_new(sizeof(int));
  int n = 2;
  for (int i = 0; i < n; ++i) {
    vdq_push_back(q_int, &i);
    int i2 = i + 10;
    vdq_push_front(q_int, &i2);
  }
  const int *old_hp = vdq_front(q_int);
  int old_h = *old_hp;
  printf("reverse start\n");
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("reverse\n");
  vdq_reverse(q_int);
  const int *new_bp = vdq_back(q_int);
  int new_b = *new_bp;
  assert(old_h == new_b);
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("reverse end\n");
  vdq_drop(q_int);
}

void test_deque_push(void) {
  vdq_t *q_int = vdq_new(sizeof(int));
  int n = 3;
  for (int i = 0; i < n; ++i) {
    vdq_push_back(q_int, &i);
    vdq_push_front(q_int, &i);
  }
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("!!!!!!!\n");
  assert(q_int->size == 6);
  const int *f = vdq_front(q_int);
  const int *b = vdq_back(q_int);
  assert(*f == *b);
  int newi = 55;
  vdq_insert(q_int, 5, &newi);
  for (int i = 0; i < q_int->size; ++i) {
    const int *o = vdq_at(q_int, i);
    printf("%d\n", *o);
  }
  printf("!!!!!!!\n");
  const int *i = vdq_at(q_int, 5);
  printf("%d\n", *i);
  assert(*i == 55);
  int newi2 = 66;
  vdq_update(q_int, 5, &newi2);
  const int *i2 = vdq_at(q_int, 5);
  assert(*i2 == 66);
  vdq_drop(q_int);
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

void test_deque_sort(void) {
  vdq_t *q = vdq_new(sizeof(int));
  int i1 = 30;
  int i2 = 51;
  int i3 = 21;
  int i4 = 24;
  int i5 = 26;
  int i6 = 10;
  vdq_push_back(q, &i1);
  vdq_push_back(q, &i2);
  vdq_push_back(q, &i3);
  vdq_push_back(q, &i4);
  vdq_push_back(q, &i5);
  vdq_push_back(q, &i6);
  printf("before sort\n");
  for (int i = 0; i < q->size; ++i) {
    const int *o = vdq_at(q, i);
    printf("%d\n", *o);
  }
  const int *front = vdq_front(q);
  printf("old front %d\n", *front);
  printf("end before sort\n");
  vdq_sort(q, int_cmp);
  printf("test sort\n");
  for (int i = 0; i < q->size; ++i) {
    const int *o = vdq_at(q, i);
    printf("%d\n", *o);
  }
  const int *back = vdq_back(q);
  printf("new back %d\n", *back);
  printf("end test sort\n");
  vdq_drop(q);
}

int main() {
  test_deque_new();
  test_deque_del();
  test_deque_reverse();
  test_deque_push();
  test_deque_sort();
  return 0;
}