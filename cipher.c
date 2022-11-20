#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"

#define CIPHER EVP_aes_256_cbc()
#define MD EVP_sha512()
#define ROUND 65536

void encrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = MD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  EVP_BytesToKey(cipher, md, salt, pwd, strlen(pwd), ROUND, key, iv);

  EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);
  EVP_EncryptUpdate(ctx, out, &len, in, inl);
  *outl = len;
  EVP_EncryptFinal_ex(ctx, out + *outl, &len);
  *outl += len;

  EVP_CIPHER_CTX_free(ctx);
}

void decrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, int inl, unsigned char *out, int *outl) {
  int len;
  const EVP_CIPHER *cipher = CIPHER;
  const EVP_MD *md = MD;
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  EVP_BytesToKey(cipher, md, salt, pwd, strlen(pwd), ROUND, key, iv);

  EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);
  EVP_DecryptUpdate(ctx, out, &len, in, inl);
  *outl = len;
  EVP_DecryptFinal_ex(ctx, out + *outl, &len);
  *outl += len;

  EVP_CIPHER_CTX_free(ctx);
}
