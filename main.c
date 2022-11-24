#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "trie.h"

#define SIZE 32

Trie *trie;

typedef struct Word {
  char eng[8];
  char chn[32];
} Word;

void add() {
  Word *word = malloc(sizeof(Word));

  printf("Word: ");
  scanf("%s", word->eng);

  if (Trie_get(trie, word->eng) != NULL) {
    printf("Word already exists!\n");
    free(word);
    return;
  }

  printf("Meaning: ");
  scanf("%s", word->chn);

  Trie_set(trie, word->eng, word);
}

void show() { // FIXME: 有点玄学问题
  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);

  for (int i = 0; i < n; i++) {
    printf("%s: %s\n", words[i]->eng, words[i]->chn);
  }
}

void edit() {
  char eng[8];

  printf("Word: ");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("Word not found!\n");
    return;
  }

  printf("Meaning: ");
  scanf("%s", word->chn);
}

void del() {
  char eng[8];

  printf("Word: ");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("Word not found!\n");
    return;
  }

  Trie_del(trie, eng);
  free(word);
}

void find() {
  char eng[8];

  printf("Word: ");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("Word not found!\n");
    return;
  }

  printf("Meaning: %s\n", word->chn);
}

void save() {
  FILE *file = fopen("dict.dat", "w");

  if (file == NULL) {
    printf("Failed to open file!\n");
    return;
  }

  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);

  for (int i = 0; i < n; i++) {
    fprintf(file, "%s %s\n", words[i]->eng, words[i]->chn);
  }

  fclose(file);
}

void exit_() {
  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);

  save();

  for (int i = 0; i < n; i++) {
    free(words[i]);
  }
  Trie_free(trie);

  exit(0);
}

void prefix_find() {
  char eng[8];

  printf("Prefix: ");
  scanf("%s", eng);

  Word *words[SIZE];
  int n = Trie_startswith(trie, eng, (void **)words);

  if (n == 0) {
    printf("Word not found!\n");
    return;
  }

  for (int i = 0; i < n; i++) {
    printf("%s: %s\n", words[i]->eng, words[i]->chn);
  }
}

int main() {
  SetConsoleOutputCP(65001);

  FILE *fp = fopen("dict.dat", "r");

  trie = Trie_new();

  if (fp != NULL) {
    while (!feof(fp)) {
      Word *word = malloc(sizeof(Word));
      fscanf(fp, "%s %s", word->eng, word->chn);
      Trie_set(trie, word->eng, word);
    }
    fclose(fp);
  }

  while (1) {
    char cmd;

    printf("\
a: 词条录入\n\
b: 信息显示\n\
c: 词条修改\n\
d: 词条删除\n\
e: 单词查询\n\
f: 信息保存\n\
g: 退出系统\n\
h: 模糊查询\n\
i: 清空屏幕\n\
> \
");

    while (!('a' <= (cmd = getchar()) && cmd <= 'z'))
      ;
    scanf("%*[^\n]%*c"); // ???
    switch (cmd) {
    case 'a': // 词条录入
      add();
      break;
    case 'b': // 信息显示
      // printf("bbb");
      show();
      break;
    case 'c': // 词条修改
      edit();
      break;
    case 'd': // 词条删除
      del();
      break;
    case 'e': // 单词查询
      find();
      break;
    case 'f': // 信息保存
      save();
      break;
    case 'g': // 退出系统
      exit_();
      break;
    case 'h': // 模糊查询
      prefix_find();
      break;
    case 'i': // 清空屏幕
      system("cls");
      break;
    default:
      printf("Invalid command!\n");
      break;
    }
  }

  return 0;
}