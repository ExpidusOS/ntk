{
  description = "Nuklear Toolkit";

  inputs.expidus-sdk = {
    url = github:ExpidusOS/sdk;
  };

  inputs.nuklear = {
    url = github:Immediate-Mode-UI/Nuklear;
    flake = false;
  };

  outputs = { self, expidus-sdk, nuklear }:
    with expidus-sdk.lib;
    expidus.flake.makeOverride {
      self = expidus.flake.makeSubmodules self {
        "nuklear/src/nuklear" = nuklear;
      };
      name = "ntk";
    };
}
