#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"

#define CIPHER EVP_aes_256_cbc()
#define CIPHERMD EVP_sha512()
#define DIGESTMD EVP_md5()
#define ROUND 65536

void hash(const void *d, size_t cnt, unsigned char *md, unsigned int *s) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, DIGESTMD, NULL);
  EVP_DigestUpdate(ctx, d, cnt);
  EVP_DigestFinal_ex(ctx, md, s);
  EVP_MD_CTX_free(ctx);
}

void encrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = CIPHERMD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  EVP_BytesToKey(cipher, md, salt, pwd, pwdl, ROUND, key, iv);

  EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);
  EVP_EncryptUpdate(ctx, out, &len, in, inl);
  *outl = len;
  EVP_EncryptFinal_ex(ctx, out + *outl, &len);
  *outl += len;

  EVP_CIPHER_CTX_free(ctx);
}

void decrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = CIPHERMD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  EVP_BytesToKey(cipher, md, salt, pwd, pwdl, ROUND, key, iv);

  EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);
  EVP_DecryptUpdate(ctx, out, &len, in, inl);
  *outl = len;
  EVP_DecryptFinal_ex(ctx, out + *outl, &len);
  *outl += len;

  EVP_CIPHER_CTX_free(ctx);
}
