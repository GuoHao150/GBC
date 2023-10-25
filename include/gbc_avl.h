#ifndef _GBC_AVL_H
#define _GBC_AVL_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbc_iterator.h"
#include "gbc_vector.h"

#define avl_max(a, b) (((a) < (b)) ? (b) : (a))

/// @brief the avl key type
typedef void *avl_key_t;

/// @brief the avl value type
typedef void *avl_val_t;

/// @brief the avl node
/// @param size_t height: the height of node
/// @param char* key_buf: the memory buffer for key
/// @param char* val_buf: the memory buffer for value
/// @param avl_node_t* parent: the parent node
/// @param avl_node_t* left: the left node
/// @param avl_node_t* right: the right node
typedef struct _avl_node avl_node_t;

/// @brief the avl tree map
/// @param avl_cmp_fn cmp_fn: the compare function for keys
/// @param avl_node_t* root: the root node for map
/// @param size_t size: the size of map
/// @param size_t key_obj_size: the key object size
/// @param size_t val_obj_size: the value object size
typedef struct _avl_map avl_map_t;

/// @brief the compare function for the keys in the map
/// return 0 if equal;
/// return -1 if the first is small;
/// return 1 if the second is big;
typedef int (*avl_cmp_fn)(const void *, const void *);

typedef void (*avl_foreach)(const void *key, const void *val);

/// @brief the key-value pair for the avl tree
typedef struct _avl_pair {
  char *key;
  char *val;
} avl_pair_t;

/// @brief avl tree node
/// @param height: the height of node
/// @param pair: the key-value pair
typedef struct _avl_node {
  size_t height;
  avl_pair_t pair;
  avl_node_t *parent;
  avl_node_t *left;
  avl_node_t *right;
} avl_node_t;

/// @brief avl_map_t
typedef struct _avl_map {
  avl_cmp_fn cmp_fn;
  avl_node_t *root;
  size_t size;
  size_t key_obj_size;
  size_t val_obj_size;
} avl_map_t;

static char set_value[0];

/// @brief avl_set_t
typedef struct _avl_set {
  avl_map_t *map;
  size_t size;
} avl_set_t;

typedef vec_t *next_nodes_ptr;
typedef avl_set_t *seen_nodes_ptr;

/// @brief avl_map_iter_t
typedef struct _avl_map_iter {
  iter_t base;
  next_nodes_ptr next_ptrs;  // vec_t<avl_node_t*>
  seen_nodes_ptr seen_ptrs;  // avl_set_t<avl_node_t*>
} avl_map_iter_t;

/// @brief avl_set_iter_t
typedef struct _avl_set_iter {
  iter_t base;
  next_nodes_ptr next_ptrs;  // vec_t<avl_node_t*>
  seen_nodes_ptr seen_ptrs;  // avl_set_t<avl_node_t*>
} avl_set_iter_t;

/// @brief create a new avl_map_t
/// @param key_obj_size: object size of the key
/// @param value_obj_size: object size of the value
/// @param cmp_fn: compare funciton of the keys
/// @return
avl_map_t *avl_map_new(size_t key_obj_size, size_t value_obj_size,
                       int (*cmp_fn)(const void *, const void *));

/// @brief adding key-value pair into the map. Update the value if the key
/// exists
/// @param map
/// @param key
/// @param value
/// @return
bool avl_map_add(avl_map_t *map, const avl_key_t key, const avl_val_t value);

/// @brief delete the key-value pair out of the map. return false if not
/// contains the key
/// @param map
/// @param key
/// @return
bool avl_map_del(avl_map_t *map, const avl_key_t key);

/// @brief check if the key exists in the map
/// @param map
/// @param key
/// @return
bool avl_map_contains(const avl_map_t *map, const avl_key_t key);

/// @brief get the const pointer of the value given a key
/// @param map
/// @param key
/// @return
const avl_val_t avl_map_get(const avl_map_t *map, const avl_key_t key);

