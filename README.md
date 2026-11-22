# CyMan-MP Server

CyMan-MP is a **modified version of the [BeamMP-Server](https://github.com/BeamMP/BeamMP-Server)**, the server for the BeamNG.drive multiplayer mod BeamMP. It is built from the BeamMP server source code and runs with its own backend, launcher and website.

> **CyMan-MP is not affiliated with, endorsed by, or supported by BeamMP Ltd. or the BeamMP team.**
> Please do not ask the BeamMP team for support with CyMan-MP.

- Website: https://cymanmp.okeyhi.com
- Discord: https://discord.gg/FgyuMNSe2x

## What's different from BeamMP-Server

- Connects to the CyMan-MP backend instead of the official BeamMP backend.
- Optional **MeshSync** (`MeshSync` and `MeshSyncIntensity` in `ServerConfig.toml`). It syncs vehicle damage and deformation between players. It is heavy on CPU, bandwidth and server load, so it is off by default.
- More reliable backend requests on hosts with strict thread or PID limits. Curl is built with c-ares, and the server resolves the backend hostname itself and caches the last good answer.
- Fixes for idle CPU usage: the Lua result-check thread no longer busy-loops, and the network accept and receive loops back off after repeated errors.

Everything else works like the original BeamMP server, including Lua server plugins. The upstream wiki is a good reference for the plugin API: https://wiki.beammp.com

## Getting a server key

1. Create an account on https://cymanmp.okeyhi.com.
2. Open the Keymaster page and create a key.
3. Put the key in `ServerConfig.toml` as `AuthKey`.

## Minimum requirements

These are rough estimates and may change between releases.

- RAM: 30-100 MiB (not counting OS overhead)
- CPU: >1 GHz, preferably multicore
- OS: Linux or Windows
- Bandwidth: 5-10 Mb/s upload
- Disk: 10 MiB plus mods and plugins

## Downloads

Prebuilt Linux binaries (Debian 12, x86_64) are published on the [Releases page](../../releases) as the `latest` release.

Runtime dependencies on Debian, Ubuntu and similar: `liblua5.3-0`.

## Building from source

On Linux you need these build dependencies:

```
liblua5.3-dev curl zip unzip tar cmake make git g++
```

If you are building for ARM (for example aarch64), run `export VCPKG_FORCE_SYSTEM_BINARIES=1` first.

```
git clone --recursive https://github.com/sadaasdasdawadsf/CyMan-MP
cd CyMan-MP
cmake -S . -B bin -DCMAKE_BUILD_TYPE=Release
cmake --build bin --parallel --config Release -t BeamMP-Server
```

The executable ends up in `bin/`. The first configure step can take a while and uses a lot of disk space because vcpkg builds all dependencies.

## License and source code

CyMan-MP is based on BeamMP-Server and is licensed under the **GNU Affero General Public License v3.0** (AGPL-3.0), the same license as the original. See [LICENSE](LICENSE).

- Original work: Copyright (C) BeamMP Ltd., BeamMP team and contributors.
- Modifications: CyMan-MP.

Under the AGPL, the complete source code of this modified version is available in this repository, and anyone who plays on a CyMan-MP server may obtain it from here.

## Support

Ask questions in the [CyMan-MP Discord](https://discord.gg/FgyuMNSe2x).
