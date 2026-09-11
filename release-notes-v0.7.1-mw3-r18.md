# DDrawCompat-MW3 v0.7.1-mw3-r18

First standalone release of the DDrawCompat compatibility fork used by
MechWarrior 3 Remastered v1.2.4.

The fork is based on upstream DDrawCompat v0.7.1 commit
`2c9a07fdf9308e2b0b117886a7e363b149ee1bc7`. The `mw3-r18` suffix identifies
this project's qualified MW3-specific revision and is not an upstream version.

Highlights:

- retains upstream 32-bit render-color-depth promotion, internal resolution
  scaling, 4x MSAA, anisotropic filtering, and borderless presentation;
- adds isolated base-game intro crop, matte, and dark-chroma cleanup;
- repairs the top/left gameplay presentation edges without modifying game data;
- provides composable Alt-Tab and cursor handling for borderless play; and
- recovers transient primary-surface loss during Pirate's Moon startup.

The release ZIP includes the qualified x86 `ddraw.dll`, separate MechWarrior 3
and Pirate's Moon profiles, installation instructions, license, and checksum.
It contains no game files.

Qualification:

- five standard launcher attempt-1 Pirate's Moon passes, including a fresh
  installed-style path;
- three additional direct launches from the exact installer-extracted tree,
  with no launcher fallback available;
- clean captured MicroProse frames on all three direct launches; and
- no new Microsoft Defender detection from the setup, extracted payload, or
  runnable Pirate's Moon tree.

Qualified DLL SHA-256:
`FD11B9B6B8A8CC23744DFEDC10E23798860F3A96BD8B2B4C75DD2FDB3BE8F8FB`
