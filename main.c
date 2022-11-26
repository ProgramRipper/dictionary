#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"
#include "trie.h"

#define SIZE 32 // 单词数量

typedef struct Word { // 单词
  char eng[8];        // 英文单词
  char chn[32];       // 中文释义
} Word;

#define BUF_SIZE SIZE * sizeof(Word) + EVP_MAX_BLOCK_LENGTH // 缓冲区大小
// EVP_MAX_BLOCK_LENGTH:
// 加密后的数据长度不会超过原数据长度加上此值，所以是个安全的冗余值

Trie *trie; // 字典树，同一时间只能使用一个字典，所以是全局变量
bool modified = false;                             // 字典是否被修改过
char filename[4096] = "dict.dat", pwd[4096] = {0}; // 字典文件名和密码

// 词条录入
void add() {
  if (*pwd) { // 如果密码为空，则不需要输入密码
    char c, password[4096] = {0};
    int len = 0;

    scanf("%*[^\n]"); // 清空输入缓冲区，防止输入密码时误读取回车
    getchar();
    printf("请输入密码：");
    while ((c = getchar()) != '\n' && c != EOF && len < 4096) {
      password[len++] = c;
    } // 支持读取空格或空行
    if (strcmp(password, pwd) != 0) {
      printf("密码错误！\n");
      return;
    }
  }

  Word *word = malloc(sizeof(Word));

  printf("单词：");
  scanf("%7s", word->eng);

  if (Trie_get(trie, word->eng) != NULL) { // 尝试获得单词
    printf("单词已存在！\n");
    free(word);
    return;
  }

  printf("释义：");
  scanf("%31s", word->chn);

  Trie_set(trie, word->eng, word);
  modified = true;
}

// 信息显示
void show() {
  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words); // 获取所有单词

  system("cls");               // 清屏，方便查看
  printf("单词     | 释义\n"); // 输出表头
  for (int i = 0; i < n; i++) {
    printf("%-8s | %-32s\n", words[i]->eng, words[i]->chn);
  }
}

