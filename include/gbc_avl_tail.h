#ifndef _GBC_AVLTAIL_H
#define _GBC_AVLTAIL_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define avl_max(a, b) (((a) < (b)) ? (b) : (a))

/// @brief the compare function for the keys in the map
/// return 0 if equal;
/// return -1 if the first is small;
/// return 1 if the second is big;
typedef int (*avl_cmp_fn)(const void *, const void *);

typedef void (*avl_foreach)(const void *key, const void *val);

/// @brief the avl key type
typedef void *avl_key_t;

/// @brief the avl value type
typedef void *avl_val_t;

/// @brief the key-value pair for the avl tree
typedef struct _avl_pair {
  char *key;
  char *val;
} avl_pair_t;

typedef enum avl_child {
  LeftChild = 0,
  RightChild,
} avl_child;

/// @brief avl tree node
/// @param height: the height of node
/// @param pair: the key-value pair
typedef struct _avl_node {
  size_t height;
  avl_pair_t pair;
  avl_node_t *left;
  avl_node_t *right;
} avl_node_t;

avl_node_t *avl_node_new(const avl_key_t _key, size_t _key_obj_size,
                         const avl_val_t _value, size_t _value_obj_size) {
  avl_node_t *node = (avl_node_t *)malloc(sizeof(avl_node_t));
  if (!node) return NULL;
  node->height = 1;
  node->left = node->right = NULL;
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

typedef struct _avl {
  avl_cmp_fn cmp_fn;
  avl_node_t *root;
  size_t size;
  size_t key_obj_size;
  size_t val_obj_size;
} avl_map_t;

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
static avl_node_t *avl_right_rotate(avl_node_t *y) {
  avl_node_t *x = y->left;
  avl_node_t *t3 = x->right;
  x->right = y;
  y->left = t3;

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
static avl_node_t *avl_left_rotate(avl_node_t *y) {
  avl_node_t *x = y->right;
  avl_node_t *t2 = x->left;
  x->left = y;
  y->right = t2;

  y->height = 1 + avl_max(avl_node_height(y->left), avl_node_height(y->right));
  x->height = 1 + avl_max(avl_node_height(x->left), avl_node_height(x->right));
  return x;
}

static avl_node_t *avl_find_min_child(avl_node_t *node) {
  if (!node) return NULL;
  avl_node_t *_node = node;
  avl_node_t *out;
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

#endif