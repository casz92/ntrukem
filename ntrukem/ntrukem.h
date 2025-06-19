#ifndef NTRUKEM_H
#define NTRUKEM_H

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int crypto_kem_keypair_from_seed(unsigned char *pk, unsigned char *sk, unsigned char *seed, int size);

int crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);

int crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);

#endif