/// @brief get the mutable pointer of the value given a key
/// @param map
/// @param key
/// @return
avl_val_t avl_map_get_mut(avl_map_t *map, const avl_key_t key);

/// @brief update the key-value pair in the map, if the key not exists then add
/// the pair into the map
/// @param map
/// @param key
/// @param value
/// @return
bool avl_map_update(avl_map_t *map, const avl_key_t key, const avl_val_t value);

/// @brief drop the avl_map_t
/// @param map
/// @return
bool avl_map_drop(avl_map_t *map);

/// @brief middle order foreach
/// @param map
/// @param fn
void avl_map_foreach(const avl_map_t *map, avl_foreach fn);

/// @brief create a new avl_set_t
/// @param key_obj_size
/// @param cmp_fn
/// @return
avl_set_t *avl_set_new(size_t key_obj_size, avl_cmp_fn cmp_fn);

/// @brief adding an element into the set
/// @param set
/// @param key
/// @return
bool avl_set_add(avl_set_t *set, const avl_key_t key);

/// @brief delete an element in the set
/// @param set
/// @param key
/// @return
bool avl_set_del(avl_set_t *set, const avl_key_t key);

/// @brief to check if the set contains an element
/// @param set
/// @param key
/// @return
bool avl_set_contains(const avl_set_t *set, const avl_key_t key);

/// @brief drop a set
/// @param set
/// @return
bool avl_set_drop(avl_set_t *set);

/// @brief intersection of two sets
/// @param set1
/// @param set2
/// @return
avl_set_t *avl_set_intersection(avl_set_t *set1, avl_set_t *set2);

/// @brief union of two sets
/// @param set1
/// @param set2
/// @return
avl_set_t *avl_set_union(avl_set_t *set1, avl_set_t *set2);

/// @brief difference operation of two sets
/// @param set1
/// @param set2
/// @return
avl_set_t *avl_set_diff(avl_set_t *set1, avl_set_t *set2);

/// @brief create an avl_map_iter_t
/// @param map
/// @return
avl_map_iter_t *avl_map_iter_new(avl_map_t *map);

/// @brief create an avl_set_iter_t
/// @param set
/// @return
avl_set_iter_t *avl_set_iter_new(avl_set_t *set);

/// @brief drop an avl_map_iter_t
/// @param iter
/// @return
bool avl_map_iter_drop(avl_map_iter_t *iter);

/// @brief drop an avl_set_iter_t
/// @param iter
/// @return
bool avl_set_iter_drop(avl_set_iter_t *iter);

/// @brief to check if the avl_map_iter_t has next element
/// @param iter
/// @return
bool avl_map_iter_has_next(const avl_map_iter_t *iter);

/// @brief to check if the avl_set_iter_t has next element
/// @param iter
/// @return
bool avl_set_iter_has_next(const avl_set_iter_t *iter);

/// @brief get next avl_pair_t*
/// @param iter
/// @return
avl_pair_t *avl_map_iter_next(avl_map_iter_t *iter);

/// @brief get next avl_pair_t*
/// @param iter
/// @return
void *avl_set_iter_next(avl_set_iter_t *iter);

avl_node_t *avl_node_new(const avl_key_t _key, size_t _key_obj_size,
                         const avl_val_t _value, size_t _value_obj_size) {
  avl_node_t *node = (avl_node_t *)malloc(sizeof(avl_node_t));
  if (!node) return NULL;
  node->height = 1;
  node->parent = node->left = node->right = NULL;
  char *key_buf = (char *)malloc(sizeof(_key_obj_size));
  if (!key_buf) {
    free(node);
    node = NULL;
    return NULL;
  }
  char *val_buf = (char *)malloc(sizeof(_value_obj_size));
  if (!val_buf) {
    free(node);
    free(key_buf);
    node = NULL;
    key_buf = NULL;
    return NULL;
  }
  avl_pair_t new_pair = {.key = key_buf, .val = val_buf};
  memcpy(new_pair.key, _key, _key_obj_size);
  memcpy(new_pair.val, _value, _value_obj_size);
  node->pair = new_pair;
  return node;
}

