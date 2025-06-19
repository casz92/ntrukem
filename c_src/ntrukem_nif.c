#include "api.h"
#include "ntrukem.h"
#include "tools.c"
#include <stdio.h>
#include <string.h>
#include "erl_nif.h"

static ERL_NIF_TERM
generate_keypair(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary pk_bin, sk_bin;
    if (argc != 0)
    {
        return enif_make_badarg(env);
    }

    unsigned char *pk, *sk;
    pk = xmalloc(NTRU_PUBLICKEYBYTES);
    sk = xmalloc(NTRU_SECRETKEYBYTES);

    memset(pk, 0, NTRU_PUBLICKEYBYTES);
    memset(sk, 0, NTRU_SECRETKEYBYTES);
    crypto_kem_keypair(pk, sk);

    if (enif_alloc_binary(NTRU_PUBLICKEYBYTES, &pk_bin))
    {
        memcpy(pk_bin.data, pk, NTRU_PUBLICKEYBYTES);
        if (enif_alloc_binary(NTRU_SECRETKEYBYTES, &sk_bin))
        {
            memcpy(sk_bin.data, sk, NTRU_SECRETKEYBYTES);
            xfree(pk);
            xfree(sk);
            return enif_make_tuple3(env, enif_make_atom(env, "ok"), enif_make_binary(env, &pk_bin), enif_make_binary(env, &sk_bin));
        }
    }
    xfree(pk);
    xfree(sk);
    return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, "mem_alloc"));
}

static ERL_NIF_TERM
derive_keypair(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary pk_bin, sk_bin, seed_bin;
    if (argc != 1 ||
        !enif_inspect_binary(env, argv[0], &seed_bin))
    {
        return enif_make_badarg(env);
    }

    unsigned char *pk, *sk;
    pk = xmalloc(NTRU_PUBLICKEYBYTES);
    sk = xmalloc(NTRU_SECRETKEYBYTES);

    memset(pk, 0, NTRU_PUBLICKEYBYTES);
    memset(sk, 0, NTRU_SECRETKEYBYTES);
    crypto_kem_keypair_from_seed(pk, sk, seed_bin.data, seed_bin.size);

    if (enif_alloc_binary(NTRU_PUBLICKEYBYTES, &pk_bin))
    {
        memcpy(pk_bin.data, pk, NTRU_PUBLICKEYBYTES);
        if (enif_alloc_binary(NTRU_SECRETKEYBYTES, &sk_bin))
        {
            memcpy(sk_bin.data, sk, NTRU_SECRETKEYBYTES);
            xfree(pk);
            xfree(sk);
            return enif_make_tuple3(env, enif_make_atom(env, "ok"), enif_make_binary(env, &pk_bin), enif_make_binary(env, &sk_bin));
        }
    }
    xfree(pk);
    xfree(sk);
    return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, "mem_alloc"));
}

static ERL_NIF_TERM
encapsulate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary pk_bin, key_bin, cipher_bin;

    if (argc != 1 ||
        !enif_inspect_binary(env, argv[0], &pk_bin))
    {
        return enif_make_badarg(env);
    }

    unsigned char *key, *cipher;
    key = xmalloc(NTRU_SHAREDKEYBYTES);
    cipher = xmalloc(NTRU_CIPHERTEXTBYTES);

    memset(key, 0, NTRU_SHAREDKEYBYTES);
    memset(cipher, 0, NTRU_CIPHERTEXTBYTES);
    crypto_kem_enc(cipher, key, pk_bin.data);

    if (enif_alloc_binary(NTRU_CIPHERTEXTBYTES, &cipher_bin))
    {
        memcpy(cipher_bin.data, cipher, NTRU_CIPHERTEXTBYTES);
        if (enif_alloc_binary(NTRU_SHAREDKEYBYTES, &key_bin))
        {
            memcpy(key_bin.data, key, NTRU_SHAREDKEYBYTES);
            xfree(key);
            xfree(cipher);

            return enif_make_tuple3(env, enif_make_atom(env, "ok"), enif_make_binary(env, &cipher_bin), enif_make_binary(env, &key_bin));
        }
    }
    xfree(key);
    xfree(cipher);
    return enif_make_atom(env, "error");
}

static ERL_NIF_TERM
decapsulate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary sk_bin, key_bin, cipher_bin;

    if (argc != 2 ||
        !enif_inspect_binary(env, argv[0], &sk_bin) ||
        !enif_inspect_binary(env, argv[1], &cipher_bin))
    {
        return enif_make_badarg(env);
    }

    unsigned char *key;
    key = xmalloc(NTRU_SHAREDKEYBYTES);

    memset(key, 0, NTRU_SHAREDKEYBYTES);
    int r = crypto_kem_dec(key, cipher_bin.data, sk_bin.data);

    if (r != 0)
    {
        return enif_make_atom(env, "error");
    }

    if (enif_alloc_binary(NTRU_SHAREDKEYBYTES, &key_bin))
    {
        memcpy(key_bin.data, key, NTRU_SHAREDKEYBYTES);
        xfree(key);

        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_binary(env, &key_bin));
    }
    xfree(key);
    return enif_make_atom(env, "error");
}

static ErlNifFunc nif_funcs[] = {
    {"generate_keypair", 0, generate_keypair},
    {"derive_keypair", 1, derive_keypair},
    {"encapsulate", 1, encapsulate},
    {"decapsulate", 2, decapsulate},
};

ERL_NIF_INIT(Elixir.NtruKem, nif_funcs, NULL, NULL, NULL, NULL)