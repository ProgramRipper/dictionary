#pragma once

typedef struct Trie Trie; // 字典树

/***************************************************************
 * @brief: 创建字典树
 * @return: 字典树
 ***************************************************************/
Trie *Trie_new(void);
/***************************************************************
 * @brief: 释放字典树
 * @param: trie: 字典树
 ***************************************************************/
void Trie_free(Trie *trie);
/***************************************************************
 * @brief: 获得键值对
 * @param: trie: 字典树
 * @param: key: 键
 * @return: 值
 ***************************************************************/
void *Trie_get(Trie *trie, const char *key);
/***************************************************************
 * @brief: 插入键值对
 * @param: trie: 字典树
 * @param: key: 键
 * @param: value: 值
 ***************************************************************/
void Trie_set(Trie *trie, const char *key, void *value);
/***************************************************************
 * @brief: 删除键值对
 * @param: trie: 字典树
 * @param: key: 键
 ***************************************************************/
void Trie_del(Trie *trie, const char *key);
/***************************************************************
 * @brief: 获得符合前缀的键值对
 * @param: trie: 字典树
 * @param: prefix: 前缀
 * @param: out: 值数组
 * @return: 键值对数量
 ***************************************************************/
int Trie_startswith(Trie *trie, const char *prefix, void **out);