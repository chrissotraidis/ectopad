# EctoPad app icon provenance

The EctoPad app icon is original project artwork generated for this repository
with OpenAI's built-in image-generation tool on 2026-08-13.

The mark depicts an abstract luminous green ectoplasm/pathogen form. It does
not use Nintendo, Metroid, Metaforce, or other third-party artwork, logos,
characters, screenshots, or extracted game assets.

## Source and derivatives

- `EctoPad-AppIcon-1024.png` — 1024×1024 sRGB master, no alpha
- `AppIcon60x60@2x.png` — 120×120 iPhone derivative
- `AppIcon76x76@2x~ipad.png` — 152×152 iPad derivative
- `Assets.xcassets/` — tracked source catalog compiled into Metaforce's
  `Assets.car` by `scripts/sync-app-icon.sh`

The loose derivatives and compiled asset catalog are installed with
`scripts/sync-app-icon.sh`. Apple applies the platform icon mask; the source
artwork intentionally has square corners.

## Generation prompt

> Create an original abstract iOS/iPadOS app icon for EctoPad: one centered,
> cartoonish luminous green ectoplasm/pathogen organism with three rounded
> tendrils and a bright inner nucleus, on a deep near-black teal background.
> Use a bold simplified silhouette that remains readable at 60 pixels. No text,
> border, rounded-corner mask, watermark, characters, armor, helmet, visor,
> morph ball, Metroid creature, game logo, Nintendo mark, or recognizable
> franchise symbol.
