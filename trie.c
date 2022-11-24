#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

#define N 26

/* Node */

typedef struct Node {
  void *value;
  struct Node *next[N];
} Node;

Node *Node_new(void *value) {
  Node *node = malloc(sizeof(Node));

  memset(node, 0, sizeof(Node));
  node->value = value;

  return node;
}

void Node_free(Node *node) {
  if (node == NULL) {
    return;
  }

  for (int i = 0; i < N; i++) {
    Node_free(node->next[i]);
  }

  free(node);
}

/* Trie */

struct Trie {
  int size;
  Node *root;
};

Trie *Trie_new(void) {
  Trie *trie = malloc(sizeof(Trie));

  trie->root = Node_new(NULL);

  return trie;
}

void Trie_free(Trie *trie) {
  if (trie == NULL) {
    return;
  }

  Node_free(trie->root);
  free(trie);
}

Node *_seek(Trie *trie, const char *key, const bool create) {
  Node *node = trie->root;

  for (int i = 0; i < strlen(key); i++) {
    int index = key[i] - 'a';

    if (node->next[index] == NULL) {
      if (create) {
        node->next[index] = Node_new(NULL);
      } else {
        return NULL;
      }
    }

    node = node->next[index];
  }

  return node;
}

int _walk(Node *node, void **values) {
  int n = 0;

  if (node->value != NULL) {
    values[n++] = node->value;
  }

  for (int i = 0; i < N; i++) {
    if (node->next[i] != NULL) {
      n += _walk(node->next[i], values + n);
    }
  }

  return n;
}

void *Trie_get(Trie *trie, const char *key) {
  Node *node = _seek(trie, key, false);
  return node == NULL ? NULL : node->value;
}

void Trie_set(Trie *trie, const char *key, void *value) {
  _seek(trie, key, true)->value = value;
}

void Trie_del(Trie *trie, const char *key) {
  Node *node = _seek(trie, key, false);

  if (node != NULL) {
    node->value = NULL;
  }
}

int Trie_startswith(Trie *trie, const char *prefix, void **out) {
  Node *node = _seek(trie, prefix, false);
  return node == NULL ? 0 : _walk(node, out);
}