bool avl_node_update(avl_node_t *node, const avl_val_t val,
                     size_t val_obj_size) {
  if (!node || !val) return false;
  memcpy(node->pair.val, val, val_obj_size);
  return true;
}

size_t avl_node_height(const avl_node_t *node) {
  if (!node) return 0;
  return node->height;
}

bool avl_node_drop(avl_node_t *node) {
  if (!node) return false;
  if (node->pair.key) {
    free(node->pair.key);
    node->pair.key = NULL;
  }
  if (node->pair.val) {
    free(node->pair.val);
    node->pair.val = NULL;
  }
  free(node);
  node = NULL;
  return true;
}

avl_map_t *avl_map_new(size_t key_obj_size, size_t value_obj_size,
                       avl_cmp_fn cmp_fn) {
  avl_map_t *tree = (avl_map_t *)malloc(sizeof(avl_map_t));
  if (!tree) return NULL;
  tree->cmp_fn = cmp_fn;
  tree->root = NULL;
  tree->size = 0;
  tree->key_obj_size = key_obj_size;
  tree->val_obj_size = value_obj_size;
  return tree;
}

static void avl_unlink(avl_node_t *child, avl_node_t *parent, avl_map_t *map) {
  if (child && parent) {
    int order = map->cmp_fn(child->pair.key, parent->pair.key);
    if (order < 0) {
      parent->left = NULL;
    } else {
      parent->right = NULL;
    }
    child->parent = NULL;
  } else if (child && !parent) {
    child->parent = NULL;
    map->root = child;
  }
}

static void avl_set_left(avl_node_t *node, avl_node_t *left_child) {
  if (node) node->left = left_child;
  if (left_child) left_child->parent = node;
}

static void avl_set_right(avl_node_t *node, avl_node_t *right_child) {
  if (node) node->right = right_child;
  if (right_child) right_child->parent = node;
}

static void avl_relink(avl_node_t *child, avl_node_t *parent, avl_map_t *map) {
  if (!child && !parent)
    return;
  else if (child && parent) {
    int order = map->cmp_fn(parent->pair.key, child->pair.key);
    if (order < 0) {
      parent->right = child;
    } else {
      parent->left = child;
    }
    child->parent = parent;
    return;
  } else if (child && !parent) {
    child->parent = NULL;
    map->root = child;
    return;
  } else {  // !child && parent
    return;
  }
}

static int avl_bf(const avl_node_t *node) {
  if (!node) return 0;
  return avl_node_height(node->left) - avl_node_height(node->right);
}

//
//        y                              x
//       / \                           /   \
//      x   T4                        z     y
//     / \       - - - - - - - ->    / \   / \
//    z   T3                       T1  T2 T3 T4
//   / \
// T1   T2
static avl_node_t *avl_right_rotate(avl_node_t *y, avl_node_t *parent_n,
                                    avl_map_t *map) {
  avl_node_t *x = y->left;
  avl_node_t *t3 = x->right;
  x->right = y;
  y->parent = x;
  y->left = t3;
  if (t3) {
    t3->parent = y;
  }
  avl_relink(x, parent_n, map);

  y->height = 1 + avl_max(avl_node_height(y->left), avl_node_height(y->right));
  x->height = 1 + avl_max(avl_node_height(x->left), avl_node_height(x->right));
  return x;
}

