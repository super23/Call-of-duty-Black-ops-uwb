# KisakBlack map editor (`src/radiant`)

All **CoDBORadiant** logic lives here. This tree is **not** part of the `KisakBlack` / `KisakBlackSP` game targets in the repo root `CMakeLists.txt`.

## Isolation rules

1. **Do not** add `src/radiant/*.cpp` to `cmake_files.cmake`, `cmake_files_sp.cmake`, or the `KisakBlack` executable.
2. **Do not** add `#ifdef KISAK_RADIANT_EDITOR` (or `RAD_*`) into `src/gfx_d3d`, `src/universal`, `src/game`, etc. Editor hooks belong in `GfxWrapper.cpp` or future `src/gfx_d3d/r_ed_*.cpp` ports compiled only by `tools/CodBORadiant`.
3. **Build the editor** only via:
   ```powershell
   cmake -S tools/CodBORadiant -B build_radiant_win32 -G "Visual Studio 17 2022" -A Win32
   cmake --build build_radiant_win32 --config Release
   ```
4. **`RAD_LINK_ENGINE=ON`** reuses existing engine `.cpp` files as **separate object files** in the `CoDBORadiant` target only. It does not change how the game exe compiles those files.
5. **`*_decomp.cpp`** under `mfc/` are Hex-Rays extracts for hand-porting. They are **not** in the default editor source list unless you explicitly add them to `tools/CodBORadiant/CMakeLists.txt` (e.g. `RAD_DECOMP_MONOLITH`).

## Game ↔ editor boundary

| Component | Location | Game target? |
|-----------|----------|--------------|
| Editor UI / map I/O | `src/radiant/` | No |
| Live-update protocol (game side) | `src/qcommon/radiant_remote.*` | Yes (MP `KisakBlack` only today) |
| Editor build glue | `tools/CodBORadiant/` | No |

Changing `radiant_remote` affects **in-game** live update when enabled; it is unrelated to building `CoDBORadiant.exe`. Keep editor protocol sends in `src/radiant/remote.cpp`.

## Layout

```
radiant_main.cpp      WinMain / message pump
mfc/                  CWinApp, CMainFrame (ports)
editor/               map_io, future brush/entity
GfxWrapper.cpp        Optional ENGINE_LINK to gfx_d3d
radiant_editor.cpp    Editor-only stubs (layers, parse hooks)
remote.cpp            Editor-side live update (stub)
```

See `tools/CodBORadiant/ENGINE_ISOLATION.md` for CMake details.
