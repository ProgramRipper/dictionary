#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"
#include "trie.h"

#define SALT 8
#define SIZE 32

Trie *trie;
bool modified = false;
char filename[4096] = "dict.dat", pwd[4096] = {0};

typedef struct Word {
  char eng[8];
  char chn[32];
} Word;

void add() {
  if (*pwd) {
    char c, password[4096] = {0};
    int len = 0;

    printf("请输入密码：");
    scanf("%*.");
    while ((c = getchar()) != '\n' && c != EOF && len < 4096) {
      password[len++] = c;
    }
    if (strcmp(password, pwd) != 0) {
      printf("密码错误！\n");
      return;
    }
  }

  Word *word = malloc(sizeof(Word));

  printf("单词：");
  scanf("%s", word->eng);

  if (Trie_get(trie, word->eng) != NULL) {
    printf("单词已存在！\n");
    free(word);
    return;
  }

  printf("释义：");
  scanf("%s", word->chn);

  Trie_set(trie, word->eng, word);
  modified = true;
}

void show() {
  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);
  for (int i = 0; i < n; i++) {
    printf("%s: %s\n", words[i]->eng, words[i]->chn);
  }
}

void edit() {
  if (*pwd) {
    char c, password[4096] = {0};
    int len = 0;

    printf("请输入密码：");
    scanf("%*.");
    while ((c = getchar()) != '\n' && c != EOF && len < 4096) {
      password[len++] = c;
    }
    if (strcmp(password, pwd) != 0) {
      printf("密码错误！\n");
      return;
    }
  }

  char eng[8];

  printf("单词：");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  printf("释义：");
  scanf("%s", word->chn);
  modified = true;
}

void del() {
  if (*pwd) {
    char c, password[4096] = {0};
    int len = 0;

    printf("请输入密码：");
    scanf("%*.");
    while ((c = getchar()) != '\n' && c != EOF && len < 4096) {
      password[len++] = c;
    }
    if (strcmp(password, pwd) != 0) {
      printf("密码错误！\n");
      return;
    }
  }

  char eng[8];

  printf("单词：");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  Trie_del(trie, eng);
  free(word);
  modified = true;
}

void find() {
  char eng[8];

  printf("单词：");
  scanf("%s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  printf("释义：%s\n", word->chn);
}

void save() {
  FILE *file = fopen(filename, "wb");

  if (file == NULL) {
    printf("无法打开文件！\n");
    return;
  }

  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words), pwdl = 0, inl = 0, outl,
      saltl;
  char c, in[4096] = {0}, out[4096] = {0}, salt[EVP_MAX_MD_SIZE] = {0};

  printf("请设置密码：");
  scanf("%*.");
  memset(pwd, 0, sizeof(pwd));
  while ((c = getchar()) != '\n' && c != EOF && pwdl < 4096) {
    pwd[pwdl++] = c;
  }

  for (int i = 0; i < n; i++) {
    inl += sprintf(in + inl, "%s %s\n", words[i]->eng, words[i]->chn);
  }

  hash(in, inl, salt, &saltl);
  encrypt(NULL, pwd, pwdl, in, inl, out, &outl);

  fwrite(salt, 1, EVP_MAX_MD_SIZE, file);
  fputs(out, file);

  fclose(file);
  modified = false;
}

void exit_() {
  if (modified) {
    printf("词典已修改，是否保存？(Y/n) ");
    char c = getchar();
    scanf("%*[^\n]");
    if (c != 'n' || c != 'N') {
      save();
    }
  }

  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);

  for (int i = 0; i < n; i++) {
    free(words[i]);
  }
  Trie_free(trie);

  exit(0);
}

void prefix_find() {
  char eng[8];

  printf("前缀：");
  scanf("%s", eng);

  Word *words[SIZE];
  int n = Trie_startswith(trie, eng, (void **)words);

  if (n == 0) {
    printf("无法找到符合此前缀的单词！\n");
    return;
  }

  for (int i = 0; i < n; i++) {
    printf("%s: %s\n", words[i]->eng, words[i]->chn);
  }
}

void open() {
  if (modified) {
    printf("词典已修改，是否保存？(Y/n) ");
    char c = getchar();
    scanf("%*[^\n]");
    if (c != 'n' || c != 'N') {
      save();
    }
  }

  printf("文件名：");
  scanf("%s", filename);

  FILE *file = fopen(filename, "rb");

  if (file == NULL) {
    printf("无法打开文件！\n");
    return;
  }

  int pwdl = 0, inl = 0, outl, saltl;
  char c, in[4096] = {0}, out[4096] = {0}, salt1[EVP_MAX_MD_SIZE] = {0},
          salt2[EVP_MAX_MD_SIZE] = {0};

  printf("请输入密码：");
  scanf("%*.");
  memset(pwd, 0, sizeof(pwd));
  while ((c = getchar()) != '\n' && c != EOF) {
    pwd[pwdl++] = c;
  }

  fread(salt1, 1, EVP_MAX_MD_SIZE, file);
  inl = fread(in, 1, 4096, file);

  decrypt(NULL, pwd, pwdl, in, inl, out, &outl);
  hash(out, outl, salt2, &saltl);

  if (memcmp(salt1, salt2, EVP_MAX_MD_SIZE) != 0) {
    printf("密码错误！\n");
    return;
  }

  Trie_free(trie);
  trie = Trie_new();

  outl = 0;
  while (true) {
    Word *word = malloc(sizeof(Word));
    char buf[4096];
    sscanf(out + outl, "%[^\n]", buf);
    if (sscanf(buf, "%s %s\n", word->eng, word->chn) != 2) {
      free(word);
      break;
    }
    outl += strlen(buf) + 1;
    Trie_set(trie, word->eng, word);
  }

  fclose(file);
  modified = false;
}

int main() {
  trie = Trie_new();

  while (true) {
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
i: 打开文件\n\
%s > \
",
           filename);

    while (!('a' <= (cmd = getchar()) && cmd <= 'z')) // 读取第一个有效字符
      ;
    scanf("%*[^\n]"); // 丢弃输入缓冲区中剩余的字符
    switch (cmd) {
    case 'a': // 词条录入
      add();
      break;
    case 'b': // 信息显示
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
    case 'i': // 打开文件
      open();
      break;
    default:
      printf("无效命令！\n");
      break;
    }
    system("pause");
    system("cls");
  }

  return 0;
}