//
//    y                             x
//  /  \                          /   \
// T1   x                        y     z
//     / \   - - - - - - - ->   / \   / \
//   T2  z                     T1 T2 T3 T4
//      / \
//     T3 T4
static avl_node_t *avl_left_rotate(avl_node_t *y, avl_node_t *parent_n,
                                   avl_map_t *map) {
  avl_node_t *x = y->right;
  avl_node_t *t2 = x->left;
  x->left = y;
  y->parent = x;
  y->right = t2;
  if (t2) {
    t2->parent = y;
  }
  avl_relink(x, parent_n, map);

  y->height = 1 + avl_max(avl_node_height(y->left), avl_node_height(y->right));
  x->height = 1 + avl_max(avl_node_height(x->left), avl_node_height(x->right));
  return x;
}

static avl_node_t *avl_find_min_child(avl_node_t *node) {
  if (!node) return NULL;
  avl_node_t *_node = node;
  avl_node_t *out = node;
  for (;;) {
    if (!_node->left) {
      out = _node;
      break;
    } else {
      _node = _node->left;
      continue;
    }
  }
  return out;
}

static avl_node_t *avl_find_max_child(avl_node_t *node) {
  if (!node) return NULL;
  avl_node_t *_node = node;
  avl_node_t *out = node;
  for (;;) {
    if (!_node->right) {
      out = _node;
      break;
    } else {
      _node = _node->right;
      continue;
    }
  }
  return out;
}

static const avl_node_t *avl_get_node(const avl_map_t *map,
                                      const avl_key_t key) {
  avl_node_t *node = map->root;
  for (;;) {
    if (!node) return NULL;
    int order = map->cmp_fn(key, node->pair.key);
    if (order == 0)
      break;
    else if (order < 0) {
      node = node->left;
      continue;
    } else {
      node = node->right;
      continue;
    }
  }
  return node;
}

static avl_node_t *avl_get_node_mut(const avl_map_t *map, const avl_key_t key) {
  avl_node_t *node = map->root;
  for (;;) {
    if (!node) return NULL;
    int order = map->cmp_fn(key, node->pair.key);
    if (order == 0)
      break;
    else if (order < 0) {
      node = node->left;
      continue;
    } else {
      node = node->right;
      continue;
    }
  }
  return node;
}

/// @brief for each input node, update its height and then try to rebalancing it
/// @param map
/// @param node
static void avl_try_reblance(avl_map_t *map, avl_node_t *node) {
  for (;;) {
    if (!node) {
      break;
    }
    node->height =
        1 + avl_max(avl_node_height(node->left), avl_node_height(node->right));
    avl_node_t *parent_n = node->parent;
    int bf = avl_bf(node);
    if (bf > 1) {
      if (avl_bf(node->left) >= 0) {
        avl_node_t *rotated_n = avl_right_rotate(node, parent_n, map);
        node = rotated_n->parent;
        continue;
      }
      if (avl_bf(node->left) < 0) {
        node->left = avl_left_rotate(node->left, node, map);
        avl_node_t *rotated_n = avl_right_rotate(node, parent_n, map);
        node = rotated_n->parent;
        continue;
      }
    } else if (bf < -1) {
      if (avl_bf(node->right) <= 0) {
        avl_node_t *rotated_n = avl_left_rotate(node, parent_n, map);
        node = rotated_n->parent;
        continue;
      }
      if (avl_bf(node->right) > 0) {
        node->right = avl_right_rotate(node->right, node, map);
        avl_node_t *rotated_n = avl_left_rotate(node, parent_n, map);
        node = rotated_n->parent;
        continue;
      }
    } else {
      node = node->parent;
      continue;
    }
  }
}

