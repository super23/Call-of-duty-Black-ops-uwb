# Single-player source file lists for KisakBlackSP.
# Requires cmake_files.cmake to be included first (shared *_FILES variables).

# ----- bgame (SP: omit MP-only dog/vehicle animation tables) -----
set(BGAME_SP_FILES ${BGAME_FILES})
list(REMOVE_ITEM BGAME_SP_FILES
	"${SRC_DIR}/bgame/bg_dog_animations_mp.cpp"
	"${SRC_DIR}/bgame/bg_dog_animations_mp.h"
	"${SRC_DIR}/bgame/bg_vehicles_mp.cpp"
	"${SRC_DIR}/bgame/bg_vehicles_mp.h"
)
list(APPEND BGAME_SP_FILES
	"${SRC_DIR}/bgame/bg_dog_animations_sp.cpp"
	"${SRC_DIR}/bgame/bg_dog_animations_sp.h"
	"${SRC_DIR}/bgame/bg_dog_animations_wrapper.h"
	"${SRC_DIR}/bgame/bg_vehicles_sp.cpp"
	"${SRC_DIR}/bgame/bg_vehicles_sp.h"
	"${SRC_DIR}/bgame/bg_sp_assets.cpp"
	"${SRC_DIR}/bgame/bg_sp_assets.h"
)

# ----- src/game (SP) -----
set(SRC_GAME_SP
	"${SRC_DIR}/game/g_main.cpp"
	"${SRC_DIR}/game/g_main.h"
	"${SRC_DIR}/game/g_scr_main.cpp"
	"${SRC_DIR}/game/g_scr_main.h"
	"${SRC_DIR}/game/g_save.cpp"
	"${SRC_DIR}/game/g_save_pending_sp.cpp"
	"${SRC_DIR}/game/g_loadgame_sp.cpp"
	"${SRC_DIR}/game/g_loadgame_sp.h"
	"${SRC_DIR}/game/g_save.h"
	"${SRC_DIR}/game/g_client_script_cmd_sp.cpp"
	"${SRC_DIR}/game/g_client_script_cmd_sp.h"
	"${SRC_DIR}/game/g_active_sp.cpp"
	"${SRC_DIR}/game/g_active_sp.h"
	"${SRC_DIR}/game/g_combat_sp.cpp"
	"${SRC_DIR}/game/g_combat_sp.h"
	"${SRC_DIR}/game/g_spawn_sp.cpp"
	"${SRC_DIR}/game/g_spawn_sp.h"
	"${SRC_DIR}/game/g_utils_sp.cpp"
	"${SRC_DIR}/game/g_utils_sp.h"
	"${SRC_DIR}/game/g_misc_sp.cpp"
	"${SRC_DIR}/game/g_misc_sp.h"
	"${SRC_DIR}/game/actor_cover.cpp"
	"${SRC_DIR}/game/actor_cover.h"
	"${SRC_DIR}/game/actor_grenade.cpp"
	"${SRC_DIR}/game/actor_grenade.h"
	"${SRC_DIR}/game/g_scr_mover.cpp"
	"${SRC_DIR}/game/g_scr_mover.h"
	"${SRC_DIR}/game/g_animscripted.cpp"
	"${SRC_DIR}/game/g_animscripted.h"
	"${SRC_DIR}/game/actor_animscripted.cpp"
	"${SRC_DIR}/game/actor_animscripted.h"
	"${SRC_DIR}/game/actor_custom_anim.cpp"
	"${SRC_DIR}/game/actor_react.cpp"
	"${SRC_DIR}/game/g_cmds_sp.cpp"
	"${SRC_DIR}/game/g_cmds_sp.h"
	"${SRC_DIR}/game/g_trigger_sp.cpp"
	"${SRC_DIR}/game/g_trigger_sp.h"
	"${SRC_DIR}/game/player_use_sp.cpp"
	"${SRC_DIR}/game/player_use_sp.h"
	"${SRC_DIR}/game/pregame_sp.cpp"
	"${SRC_DIR}/game/pregame_sp.h"
	"${SRC_DIR}/game/pregame_wrapper.h"
	"${SRC_DIR}/game/g_team_sp.cpp"
	"${SRC_DIR}/game/g_team_sp.h"
	"${SRC_DIR}/game/g_spawnsystem_sp_stubs.cpp"
	"${SRC_DIR}/game/sp_link_stubs.cpp"
	"${SRC_DIR}/game/sentient_script_cmd.cpp"
	"${SRC_DIR}/game/sentient_script_cmd.h"
	# Shared actor core (types in actor.h; implementation still in game_mp until full SP actor port)
	"${SRC_DIR}/game_mp/actor_mp.cpp"
	"${SRC_DIR}/game_mp/actor_mp.h"
)
source_group("game_sp" FILES ${SRC_GAME_SP})

set(GAME_SP_FILES
	${SRC_GAME_SP}
)

# ----- src/cgame_sp (mirror cgame_mp) -----
set(SRC_CGAME_SP)
foreach(_file IN LISTS SRC_CGAME_MP)
	string(REPLACE "/cgame_mp/" "/cgame_sp/" _sp_file "${_file}")
	string(REPLACE "_mp." "_sp." _sp_file "${_sp_file}")
	list(APPEND SRC_CGAME_SP "${_sp_file}")
endforeach()
source_group("cgame_sp" FILES ${SRC_CGAME_SP})

set(CGAME_SP_FILES
	${SRC_CGAME_SP}
)

# ----- src/client_sp (mirror client_mp) -----
set(SRC_CLIENT_SP)
foreach(_file IN LISTS SRC_CLIENT_MP)
	string(REPLACE "/client_mp/" "/client_sp/" _sp_file "${_file}")
	string(REPLACE "_mp." "_sp." _sp_file "${_sp_file}")
	list(APPEND SRC_CLIENT_SP "${_sp_file}")
