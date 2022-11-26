#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"

#define CIPHER EVP_aes_256_cbc() // 加密算法，AES-256 的 CBC 模式
#define CIPHERMD EVP_sha512() // 根据密码生成 key 和 iv 的摘要算法，SHA-512
#define DIGESTMD EVP_md5() // 生成哈希值的摘要算法，MD5
#define ROUND 65536        // 生成 key 和 iv 的摘要算法迭代次数

void hash(const void *d, size_t cnt, unsigned char *md, unsigned int *s) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();     // 创建哈希上下文
  EVP_DigestInit_ex(ctx, DIGESTMD, NULL); // 初始化哈希上下文
  EVP_DigestUpdate(ctx, d, cnt);          // 更新哈希数据
  EVP_DigestFinal_ex(ctx, md, s);         // 计算哈希值
  EVP_MD_CTX_free(ctx);                   // 释放哈希上下文
}

void encrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = CIPHERMD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  // 用于 AES-256 的 key 和 iv
  // EVP_MAX_KEY_LENGTH：最大密钥长度，是个安全的冗余值
  // EVP_MAX_IV_LENGTH: 最大初始化向量长度，是个安全的冗余值
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); // 创建加密上下文

  EVP_BytesToKey(cipher, md, salt, pwd, pwdl, ROUND, key, iv);
  // 用密码生成 key 和 iv

  EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv); // 初始化加密上下文
  EVP_EncryptUpdate(ctx, out, &len, in, inl);     // 加密数据
  *outl = len;
  EVP_EncryptFinal_ex(ctx, out + *outl, &len); // 结束加密，将剩余数据加密
  *outl += len;

  EVP_CIPHER_CTX_free(ctx); // 释放加密上下文
}

void decrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = CIPHERMD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  EVP_BytesToKey(cipher, md, salt, pwd, pwdl, ROUND, key, iv);

  EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv); // 初始化解密上下文
  EVP_DecryptUpdate(ctx, out, &len, in, inl);     // 解密数据
  *outl = len;
  EVP_DecryptFinal_ex(ctx, out + *outl, &len); // 结束解密，将剩余数据解密
  *outl += len;

  EVP_CIPHER_CTX_free(ctx);
}
