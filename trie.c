#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

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

/* Trie */

struct Trie {
  Node *root;
};

Trie *Trie_new(void) {
  Trie *trie = malloc(sizeof(Trie));

  trie->root = Node_new(NULL);

  return trie;
}

void Trie_free(Trie *trie) {
  bool layer = false;
  int sizes[2] = {1, 0};
  Node *node, *stacks[2][SIZE] = {NULL};

  stacks[0][0] = trie->root;

  while (sizes[layer]) {
    for (int i = 0; i < sizes[layer]; i++) {
      node = stacks[layer][i];

      for (int j = 0; j < N; j++) {
        if (node->next[j] != NULL) {
          stacks[!layer][sizes[!layer]++] = node->next[j];
        }
      }

      free(node);
    }

    sizes[layer] = 0;
    layer = !layer;
  }

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
  int size = 0;

  if (node != NULL) {
    bool layer = false;
    int sizes[2] = {1, 0};
    Node *stacks[2][SIZE] = {NULL};

    stacks[0][0] = node;

    while (sizes[layer]) {
      for (int i = 0; i < sizes[layer]; i++) {
        node = stacks[layer][i];

        if (node->value != NULL) {
          out[size++] = node->value;
        }

        for (int j = 0; j < N; j++) {
          if (node->next[j] != NULL) {
            stacks[!layer][sizes[!layer]++] = node->next[j];
          }
        }
      }

      sizes[layer] = 0;
      layer = !layer;
    }
  }

  return size;
}
