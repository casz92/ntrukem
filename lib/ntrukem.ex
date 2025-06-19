defmodule NtruKem do
  @compile {:autoload, false}
  @on_load :load_nifs

  def load_nifs do
    :code.priv_dir(:ntrukem)
    |> :filename.join(~c"ntrukem_nif")
    |> :erlang.load_nif(0)
  end

  @spec generate_keypair() :: {:ok, pubkey :: binary(), privkey :: binary()} | {:error, atom()}
  def generate_keypair, do: raise("Function generate_keypair is not implemented!")

  @spec derive_keypair(binary()) ::
          {:ok, pubkey :: binary(), privkey :: binary()} | {:error, atom()}
  def derive_keypair(_seed),
    do: raise("Function derive_keypair is not implemented!")

  @spec encapsulate(binary()) :: {:ok, ciphertext :: binary, sharedkey :: binary} | :error
  def encapsulate(_), do: raise("Function enc is not implemented!")

  @spec decapsulate(binary(), binary()) :: {:ok, sharekey :: binary()} | :error
  def decapsulate(_privkey, _ciphertext), do: raise("Function dec is not implemented!")
end
