{
  description = "Nuklear Toolkit";

  inputs.expidus-sdk = {
    url = github:ExpidusOS/sdk;
  };

  outputs = { self, expidus-sdk }: expidus-sdk.libExpidus.mkFlake { inherit self; name = "ntk"; };
}
