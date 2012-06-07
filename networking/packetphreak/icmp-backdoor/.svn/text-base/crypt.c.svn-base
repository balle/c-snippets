#include <openssl/blowfish.h>

void decrypt(char *inbuf, char *outbuf, int blklen, char *key)
{
  int counter = 0;
  char iv[8];
  BF_KEY keyStruct;

  memset(iv, 0, 8);
  BF_set_key(&keyStruct, strlen(key), key);
  BF_cfb64_encrypt(inbuf, outbuf, blklen, &keyStruct,
                   iv, &counter, BF_DECRYPT);
}

void encrypt(char *inbuf, char *outbuf, int blklen, char *key)
{
  int counter = 0;
  char iv[8];
  BF_KEY keyStruct;

  memset(iv, 0, 8);
  BF_set_key(&keyStruct, strlen(key), key);
  BF_cfb64_encrypt(inbuf, outbuf, blklen, &keyStruct,
                   iv, &counter, BF_ENCRYPT);
}
