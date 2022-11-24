#pragma once

typedef struct Trie Trie;

Trie *Trie_new(void);
void Trie_free(Trie *trie);
void *Trie_get(Trie *trie, const char *key);
void Trie_set(Trie *trie, const char *key, void *value);
void Trie_del(Trie *trie, const char *key);
int Trie_startswith(Trie *trie, const char *prefix, void **out);