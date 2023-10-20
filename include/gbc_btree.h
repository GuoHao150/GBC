#ifndef _GBC_BTREE_H
#define _GBC_BTREE_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbc_deque.h"

typedef void *btree_key_t;
typedef void *btree_val_t;
typedef int (*btree_cmp_fn)(const void *, const void *);

typedef struct _bt_data {
  char *key;
  char *val;
} btree_data_t;

typedef struct btree_node_t {
  struct btree_node_t *parent;
  vdq_t *data_ptr_inner;     // vdq_t<btree_data_t>
  vdq_t *node_ptr_children;  // vdq_t<btree_node_t*>
} btree_node_t;

btree_node_t *bt_node_new(size_t key_obj_size, size_t val_obj_size) {
  vdq_t *inner_data = vdq_new_with_cap(sizeof(btree_data_t *), 4);
  vdq_t *children = vdq_new_with_cap(sizeof(btree_node_t *), 4);
  btree_node_t *node = (btree_node_t *)malloc(sizeof(btree_node_t));
  if (!inner_data || !children || !node) {
    if (inner_data) {
      free(inner_data);
      inner_data = NULL;
    }
    if (children) {
      free(children);
      children = NULL;
    }
    if (node) {
      free(node);
      node = NULL;
    }
    return NULL;
  }
  node->parent = NULL;
  node->data_ptr_inner = inner_data;
  node->node_ptr_children = children;
  return node;
}

bool bt_node_drop(btree_node_t *node) {
  if (!node) return false;
  if (node->node_ptr_children->size != 0)
    return false;  // only drop the node when no children for it
  vdq_t *inner_data = node->data_ptr_inner;
  for (int i = 0; i < inner_data->size; ++i) {
    btree_data_t **d = (btree_data_t **)vdq_at_mut(inner_data, i);
    if (d) {
      if (*d) {
        btree_data_t *di = *d;
        if (di->key) free(di->key);
        if (di->val) free(di->val);
      }
    }
  }
  if (node->node_ptr_children) vdq_drop(node->node_ptr_children);
  if (node->data_ptr_inner) vdq_drop(node->data_ptr_inner);
  free(node);
  node = NULL;
  return true;
}

typedef struct _btree_map {
  btree_node_t *root;
  btree_cmp_fn cmp;
  size_t key_obj_size;
  size_t val_obj_size;
  size_t max_key_num;
  size_t min_key_num;
} bt_map_t;

bt_map_t *bt_map_new(size_t key_obj_size, size_t val_obj_size,
                     btree_cmp_fn cmp) {
  size_t order = 3;
  size_t max_c = order + 1;
  size_t min_c = ((max_c + 1) / 2) - 1;
  bt_map_t *map = (bt_map_t *)malloc(sizeof(bt_map_t));
  if (!map) return NULL;
  map->root = NULL;
  map->cmp = cmp;
  map->key_obj_size = key_obj_size;
  map->val_obj_size = val_obj_size;
  map->max_key_num = max_c;
  map->min_key_num = min_c;
  return map;
}

btree_node_t *bt_adding_data(btree_node_t *node,
                             const btree_data_t *new_data_ptr, bt_map_t *map) {
  if (!node || !node->data_ptr_inner || node->data_ptr_inner->size == 0) {
    btree_node_t *new_node = bt_node_new(map->key_obj_size, map->val_obj_size);
    if (!new_node) return NULL;
    vdq_push_back(new_node->data_ptr_inner, new_data_ptr);
    return new_node;
  }
  vdq_t *inner_data = node->data_ptr_inner;
  size_t data_size = inner_data->size;
  for (int i = 0; i < data_size; ++i) {
    const btree_data_t **cur_data_ptr = (btree_data_t **)vdq_at(inner_data, i);
    int order = map->cmp((*cur_data_ptr)->key, new_data_ptr->key);
    if (order == 0) {
      memcpy((*cur_data_ptr)->val, new_data_ptr->val, map->val_obj_size);
      return node;
    } else if (order < 0) {
      if (i < data_size - 1) {
        continue;
      } else {
        vdq_insert(inner_data, data_size, new_data_ptr);
        return node;
      }
    } else {
      vdq_insert(inner_data, i, new_data_ptr);
      return node;
    }
  }
}

bool bt_map_add(bt_map_t *map, const btree_key_t key, const btree_val_t val) {
  btree_data_t *new_data = (btree_data_t *)malloc(sizeof(btree_data_t));
  char *new_key_buf = (char *)malloc(map->key_obj_size);
  char *new_val_buf = (char *)malloc(map->val_obj_size);
  if (!new_data || !new_key_buf || !new_val_buf) {
    if (new_data) free(new_data);
    if (new_key_buf) free(new_key_buf);
    if (new_val_buf) free(new_val_buf);
    return false;
  }
  memcpy(new_key_buf, key, map->key_obj_size);
  memcpy(new_val_buf, val, map->val_obj_size);
  new_data->key = new_key_buf;
  new_data->val = new_val_buf;
  btree_node_t *cur_node = map->root;
  for (;;) {
    // btree_adding_data
  }
}

#endif