bool avl_map_add(avl_map_t *map, const avl_key_t _key, const avl_val_t _val) {
  avl_node_t *new_node =
      avl_node_new(_key, map->key_obj_size, _val, map->val_obj_size);
  if (!new_node) return false;
  if (map->size == 0) {
    map->size++;
    new_node->parent = NULL;
    map->root = new_node;
    return true;
  }
  avl_node_t *parent_n = map->root;
  for (;;) {
    int order = map->cmp_fn(_key, parent_n->pair.key);
    if (order == 0) {
      avl_node_update(parent_n, _val, map->val_obj_size);
      return true;
    } else if (order < 0) {
      if (parent_n->left) {
        parent_n = parent_n->left;
        continue;
      } else {
        new_node->parent = parent_n;
        parent_n->left = new_node;
        map->size++;
        break;
      }
    } else {
      if (parent_n->right) {
        parent_n = parent_n->right;
        continue;
      } else {
        new_node->parent = parent_n;
        parent_n->right = new_node;
        map->size++;
        break;
      }
    }
  }
  avl_try_reblance(map, new_node);
  return true;
}

bool avl_map_del(avl_map_t *map, const avl_key_t key) {
  avl_node_t *target_node = avl_get_node_mut(map, key);
  if (!target_node) return false;
  map->size--;
  avl_node_t *cur_parent = target_node->parent;
  avl_node_t *cur_left = target_node->left;
  avl_node_t *cur_right = target_node->right;
  if (cur_left && cur_right) {
    avl_node_t *cur_left_max = avl_find_max_child(cur_left);
    avl_unlink(cur_left_max, cur_left_max->parent, map);
    avl_relink(cur_left_max, cur_parent, map);
    avl_set_right(cur_left_max, cur_right);
    if (cur_left_max != cur_left) {
      avl_set_left(cur_left_max, cur_left);
    }
    avl_try_reblance(map, cur_left_max);
  } else if (cur_left && !cur_right && cur_parent) {
    cur_left->parent = NULL;
    target_node->left = NULL;
    avl_relink(cur_left, cur_parent, map);
    avl_try_reblance(map, cur_left);
  } else if (cur_left && !cur_right && !cur_parent) {
    target_node->left = NULL;
    cur_left->parent = NULL;
    map->root = cur_left;
    avl_try_reblance(map, cur_left);
  } else if (!cur_left && cur_right && cur_parent) {
    target_node->right = NULL;
    cur_right->parent = NULL;
    avl_relink(cur_right, cur_parent, map);
    avl_try_reblance(map, cur_right);
  } else if (!cur_left && cur_right && !cur_parent) {
    target_node->right = NULL;
    cur_right->parent = NULL;
    map->root = cur_right;
    avl_try_reblance(map, cur_right);
  } else if (!cur_left && !cur_right && cur_parent) {
    avl_unlink(target_node, cur_parent, map);
    avl_try_reblance(map, cur_parent);
  } else {
    // !cur_left && !cur_right && !cur_parent
    map->root = NULL;
  }
  avl_node_drop(target_node);
  return true;
}

bool avl_map_contains(const avl_map_t *map, const avl_key_t key) {
  const avl_node_t *node = avl_get_node(map, key);
  if (node)
    return true;
  else
    return false;
}

const avl_val_t avl_map_get(const avl_map_t *map, const avl_key_t key) {
  const avl_node_t *node = avl_get_node(map, key);
  if (!node) return NULL;
  return (void *)node->pair.val;
}

avl_val_t avl_map_get_mut(avl_map_t *map, const avl_key_t key) {
  avl_node_t *node = avl_get_node_mut(map, key);
  if (!node) return NULL;
  return (void *)node->pair.val;
}

bool avl_map_update(avl_map_t *map, const avl_key_t key, const avl_val_t val) {
  avl_node_t *node = avl_get_node_mut(map, key);
  if (!node) return avl_map_add(map, key, val);
  avl_node_update(node, val, map->val_obj_size);
  return true;
}

bool avl_map_del_min(avl_map_t *map) {
  assert(map);
  if (map->size == 0) return false;
  avl_node_t *min_node = avl_find_min_child(map->root);
  if (!min_node) return false;
  return avl_map_del(map, min_node->pair.key);
}

bool avl_map_del_max(avl_map_t *map) {
  assert(map);
  if (map->size == 0) return false;
  avl_node_t *max_node = avl_find_max_child(map->root);
  if (!max_node) return false;
  return avl_map_del(map, max_node->pair.key);
}

