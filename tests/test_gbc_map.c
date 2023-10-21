#include "../include/gbc_avl.h"

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

void int_pair_print(const void *a, const void *b) {
  printf("key: %d; val %d\n", *(int *)a, *(int *)b);
}

void test_map_del(void) {
  avl_map_t *map = avl_map_new(sizeof(int), sizeof(int), int_cmp);

  assert(map->key_obj_size == 4 && map->key_obj_size == 4 && map->size == 0);
  int n = 15;
  for (int i = 0; i < n; ++i) {
    avl_map_add(map, &i, &i);
  }
  assert(map->size == n);
  int k1 = 5;
  int k2 = 10;
  int k3 = 13;
  avl_map_del(map, &k1);
  avl_map_del(map, &k2);
  avl_map_del(map, &k3);
  avl_map_middle_order(map, int_pair_print);
  printf("~~~~~~~~~~~~~~\n");
  assert(map->size == n - 3);
  for (int i = 0; i < n; ++i) {
    avl_map_del(map, &i);
    avl_map_middle_order(map, int_pair_print);
    printf("~~~~~~~~~~~~~~\n");
  }
  assert(map->size == 0);
  int n2 = 3;
  for (int i = 0; i < n2; ++i) {
    avl_map_add(map, &i, &i);
  }
  assert(map->size == n2);
  avl_map_drop(map);
  printf("\n");
  assert(map->size == 0);
}

void test_map_new(void) {
  avl_map_t *map = avl_map_new(sizeof(int), sizeof(int), int_cmp);

  assert(map->key_obj_size == 4 && map->key_obj_size == 4 && map->size == 0);
  int n = 15;
  for (int i = 0; i < n; ++i) {
    avl_map_add(map, &i, &i);
  }
  assert(map->size == n);
  avl_map_middle_order(map, int_pair_print);
  printf("del\n");
  int k = 5;
  int k2 = 10;
  int k3 = 13;
  avl_map_del(map, &k);
  avl_map_del(map, &k2);
  avl_map_del(map, &k3);
  assert(map->size == n - 3);
  avl_map_middle_order(map, int_pair_print);
  int x = 0;
  for (int i = 0; i < map->size; ++i) {
    bool flag = avl_map_del_min(map);
    x++;
  }
  printf("del2\n");
  avl_map_middle_order(map, int_pair_print);
  avl_map_drop(map);
}

int main() {
  test_map_del();
  // test_map_new();
  return 0;
}