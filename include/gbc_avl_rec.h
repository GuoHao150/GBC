#ifndef _GBC_AVLREC_H
#define _GBC_AVLREC_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef struct _avl avl_map_t;

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
  avl_node_t *left;
  avl_node_t *right;
} avl_node_t;

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

avl_node_t *avl_map_add_node(avl_map_t *map, avl_node_t *node,
                             const avl_key_t key, const avl_val_t val) {
  if (!node) {
    map->size++;
    avl_node_t *new_node =
        avl_node_new(key, map->key_obj_size, val, map->val_obj_size);
    return new_node;
  }
  int order = map->cmp_fn(key, node->pair.key);
  if (order < 0) {
    node->left = avl_map_add_node(map, node->left, key, val);
  } else if (order > 0) {
    node->right = avl_map_add_node(map, node->right, key, val);
  } else {
    avl_node_update(node, val, map->val_obj_size);
  }
  node->height =
      1 + avl_max(avl_node_height(node->left), avl_node_height(node->right));
  int bf = avl_bf(node);
  // LL
  if (bf > 1 && avl_bf(node->left) >= 0) return avl_right_rotate(node);
  // RR
  if (bf < -1 && avl_bf(node->right) <= 0) return avl_left_rotate(node);
  // LR
  if (bf > 1 && avl_bf(node->left) < 0) {
    node->left = avl_left_rotate(node->left);
    return avl_right_rotate(node);
  }
  // RL
  if (bf < -1 && avl_bf(node->right) > 0) {
    node->right = avl_right_rotate(node->right);
    return avl_left_rotate(node);
  }
  return node;
}

bool avl_map_add(avl_map_t *map, const avl_key_t key, const avl_val_t val) {
  map->root = avl_map_add_node(map, map->root, key, val);
  return true;
}

avl_node_t *avl_find_min_child(avl_node_t *node) {
  if (!node->left) return node;
  return avl_find_min_child(node->left);
}

static void avl_swap_kv(avl_node_t *node1, avl_node_t *node2) {
  char *tmp_key = node1->pair.key;
  char *tmp_val = node1->pair.val;
  node1->pair.key = node2->pair.key;
  node1->pair.val = node2->pair.val;
  node2->pair.key = tmp_key;
  node2->pair.val = tmp_val;
}

avl_node_t *avl_map_del_node(avl_map_t *map, avl_node_t *node,
                             const avl_key_t key, bool drop_node) {
  if (!node) {
    return NULL;
  }
  avl_node_t *retNode = NULL;
  int order = map->cmp_fn(key, node->pair.key);
  if (order < 0) {
    node->left = avl_map_del_node(map, node->left, key, drop_node);
    retNode = node;
  } else if (order > 0) {
    node->right = avl_map_del_node(map, node->right, key, drop_node);
    retNode = node;
  } else {
    // order == 0
    if (!node->left) {
      avl_node_t *rightNode = node->right;
      map->size--;
      if (drop_node) {
        avl_node_drop(node);
      } else {
        node->left = NULL;
        node->right = NULL;
      }
      retNode = rightNode;
    } else if (!node->right) {
      avl_node_t *leftNode = node->left;
      map->size--;
      if (drop_node) {
        avl_node_drop(node);
      } else {
        node->left = NULL;
        node->right = NULL;
      }
      retNode = leftNode;
    } else {
      avl_node_t *successor = avl_find_min_child(node->right);
      successor->right =
          avl_map_del_node(map, node->right, successor->pair.key, false);
      successor->left = node->left;
      avl_swap_kv(successor, node);
      avl_node_drop(node);
      retNode = successor;
    }
  }
  if (!retNode) {
    return NULL;
  }
  retNode->height = 1 + avl_max(avl_node_height(retNode->left),
                                avl_node_height(retNode->right));
  int bf = avl_bf(retNode);
  // LL
  if (bf > 1 && avl_bf(retNode->left) >= 0) return avl_right_rotate(retNode);
  // RR
  if (bf < -1 && avl_bf(retNode->right) <= 0) return avl_left_rotate(retNode);
  // LR
  if (bf > 1 && avl_bf(retNode->left) < 0) {
    retNode->left = avl_left_rotate(retNode->left);
    return avl_right_rotate(retNode);
  }
  // RL
  if (bf < -1 && avl_bf(retNode->right) > 0) {
    retNode->right = avl_right_rotate(retNode->right);
    return avl_left_rotate(retNode);
  }
  return retNode;
}

static avl_node_t *avl_get_node_mut(avl_map_t *map, const avl_key_t key) {
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

bool avl_map_del(avl_map_t *map, const avl_key_t key) {
  avl_node_t *node = avl_get_node_mut(map, key);
  if (!node) return false;
  map->root = avl_map_del_node(map, map->root, key, true);
  return true;
}

#endif