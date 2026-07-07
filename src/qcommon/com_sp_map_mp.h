#pragma once

// Changed-For-SP-Map
// Lets the MP executable load single-player campaign fastfiles / BSPs.
// Search the codebase for "Changed-For-SP-Map" to find every touch point.
// Touches: db_load (game_map_sp), com_files (BSP path), cscr_parser (scripts),
//          db_registry (missing rumble/*.rmb rawfile stubs).
// Set to 0 to revert to retail MP-only map loading.
#ifdef KISAK_SP
#define KISAK_MP_SP_MAP_SUPPORT 1
#elif !defined(KISAK_MP_SP_MAP_SUPPORT)
#define KISAK_MP_SP_MAP_SUPPORT 1
#endif
