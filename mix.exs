defmodule NtruKem.MixProject do
  use Mix.Project

  @version "0.1.0"

  def project do
    [
      app: :ntrukem,
      version: @version,
      elixir: "~> 1.3",
      start_permanent: Mix.env() == :prod,
      compilers: [:elixir_make] ++ Mix.compilers(),
      deps: deps(),
      package: package()
    ]
  end

  def application do
    []
  end

  defp deps do
    [
      {:elixir_make, "~> 0.4", runtime: false},
      {:ex_doc, "~> 0.31", only: :dev, runtime: false}
    ]
  end

  defp package do
    [
      name: "ntrukem",
      description: "Post-quantum NTRU Key Encapsulation Mechanism (NTRU KEM) NIF for Elixir",
      files: ~w(lib c_src mix.exs README* LICENSE* Makefile),
      maintainers: ["Carlos Suarez"],
      licenses: ["MIT"],
      links: %{"GitHub" => "https://github.com/casz92/ntrukem"}
    ]
  end
end
