#include <string.h>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include "cipher.h"

int main() {

  unsigned char salt[8], pwd[8] = "password", a[1024] = {0}, b[1024] = {0},
                         c[1024] = {0};

  RAND_bytes(salt, 8);

  for (char i = 0; i < 96; i++) {
    a[i] = i + 32;
  }
  int inl = 96, outl = 0;
  encrypt(salt, pwd, a, inl, b, &outl);
  printf("加密结果长度：%d\n", outl);
  inl = outl;
  decrypt(salt, pwd, b, inl, c, &outl);

  printf("%s\n%s\n", a, c);
  printf("%s\n", memcmp(a, c, 96) ? "fail" : "pass");

  return 0;
}