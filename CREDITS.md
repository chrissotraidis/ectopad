# Credits and project relationship

EctoPad is downstream work built on Metaforce and Aurora. It is not an
independent reimplementation of Metroid Prime and does not claim to have first
introduced Metaforce to iOS.

## Upstream foundation

- **Metaforce** is the native Metroid Prime engine reimplementation. Its MIT
  license names **Jack Andersen** and **Phillip "Antidote" Stephens** as the
  original authors. The engine, gameplay systems, and the overwhelming majority
  of the work that makes Metroid Prime run belong to the Metaforce project and
  its contributors.
- **Metaforce iOS/tvOS support** predates EctoPad. Luke Street introduced the
  upstream iOS and tvOS build support in
  [AxioDL/metaforce commit `4048492`](https://github.com/AxioDL/metaforce/commit/4048492279),
  with later work by the Metaforce and Aurora contributors.
- **Aurora** provides the SDL3 application layer and the GX-to-WebGPU/Dawn
  rendering path used by Metaforce, including its Apple platform foundation.

See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) and
[docs/DEPENDENCIES.md](docs/DEPENDENCIES.md) for the full dependency and license
inventory.

## EctoPad's scope

EctoPad maintains a pinned downstream integration and patch record focused on
adding a complete mobile workflow and recording what has actually been tested
on current iPhone, iPad, and Apple Silicon Mac hardware. Its work includes a
SunPad-derived touch and settings interface, touch/controller handoff and
reconnect reconciliation, private game-data import, iOS memory-card
provisioning and preservation, targeted audio and lifecycle repairs,
diagnostics, packaging checks, and physical-device validation.

The recorded device sessions demonstrate sustained gameplay on the tested
hardware. They are not a claim of universal stability, full-game completion,
or first-ever iOS execution.

Those additions do not replace or diminish upstream authorship. Where a fix is
generally useful to Metaforce or Aurora, the preferred long-term home is
upstream.
