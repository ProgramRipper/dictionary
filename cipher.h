#pragma once

void encrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, unsigned char *out);
void decrypt(const unsigned char *salt, const unsigned char *pwd,
             const unsigned char *in, unsigned char *out);