bool avl_map_drop(avl_map_t *map) {
  assert(map);
  size_t len = map->size;
  for (int i = 0; i < len; ++i) {
    bool flag = avl_map_del_min(map);
    if (!flag) return false;
  }
  map = NULL;
  return true;
}

void avl_map_middle_order_impl(const avl_node_t *node, avl_foreach fn) {
  if (node) {
    avl_map_middle_order_impl(node->left, fn);
    fn(node->pair.key, node->pair.val);
    avl_map_middle_order_impl(node->right, fn);
  }
}

void avl_map_foreach(const avl_map_t *map, avl_foreach fn) {
  avl_map_middle_order_impl(map->root, fn);
}

avl_set_t *avl_set_new(size_t key_obj_size, avl_cmp_fn cmp_fn) {
  avl_map_t *map = avl_map_new(key_obj_size, sizeof(set_value), cmp_fn);
  if (!map) return NULL;
  avl_set_t *set = (avl_set_t *)malloc(sizeof(avl_set_t));
  if (!set) {
    avl_map_drop(map);
    map = NULL;
    return NULL;
  }
  set->map = map;
  set->size = 0;
}

bool avl_set_add(avl_set_t *set, const avl_key_t key) {
  bool contains = avl_map_contains(set->map, key);
  if (contains) return false;
  bool flag = avl_map_add(set->map, key, set_value);
  if (flag) set->size++;
  return flag;
}

bool avl_set_del(avl_set_t *set, const avl_key_t key) {
  bool flag = avl_map_del(set->map, key);
  if (flag) set->size--;
  return flag;
}

bool avl_set_contains(const avl_set_t *set, const avl_key_t key) {
  return avl_map_contains(set->map, key);
}

bool avl_set_drop(avl_set_t *set) {
  bool flag = avl_map_drop(set->map);
  free(set);
  set = NULL;
  return flag;
}

bool _avl_map_iter_has_next(const iter_t *_iter) {
  const avl_map_iter_t *iter = (avl_map_iter_t *)_iter;
  return iter->next_ptrs->size > 0;
}

bool _avl_set_iter_has_next(const iter_t *_iter) {
  const avl_set_iter_t *iter = (avl_set_iter_t *)_iter;
  return iter->next_ptrs->size > 0;
}

#define _avl_iter_next                                                         \
  avl_node_t *out = NULL;                                                      \
  for (;;) {                                                                   \
    if (iter->next_ptrs->size == 0) break;                                     \
    avl_node_t **cur_node_ptr =                                                \
        (avl_node_t **)vec_at_mut(iter->next_ptrs, iter->next_ptrs->size - 1); \
    if (!cur_node_ptr) {                                                       \
      vec_del_top(iter->next_ptrs);                                            \
      continue;                                                                \
    }                                                                          \
    avl_node_t *cur_left = (*cur_node_ptr)->left;                              \
    avl_node_t *cur_right = (*cur_node_ptr)->right;                            \
    avl_node_t *cur_left_ptr = NULL;                                           \
    avl_node_t *cur_right_ptr = NULL;                                          \
    if (cur_left && !avl_set_contains(iter->seen_ptrs, &cur_left)) {           \
      cur_left_ptr = cur_left;                                                 \
    }                                                                          \
    if (cur_right && !avl_set_contains(iter->seen_ptrs, &cur_right)) {         \
      cur_right_ptr = cur_right;                                               \
    }                                                                          \
                                                                               \
    if (!cur_left_ptr && cur_right_ptr) {                                      \
      out = *cur_node_ptr;                                                     \
      vec_del_top(iter->next_ptrs);                                            \
      avl_set_add(iter->seen_ptrs, &out);                                      \
      vec_push(iter->next_ptrs, &cur_right_ptr);                               \
      break;                                                                   \
    } else if (cur_left_ptr && cur_right_ptr) {                                \
      vec_push(iter->next_ptrs, &cur_left_ptr);                                \
      continue;                                                                \
    } else if (!cur_left_ptr && !cur_right_ptr) {                              \
      out = *cur_node_ptr;                                                     \
      vec_del_top(iter->next_ptrs);                                            \
      avl_set_add(iter->seen_ptrs, &out);                                      \
      break;                                                                   \
    } else {                                                                   \
      /* cur_left && !cur_right */                                             \
      vec_push(iter->next_ptrs, &cur_left_ptr);                                \
      continue;                                                                \
    }                                                                          \
  }                                                                            \
  if (!out) return NULL;                                                       \
  avl_pair_t *pair_ptr = &out->pair;                                           \
  return pair_ptr;

