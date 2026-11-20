# EctoPad

> [!IMPORTANT]
> **EctoPad is paused until further notice.** There are no builds, downloads or
> support, and PadMint no longer offers it. Please don't ask the Metaforce
> developers for help with EctoPad.

EctoPad was an Apple port layer for [Metaforce](https://github.com/AxioDL/metaforce),
the native Metroid Prime engine. It added touch controls, on-device game import,
native settings and packaging for iPhone, iPad and Apple Silicon Macs.

## Credit

Metaforce is the engine, and it does the work that makes Metroid Prime run.
It was originally written by Jack Andersen and Phillip "Antidote" Stephens and
is developed by the Metaforce contributors. Luke Street (encounter) built
Metaforce's iOS and tvOS support and [Aurora](https://github.com/encounter/aurora),
the rendering and platform layer underneath it. EctoPad would not exist without
their work. See [CREDITS.md](CREDITS.md).

EctoPad's own part was the mobile layer: the SunPad-derived touch controls and
settings, Files-based game import, save handling, Apple lifecycle fixes and the
build scripts.

## Why it's paused

Metaforce is now developed privately ahead of its own release. EctoPad will not
redistribute it in the meantime, so EctoPad cannot be built from this
repository.

| Option | Status |
|---|---|
| Public IPA | **Not available** |
| PadMint build | Not available |
| Building from source | Only with your own copies of the sources (below) |

There is no public IPA right now, and none is planned while EctoPad is paused.

## Building from source

This is unsupported, and no help is offered. You need an Apple Silicon Mac with
Xcode, CMake, Ninja and Rust from rustup, your own Metroid Prime (USA, Rev 2)
disc image, and your own copies of the exact source commits listed in
[docs/DEPENDENCIES.md](docs/DEPENDENCIES.md). EctoPad's source mirrors are
private. Point the bootstrap at your copies and build:

```sh
ECTOPAD_SOURCE_BASE=https://example.com/yours/ectopad bash scripts/bootstrap-workspace.sh
bash scripts/build-padmint-ios.sh EctoPad.ipa    # or scripts/build-padmint-macos.sh EctoPad.zip
```

The bootstrap fetches `$ECTOPAD_SOURCE_BASE-metaforce.git`,
`-amuse.git`, `-logvisor.git`, `-lzokay.git`, `-jbus.git` and
`-libjpeg-turbo.git` at the pinned commits. [docs/BUILDING.md](docs/BUILDING.md)
has the details.

## What's in this repository

EctoPad's own patches, scripts and documentation, kept as a record. That
includes EctoPad's replacements for two libraries that never carried a license
(`components/zeus` and `components/kabufuda`). No game data, disc images,
extracted assets, saves, keys or signing material are included.

> [!NOTE]
> **AI disclosure:** EctoPad was built with substantial AI assistance for code,
> tests, documentation and debugging.

## Legal

EctoPad is an unofficial fan project. It is not affiliated with, endorsed by or
sponsored by Nintendo or Retro Studios. Metroid Prime and related names are
trademarks of Nintendo. Each component keeps its own copyright and license
([THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)); EctoPad's own code is
GPL-3.0-or-later ([LICENSE](LICENSE)).
