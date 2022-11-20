#include <string.h>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include "cipher.h"

int main() {

  unsigned char salt[8], pwd[512] = {0}, a[1024] = {0}, b[1024] = {0},
                         c[1024] = {0};

  fgets(pwd, 512, stdin);
  printf("%s%ld\n", pwd, strlen(pwd));

  RAND_bytes(salt, 8);
  printf("%.8s\n", salt);

  for (char i = 0; i < 96; i++) {
    a[95 - i] = i + 32;
  }

  int inl = 96, outl = 0;
  
  encrypt(salt, pwd, strlen(pwd), a, inl, b, &outl);
  printf("加密结果长度：%d\n", outl);
  inl = outl;
  decrypt(salt, pwd, strlen(pwd), b, inl, c, &outl);
  printf("解密结果长度：%d\n", outl);

  printf("%s\n%s\n", a, c);
  printf("%s\n", memcmp(a, c, outl) ? "fail" : "pass");

  return 0;
}