void *_avl_map_iter_next(iter_t *_iter) {
  avl_map_iter_t *iter = (avl_map_iter_t *)_iter;
  _avl_iter_next
}

void *_avl_set_iter_next(iter_t *_iter) {
  avl_set_iter_t *iter = (avl_set_iter_t *)_iter;
  _avl_iter_next
}

typedef avl_node_t *avl_node_ptr;

static int avl_node_ptr_cmp(const void *n1, const void *n2) {
  const avl_node_ptr *a1 = (avl_node_ptr *)n1;
  const avl_node_ptr *a2 = (avl_node_ptr *)n2;
  if (*a1 == *a2)
    return 0;
  else if (*a1 < *a2)
    return -1;
  else
    return 1;
}

avl_map_iter_t *avl_map_iter_new(avl_map_t *map) {
  assert(map);
  avl_map_iter_t *iter = (avl_map_iter_t *)malloc(sizeof(avl_map_iter_t));
  if (!iter) {
    return NULL;
  }
  iter_t base = {.obj_size = sizeof(avl_pair_t),
                 .has_next = _avl_map_iter_has_next,
                 .next = _avl_map_iter_next};
  iter->base = base;
  next_nodes_ptr next_ptrs = vec_new(sizeof(avl_node_t *));
  if (!next_ptrs) {
    free(iter);
    return NULL;
  }
  seen_nodes_ptr seen_ptrs =
      avl_set_new(sizeof(avl_node_t *), avl_node_ptr_cmp);
  if (!seen_ptrs) {
    free(iter);
    vec_drop(next_ptrs);
    return NULL;
  }
  if (map->size > 0) {
    vec_push(next_ptrs, &map->root);
  }
  iter->base = base;
  iter->next_ptrs = next_ptrs;
  iter->seen_ptrs = seen_ptrs;
  return iter;
}

avl_set_iter_t *avl_set_iter_new(avl_set_t *set) {
  assert(set);
  avl_set_iter_t *iter = (avl_set_iter_t *)malloc(sizeof(avl_set_iter_t));
  if (!iter) return NULL;
  iter_t base = {.obj_size = sizeof(avl_pair_t),
                 .has_next = _avl_set_iter_has_next,
                 .next = _avl_set_iter_next};
  iter->base = base;
  next_nodes_ptr next_ptrs = vec_new(sizeof(avl_node_t *));
  if (!next_ptrs) {
    free(iter);
    return NULL;
  }
  seen_nodes_ptr seen_ptrs =
      avl_set_new(sizeof(avl_node_t *), avl_node_ptr_cmp);
  if (!seen_ptrs) {
    free(iter);
    vec_drop(next_ptrs);
    return NULL;
  }
  if (set->size > 0) {
    vec_push(next_ptrs, &(set->map->root));
  }
  iter->base = base;
  iter->next_ptrs = next_ptrs;
  iter->seen_ptrs = seen_ptrs;
  return iter;
}

