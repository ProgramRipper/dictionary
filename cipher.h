#pragma once

void hash(const void *d, size_t cnt, unsigned char *md, unsigned int *s);
void encrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
void decrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
