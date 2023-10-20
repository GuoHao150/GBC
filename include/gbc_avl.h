#ifndef _GBC_AVL_H
#define _GBC_AVL_H
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

typedef struct _avl_pair {
  char *key;
  char *val;
} avl_pair_t;

typedef struct _avl_node {
  size_t height;
  avl_pair_t pair;
  avl_node_t *parent;
  avl_node_t *left;
  avl_node_t *right;
} avl_node_t;

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
  t3->parent = y;
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
  t2->parent = y;
  avl_relink(x, parent_n, map);

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

static avl_node_t *avl_find_max_child(avl_node_t *node) {
  if (!node) return NULL;
  avl_node_t *_node = node;
  avl_node_t *out;
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

/// @brief swap the key-value pair in node1 to the pair in node2
/// @param node1
/// @param node2
static void avl_swap_kv(avl_node_t *node1, avl_node_t *node2) {
  char *tmp_key = node1->pair.key;
  char *tmp_val = node1->pair.val;
  node1->pair.key = node2->pair.key;
  node1->pair.val = node2->pair.val;
  node2->pair.key = tmp_key;
  node2->pair.val = tmp_val;
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
    return;
  }
  avl_node_t *parent_n = map->root;
  for (;;) {
    int order = map->cmp_fn(_key, parent_n->pair.key);
    if (order == 0) {
      avl_node_update(parent_n, _val, map->val_obj_size);
      return;
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
  avl_node_t *del_node = avl_get_node_mut(map, key);
  if (!del_node) {
    return false;
  }
  avl_node_t *fix_node = NULL;
  for (;;) {
    if (!del_node) {
      break;
    }
    avl_node_t *cur_parent = del_node->parent;
    avl_node_t *cur_left = del_node->left;
    avl_node_t *cur_right = del_node->right;
    int order = map->cmp_fn(cur_parent->pair.key, del_node->pair.key);

    if (cur_left && cur_right) {
      avl_node_t *succrssor_node = avl_find_min_child(cur_right);
      avl_swap_kv(del_node, succrssor_node);
      del_node = succrssor_node;
      continue;
    } else if (cur_left && !cur_right) {
      avl_relink(cur_left, cur_parent, map);
      avl_node_drop(del_node);
      map->size--;
      fix_node = cur_left;
      break;
    } else if (!cur_left && cur_right) {
      avl_relink(cur_right, cur_parent, map);
      avl_node_drop(del_node);
      fix_node = cur_right;
      break;
    } else {
      // !cur_right && !cur_left
      map->size--;
      if (!cur_parent) {
        map->root = NULL;
        avl_node_drop(del_node);
        fix_node = NULL;
        break;
      } else {
        if (order < 0)
          cur_parent->right = NULL;
        else
          cur_parent->left = NULL;
        avl_node_drop(del_node);
        fix_node = cur_parent;
        break;
      }
    }
  }
  avl_try_reblance(map, fix_node);
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

bool avl_map_set(avl_map_t *map, const avl_key_t key, const avl_val_t val) {
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
  for (int i = 0; i < map->size; ++i) {
    bool flag = avl_map_del_min(map);
    if (!flag) return false;
  }
  return true;
}

void avl_map_middle_order(const avl_map_t *map, avl_foreach fn) {
  _avl_map_middle_order(map->root, fn);
}

void _avl_map_middle_order(const avl_node_t *node, avl_foreach fn) {
  if (node) {
    _avl_map_middle_order(node->left, fn);
    fn(node->pair.key, node->pair.val);
    _avl_map_middle_order(node->right, fn);
  }
}

#endif