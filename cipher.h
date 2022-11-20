#pragma once

void encrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
void decrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
