#include <string.h>

#include <openssl/evp.h>

#include "cipher.h"

int main() {

  unsigned char salt[8] = "ssaalltt", pwd[8] = "password", a[256], b[256],
                c[256];

  for (int i = 255; i >= 0; i--) {
    a[i] = i;
  }

  encrypt(salt, pwd, a, b);
  decrypt(salt, pwd, b, c);

  printf("%s\n", strcmp(a, c) ? "fail" : "pass");

  return 0;
}