#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

#define N 26 // 字母表大小，26 个小写字母

/* Node */

typedef struct Node {   // 字典树节点
  void *value;          // 值
  struct Node *next[N]; // 子节点
} Node;

/***************************************************************
 * @brief: 创建节点
 * @param: value: 值
 * @return: 节点
 ***************************************************************/
Node *Node_new(void *value) {
  Node *node = malloc(sizeof(Node));

  memset(node, 0, sizeof(Node)); // 初始化为 0
  node->value = value;

  return node;
}

/***************************************************************
 * @brief: 释放节点
 * @param: node: 节点
 ***************************************************************/
void Node_free(Node *node) {
  if (node == NULL) { // 避免 free(NULL)
    return;
  }

  for (int i = 0; i < N; i++) { // 递归释放子节点
    Node_free(node->next[i]);
  }

  free(node); // 释放本节点
}

/* Trie */

struct Trie { // 字典树
  Node *root; // 根节点
};

Trie *Trie_new(void) {
  Trie *trie = malloc(sizeof(Trie)); // 创建字典树

  trie->root = Node_new(NULL); // 创建根节点

  return trie;
}

void Trie_free(Trie *trie) {
  if (trie == NULL) { // 避免 free(NULL)
    return;
  }

  Node_free(trie->root); // 释放根节点
  free(trie);            // 释放字典树
}

/***************************************************************
 * @brief: 获得键对应的节点，仅供内部使用
 * @param: trie: 字典树
 * @param: key: 键
 * @param: create: 如果不存在，是否创建
 * @return: 节点
 ***************************************************************/
Node *_seek(Trie *trie, const char *key, const bool create) {
  Node *node = trie->root;

  for (int i = 0; i < strlen(key); i++) {
    int index = key[i] - 'a';

    if (node->next[index] == NULL) {
      if (create) { // 如果 create 为 true，则沿途创建节点，否则返回 NULL
        node->next[index] = Node_new(NULL);
      } else {
        return NULL;
      }
    }

    node = node->next[index];
  }

  return node;
}

/***************************************************************
 * @brief: 递归遍历子节点，仅供内部使用
 * @param: node: 节点
 * @param: values: 值数组
 * @return: 节点数量
 ***************************************************************/
int _walk(Node *node, void **values) {
  int n = 0;

  if (node->value != NULL) { // 如果节点有值，则将值添加到数组中
    values[n++] = node->value;
  }

  for (int i = 0; i < N; i++) { // 递归遍历子节点
    if (node->next[i] != NULL) {
      n += _walk(node->next[i], values + n);
    }
  }

  return n;
}

void *Trie_get(Trie *trie, const char *key) {
  Node *node = _seek(trie, key, false);
  return node == NULL ? NULL : node->value; // 如果节点不存在，则返回 NULL
}

void Trie_set(Trie *trie, const char *key, void *value) {
  _seek(trie, key, true)->value = value;
}

void Trie_del(Trie *trie, const char *key) {
  Node *node = _seek(trie, key, false);

  if (node != NULL) { // 如果节点不存在，则不做任何操作
    node->value = NULL;
  }
}

int Trie_startswith(Trie *trie, const char *prefix, void **out) {
  Node *node = _seek(trie, prefix, false);    // 获得前缀对应的节点
  return node == NULL ? 0 : _walk(node, out); // 遍历子节点
}
