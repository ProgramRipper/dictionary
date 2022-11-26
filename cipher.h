#pragma once

/***************************************************************
 * @brief: 计算哈希值，用于验证数据完整性
 * @param: d: 待计算哈希值的数据
 * @param: cnt: 数据长度
 * @param: md: 哈希值
 * @param: s: 哈希值长度
 ***************************************************************/
void hash(const void *d, size_t cnt, unsigned char *md, unsigned int *s);
/***************************************************************
 * @brief: 加密数据
 * @param: salt: 盐值
 * @param: pwd: 密码
 * @param: pwdl: 密码长度
 * @param: in: 待加密数据
 * @param: inl: 待加密数据长度
 * @param: out: 加密后数据
 * @param: outl: 加密后数据长度
 ***************************************************************/
void encrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
/***************************************************************
 * @brief: 解密数据
 * @param: salt: 盐值
 * @param: pwd: 密码
 * @param: pwdl: 密码长度
 * @param: in: 待解密数据
 * @param: inl: 待解密数据长度
 * @param: out: 解密后数据
 * @param: outl: 解密后数据长度
 ***************************************************************/
void decrypt(const unsigned char *salt, const unsigned char *pwd, int pwdl,
             const unsigned char *in, int inl, unsigned char *out, int *outl);
