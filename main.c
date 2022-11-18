#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>

#include "trie.h"

int main(int argc, char *argv[]) {
  Trie *trie = Trie_new();
  char *values[] = {"bar", "baz", "foo"};

  Trie_set(trie, "foo", values[0]);
  Trie_set(trie, "bar", values[1]);
  Trie_set(trie, "baz", values[2]);

  printf("%s\n", Trie_get(trie, "foo"));
  printf("%s\n", Trie_get(trie, "bar"));
  printf("%s\n", Trie_get(trie, "baz"));

  char *result[3] = {""};
  int n = Trie_startswith(trie, "b", values);

  for (int i = 0; i < n; i++) {
    printf("%d:%s\n", i, values[i]);
  }

  Trie_del(trie, "bar");
  Trie_get(trie, "bar");
  printf("%p\n", Trie_get(trie, "bar")); // null
  printf("%s\n", Trie_get(trie, "baz")); // not null

  Trie_free(trie);
}
