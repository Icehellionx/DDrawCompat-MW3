# DDrawCompat-MW3

DDrawCompat-MW3 is the standalone graphics compatibility component from
MechWarrior 3 Remastered. It does not contain either game, the official patch,
music, videos, manuals, or disc images.

## Provenance

- Upstream project: https://github.com/narzoul/DDrawCompat
- Upstream base: v0.7.1, commit `2c9a07fdf9308e2b0b117886a7e363b149ee1bc7`
- MW3 revision: `r18`
- Qualified DLL SHA-256: `FD11B9B6B8A8CC23744DFEDC10E23798860F3A96BD8B2B4C75DD2FDB3BE8F8FB`
- Qualified build: Release/x86 with Visual Studio 2022 Build Tools and Windows SDK 10.0.26100.0

The `r18` suffix belongs to this MW3 fork; it is not an upstream DDrawCompat
version.

## Installation

1. Back up any existing `ddraw.dll` and `DDrawCompat.ini` beside the game's
   executable.
2. Copy the release `ddraw.dll` beside `Mech3.exe` or `Mech3fixup.exe`.
3. Copy the appropriate profile from `profiles/` beside the executable and
   name it `DDrawCompat.ini`.
4. Start the game normally.

Use `profiles/MechWarrior3/DDrawCompat.ini` for the base game and
`profiles/PiratesMoon/DDrawCompat.ini` for the expansion. The profiles are not
interchangeable: the base game has intro-specific cleanup and edge repair,
while Pirate's Moon uses its separately qualified startup presentation policy.

To uninstall the wrapper, restore the files backed up in step 1 or remove the
fork's `ddraw.dll` and `DDrawCompat.ini`.

## Scope of the fork

The fork preserves DDrawCompat's upstream 32-bit render-color-depth promotion,
resolution scaling, MSAA, texture filtering, and borderless presentation. Its
MW3-specific changes include:

- isolated base-game `INTRO.AVI` crop, matte, and dark-chroma cleanup;
- output-only top/left gameplay edge repair;
- composable borderless Alt-Tab and cursor ownership behavior;
- bounded clipper and Direct3D initialization recovery; and
- bounded recovery when Pirate's Moon sees `DDERR_SURFACELOST` while attaching
  primary or depth surfaces during its startup transition.

The opening videos are not skipped or replaced.

## Support and diagnostics

This fork is qualified specifically for MechWarrior 3 and Pirate's Moon. It is
not presented as a general replacement for upstream DDrawCompat. Include the
game, Windows version, GPU, exact fork release, and the matching
`DDrawCompat-*.log` when reporting a compatibility problem.

Do not disable Microsoft Defender or create an antivirus exclusion. Verify the
published SHA-256 before running a downloaded binary.

## Synchronization policy

The primary remaster repository remains the source of truth for the two
end-user INI profiles. The hourly and manually runnable
`sync-remaster-profiles` workflow compares those public files with `profiles/`
and commits a synchronization update when they change. C++ renderer changes
are developed and reviewed in this fork.

Profile synchronization never creates a binary release automatically. A new
tag and downloadable DLL are published only after interactive game-launch and
capture qualification.
