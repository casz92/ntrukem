# NtruKem

`ntrukem` is a NIF C library for Elixir, implementing the post-quantum cryptography algorithm NTRU-HRSS-KEM. This algorithm is a fundamental piece for public-key encryption and secure key establishment in a quantum-resistant future.

## What is NTRU-HRSS-KEM?

NTRU-HRSS-KEM is a Key Encapsulation Mechanism (KEM) algorithm that belongs to the family of lattice-based cryptography. It offers robust security against quantum threats, making it a promising solution for protecting long-term communications.

## Features

This implementation provides essential functions for handling NTRU-HRSS-KEM:

| Feature      | Size (bytes) | Description                                    |
| :----------- | :----------- | :--------------------------------------------- |
| `Seed`       | 48           | Seed used for key generation.                  |
| `Public key` | 1138         | Public key for encryption.                     |
| `Secret key` | 1418         | Secret key for decryption.                     |
| `Ciphertext` | 1278         | Ciphertext resulting from encapsulation.       |
| `Shared Key` | 32           | Generated shared symmetric key.                |

## 🛠️ Compilation

To compile the NIF library, make sure you have `make` and `erlang-dev` installed on your system.

```bash
mix deps.get
mix compile
```

## 📦 Installation
Add ntrukem to your list of dependencies in mix.exs:

```elixir
def deps do
  [
    {:ntrukem, "~> 0.1.0"}
  ]
end
```

## Usage

```elixir
# Generate a key pair
{:ok, pk, sk} = NtruKem.generate_keypair()

# Encapsulate a shared key
{:ok, cipher, shared_key} = NtruKem.encapsulate(pk)

# Decapsulate the shared key
{:ok, sharedkey_dec} = NtruKem.decapsulate(sk, cipher)

# Verify the shared key
IO.puts(sharedkey_dec == shared_key)
```

## 🧪 Testing
To run the project tests:

```bash
mix run test/ntru_test.exs
```

## 📚 More Info
- [NTRU-KEM specification](https://joostrijneveld.nl/papers/ntrukem/20170711_ntrukem.pdf)

## ⚖️ License
This project is licensed under the MIT License. You can find the full details in the LICENSE file.