// 词条修改
void edit() {
  if (*pwd) {
    char c, password[4096] = {0};
    int len = 0;

    scanf("%*[^\n]");
    getchar();
    printf("请输入密码：");
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
  scanf("%7s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  printf("释义：");
  scanf("%31s", word->chn);
  modified = true;
}

// 词条删除
void del() {
  if (*pwd) {
    char c, password[4096] = {0};
    int len = 0;

    scanf("%*[^\n]");
    getchar();
    printf("请输入密码：");
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
  scanf("%7s", eng);

  Word *word = Trie_get(trie, eng); // 获得单词

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  Trie_del(trie, eng);
  free(word);
  modified = true;
}

// 单词查询
void find() {
  char eng[8];

  printf("单词：");
  scanf("%7s", eng);

  Word *word = Trie_get(trie, eng);

  if (word == NULL) {
    printf("单词不存在！\n");
    return;
  }

  printf("释义：%s\n", word->chn);
}

// 信息保存
void save() {
  FILE *file = fopen(filename, "wb");

  if (file == NULL) {
    printf("无法打开文件！\n");
    return;
  }

  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words), pwdl = 0, inl = 0, outl,
      saltl;
  // pwdl: 密码长度，inl: 明文长度，outl: 密文长度，saltl: 盐长度
  char c, in[BUF_SIZE] = {0}, out[BUF_SIZE] = {0}, salt[EVP_MAX_MD_SIZE] = {0};
  // in: 明文，out: 密文，EVP_MAX_MD_SIZE: 最大散列长度，是个安全的冗余值

  scanf("%*[^\n]");
  getchar();
  printf("请设置密码：");
  memset(pwd, 0, sizeof(pwd)); // 清空密码
  while ((c = getchar()) != '\n' && c != EOF && pwdl < 4096) {
    pwd[pwdl++] = c;
  }

  for (int i = 0; i < n; i++) {
    inl += sprintf(in + inl, "%s %s\n", words[i]->eng, words[i]->chn);
  } // 将字典序列化

  hash(in, inl, salt, &saltl);                   // 生成盐
  encrypt(NULL, pwd, pwdl, in, inl, out, &outl); // 加密

  fwrite(salt, 1, EVP_MAX_MD_SIZE, file); // 写入盐，用于读取时验证密码
  fputs(out, file);                       // 写入密文

  fclose(file);
  modified = false; // 保存成功，标记为未修改
}

// 退出系统
void exit_() {
  if (modified) { // 有修改未保存
    printf("词典已修改，是否保存？(Y/n) ");
    char c = getchar();
    scanf("%*[^\n]"); // 清空输入缓冲区
    if (c != 'n' || c != 'N') {
      save();
    } // 默认保存
  }

  Word *words[SIZE];
  int n = Trie_startswith(trie, "\0", (void **)words);

  for (int i = 0; i < n; i++) {
    free(words[i]);
  }                // 清理单词
  Trie_free(trie); // 清理字典

  exit(0); // 退出程序
}

// 模糊查询
void prefix_find() {
  char eng[8];

  printf("前缀：");
  scanf("%s", eng);

  Word *words[SIZE];
  int n = Trie_startswith(trie, eng, (void **)words); // 获得符合前缀的单词

  system("cls");
  printf("单词     | 释义\n");
  for (int i = 0; i < n; i++) {
    printf("%-8s | %-32s\n", words[i]->eng, words[i]->chn);
  }
}

// 打开文件
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
    printf("文件不存在，是否创建？(Y/n) ");
    char c = getchar();
    scanf("%*[^\n]"); // 清空输入缓冲区
    if (c != 'n' || c != 'N') {
      Trie_free(trie);
      trie = Trie_new();
      save();
    } // 默认创建
    return;
  }

  int pwdl = 0, inl = 0, outl, saltl;
  // saltl: salt2 长度
  char c, in[BUF_SIZE] = {0}, out[BUF_SIZE] = {0}, salt1[EVP_MAX_MD_SIZE] = {0},
          salt2[EVP_MAX_MD_SIZE] = {0};
  // salt1: 从文件读取的盐，salt2: 生成的盐

  scanf("%*[^\n]");
  getchar();
  printf("请输入密码：");
  memset(pwd, 0, sizeof(pwd));
  while ((c = getchar()) != '\n' && c != EOF) {
    pwd[pwdl++] = c;
  }

  fread(salt1, 1, EVP_MAX_MD_SIZE, file); // 读取 salt1
  inl = fread(in, 1, BUF_SIZE, file);     // 读取密文

  decrypt(NULL, pwd, pwdl, in, inl, out, &outl); // 解密
  hash(out, outl, salt2, &saltl);                // 生成 salt2

  if (memcmp(salt1, salt2, EVP_MAX_MD_SIZE) != 0) { // 验证密码
    printf("密码错误！\n");
    return;
  }

  Trie_free(trie);
  trie = Trie_new();

  outl = 0;
  while (true) {
    Word *word = malloc(sizeof(Word));
    char buf[64];

    sscanf(out + outl, "%[^\n]", buf);
    if (sscanf(buf, "%s %s\n", word->eng, word->chn) != 2) { // 反序列化字典
      free(word);
      break;
    }
    outl += strlen(buf) + 1;
    Trie_set(trie, word->eng, word);
  }

  fclose(file);
  modified = false;
}

// 主函数
int main() {
  trie = Trie_new(); // 初始化字典树

  while (true) { // 主循环
    char cmd;    // 命令

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
           filename); // 命令列表，%s 为当前操作的字典文件名

    while (!('a' <= (cmd = getchar()) && cmd <= 'z')) // 读取第一个有效字符
      ;
    scanf("%*[^\n]");
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
    system("pause"); // 暂停，便于查看输出
    system("cls");   // 清屏
  }

  return 0;
}