#define _avl_iter_drop             \
  if (!iter) return false;         \
  if (iter->next_ptrs) {           \
    vec_drop(iter->next_ptrs);     \
    iter->next_ptrs = NULL;        \
  }                                \
  if (iter->seen_ptrs) {           \
    avl_set_drop(iter->seen_ptrs); \
    iter->seen_ptrs = NULL;        \
  }                                \
  free(iter);                      \
  return true;

bool avl_map_iter_drop(avl_map_iter_t *iter) { _avl_iter_drop }

bool avl_set_iter_drop(avl_set_iter_t *iter) { _avl_iter_drop }

bool avl_map_iter_has_next(const avl_map_iter_t *iter) {
  return iter->base.has_next((iter_t *)iter);
}

bool avl_set_iter_has_next(const avl_set_iter_t *iter) {
  return iter->base.has_next((iter_t *)iter);
}

avl_pair_t *avl_map_iter_next(avl_map_iter_t *iter) {
  if (!avl_map_iter_has_next(iter)) return NULL;
  void *out = iter->base.next((iter_t *)iter);
  return (avl_pair_t *)out;
}

void *avl_set_iter_next(avl_set_iter_t *iter) {
  if (!avl_set_iter_has_next(iter)) return NULL;
  avl_pair_t *out = (avl_pair_t *)iter->base.next((iter_t *)iter);
  return out->key;
}

avl_set_t *avl_set_intersection(avl_set_t *set1, avl_set_t *set2) {
  assert(set1 && set2);
  avl_set_t *out = avl_set_new(set1->map->key_obj_size, set1->map->cmp_fn);
  avl_set_iter_t *iter1 = avl_set_iter_new(set1);
  avl_set_iter_t *iter2 = avl_set_iter_new(set2);
  while (avl_set_iter_has_next(iter1)) {
    const avl_key_t n = avl_set_iter_next(iter1);
    bool in_set1 = avl_set_contains(set1, n);
    bool in_set2 = avl_set_contains(set2, n);
    if (in_set1 && in_set2) {
      avl_set_add(out, n);
    }
  }
  while (avl_set_iter_has_next(iter2)) {
    const avl_key_t n = avl_set_iter_next(iter2);
    bool in_set1 = avl_set_contains(set1, n);
    bool in_set2 = avl_set_contains(set2, n);
    if (in_set1 && in_set2) {
      avl_set_add(out, n);
    }
  }
  avl_set_iter_drop(iter1);
  avl_set_iter_drop(iter2);
  return out;
}

avl_set_t *avl_set_union(avl_set_t *set1, avl_set_t *set2) {
  assert(set1 && set2);
  avl_set_t *out = avl_set_new(set1->map->key_obj_size, set1->map->cmp_fn);
  avl_set_iter_t *iter1 = avl_set_iter_new(set1);
  avl_set_iter_t *iter2 = avl_set_iter_new(set2);
  while (avl_set_iter_has_next(iter1)) {
    const avl_key_t n = avl_set_iter_next(iter1);
    avl_set_add(out, n);
  }
  while (avl_set_iter_has_next(iter2)) {
    const avl_key_t n = avl_set_iter_next(iter2);
    avl_set_add(out, n);
  }
  avl_set_iter_drop(iter1);
  avl_set_iter_drop(iter2);
  return out;
}

avl_set_t *avl_set_diff(avl_set_t *set1, avl_set_t *set2) {
  assert(set1 && set2);
  avl_set_t *out = avl_set_new(set1->map->key_obj_size, set1->map->cmp_fn);
  avl_set_iter_t *iter1 = avl_set_iter_new(set1);
  while (avl_set_iter_has_next(iter1)) {
    const avl_key_t n = avl_set_iter_next(iter1);
    bool in_set1 = avl_set_contains(set1, n);
    bool in_set2 = avl_set_contains(set2, n);
    if (in_set1 && !in_set2) {
      avl_set_add(out, n);
    }
  }
  avl_set_iter_drop(iter1);
  return out;
}

#endif