endforeach()
source_group("client_sp" FILES ${SRC_CLIENT_SP})

# SP-only map-load cinematic (cin_levels.txt VM); no client_mp mirror.
list(APPEND SRC_CLIENT_SP
	"${SRC_DIR}/client_sp/cl_mapload_cin_sp.cpp"
	"${SRC_DIR}/client_sp/cl_mapload_cin_sp.h"
)

set(CLIENT_SP_FILES
	${SRC_CLIENT_SP}
)

# ----- src/server_sp (mirror server_mp) -----
set(SRC_SERVER_SP)
foreach(_file IN LISTS SRC_SERVER_MP)
	string(REPLACE "/server_mp/" "/server_sp/" _sp_file "${_file}")
	string(REPLACE "_mp." "_sp." _sp_file "${_sp_file}")
	list(APPEND SRC_SERVER_SP "${_sp_file}")
endforeach()
source_group("server_sp" FILES ${SRC_SERVER_SP})

set(SERVER_SP_FILES
	${SRC_SERVER_SP}
)

# ----- src/ui_sp -----
set(SRC_UI_SP
	"${SRC_DIR}/ui_sp/ui_main_sp.cpp"
	"${SRC_DIR}/ui_sp/ui_main_sp.h"
	"${SRC_DIR}/ui_sp/ui_gametype_custom_sp.cpp"
	"${SRC_DIR}/ui_sp/ui_gametype_custom_sp.h"
	"${SRC_DIR}/ui_sp/ui_gametype_variants_sp.cpp"
	"${SRC_DIR}/ui_sp/ui_gametype_variants_sp.h"
	"${SRC_DIR}/ui_sp/ui_gameinfo_sp.cpp"
	"${SRC_DIR}/ui_sp/ui_gameinfo_sp.h"
	"${SRC_DIR}/ui_sp/ui_feeders_sp.cpp"
	"${SRC_DIR}/ui_sp/ui_feeders_sp.h"
)
source_group("ui_sp" FILES ${SRC_UI_SP})

set(UI_SP_FILES
	${SRC_UI_SP}
)

# ----- src/record -----
set(SRC_RECORD
	"${SRC_DIR}/record/recorder.cpp"
	"${SRC_DIR}/record/recorder.h"
	"${SRC_DIR}/record/record_primitives.cpp"
	"${SRC_DIR}/record/record_replay_mgr.cpp"
)
source_group("record" FILES ${SRC_RECORD})

# SP theater/combat-record: core API in recorder.cpp; primitive/replay_mgr stubs excluded until decomp port.
set(RECORD_FILES
	"${SRC_DIR}/record/recorder.cpp"
	"${SRC_DIR}/record/recorder.h"
)

# ----- bgame SP extras -----
list(APPEND BGAME_SP_FILES
	"${SRC_DIR}/bgame/bg_collectibles_sp.cpp"
	"${SRC_DIR}/bgame/bg_collectibles_sp.h"
)

# ----- cgame_sp extras -----
list(APPEND CGAME_SP_FILES
	"${SRC_DIR}/cgame_sp/cg_modelpreviewer_sp.cpp"
	"${SRC_DIR}/cgame_sp/cg_modelpreviewer_sp.h"
	"${SRC_DIR}/cgame_sp/cg_compassfriendlies_sp.cpp"
	"${SRC_DIR}/cgame_sp/cg_draw_net_sp.cpp"
	"${SRC_DIR}/cgame_sp/cg_scoreboard_sp.cpp"
)

# ----- full SP executable source list -----
set(KISAK_SP_SOURCES
	${DW_DEMONWAREROOT_FILES}
	${DW_BDCORE_FILES}
	${DW_BDPLATFORM_FILES}
	${CUBEMAPGENLIB_FILES}
	${DW_FILES}
	${DYNENTITY_FILES}
	${EFFECTSCORE_FILES}
	${AIM_ASSIST_FILES}
	${BGAME_SP_FILES}
	${BINKLIB_FILES}
	${CGAME_FILES}
	${CGAME_SP_FILES}
	${CLIENT_FILES}
	${CLIENT_SP_FILES}
	${CLIENTSCRIPT_FILES}
	${COMMON_FILES}
	${DATABASE_FILES}
	${DDL_FILES}
	${DEVGUI_FILES}
	${FLAME_FILES}
	${GAME_FILES}
	${GAME_SP_FILES}
	${GFX_D3D_FILES}
	${GLASS_FILES}
	${GROUPVOICE_FILES}
	${IK_FILES}
	${JPEG_FILES}
	${LIBS_FILES}
	${MINILZO_FILES}
	${MJPEG_FILES}
	${MONKEY_FILES}
	${NVAPI_FILES}
	${PHYSICS_FILES}
	${QCOMMON_FILES}
	${RAGDOLL_FILES}
	${RECORD_FILES}
	${SERVER_FILES}
	${SERVER_SP_FILES}
	${SOUND_FILES}
	${SPEEX_FILES}
	${STEAM_FILES}
	${STRINGED_FILES}
	${TURRET_FILES}
	${UI_FILES}
	${UI_SP_FILES}
	${UNIVERSAL_FILES}
	${VEHICLE_FILES}
	${VPX_FILES}
	${WIN32_FILES}
	${XANIM_FILES}
	${ZLIB_FILES}
	${TL_TLROOT_FILES}
	${TL_BASE_FILES}
	${TL_JOBQUEUE_FILES}
	${TL_PHYSICS_FILES}
)
