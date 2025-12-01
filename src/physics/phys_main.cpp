#include "phys_main.h"

void __cdecl destroy_gjk_geom(gjk_aabb_t *geom)
{
  switch ( geom->get_type(geom) )
  {
    case 1u:
      gjk_aabb_t::destroy(geom);
      break;
    case 2u:
      gjk_brush_t::destroy((gjk_brush_t *)geom);
      break;
    case 3u:
      gjk_partition_t::destroy((gjk_partition_t *)geom);
      break;
    case 4u:
      gjk_double_sphere_t::destroy((gjk_double_sphere_t *)geom);
      break;
    case 5u:
      gjk_cylinder_t::destroy((gjk_cylinder_t *)geom);
      break;
    case 6u:
      gjk_obb_t::destroy((gjk_cylinder_t *)geom);
      break;
    case 7u:
      gjk_polygon_cylinder_t::destroy((gjk_polygon_cylinder_t *)geom);
      break;
    default:
      if ( _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
             403,
             "0",
             "Trying to free up unkownn geom. Leak!!!") )
      {
        __debugbreak();
      }
      break;
  }
}

void __cdecl Phys_Init()
{
  phys_surface_type_info v0; // [esp+30h] [ebp-68h] BYREF
  phys_surface_type_info v1; // [esp+40h] [ebp-58h] BYREF
  phys_surface_type_info v2; // [esp+50h] [ebp-48h] BYREF
  phys_surface_type_info v3; // [esp+60h] [ebp-38h] BYREF
  phys_surface_type_info pst; // [esp+70h] [ebp-28h] BYREF
  float surfaceFriction; // [esp+80h] [ebp-18h]
  float surfaceBounce; // [esp+84h] [ebp-14h]
  int i; // [esp+88h] [ebp-10h]
  broad_phase_memory_info bpmi; // [esp+8Ch] [ebp-Ch] BYREF

  physGlob.timeLastSnapshot = 0;
  physGlob.timeLastUpdate = 0;
  Phys_InitSoundAliases();
  if ( !physInited )
  {
    physInited = 1;
    phys_gravity = _Dvar_RegisterFloat(
                     "phys_gravity",
                     -800.0,
                     -3.4028235e38,
                     3.4028235e38,
                     0x1000u,
                     "Physics gravity in units/sec^2.");
    phys_gravity_dir = _Dvar_RegisterVec3(
                         "phys_gravity_dir",
                         COERCE_UNSIGNED_INT(0.0),
                         COERCE_UNSIGNED_INT(0.0),
                         COERCE_UNSIGNED_INT(1.0),
                         -1.0,
                         1.0,
                         0x1000u,
                         "Sets the gravity direction.");
    phys_vehicleGravityMultiplier = _Dvar_RegisterFloat(
                                      "phys_vehicleGravityMultiplier",
                                      1.0,
                                      0.5,
                                      2.0,
                                      0x1000u,
                                      "Physics gravity multiplier for vehicles.");
    phys_vehicleDamageFroceScale = _Dvar_RegisterFloat(
                                     "phys_vehicleDamageFroceScale",
                                     0.30000001,
                                     0.0,
                                     10.0,
                                     0x1000u,
                                     "Physics vehicle damamge force scale.");
    phys_vehicleUsePredictedPosition = _Dvar_RegisterBool(
                                         "phys_vehicleUsePredictedPosition",
                                         0,
                                         0x1000u,
                                         "Turning this on forces the client to use the vehicles predicted position.");
    phys_bulletUpBias = _Dvar_RegisterFloat(
                          "phys_bulletUpBias",
                          0.5,
                          0.0,
                          2.0,
                          0,
                          "Up Bias for the direction of the bullet impact.");
    phys_bulletSpinScale = _Dvar_RegisterFloat(
                             "phys_bulletSpinScale",
                             3.0,
                             0.0099999998,
                             100.0,
                             0,
                             "Scale of the effective offset from the center of mass for the bullet impacts.");
    phys_msecStep = _Dvar_RegisterInt(
                      "phys_msecstep",
                      0,
                      0,
                      1000,
                      0,
                      "Physics simulation update increment in milliseconds");
    phys_drawcontacts = _Dvar_RegisterBool("phys_drawcontacts", 0, 0, "Debug draw contact points");
    phys_drawCollisionObj = _Dvar_RegisterBool(
                              "phys_drawCollisionObj",
                              0,
                              0,
                              "Debug draw collision geometry for each physics object");
    phys_debugBigQueries = _Dvar_RegisterBool(
                             "phys_debugBigQueries",
                             0,
                             0,
                             "Draw debug info when phys object moves very fast therefore generating huge query boxes");
    phys_debugCallback = _Dvar_RegisterBool("phys_debugCallback", 0, 0, "Enbles the physics debug callback");
    phys_debugDangerousRigidBodies = _Dvar_RegisterBool(
                                       "phys_debugDangerousRigidBodies",
                                       1,
                                       0,
                                       "Draws debug line off the origins of dangerous rigid bodies");
    phys_debugExpensivePushout = _Dvar_RegisterBool(
                                   "phys_debugExpensivePushout",
                                   1,
                                   0,
                                   "Draw debug info when the engine resort to exensive Player/AI pushouts");
    phys_drawNitrousVehicle = _Dvar_RegisterBool(
                                "phys_drawNitrousVehicle",
                                0,
                                0,
                                "Print info about the nitrous vehicles");
    phys_drawNitrousVehicleEffects = _Dvar_RegisterBool(
                                       "phys_drawNitrousVehicleEffects",
                                       0,
                                       0,
                                       "Print info about the nitrous vehicle wheel effects");
    phys_entityCollision = _Dvar_RegisterBool("phys_entityCollision", 1, 0, "Enable to turn on entity collision.");
    phys_vehicleWheelEntityCollision = _Dvar_RegisterBool(
                                         "phys_vehicleWheelEntityCollision",
                                         1,
                                         0x1000u,
                                         "Enable to turn on vehicle wheel entity collision.");
    phys_vehicleFriction = _Dvar_RegisterFloat(
                             "phys_vehicleFriction",
                             0.1,
                             0.0,
                             2.0,
                             0,
                             "Default vehicle friction with the world and other ents.");
    phys_ragdoll_joint_damp_scale = _Dvar_RegisterFloat(
                                      "phys_ragdoll_joint_damp_scale",
                                      0.039999999,
                                      0.0,
                                      3.4028235e38,
                                      0,
                                      "Sets the phys joint damp for ragdolls.");
    phys_dragLinear = _Dvar_RegisterFloat(
                        "phys_dragLinear",
                        0.029999999,
                        0.0,
                        3.4028235e38,
                        0,
                        "The amount of linear drag, applied globally");
    phys_dragAngular = _Dvar_RegisterFloat(
                         "phys_dragAngular",
                         0.5,
                         0.0,
                         3.4028235e38,
                         0,
                         "The amount of angular drag, applied globally");
    phys_userRigidBodies = _Dvar_RegisterBool("phys_userRigidBodies", 1, 0, "Enables user rigid bodies.");
    phys_waterDragLinear = _Dvar_RegisterFloat(
                             "phys_waterDragLinear",
                             -1.0,
                             -3.4028235e38,
                             0.0,
                             0,
                             "The amount of linear drag for buoyant objects");
    phys_waterDragAngular = _Dvar_RegisterFloat(
                              "phys_waterDragAngular",
                              -2.0,
                              -3.4028235e38,
                              0.0,
                              0,
                              "The amount of angular drag for buoyant objects");
    phys_maxFloatTime = _Dvar_RegisterInt(
                          "phys_maxFloatTime",
                          30000,
                          10000,
                          0x7FFFFFFF,
                          0x1000u,
                          "Maximum time objects can float (in ms)");
    phys_buoyancyDistanceCutoff = _Dvar_RegisterFloat(
                                    "phys_buoyancyDistanceCutoff",
                                    2500.0,
                                    0.0,
                                    3.4028235e38,
                                    0x1000u,
                                    "Objects that are too far will sink.");
    phys_piecesSpawnDistanceCutoff = _Dvar_RegisterFloat(
                                       "phys_piecesSpawnDistanceCutoff",
                                       1000.0,
                                       0.0,
                                       3.4028235e38,
                                       0x1000u,
                                       "Determine how far we can spawn destructible pieces.");
    phys_floatTimeVariance = _Dvar_RegisterInt(
                               "phys_floatTimeVariance",
                               10000,
                               0,
                               0x7FFFFFFF,
                               0,
                               "Variance in time (in ms) objects will float for");
    phys_buoyancyRippleFrequency = _Dvar_RegisterInt(
                                     "phys_buoyancyRippleFrequency",
                                     3000,
                                     0,
                                     0x7FFFFFFF,
                                     0,
                                     "How often (in ms) floating objects will play the ripple fx");
    phys_buoyancyRippleVariance = _Dvar_RegisterInt(
                                    "phys_buoyancyRippleVariance",
                                    2000,
                                    0,
                                    0x7FFFFFFF,
                                    0,
                                    "How late (in ms) the ripple fx can be played");
    phys_buoyancyFastComputation = _Dvar_RegisterBool(
                                     "phys_buoyancyFastComputation",
                                     0,
                                     0,
                                     "Simplify the buoyancy calculations");
    phys_buoyancy = _Dvar_RegisterBool("phys_buoyancy", 0, 0x1000u, "Used to turn on/off buoyancy");
    phys_ragdoll_buoyancy = _Dvar_RegisterBool(
                              "phys_ragdoll_buoyancy",
                              0,
                              0x1000u,
                              "Used to turn on/off the buoyancy for he ragdolls");
    debug_trace = _Dvar_RegisterInt("debug_trace", 0, 0, 0x7FFFFFFF, 0, "Does a trace and draws the hit point");
    g_bDebugRenderBulletMeshes = _Dvar_RegisterBool(
                                   "debugRenderBulletMeshes",
                                   0,
                                   0,
                                   "Render bullet meshes (point to entity)");
    g_bDebugRenderEntityBrushes = _Dvar_RegisterBool(
                                    "debugRenderEntityBrushes",
                                    0,
                                    0,
                                    "Render the brushes associated with the entity you are looking at");
    g_bDebugRenderPatches = _Dvar_RegisterBool("debugRenderPatches", 0, 0, "Render patches");
    g_bDebugRenderBrushes = _Dvar_RegisterBool("debugRenderBrushes", 0, 0, "Render brushes");
    g_bDebugRenderColoredPatches = _Dvar_RegisterBool(
                                     "debugRenderColoredPatches",
                                     0,
                                     0,
                                     "Render each patch with a different color (Render Patches must be on)");
    g_debugRenderMask = _Dvar_RegisterEnum(
                          "debugRenderMask",
                          g_debugRenderMaskNames,
                          0,
                          0x80u,
                          "Mask for collision rendering");
    g_debugRenderCollisionDistance = _Dvar_RegisterFloat(
                                       "debugRenderCollisionDistance",
                                       800.0,
                                       100.0,
                                       2000.0,
                                       0x80u,
                                       "Distance to render debug brushes and patches");
    g_bDebugRenderStaticModelsBounds = _Dvar_RegisterBool(
                                         "debugRenderStaticModelsBounds",
                                         0,
                                         0,
                                         "Render the bounds of the nearby static models");
    g_dumpStaticModels = _Dvar_RegisterBool("dumpStaticModels", 0, 0, "Prints info about the static models");
    g_debugRenderGjkTraceGeom = _Dvar_RegisterBool("debugRenderGjkTraceGeom", 0, 0, "Render gjk trace meshes");
    phys_player_collision_mode = _Dvar_RegisterInt(
                                   "phys_player_collision_mode",
                                   1,
                                   0,
                                   1,
                                   0,
                                   "phys player collision mode");
    phys_player_collision_adjust_height = _Dvar_RegisterFloat(
                                            "phys_player_collision_adjust_height",
                                            0.0,
                                            0.0,
                                            5.0,
                                            0,
                                            "adjust the height of the gjk player collision capsule");
    phys_ai_collision_mode = _Dvar_RegisterInt("phys_ai_collision_mode", 1, 0, 1, 0, "phys ai collision mode");
    render_player_collision = _Dvar_RegisterInt("render_player_collision", 0, 0, 1, 0, "render player collision");
    render_actor_collision = _Dvar_RegisterInt("render_actor_collision", 0, 0, 1, 0, "render actor collision");
    render_bpi_env_collision = _Dvar_RegisterInt("render_bpi_env_collision", 0, 0, 5000, 0, "render bpi_env collision");
    enable_moving_paths = _Dvar_RegisterInt("enable_moving_paths", 1, 0, 1, 0, "enable moving paths");
    enable_new_prone_check = _Dvar_RegisterInt("enable_new_prone_check", 0, 0, 1, 0x80u, "enable new prone check");
    phys_heavyTankSwitch = _Dvar_RegisterBool(
                             "phys_heavyTankSwitch",
                             0,
                             0,
                             "Enable 'heavier' tank physics in multiplayer");
    phys_fluid = _Dvar_RegisterInt("phys_fluid", 0, 0, 5, 0, "Turn on and off the fluid simulation code");
    Phys_EffectsInit();
    if ( G_PHYSICS_TOTAL_MEMORY_BUFFER
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
           341,
           "G_PHYSICS_TOTAL_MEMORY_BUFFER == NULL",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    G_PHYSICS_TOTAL_MEMORY_BUFFER = tlMemAlloc(0x380000u, 0x10u, 0);
    phys_memory_manager_init(G_PHYSICS_TOTAL_MEMORY_BUFFER, 3670016);
    phys_task_manager_init();
    phys_sys::phys_init();
    broad_phase_memory_info::broad_phase_memory_info(&bpmi);
    bpmi.m_max_num_gjk_ci = 2048;
    bpmi.m_max_num_sap_active_pairs = 2048;
    bpmi.m_max_num_surface_types = 35;
    broad_phase_system_init(
      &bpmi,
      (bool (__cdecl *)(const broad_phase_base *, const broad_phase_base *))Phys_ShouldCollideCallback);
    phys_sys::set_collision_callback((void (__cdecl *)())Phys_CollisionCallback);
    phys_sys::set_max_delta_t(1.0);
    phys_sys::set_v_tol(8);
    set_bp_standard_query();
    set_debug_callback();
    pst.m_friction_coef = 0.4f;
    pst.m_bounce_coef = 0.1f;
    pst.m_solver_priority = 0;
    pst.m_no_overflow_error = 1;
    surface_type_info_database_set(31, 31, &pst);
    v3.m_friction_coef = 0.5f;
    v3.m_bounce_coef = 0.1f;
    v3.m_solver_priority = 0;
    v3.m_no_overflow_error = 1;
    surface_type_info_database_set(31, 32, &v3);
    v2.m_friction_coef = 0.5f;
    v2.m_bounce_coef = 0.1f;
    v2.m_solver_priority = 0;
    v2.m_no_overflow_error = 1;
    surface_type_info_database_set(32, 32, &v2);
    for ( i = 0; i < 31; ++i )
    {
      surfaceFriction = Com_SurfaceFrictionScale(i);
      surfaceBounce = Com_SurfaceBounceScale(i);
      v1.m_friction_coef = surfaceFriction;
      v1.m_bounce_coef = surfaceBounce;
      v1.m_solver_priority = 1;
      v1.m_no_overflow_error = 0;
      surface_type_info_database_set(31, i, &v1);
      v0.m_friction_coef = surfaceFriction;
      v0.m_bounce_coef = surfaceBounce;
      v0.m_solver_priority = 1;
      v0.m_no_overflow_error = 0;
      surface_type_info_database_set(32, i, &v0);
    }
    RB_SetDebugBrushesAndPatchesCallback(render_debug_brushes_and_patches);
  }
}

void __cdecl surface_type_info_database_set(int surface_type_1, int surface_type_2, const phys_surface_type_info *pst)
{
  if ( !G_BPM->g_surface_type_info_database )
    break_here_27 = 1;
  if ( !G_BPM->g_surface_type_info_database
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         351,
         "G_BPM->g_surface_type_info_database",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  G_BPM->g_surface_type_info_database[surface_type_info_database_get_index(surface_type_1, surface_type_2)] = *pst;
}

void __cdecl create_broad_phase_info(rigid_body *body)
{
  unsigned int v1; // eax
  unsigned int geom_id; // eax
  gjk_base_t **p_m_first_geom; // [esp+4h] [ebp-2Ch]
  gjk_base_t **v4; // [esp+10h] [ebp-20h]
  broad_phase_info *broad_phase_info; // [esp+18h] [ebp-18h]
  gjk_base_t *gjk_geom; // [esp+1Ch] [ebp-14h]
  broad_phase_info *bpi; // [esp+20h] [ebp-10h]
  broad_phase_group *bpg; // [esp+24h] [ebp-Ch]
  gjk_base_t *geom; // [esp+28h] [ebp-8h]
  PhysObjUserData *userData; // [esp+2Ch] [ebp-4h]

  userData = (PhysObjUserData *)body->m_userdata;
  if ( gjk_geom_list_t::get_geom_count(&userData->m_gjk_geom_list) <= 1 )
  {
    if ( gjk_geom_list_t::get_geom_count(&userData->m_gjk_geom_list) != 1
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
           429,
           "userData->m_gjk_geom_list.get_geom_count() == 1",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    p_m_first_geom = &userData->m_gjk_geom_list.m_first_geom;
    if ( userData->m_gjk_geom_list.m_geom_count < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
           1035,
           "m_geom_count >= 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    gjk_geom = *p_m_first_geom;
    if ( !*p_m_first_geom
      && _tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 431, "gjk_geom", &toastPopupTitle) )
    {
      __debugbreak();
    }
    broad_phase_info = create_broad_phase_info();
    geom_id = gjk_base_t::get_geom_id(gjk_geom);
    broad_phase_info::set(
      broad_phase_info,
      body,
      &body->m_mat,
      &userData->cg2w,
      &userData->cg2rb,
      gjk_geom,
      geom_id,
      1,
      31,
      0,
      0x1C7u);
    userData->m_bpb = broad_phase_info;
    if ( rigid_body::get_flag(body, 0x20u) )
      broad_phase_info->m_env_collision_flags &= ~1u;
  }
  else
  {
    bpg = create_broad_phase_group();
    broad_phase_group::set(bpg);
    v4 = &userData->m_gjk_geom_list.m_first_geom;
    if ( userData->m_gjk_geom_list.m_geom_count < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
           1035,
           "m_geom_count >= 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    geom = *v4;
    if ( !*v4 && _tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 417, "geom", &toastPopupTitle) )
      __debugbreak();
    while ( geom )
    {
      bpi = create_broad_phase_info();
      v1 = gjk_base_t::get_geom_id(geom);
      broad_phase_info::set(bpi, body, &body->m_mat, &userData->cg2w, &userData->cg2rb, geom, v1, 1, 31, 0, 0x1C7u);
      broad_phase_group::add_bpi(bpg, bpi);
      geom = geom->m_next_geom;
    }
    userData->m_bpb = bpg;
  }
  environment_collision_list_add(userData->m_bpb);
  aasap_list_add(userData->m_bpb);
}

unsigned int __thiscall rigid_body::get_flag(rigid_body *this, unsigned int f)
{
  if ( (f & 0xFFFFFF00) != 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rigid_body.h",
         60,
         "(f & ~(FLAG_CLIENT_FLAGS_START - 1)) == 0",
         "invalid rigid body flag") )
  {
    __debugbreak();
  }
  return f & this->m_flags;
}

void __thiscall broad_phase_group::set(broad_phase_group *this)
{
  this->m_list_bpi_head = 0;
  this->m_bpi_count = 0;
  this->m_flags = 0;
  this->m_rbvm = 0;
  this->m_list_wci = 0;
  this->m_sap_node = 0;
  this->m_list_bpb_next = 0;
  this->m_user_data = 0;
  this->m_flags |= 2u;
  this->m_env_collision_flags = 0;
  this->m_my_collision_type_flags = 0;
}

void __thiscall broad_phase_group::add_bpi(broad_phase_group *this, broad_phase_info *bpi)
{
  if ( (bpi->m_flags & 0x40) != 0
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase.h",
         92,
         "!bpi->get_flag(broad_phase_info::FLAG_ON_BPG_LIST)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  environment_collision_list_remove(bpi);
  bpi->m_flags |= 0x40u;
  bpi->m_list_bpb_next = this->m_list_bpi_head;
  this->m_list_bpi_head = bpi;
  ++this->m_bpi_count;
  this->m_env_collision_flags |= bpi->m_env_collision_flags;
  this->m_my_collision_type_flags |= bpi->m_my_collision_type_flags;
}

void __cdecl environment_collision_list_add(broad_phase_base *bpb)
{
  broad_phase_memory *v1; // [esp+4h] [ebp-4h]

  if ( (bpb->m_flags & 1) == 0
    && (bpb->m_flags & 2) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         235,
         "bpb->is_bpi() || bpb->is_bpg()",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (bpb->m_flags & 0x40) != 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         236,
         "!bpb->get_flag(broad_phase_base::FLAG_ON_BPG_LIST)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (bpb->m_flags & 0x20) == 0 )
  {
    if ( bpb->m_list_bpb_next )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
             239,
             "bpb->m_list_bpb_next == NULL",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    v1 = G_BPM;
    bpb->m_list_bpb_next = G_BPM->g_list_bpb;
    v1->g_list_bpb = bpb;
    ++v1->g_list_bpb_count;
    bpb->m_flags |= 0x20u;
  }
}

void __cdecl environment_collision_list_remove(broad_phase_base *bpb)
{
  if ( (bpb->m_flags & 1) == 0
    && (bpb->m_flags & 2) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         247,
         "bpb->is_bpi() || bpb->is_bpg()",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (bpb->m_flags & 0x40) != 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         248,
         "!bpb->get_flag(broad_phase_base::FLAG_ON_BPG_LIST)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (bpb->m_flags & 0x20) != 0 )
  {
    broad_phase_memory::list_bpb_remove(G_BPM, bpb);
    bpb->m_list_bpb_next = 0;
    bpb->m_flags &= ~0x20u;
  }
}

void __thiscall broad_phase_memory::list_bpb_remove(broad_phase_memory *this, broad_phase_base *bpb_to_remove)
{
  broad_phase_base *bpb; // [esp+4h] [ebp-8h]
  broad_phase_base **bpb_cur; // [esp+8h] [ebp-4h]

  bpb_cur = &this->g_list_bpb;
  for ( bpb = this->g_list_bpb; bpb != bpb_to_remove; bpb = bpb->m_list_bpb_next )
  {
    if ( !bpb )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
             105,
             "bpb",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    bpb_cur = &bpb->m_list_bpb_next;
  }
  *bpb_cur = bpb->m_list_bpb_next;
  --this->g_list_bpb_count;
}

int __thiscall gjk_geom_list_t::get_geom_count(gjk_geom_list_t *this)
{
  if ( this->m_geom_count < 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
         1036,
         "m_geom_count >= 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return this->m_geom_count;
}

void __cdecl destroy_broad_phase_info(rigid_body *body)
{
  broad_phase_info *bpi; // eax
  broad_phase_group *bpg; // eax
  broad_phase_group *v3; // eax
  PhysObjUserData *userData; // [esp+8h] [ebp-4h]

  userData = (PhysObjUserData *)body->m_userdata;
  if ( userData->m_bpb )
  {
    aasap_list_remove(userData->m_bpb);
    environment_collision_list_remove(userData->m_bpb);
    if ( (userData->m_bpb->m_flags & 1) != 0 )
    {
      bpi = broad_phase_base::get_bpi(userData->m_bpb);
      destroy_broad_phase_info(bpi);
    }
    else
    {
      if ( (userData->m_bpb->m_flags & 2) == 0 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
               458,
               "userData->m_bpb->is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      bpg = broad_phase_base::get_bpg(userData->m_bpb);
      destroy_broad_phase_info_list(bpg->m_list_bpi_head);
      v3 = broad_phase_base::get_bpg(userData->m_bpb);
      destroy_broad_phase_group(v3);
    }
    userData->m_bpb = 0;
  }
}

// local variable allocation has failed, the output may be wrong!
PhysObjUserData * Phys_CreateUserBody@<eax>(
        float a1@<ebp>,
        float *position,
        int id,
        PhysicsGeomType geomType)
{
  int j; // [esp+14h] [ebp-21Ch]
  phys_vec3 *p_w; // [esp+20h] [ebp-210h]
  float v7; // [esp+28h] [ebp-208h]
  float v8; // [esp+2Ch] [ebp-204h]
  float x; // [esp+94h] [ebp-19Ch]
  float y; // [esp+98h] [ebp-198h]
  float z; // [esp+9Ch] [ebp-194h]
  float v12[3]; // [esp+ACh] [ebp-184h] BYREF
  float v13[3]; // [esp+B8h] [ebp-178h] BYREF
  float v14[3]; // [esp+C4h] [ebp-16Ch] BYREF
  float mx[3]; // [esp+D0h] [ebp-160h] BYREF
  float mn[16]; // [esp+DCh] [ebp-154h] BYREF
  float v17[3]; // [esp+11Ch] [ebp-114h] BYREF
  gjk_base_t *cylinder_gjk_geom; // [esp+128h] [ebp-108h]
  float v19[3]; // [esp+12Ch] [ebp-104h] OVERLAPPED BYREF
  float orientation[3][3]; // [esp+138h] [ebp-F8h] BYREF
  float hheight; // [esp+15Ch] [ebp-D4h]
  float radius; // [esp+160h] [ebp-D0h]
  int v23; // [esp+164h] [ebp-CCh]
  user_rigid_body *user_rigid_body; // [esp+168h] [ebp-C8h]
  PhysObjUserData *v25; // [esp+16Ch] [ebp-C4h]
  PhysObjUserData *v26; // [esp+170h] [ebp-C0h]
  user_rigid_body *body; // [esp+174h] [ebp-BCh]
  int bodyId; // [esp+178h] [ebp-B8h]
  PhysObjUserData *userData; // [esp+17Ch] [ebp-B4h]
  int v30; // [esp+180h] [ebp-B0h]
  phys_free_list<PhysObjUserData>::iterator i; // [esp+184h] [ebp-ACh]
  int v32; // [esp+188h] [ebp-A8h]
  float centerOfMass[14]; // [esp+18Ch] [ebp-A4h]
  unsigned int v34[3]; // [esp+1C4h] [ebp-6Ch] BYREF
  phys_mat44 dictator; // [esp+1D0h] [ebp-60h] BYREF
  unsigned int v36[3]; // [esp+214h] [ebp-1Ch] BYREF
  phys_vec3 pos; // [esp+220h] [ebp-10h] BYREF
  float retaddr; // [esp+230h] [ebp+0h]

  pos.y = a1;
  pos.z = retaddr;
  Phys_Vec3ToNitrousVec(position, (phys_vec3 *)v36);
  dictator.w.y = 1.0f;
  dictator.w.z = 0.0f;
  dictator.w.w = 0.0f;
  *(float *)v34 = 1.0f;
  v34[1] = 0;
  v34[2] = 0;
  centerOfMass[10] = 0.0f;
  centerOfMass[11] = 1.0f;
  centerOfMass[12] = 0.0f;
  LODWORD(centerOfMass[9]) = &dictator.x.y;
  dictator.x.y = 0.0f;
  dictator.x.z = 1.0f;
  dictator.x.w = 0.0f;
  centerOfMass[2] = 0.0f;
  centerOfMass[3] = 0.0f;
  centerOfMass[4] = 1.0f;
  LODWORD(centerOfMass[1]) = &dictator.y.y;
  dictator.y.y = 0.0f;
  dictator.y.z = 0.0f;
  dictator.y.w = 1.0f;
  LODWORD(centerOfMass[0]) = &dictator.z.y;
  LODWORD(dictator.z.y) = v36[0];
  LODWORD(dictator.z.z) = v36[1];
  LODWORD(dictator.z.w) = v36[2];
  v30 = 0;
  i.m_ptr = *(phys_free_list<PhysObjUserData>::T_internal_base **)&FLOAT_0_0;
  v32 = 0;
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  userData = (PhysObjUserData *)physGlob.objects.m_dummy_head.m_next_T_internal;
  for ( bodyId = (int)physGlob.objects.m_dummy_head.m_next_T_internal; ; bodyId = *(unsigned int *)(bodyId + 4) )
  {
    body = (user_rigid_body *)&physGlob;
    if ( &physGlob == (PhysGlob *)bodyId )
      break;
    v26 = (PhysObjUserData *)(bodyId + 16);
    if ( *(unsigned int *)(bodyId + 236) == id )
    {
      if ( !rigid_body::is_user_rigid_body(v26->body)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
              506,
              0,
              "%s",
              "userData->body->is_user_rigid_body()") )
      {
        __debugbreak();
      }
      ++v26->refcount;
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return v26;
    }
  }
  user_rigid_body = phys_sys::create_user_rigid_body(1);
  if ( user_rigid_body )
  {
    v26 = Phys_CreateUserData(0);
    if ( v26 )
    {
      v23 = geomType - 3;
      switch ( geomType )
      {
        case PHYS_GEOM_CYLINDER:
          hheight = 0.0f;
          radius = FLOAT_35_0;
          *(_QWORD *)&orientation[2][1] = __PAIR64__(0, LODWORD(15.0f));
          orientation[2][0] = FLOAT_35_0;
          v19[0] = 0.0f;
          v19[1] = 0.0f;
          v19[2] = 1.0f;
          *(_QWORD *)&orientation[0][0] = __PAIR64__(LODWORD(-1.0f), 0);
          orientation[0][2] = 0.0f;
          *(_QWORD *)&orientation[1][0] = __PAIR64__(0, LODWORD(1.0f));
          orientation[1][2] = 0.0f;
          cylinder_gjk_geom = create_cylinder_gjk_geom(
                                COERCE_FLOAT((phys_vec3 *)&pos.y),
                                (float (*)[3])v19,
                                &orientation[2][2],
                                15.0,
                                35.0,
                                7,
                                &g_empty_collision_visitor);
          break;
        case PHYS_GEOM_CYLINDER_LARGE:
          v17[0] = 0.0f;
          v17[1] = 0.0f;
          v17[2] = 20.0f;
          mn[15] = FLOAT_35_0;
          mn[14] = 20.0f;
          mn[5] = 0.0f;
          mn[6] = 0.0f;
          mn[7] = 1.0f;
          mn[8] = 0.0f;
          mn[9] = -1.0f;
          mn[10] = 0.0f;
          mn[11] = 1.0f;
          mn[12] = 0.0f;
          mn[13] = 0.0f;
          cylinder_gjk_geom = create_cylinder_gjk_geom(
                                COERCE_FLOAT((phys_vec3 *)&pos.y),
                                (float (*)[3])&mn[5],
                                v17,
                                35.0,
                                20.0,
                                7,
                                &g_empty_collision_visitor);
          break;
        case PHYS_GEOM_CAPSULE:
          mn[2] = 0.0f;
          mn[3] = 0.0f;
          mn[4] = 45.0f;
          mn[1] = 15.0f;
          mn[0] = 5.0f;
          cylinder_gjk_geom = create_capsule_gjk_geom(
                                COERCE_FLOAT((phys_vec3 *)&pos.y),
                                &mn[2],
                                15.0,
                                5.0,
                                2u,
                                7,
                                &g_empty_collision_visitor);
          break;
        case PHYS_GEOM_POINT:
          mx[0] = -1.0f;
          mx[1] = -1.0f;
          mx[2] = -1.0f;
          v14[0] = 1.0f;
          v14[1] = 1.0f;
          v14[2] = 1.0f;
          cylinder_gjk_geom = create_aabb_gjk_geom(
                                COERCE_FLOAT((phys_vec3 *)&pos.y),
                                mx,
                                v14,
                                7,
                                &g_empty_collision_visitor);
          break;
        default:
          v13[0] = -15.0f;
          v13[1] = -15.0f;
          v13[2] = 0.0f;
          v12[0] = 15.0f;
          v12[1] = 15.0f;
          v12[2] = 60.0f;
          cylinder_gjk_geom = create_aabb_gjk_geom(
                                COERCE_FLOAT((phys_vec3 *)&pos.y),
                                v13,
                                v12,
                                7,
                                &g_empty_collision_visitor);
          break;
      }
      cylinder_gjk_geom->comp_aabb_loc(cylinder_gjk_geom);
      phys_mat44::operator=(&v26->cg2rb, &PHYS_IDENTITY_MATRIX_43);
      phys_mat44::operator=(&v26->cg2w, &PHYS_IDENTITY_MATRIX_43);
      if ( (cylinder_gjk_geom->m_flags & 2) == 0
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
             83,
             "get_flag(FLAG_AABB_LOC_VALID)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      x = cylinder_gjk_geom->m_aabb_mx_loc.x;
      y = cylinder_gjk_geom->m_aabb_mx_loc.y;
      z = cylinder_gjk_geom->m_aabb_mx_loc.z;
      if ( (cylinder_gjk_geom->m_flags & 2) == 0
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
             82,
             "get_flag(FLAG_AABB_LOC_VALID)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      LODWORD(v7) = COERCE_UNSIGNED_INT(0.5 * (float)(cylinder_gjk_geom->m_aabb_mn_loc.y + y)) ^ _mask__NegFloat_;
      LODWORD(v8) = COERCE_UNSIGNED_INT(0.5 * (float)(cylinder_gjk_geom->m_aabb_mn_loc.z + z)) ^ _mask__NegFloat_;
      p_w = &v26->cg2rb.w;
      LODWORD(v26->cg2rb.w.x) = COERCE_UNSIGNED_INT(0.5 * (float)(cylinder_gjk_geom->m_aabb_mn_loc.x + x))
                              ^ _mask__NegFloat_;
      p_w->y = v7;
      p_w->z = v8;
      if ( gjk_geom_list_t::get_geom_count(&v26->m_gjk_geom_list)
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
             595,
             "userData->m_gjk_geom_list.get_geom_count() == 0",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      gjk_geom_list_t::add_geom(&v26->m_gjk_geom_list, cylinder_gjk_geom);
      v26->m_time_since_last_event = phys_impact_silence_window->current.value + 0.0099999998;
      v26->m_time_since_last_reeval = phys_reeval_frequency->current.value + 0.0099999998;
      v26->m_flags = 0;
      dictator.z.y = dictator.z.y - v26->cg2rb.w.x;
      dictator.z.z = dictator.z.z - v26->cg2rb.w.y;
      dictator.z.w = dictator.z.w - v26->cg2rb.w.z;
      user_rigid_body::set(user_rigid_body, (const phys_mat44 *const)v34);
      user_rigid_body->m_userdata = v26;
      rigid_body::set_flag(user_rigid_body, 0x40u, 1);
      v26->body = user_rigid_body;
      v26->refcount = 1;
      v26->id = id;
      v25 = v26;
      for ( j = 0; j < 16; ++j )
      {
        if ( !physGlob.userRigidBodies[j] )
        {
          physGlob.userRigidBodies[j] = v26;
          break;
        }
      }
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return v25;
    }
    else
    {
      phys_sys::destroy(user_rigid_body);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return 0;
    }
  }
  else
  {
    Com_PrintWarning(20, "Maximum number of physics bodies exceeded \n");
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    return 0;
  }
}

PhysObjUserData *__cdecl Phys_CreateUserData(int worldIndex)
{
  PhysObjUserData *userData; // [esp+2Ch] [ebp-4h]

  userData = phys_free_list<PhysObjUserData>::add(&physGlob.objects, 1, "phys_free_list error: out of memory.");
  if ( !userData )
    return 0;
  memset((unsigned __int8 *)userData, 0, sizeof(PhysObjUserData));
  phys_link_list1<PhysObjUserData>::add(&physGlob.objects_by_world[worldIndex], userData);
  return userData;
}

PhysObjUserData * Phys_CreateBodyFromState@<eax>(
        int a1@<ebp>,
        int worldIndex,
        const BodyState *state,
        gjk_geom_list_t *gjk_geom_list,
        bool do_collision_test)
{
  int m_geom_count; // ecx
  PhysObjUserData *v7; // edx
  phys_vec3 v8; // [esp+28h] [ebp-36Ch] BYREF
  float v9; // [esp+44h] [ebp-350h] BYREF
  phys_vec3 v10; // [esp+48h] [ebp-34Ch] BYREF
  float v11; // [esp+60h] [ebp-334h]
  float v12; // [esp+64h] [ebp-330h]
  float v13; // [esp+68h] [ebp-32Ch]
  float v14; // [esp+6Ch] [ebp-328h]
  float m_inv_mass; // [esp+70h] [ebp-324h]
  rigid_body *v16; // [esp+74h] [ebp-320h]
  float *mass; // [esp+78h] [ebp-31Ch]
  float *centerOfMassOffset; // [esp+7Ch] [ebp-318h]
  float *v19; // [esp+80h] [ebp-314h]
  float *buoyancyBoxMax; // [esp+84h] [ebp-310h]
  float *v21; // [esp+88h] [ebp-30Ch]
  float *buoyancyBoxMin; // [esp+8Ch] [ebp-308h]
  float *savedPos; // [esp+90h] [ebp-304h]
  int v24; // [esp+94h] [ebp-300h]
  float v25; // [esp+98h] [ebp-2FCh]
  float v26; // [esp+9Ch] [ebp-2F8h]
  float v27; // [esp+A0h] [ebp-2F4h]
  float v28; // [esp+A8h] [ebp-2ECh]
  float v29; // [esp+ACh] [ebp-2E8h]
  float v30; // [esp+B0h] [ebp-2E4h]
  float value; // [esp+B4h] [ebp-2E0h]
  float v32; // [esp+B8h] [ebp-2DCh] BYREF
  float v33; // [esp+BCh] [ebp-2D8h]
  float v34; // [esp+C0h] [ebp-2D4h]
  phys_vec3 gravity_dir; // [esp+C4h] [ebp-2D0h] BYREF
  phys_mat44 rb2w; // [esp+D4h] [ebp-2C0h] BYREF
  phys_vec3 *p_w; // [esp+154h] [ebp-240h]
  int v38; // [esp+158h] [ebp-23Ch]
  float v39; // [esp+15Ch] [ebp-238h]
  float v40; // [esp+160h] [ebp-234h]
  int v41; // [esp+16Ch] [ebp-228h]
  int v42; // [esp+170h] [ebp-224h]
  int v43; // [esp+174h] [ebp-220h]
  float v44[3]; // [esp+178h] [ebp-21Ch] BYREF
  phys_vec3 com_offset; // [esp+184h] [ebp-210h]
  phys_vec3 center; // [esp+194h] [ebp-200h]
  float v47; // [esp+1A4h] [ebp-1F0h]
  float v48; // [esp+1A8h] [ebp-1ECh]
  float v49; // [esp+1ACh] [ebp-1E8h]
  float v50; // [esp+1B0h] [ebp-1E4h]
  float v51; // [esp+1BCh] [ebp-1D8h]
  float v52; // [esp+1C0h] [ebp-1D4h]
  float v53; // [esp+1C4h] [ebp-1D0h]
  float v54; // [esp+1C8h] [ebp-1CCh] BYREF
  float v55; // [esp+1CCh] [ebp-1C8h]
  float v56; // [esp+1D0h] [ebp-1C4h]
  phys_vec3 gjk_geom_list_aabb_mn_loc; // [esp+1D4h] [ebp-1C0h] BYREF
  float *v58; // [esp+1F4h] [ebp-1A0h]
  float v59; // [esp+1F8h] [ebp-19Ch]
  float v60; // [esp+1FCh] [ebp-198h]
  float v61; // [esp+200h] [ebp-194h]
  float v62; // [esp+208h] [ebp-18Ch]
  float v63; // [esp+20Ch] [ebp-188h]
  float v64; // [esp+210h] [ebp-184h]
  float *v65; // [esp+214h] [ebp-180h]
  float v66; // [esp+218h] [ebp-17Ch]
  float v67; // [esp+21Ch] [ebp-178h]
  float v68; // [esp+220h] [ebp-174h]
  float v69; // [esp+228h] [ebp-16Ch]
  float v70; // [esp+22Ch] [ebp-168h]
  float v71; // [esp+230h] [ebp-164h]
  float *p_y; // [esp+234h] [ebp-160h]
  float v73; // [esp+238h] [ebp-15Ch]
  float v74; // [esp+23Ch] [ebp-158h]
  float v75; // [esp+240h] [ebp-154h]
  float v76; // [esp+24Ch] [ebp-148h]
  float v77; // [esp+250h] [ebp-144h]
  float v78; // [esp+254h] [ebp-140h]
  unsigned int v79[3]; // [esp+258h] [ebp-13Ch] BYREF
  phys_mat44 m2w; // [esp+264h] [ebp-130h] BYREF
  float v81; // [esp+2ACh] [ebp-E8h]
  float v82; // [esp+2B0h] [ebp-E4h]
  float v83; // [esp+2B4h] [ebp-E0h]
  float v84[3]; // [esp+2B8h] [ebp-DCh] BYREF
  phys_vec3 a_vel; // [esp+2C4h] [ebp-D0h]
  float v86; // [esp+2D4h] [ebp-C0h]
  float v87[3]; // [esp+2D8h] [ebp-BCh] BYREF
  phys_vec3 t_vel; // [esp+2E4h] [ebp-B0h]
  float v89; // [esp+2F4h] [ebp-A0h]
  float v90; // [esp+2F8h] [ebp-9Ch]
  int v91; // [esp+2FCh] [ebp-98h]
  float v92; // [esp+300h] [ebp-94h]
  float v93; // [esp+304h] [ebp-90h]
  int minStableContactCount; // [esp+308h] [ebp-8Ch] BYREF
  float maxAVel; // [esp+30Ch] [ebp-88h]
  float v96; // [esp+310h] [ebp-84h]
  float v97; // [esp+324h] [ebp-70h] BYREF
  unsigned int v98[2]; // [esp+328h] [ebp-6Ch] BYREF
  float volume; // [esp+330h] [ebp-64h]
  phys_vec3 dim; // [esp+334h] [ebp-60h]
  PhysObjUserData *v101; // [esp+344h] [ebp-50h]
  PhysObjUserData *v102; // [esp+348h] [ebp-4Ch]
  rigid_body *body; // [esp+34Ch] [ebp-48h]
  int bodyId; // [esp+350h] [ebp-44h]
  PhysObjUserData *userData; // [esp+354h] [ebp-40h]
  float v106; // [esp+358h] [ebp-3Ch]
  phys_free_list<PhysObjUserData>::iterator i; // [esp+35Ch] [ebp-38h]
  float v108; // [esp+360h] [ebp-34h]
  float v109; // [esp+364h] [ebp-30h]
  float v110; // [esp+368h] [ebp-2Ch]
  float v111; // [esp+36Ch] [ebp-28h]
  float v112; // [esp+370h] [ebp-24h]
  float v113; // [esp+374h] [ebp-20h]
  float v114; // [esp+378h] [ebp-1Ch]
  float v115; // [esp+37Ch] [ebp-18h]
  float v116; // [esp+380h] [ebp-14h]
  float v117; // [esp+384h] [ebp-10h]
  unsigned int v118[3]; // [esp+388h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+394h] [ebp+0h]

  v118[0] = a1;
  v118[1] = retaddr;
  if ( !state && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 754, 0, "%s", "state") )
    __debugbreak();
  v117 = state->position[0];
  if ( (LODWORD(v117) & 0x7F800000) == 0x7F800000
    || (v116 = state->position[1], (LODWORD(v116) & 0x7F800000) == 0x7F800000)
    || (v115 = state->position[2], (LODWORD(v115) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            756,
            0,
            "%s",
            "!IS_NAN((state->position)[0]) && !IS_NAN((state->position)[1]) && !IS_NAN((state->position)[2])") )
      __debugbreak();
  }
  v114 = state->velocity[0];
  if ( (LODWORD(v114) & 0x7F800000) == 0x7F800000
    || (v113 = state->velocity[1], (LODWORD(v113) & 0x7F800000) == 0x7F800000)
    || (v112 = state->velocity[2], (LODWORD(v112) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            757,
            0,
            "%s",
            "!IS_NAN((state->velocity)[0]) && !IS_NAN((state->velocity)[1]) && !IS_NAN((state->velocity)[2])") )
      __debugbreak();
  }
  v111 = state->angVelocity[0];
  if ( (LODWORD(v111) & 0x7F800000) == 0x7F800000
    || (v110 = state->angVelocity[1], (LODWORD(v110) & 0x7F800000) == 0x7F800000)
    || (v109 = state->angVelocity[2], (LODWORD(v109) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            758,
            0,
            "%s",
            "!IS_NAN((state->angVelocity)[0]) && !IS_NAN((state->angVelocity)[1]) && !IS_NAN((state->angVelocity)[2])") )
      __debugbreak();
  }
  v108 = state->centerOfMassOffset[0];
  if ( (LODWORD(v108) & 0x7F800000) == 0x7F800000
    || (i.m_ptr = (phys_free_list<PhysObjUserData>::T_internal_base *)LODWORD(state->centerOfMassOffset[1]),
        ((unsigned int)i.m_ptr & 0x7F800000) == 0x7F800000)
    || (v106 = state->centerOfMassOffset[2], (LODWORD(v106) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            759,
            0,
            "%s",
            "!IS_NAN((state->centerOfMassOffset)[0]) && !IS_NAN((state->centerOfMassOffset)[1]) && !IS_NAN((state->center"
            "OfMassOffset)[2])") )
      __debugbreak();
  }
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  if ( state->id != -1 )
  {
    userData = (PhysObjUserData *)physGlob.objects.m_dummy_head.m_next_T_internal;
    for ( bodyId = (int)physGlob.objects.m_dummy_head.m_next_T_internal; ; bodyId = *(unsigned int *)(bodyId + 4) )
    {
      body = (rigid_body *)&physGlob;
      if ( &physGlob == (PhysGlob *)bodyId )
        break;
      v102 = (PhysObjUserData *)(bodyId + 16);
      if ( *(unsigned int *)(bodyId + 236) == state->id )
      {
        destroy_gjk_geom(gjk_geom_list);
        ++v102->refcount;
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        return v102;
      }
    }
  }
  LODWORD(dim.w) = phys_sys::create_rigid_body(1);
  if ( LODWORD(dim.w) )
  {
    v102 = Phys_CreateUserData(worldIndex);
    if ( v102 )
    {
      *(float *)v98 = 3.0f;
      *(float *)&v98[1] = 3.0f;
      volume = 3.0f;
      nuge::calc_box_inertia((const phys_vec3 *)v98, (phys_vec3 *)&minStableContactCount, &v97);
      v93 = state->mass / v97;
      *(float *)&minStableContactCount = *(float *)&minStableContactCount * v93;
      maxAVel = maxAVel * v93;
      v96 = v96 * v93;
      v92 = 16.0f;
      v91 = 3;
      if ( worldIndex == 2 )
        v91 = 1;
      v90 = state->velocity[0];
      v89 = state->velocity[1];
      t_vel.w = state->velocity[2];
      v87[0] = v90;
      v87[1] = v89;
      v87[2] = t_vel.w;
      v86 = state->angVelocity[0];
      a_vel.w = state->angVelocity[1];
      a_vel.z = state->angVelocity[2];
      v84[0] = v86;
      v84[1] = a_vel.w;
      v84[2] = a_vel.z;
      v83 = state->rotation[0][0];
      v82 = state->rotation[0][1];
      v81 = state->rotation[0][2];
      m2w.w.y = v83;
      m2w.w.z = v82;
      m2w.w.w = v81;
      *(float *)v79 = v83;
      *(float *)&v79[1] = v82;
      *(float *)&v79[2] = v81;
      v78 = state->rotation[1][0];
      v77 = state->rotation[1][1];
      v76 = state->rotation[1][2];
      v73 = v78;
      v74 = v77;
      v75 = v76;
      p_y = &m2w.x.y;
      m2w.x.y = v78;
      m2w.x.z = v77;
      m2w.x.w = v76;
      v71 = state->rotation[2][0];
      v70 = state->rotation[2][1];
      v69 = state->rotation[2][2];
      v66 = v71;
      v67 = v70;
      v68 = v69;
      v65 = &m2w.y.y;
      m2w.y.y = v71;
      m2w.y.z = v70;
      m2w.y.w = v69;
      v64 = state->position[0];
      v63 = state->position[1];
      v62 = state->position[2];
      v59 = v64;
      v60 = v63;
      v61 = v62;
      v58 = &m2w.z.y;
      m2w.z.y = v64;
      m2w.z.z = v63;
      m2w.z.w = v62;
      v101 = v102;
      m_geom_count = gjk_geom_list->m_geom_count;
      v7 = v102;
      v102->m_gjk_geom_list.m_first_geom = gjk_geom_list->m_first_geom;
      v7->m_gjk_geom_list.m_geom_count = m_geom_count;
      gjk_geom_list_t::comp_aabb_loc(
        &v102->m_gjk_geom_list,
        (int)v118,
        (phys_vec3 *)&v54,
        (phys_vec3 *)&gjk_geom_list_aabb_mn_loc.y);
      phys_mat44::operator=(&v102->cg2rb, &PHYS_IDENTITY_MATRIX_43);
      phys_mat44::operator=(&v102->cg2w, &PHYS_IDENTITY_MATRIX_43);
      v53 = v54 + gjk_geom_list_aabb_mn_loc.y;
      v52 = v55 + gjk_geom_list_aabb_mn_loc.z;
      v51 = v56 + gjk_geom_list_aabb_mn_loc.w;
      v48 = v54 + gjk_geom_list_aabb_mn_loc.y;
      v49 = v55 + gjk_geom_list_aabb_mn_loc.z;
      v50 = v56 + gjk_geom_list_aabb_mn_loc.w;
      v47 = 0.5 * (float)(v54 + gjk_geom_list_aabb_mn_loc.y);
      center.w = 0.5 * (float)(v55 + gjk_geom_list_aabb_mn_loc.z);
      center.z = 0.5 * (float)(v56 + gjk_geom_list_aabb_mn_loc.w);
      com_offset.y = v47;
      com_offset.z = center.w;
      com_offset.w = center.z;
      Phys_Vec3ToNitrousVec(state->centerOfMassOffset, (phys_vec3 *)v44);
      com_offset.y = com_offset.y + v44[0];
      com_offset.z = com_offset.z + v44[1];
      com_offset.w = com_offset.w + v44[2];
      v43 = LODWORD(com_offset.y) ^ _mask__NegFloat_;
      v42 = LODWORD(com_offset.z) ^ _mask__NegFloat_;
      v41 = LODWORD(com_offset.w) ^ _mask__NegFloat_;
      v38 = LODWORD(com_offset.y) ^ _mask__NegFloat_;
      LODWORD(v39) = LODWORD(com_offset.z) ^ _mask__NegFloat_;
      LODWORD(v40) = LODWORD(com_offset.w) ^ _mask__NegFloat_;
      p_w = &v102->cg2rb.w;
      LODWORD(v102->cg2rb.w.x) = LODWORD(com_offset.y) ^ _mask__NegFloat_;
      p_w->y = v39;
      p_w->z = v40;
      phys_full_inverse((int)v118, (phys_mat44 *)&rb2w.w.y, &v102->cg2rb);
      phys_full_multiply_mat((int)v118, (phys_mat44 *)&gravity_dir.y, (const phys_mat44 *)v79, (phys_mat44 *)&rb2w.w.y);
      if ( Abs(v87) >= 100000.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
              862,
              0,
              "%s",
              "Abs(t_vel) < 100000.0f") )
      {
        __debugbreak();
      }
      if ( Abs(v84) >= 1000.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
              863,
              0,
              "%s",
              "Abs(a_vel) < 1000.0f") )
      {
        __debugbreak();
      }
      rigid_body::set(
        (rigid_body *)LODWORD(dim.w),
        state->mass,
        (const phys_vec3 *)&minStableContactCount,
        (const phys_mat44 *)&gravity_dir.y,
        (const phys_vec3 *)v87,
        (const phys_vec3 *)v84,
        v91);
      *(unsigned int *)(LODWORD(dim.w) + 292) = v102;
      Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, (phys_vec3 *)&v32);
      value = phys_gravity->current.value;
      v30 = value * v32;
      v29 = value * v33;
      v28 = value * v34;
      v25 = value * v32;
      v26 = value * v33;
      v27 = value * v34;
      v24 = LODWORD(dim.w) + 128;
      *(float *)(LODWORD(dim.w) + 128) = value * v32;
      *(float *)(v24 + 4) = v26;
      *(float *)(v24 + 8) = v27;
      *(float *)(LODWORD(dim.w) + 244) = v92;
      v102->body = (rigid_body *)LODWORD(dim.w);
      savedPos = v102->savedPos;
      v102->savedPos[0] = state->position[0];
      savedPos[1] = state->position[1];
      savedPos[2] = state->position[2];
      memcpy(v102->savedRot, state->rotation, sizeof(v102->savedRot));
      v102->bounce = state->bounce;
      v102->buoyancy = state->buoyancy;
      v102->friction = state->friction;
      v102->underwater = state->underwater;
      v102->mass = state->mass;
      v102->id = state->id;
      v102->refcount = 1;
      v102->timeBuoyant = -1;
      v102->timeRipple = -1;
      v102->m_time_since_last_event = phys_impact_silence_window->current.value + 0.0099999998;
      v102->m_time_since_last_reeval = phys_reeval_frequency->current.value + 0.0099999998;
      v102->m_flags = 0;
      buoyancyBoxMin = v102->buoyancyBoxMin;
      v21 = state->buoyancyBoxMin;
      v102->buoyancyBoxMin[0] = state->buoyancyBoxMin[0];
      buoyancyBoxMin[1] = v21[1];
      buoyancyBoxMin[2] = v21[2];
      buoyancyBoxMax = v102->buoyancyBoxMax;
      v19 = state->buoyancyBoxMax;
      v102->buoyancyBoxMax[0] = state->buoyancyBoxMax[0];
      buoyancyBoxMax[1] = v19[1];
      buoyancyBoxMax[2] = v19[2];
      centerOfMassOffset = v102->centerOfMassOffset;
      mass = state->centerOfMassOffset;
      v102->centerOfMassOffset[0] = state->centerOfMassOffset[0];
      centerOfMassOffset[1] = mass[1];
      centerOfMassOffset[2] = mass[2];
      v16 = v102->body;
      m_inv_mass = v16->m_inv_mass;
      v14 = 1.0 / m_inv_mass;
      v13 = gjk_geom_list_aabb_mn_loc.y - v54;
      v12 = gjk_geom_list_aabb_mn_loc.z - v55;
      v11 = gjk_geom_list_aabb_mn_loc.w - v56;
      v10.x = gjk_geom_list_aabb_mn_loc.y - v54;
      v10.y = gjk_geom_list_aabb_mn_loc.z - v55;
      v10.z = gjk_geom_list_aabb_mn_loc.w - v56;
      if ( Abs(&v10.x) >= 10000.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
              902,
              0,
              "%s",
              "Abs(dim) < 10000.0f") )
      {
        __debugbreak();
      }
      nuge::calc_box_inertia(&v10, &v8, &v9);
      v8.x = v8.x * (float)(v14 / v9);
      v8.y = v8.y * (float)(v14 / v9);
      v8.z = v8.z * (float)(v14 / v9);
      rigid_body::set_inertia(v16, &v8);
      create_broad_phase_info(v102->body);
      if ( do_collision_test )
        BLOPS_NULLSUB();
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return v101;
    }
    else
    {
      destroy_gjk_geom(gjk_geom_list);
      phys_sys::destroy((rigid_body *const)LODWORD(dim.w));
      Com_PrintWarning(20, "Maximum number of Phys User Data exceeded\n");
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return 0;
    }
  }
  else
  {
    destroy_gjk_geom(gjk_geom_list);
    Com_PrintWarning(20, "Maximum number of physics bodies exceeded\n");
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    return 0;
  }
}

void  phys_full_inverse(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *source)
{
  _BYTE v3[12]; // [esp-Ch] [ebp-8Ch] BYREF
  phys_vec3 *p_w; // [esp+40h] [ebp-40h]
  float v5; // [esp+48h] [ebp-38h]
  float v6; // [esp+4Ch] [ebp-34h]
  float v7; // [esp+54h] [ebp-2Ch]
  float v8; // [esp+58h] [ebp-28h]
  float v9; // [esp+5Ch] [ebp-24h]
  const phys_vec3 *v10; // [esp+60h] [ebp-20h]
  phys_vec3 v11; // [esp+64h] [ebp-1Ch] BYREF
  int v12; // [esp+74h] [ebp-Ch]
  void *v13; // [esp+78h] [ebp-8h]
  void *retaddr; // [esp+80h] [ebp+0h]

  v12 = a1;
  v13 = retaddr;
  if ( dest == source )
  {
    phys_full_inverse((phys_mat44 *)v3, source);
    phys_mat44::operator=(dest, (const phys_mat44 *)v3);
  }
  else
  {
    phys_transpose(dest, source);
    v10 = phys_multiply(&v11, dest, &source->w);
    LODWORD(v9) = LODWORD(v10->x) ^ _mask__NegFloat_;
    LODWORD(v8) = LODWORD(v10->y) ^ _mask__NegFloat_;
    LODWORD(v7) = LODWORD(v10->z) ^ _mask__NegFloat_;
    v5 = v8;
    v6 = v7;
    p_w = &dest->w;
    dest->w.x = v9;
    p_w->y = v5;
    p_w->z = v6;
  }
}

void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list)
{
  gjk_base_t *next_geom; // [esp+0h] [ebp-8h]
  gjk_aabb_t *geom; // [esp+4h] [ebp-4h]

  if ( gjk_geom_list->m_geom_count < 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
         1035,
         "m_geom_count >= 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  for ( geom = (gjk_aabb_t *)gjk_geom_list->m_first_geom; geom; geom = (gjk_aabb_t *)next_geom )
  {
    next_geom = geom->m_next_geom;
    destroy_gjk_geom(geom);
  }
  gjk_geom_list->m_first_geom = 0;
  gjk_geom_list->m_geom_count = 0;
}

PhysObjUserData *__cdecl Phys_ObjCreate(
        int worldIndex,
        const float *position,
        const float *quat,
        const float *velocity,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list,
        bool do_collision_test,
        int id)
{
  float axis[3][3]; // [esp+24h] [ebp-24h] BYREF

  if ( ((*(unsigned int *)position & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          964,
          0,
          "%s",
          "!IS_NAN((position)[0]) && !IS_NAN((position)[1]) && !IS_NAN((position)[2])") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)quat & 0x7F800000) == 0x7F800000
     || ((unsigned int)quat[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)quat[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          965,
          0,
          "%s",
          "!IS_NAN((quat)[0]) && !IS_NAN((quat)[1]) && !IS_NAN((quat)[2])") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)velocity & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          966,
          0,
          "%s",
          "!IS_NAN((velocity)[0]) && !IS_NAN((velocity)[1]) && !IS_NAN((velocity)[2])") )
  {
    __debugbreak();
  }
  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 968, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  if ( !physPreset
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 969, 0, "%s", "physPreset") )
  {
    __debugbreak();
  }
  if ( !gjk_geom_list
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 970, 0, "%s", "gjk_geom_list") )
  {
    __debugbreak();
  }
  QuatToAxis(quat, axis);
  return Phys_ObjCreateAxis(worldIndex, position, axis, velocity, physPreset, gjk_geom_list, id, do_collision_test);
}

PhysObjUserData *__cdecl Phys_ObjCreateAxis(
        int worldIndex,
        const float *position,
        const float (*axis)[3],
        const float *velocity,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list,
        int id,
        bool do_collision_test)
{
  float v8; // xmm0_4
  BodyState state; // [esp+24h] [ebp-90h] BYREF
  int savedregs; // [esp+B4h] [ebp+0h] BYREF

  if ( ((*(unsigned int *)position & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          928,
          0,
          "%s",
          "!IS_NAN((position)[0]) && !IS_NAN((position)[1]) && !IS_NAN((position)[2])") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)velocity & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          929,
          0,
          "%s",
          "!IS_NAN((velocity)[0]) && !IS_NAN((velocity)[1]) && !IS_NAN((velocity)[2])") )
  {
    __debugbreak();
  }
  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 931, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  if ( !physPreset
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 932, 0, "%s", "physPreset") )
  {
    __debugbreak();
  }
  AxisCopy(axis, state.rotation);
  state.position[0] = *position;
  state.position[1] = position[1];
  state.position[2] = position[2];
  state.velocity[0] = *velocity;
  state.velocity[1] = velocity[1];
  state.velocity[2] = velocity[2];
  memset(state.angVelocity, 0, sizeof(state.angVelocity));
  state.mass = physPreset->mass;
  state.bounce = physPreset->bounce;
  state.friction = physPreset->friction;
  state.timeLastAsleep = physGlob.timeLastUpdate;
  state.underwater = 0;
  state.id = id;
  if ( physPreset->canFloat )
    v8 = phys_gravity->current.value * -1.1;
  else
    v8 = phys_gravity->current.value * -0.89999998;
  state.buoyancy = (int)v8;
  state.buoyancyBoxMin[0] = physPreset->buoyancyBoxMin[0];
  state.buoyancyBoxMin[1] = physPreset->buoyancyBoxMin[1];
  state.buoyancyBoxMin[2] = physPreset->buoyancyBoxMin[2];
  state.buoyancyBoxMax[0] = physPreset->buoyancyBoxMax[0];
  state.buoyancyBoxMax[1] = physPreset->buoyancyBoxMax[1];
  state.buoyancyBoxMax[2] = physPreset->buoyancyBoxMax[2];
  state.centerOfMassOffset[0] = physPreset->centerOfMassOffset[0];
  state.centerOfMassOffset[1] = physPreset->centerOfMassOffset[1];
  state.centerOfMassOffset[2] = physPreset->centerOfMassOffset[2];
  return Phys_CreateBodyFromState((int)&savedregs, worldIndex, &state, gjk_geom_list, do_collision_test);
}

void  Phys_ObjSetPosition(int a1@<ebp>, int id, float *newPosition)
{
  int v3; // [esp-B8h] [ebp-DCh]
  float *savedPos; // [esp-64h] [ebp-88h]
  phys_vec3 *p_m_last_position; // [esp-48h] [ebp-6Ch]
  int p_m_mat; // [esp-40h] [ebp-64h]
  phys_vec3 v7; // [esp-38h] [ebp-5Ch] BYREF
  float v8; // [esp-1Ch] [ebp-40h]
  float v9; // [esp-18h] [ebp-3Ch]
  float v10; // [esp-14h] [ebp-38h]
  phys_vec3 *p_w; // [esp-10h] [ebp-34h]
  PhysObjUserData *v12; // [esp-Ch] [ebp-30h]
  float v13[2]; // [esp-8h] [ebp-2Ch] BYREF
  PhysObjUserData *userData; // [esp+0h] [ebp-24h]
  phys_vec3 newPos; // [esp+4h] [ebp-20h]
  int v16; // [esp+14h] [ebp-10h]
  int v17; // [esp+18h] [ebp-Ch]
  void *v18; // [esp+1Ch] [ebp-8h]
  void *retaddr; // [esp+24h] [ebp+0h]

  v17 = a1;
  v18 = retaddr;
  v16 = *(unsigned int *)newPosition;
  if ( (v16 & 0x7F800000) == 0x7F800000
    || (newPos.w = newPosition[1], (LODWORD(newPos.w) & 0x7F800000) == 0x7F800000)
    || (newPos.z = newPosition[2], (LODWORD(newPos.z) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            979,
            0,
            "%s",
            "!IS_NAN((newPosition)[0]) && !IS_NAN((newPosition)[1]) && !IS_NAN((newPosition)[2])") )
      __debugbreak();
  }
  Phys_Vec3ToNitrousVec(newPosition, (phys_vec3 *)v13);
  v12 = Phys_GetUserData(id);
  p_w = &v12->cg2rb.w;
  v10 = v13[0] + v12->cg2rb.w.x;
  v9 = v13[1] + v12->cg2rb.w.y;
  v8 = *(float *)&userData + v12->cg2rb.w.z;
  v7.x = v10;
  v7.y = v9;
  v7.z = v8;
  phys_vec3::operator=(&v12->body->m_mat.w, &v7);
  p_m_mat = (int)&v12->body->m_mat;
  p_m_last_position = &v12->body->m_last_position;
  p_m_last_position->x = v12->body->m_mat.w.x;
  p_m_last_position->y = *(float *)(p_m_mat + 52);
  p_m_last_position->z = *(float *)(p_m_mat + 56);
  if ( ((LODWORD(v12->body->m_last_position.x) & 0x7F800000) == 0x7F800000
     || (LODWORD(v12->body->m_last_position.y) & 0x7F800000) == 0x7F800000
     || (LODWORD(v12->body->m_last_position.z) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          985,
          0,
          "%s",
          "!IS_NAN((userData->body->m_last_position)[0]) && !IS_NAN((userData->body->m_last_position)[1]) && !IS_NAN((use"
          "rData->body->m_last_position)[2])") )
  {
    __debugbreak();
  }
  savedPos = v12->savedPos;
  v12->savedPos[0] = *newPosition;
  savedPos[1] = newPosition[1];
  savedPos[2] = newPosition[2];
  if ( fabs(v12->body->m_last_position.x) > 100000.0
    || fabs(v12->body->m_last_position.y) > 100000.0
    || fabs(v12->body->m_last_position.z) > 100000.0 )
  {
    phys_exec_debug_callback(v12->body);
  }
  if ( fabs(v12->body->m_mat.w.x) > 100000.0
    || fabs(v12->body->m_mat.w.y) > 100000.0
    || (v3 = (int)&v12->body->m_mat, *(float *)(v3 + 56) != *phys_vec3::operator[]<int>(&v12->body->m_mat.w, 2u))
    || COERCE_FLOAT(*(unsigned int *)phys_vec3::operator[]<int>(&v12->body->m_mat.w, 2u) & _mask__AbsFloat_) > 100000.0 )
  {
    phys_exec_debug_callback(v12->body);
  }
}

void __cdecl Phys_ObjSetOrientation(int id, const float *newPosition, const float *newOrientation)
{
  float *v3; // esi
  phys_mat44 *p_m_mat; // [esp+Ch] [ebp-CCh]
  phys_vec3 *p_w; // [esp+7Ch] [ebp-5Ch]
  rigid_body *body; // [esp+80h] [ebp-58h]
  float *savedPos; // [esp+84h] [ebp-54h]
  float newRotation[3][3]; // [esp+88h] [ebp-50h] BYREF
  float newCenterOfGravity[3]; // [esp+ACh] [ebp-2Ch] BYREF
  float rotatedCGOffset[3]; // [esp+B8h] [ebp-20h] BYREF
  PhysObjUserData *userData; // [esp+C4h] [ebp-14h]
  phys_mat44 *rb2w; // [esp+C8h] [ebp-10h]
  float trans[3]; // [esp+CCh] [ebp-Ch] BYREF

  userData = Phys_GetUserData(id);
  rb2w = &userData->body->m_mat;
  Phys_NitrousVecToVec3(&userData->cg2rb.w, trans);
  QuatToAxis(newOrientation, newRotation);
  AxisTransformVec3(newRotation, trans, rotatedCGOffset);
  newCenterOfGravity[0] = *newPosition - rotatedCGOffset[0];
  newCenterOfGravity[1] = newPosition[1] - rotatedCGOffset[1];
  newCenterOfGravity[2] = newPosition[2] - rotatedCGOffset[2];
  savedPos = userData->savedPos;
  userData->savedPos[0] = newCenterOfGravity[0];
  savedPos[1] = newCenterOfGravity[1];
  savedPos[2] = newCenterOfGravity[2];
  AxisCopy(newRotation, userData->savedRot);
  Phys_Vec3ToNitrousVec(newCenterOfGravity, &rb2w->w);
  Phys_AxisToNitrousMat(newRotation, rb2w);
  p_w = &rb2w->w;
  body = userData->body;
  body->m_last_position.x = rb2w->w.x;
  body->m_last_position.y = p_w->y;
  body->m_last_position.z = p_w->z;
  if ( ((LODWORD(userData->body->m_last_position.x) & 0x7F800000) == 0x7F800000
     || (LODWORD(userData->body->m_last_position.y) & 0x7F800000) == 0x7F800000
     || (LODWORD(userData->body->m_last_position.z) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1011,
          0,
          "%s",
          "!IS_NAN((userData->body->m_last_position)[0]) && !IS_NAN((userData->body->m_last_position)[1]) && !IS_NAN((use"
          "rData->body->m_last_position)[2])") )
  {
    __debugbreak();
  }
  if ( fabs(userData->body->m_last_position.x) > 100000.0
    || fabs(userData->body->m_last_position.y) > 100000.0
    || fabs(userData->body->m_last_position.z) > 100000.0 )
  {
    phys_exec_debug_callback(userData->body);
  }
  if ( fabs(userData->body->m_mat.w.x) > 100000.0
    || fabs(userData->body->m_mat.w.y) > 100000.0
    || (p_m_mat = &userData->body->m_mat,
        v3 = phys_vec3::operator[]<int>(&userData->body->m_mat.w, 2u),
        *v3 != *phys_vec3::operator[]<int>(&p_m_mat->w, 2u))
    || COERCE_FLOAT(*(unsigned int *)phys_vec3::operator[]<int>(&userData->body->m_mat.w, 2u) & _mask__AbsFloat_) > 100000.0 )
  {
    phys_exec_debug_callback(userData->body);
  }
}

void  Phys_ObjSetAngularVelocity(int a1@<ebp>, int id, float *angularVel)
{
  phys_vec3 v3; // [esp-20h] [ebp-2Ch] BYREF
  float v4; // [esp-10h] [ebp-1Ch]
  float v5; // [esp-Ch] [ebp-18h]
  float v6; // [esp-8h] [ebp-14h]
  rigid_body *body; // [esp-4h] [ebp-10h]
  int v8; // [esp+0h] [ebp-Ch]
  void *v9; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v8 = a1;
  v9 = retaddr;
  body = Phys_GetUserData(id)->body;
  v6 = angularVel[2];
  v5 = *angularVel;
  v4 = angularVel[1];
  v3.x = v6;
  v3.y = v5;
  v3.z = v4;
  rigid_body::dangerous_set_a_vel(body, &v3);
}

void __thiscall rigid_body::dangerous_set_a_vel(rigid_body *this, const phys_vec3 *a_vel)
{
  this->m_a_vel.x = a_vel->x;
  this->m_a_vel.y = a_vel->y;
  this->m_a_vel.z = a_vel->z;
  if ( fabs(this->m_a_vel.x) > 100000.0
    || fabs(this->m_a_vel.y) > 100000.0
    || fabs(this->m_a_vel.z) > 100000.0 )
  {
    phys_exec_debug_callback(this);
  }
}

void  Phys_ObjSetVelocity(int a1@<ebp>, int id, float *velocity)
{
  phys_vec3 v3; // [esp-20h] [ebp-2Ch] BYREF
  float v4; // [esp-10h] [ebp-1Ch]
  float v5; // [esp-Ch] [ebp-18h]
  float v6; // [esp-8h] [ebp-14h]
  rigid_body *body; // [esp-4h] [ebp-10h]
  int v8; // [esp+0h] [ebp-Ch]
  void *v9; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v8 = a1;
  v9 = retaddr;
  body = Phys_GetUserData(id)->body;
  v6 = *velocity;
  v5 = velocity[1];
  v4 = velocity[2];
  v3.x = v6;
  v3.y = v5;
  v3.z = v4;
  rigid_body::dangerous_set_t_vel(body, &v3);
}

void __thiscall rigid_body::dangerous_set_t_vel(rigid_body *this, const phys_vec3 *t_vel)
{
  this->m_t_vel.x = t_vel->x;
  this->m_t_vel.y = t_vel->y;
  this->m_t_vel.z = t_vel->z;
  if ( fabs(this->m_t_vel.x) > 100000.0
    || fabs(this->m_t_vel.y) > 100000.0
    || fabs(this->m_t_vel.z) > 100000.0 )
  {
    phys_exec_debug_callback(this);
  }
}

void  Phys_ObjSetAngularVelocityRaw(int a1@<ebp>, int id, float *angularVel)
{
  phys_vec3 v3; // [esp-20h] [ebp-2Ch] BYREF
  float v4; // [esp-10h] [ebp-1Ch]
  float v5; // [esp-Ch] [ebp-18h]
  float v6; // [esp-8h] [ebp-14h]
  rigid_body *body; // [esp-4h] [ebp-10h]
  int v8; // [esp+0h] [ebp-Ch]
  void *v9; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v8 = a1;
  v9 = retaddr;
  body = Phys_GetUserData(id)->body;
  v6 = *angularVel;
  v5 = angularVel[1];
  v4 = angularVel[2];
  v3.x = v6;
  v3.y = v5;
  v3.z = v4;
  rigid_body::dangerous_set_a_vel(body, &v3);
}

void  Phys_ObjGetPosition(int a1@<ebp>, int id, float *outPosition, float (*outRotation)[3])
{
  _BYTE v4[12]; // [esp-Ch] [ebp-5Ch] BYREF
  phys_mat44 m2w; // [esp+0h] [ebp-50h] BYREF
  PhysObjUserData *UserData; // [esp+40h] [ebp-10h]
  int v7; // [esp+44h] [ebp-Ch] BYREF
  const phys_mat44 *rb2w; // [esp+48h] [ebp-8h]
  const phys_mat44 *retaddr; // [esp+50h] [ebp+0h]

  v7 = a1;
  rb2w = retaddr;
  UserData = Phys_GetUserData(id);
  LODWORD(m2w.w.w) = &UserData->body->m_mat;
  phys_full_multiply_mat((int)&v7, (phys_mat44 *)v4, (const phys_mat44 *)LODWORD(m2w.w.w), &UserData->cg2rb);
  Phys_NitrousVecToVec3((const phys_vec3 *)&m2w.z.y, outPosition);
  Phys_NitrousMat44ToVec33((const phys_mat44 *)v4, outRotation);
}

void __cdecl Phys_ObjGetVelocities(int id, float *tvel, float *avel)
{
  rigid_body *body; // [esp+0h] [ebp-4h]

  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1047, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  body = Phys_GetUserData(id)->body;
  Phys_NitrousVecToVec3(&body->m_t_vel, tvel);
  Phys_NitrousVecToVec3(&body->m_a_vel, avel);
}

void __cdecl Phys_ObjGetCenterOfMass(int id, float *outPosition)
{
  PhysObjUserData *UserData; // eax

  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1055, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  UserData = Phys_GetUserData(id);
  Phys_NitrousVecToVec3(&UserData->body->m_mat.w, outPosition);
}

void __cdecl Phys_ObjAddCollFlags(int physObjId, int collFlags)
{
  broad_phase_info *i; // [esp+Ch] [ebp-14h]
  broad_phase_info *bpi; // [esp+14h] [ebp-Ch]

  if ( !physObjId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1063,
          0,
          "%s",
          "physObjId != PHYS_OBJ_ID_NULL") )
  {
    __debugbreak();
  }
  if ( (*(unsigned int *)(*(unsigned int *)(physObjId + 160) + 48) & 1) != 0 )
  {
    bpi = broad_phase_base::get_bpi(*(broad_phase_base **)(physObjId + 160));
    bpi->m_env_collision_flags |= collFlags;
  }
  else
  {
    for ( i = broad_phase_base::get_bpg(*(broad_phase_base **)(physObjId + 160))->m_list_bpi_head;
          i;
          i = (broad_phase_info *)i->m_list_bpb_next )
    {
      i->m_env_collision_flags |= collFlags;
    }
  }
}

void __cdecl Phys_ObjRemoveCollFlags(int physObjId, int collFlags)
{
  broad_phase_info *i; // [esp+Ch] [ebp-14h]
  broad_phase_info *bpi; // [esp+14h] [ebp-Ch]

  if ( !physObjId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1087,
          0,
          "%s",
          "physObjId != PHYS_OBJ_ID_NULL") )
  {
    __debugbreak();
  }
  if ( (*(unsigned int *)(*(unsigned int *)(physObjId + 160) + 48) & 1) != 0 )
  {
    bpi = broad_phase_base::get_bpi(*(broad_phase_base **)(physObjId + 160));
    bpi->m_env_collision_flags &= ~collFlags;
  }
  else
  {
    for ( i = broad_phase_base::get_bpg(*(broad_phase_base **)(physObjId + 160))->m_list_bpi_head;
          i;
          i = (broad_phase_info *)i->m_list_bpb_next )
    {
      i->m_env_collision_flags &= ~collFlags;
    }
  }
}

void __cdecl fixup_wheel_constraints(rigid_body *rb)
{
  phys_free_list<rigid_body_constraint_wheel>::T_internal_base *wci; // [esp+Ch] [ebp-8h]
  phys_free_list<rigid_body_constraint_wheel> *wci_end; // [esp+10h] [ebp-4h]

  wci = g_physics_system->m_list_rbc_wheel.m_dummy_head.m_next_T_internal;
  wci_end = &g_physics_system->m_list_rbc_wheel;
  while ( wci_end != (phys_free_list<rigid_body_constraint_wheel> *)wci )
  {
    if ( wci != (phys_free_list<rigid_body_constraint_wheel>::T_internal_base *)-16
      && (rigid_body *)wci[2].m_next_T_internal == rb )
    {
      rigid_body_constraint_wheel::set_no_collision((rigid_body_constraint_wheel *)&wci[2]);
    }
    wci = wci->m_next_T_internal;
  }
}

void __cdecl Phys_ObjDestroy(int worldIndex, int id)
{
  int i; // [esp+Ch] [ebp-8h]
  PhysObjUserData *userData; // [esp+10h] [ebp-4h]

  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1126, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  userData = Phys_GetUserData(id);
  if ( --userData->refcount <= 0 )
  {
    fixup_wheel_constraints(userData->body);
    destroy_broad_phase_info(userData->body);
    if ( (userData->body->m_flags & 0x20) != 0 )
    {
      for ( i = 0; i < 16; ++i )
      {
        if ( physGlob.userRigidBodies[i] == userData )
          physGlob.userRigidBodies[i] = 0;
      }
      phys_sys::destroy((user_rigid_body *const)userData->body);
    }
    else
    {
      phys_sys::destroy(userData->body);
    }
    userData->body = 0;
    destroy_gjk_geom(&userData->m_gjk_geom_list);
    Phys_DestroyUserData(worldIndex, userData);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  }
  else
  {
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  }
}

void __cdecl Phys_DestroyUserData(int worldIndex, PhysObjUserData *userData)
{
  phys_link_list1<PhysObjUserData>::remove(&physGlob.objects_by_world[worldIndex], userData);
  phys_free_list<PhysObjUserData>::remove(&physGlob.objects, userData);
}

void  Phys_AddCacheImpulses(int a1@<ebp>)
{
  phys_vec3 v1; // [esp+14h] [ebp-5Ch] BYREF
  float v2; // [esp+28h] [ebp-48h]
  float v3; // [esp+2Ch] [ebp-44h]
  float v4; // [esp+30h] [ebp-40h]
  float v5; // [esp+34h] [ebp-3Ch] BYREF
  float v6; // [esp+38h] [ebp-38h]
  float v7; // [esp+3Ch] [ebp-34h]
  phys_vec3 force; // [esp+40h] [ebp-30h] BYREF
  phys_vec3 pos; // [esp+50h] [ebp-20h]
  rigid_body *body; // [esp+60h] [ebp-10h]
  int v11; // [esp+64h] [ebp-Ch]
  PhysImpulse *impulse; // [esp+68h] [ebp-8h]
  PhysImpulse *retaddr; // [esp+70h] [ebp+0h]

  v11 = a1;
  impulse = retaddr;
  //PIXBeginNamedEvent(-1, "Phys_AddCacheImpulses");
  for ( body = 0; (int)body < gImpulseCacheNum; body = (rigid_body *)((char *)body + 1) )
  {
    LODWORD(pos.w) = &gImpulseCache[(unsigned int)body];
    pos.z = *(float *)LODWORD(pos.w);
    LODWORD(pos.y) = Phys_GetUserData(SLODWORD(pos.z))->body;
    if ( LODWORD(pos.y) )
    {
      Phys_Vec3ToNitrousVec((float *)(LODWORD(pos.w) + 4), (phys_vec3 *)&force.y);
      Phys_Vec3ToNitrousVec((float *)(LODWORD(pos.w) + 16), (phys_vec3 *)&v5);
      if ( Abs(&v5) >= 1000000.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
              1206,
              0,
              "%s",
              "Abs(force) < 1000000.0f") )
      {
        __debugbreak();
      }
      v4 = 0.001 * v5;
      v3 = 0.001 * v6;
      v2 = 0.001 * v7;
      v1.x = 0.001 * v5;
      v1.y = 0.001 * v6;
      v1.z = 0.001 * v7;
      rigid_body::add_force((rigid_body *)LODWORD(pos.y), &v1, (phys_vec3 *)&force.y, 1.0);
    }
  }
  gImpulseCacheNum = 0;
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl Phys_ObjAddCustomForce(
        int physObjId,
        const float *hitPos,
        const float *hitDir,
        int mod,
        PhysPreset *physPreset)
{
  float v5; // [esp+24h] [ebp-3Ch]
  float v6; // [esp+28h] [ebp-38h]
  float offset_4; // [esp+30h] [ebp-30h]
  float offset_8; // [esp+34h] [ebp-2Ch]
  float torque[3]; // [esp+38h] [ebp-28h] BYREF
  float modHitPos[3]; // [esp+44h] [ebp-1Ch] BYREF
  float scale; // [esp+50h] [ebp-10h]
  float modHitDir[3]; // [esp+54h] [ebp-Ch] BYREF
  int savedregs; // [esp+60h] [ebp+0h] BYREF

  if ( physObjId )
  {
    switch ( mod )
    {
      case 0:
        modHitDir[0] = *hitDir;
        modHitDir[1] = hitDir[1];
        modHitDir[2] = hitDir[2];
        scale = Vec3Normalize(modHitDir);
        break;
      case 4:
      case 6:
      case 15:
        scale = dynEnt_explodeForce->current.value;
        scale = scale * physPreset->explosiveForceScale;
        Phys_ObjGetCenterOfMass(physObjId, modHitPos);
        offset_4 = hitPos[1] - modHitPos[1];
        offset_8 = hitPos[2] - modHitPos[2];
        modHitPos[0] = (float)(spin_scale_0 * (float)(*hitPos - modHitPos[0])) + modHitPos[0];
        modHitPos[1] = (float)(spin_scale_0 * offset_4) + modHitPos[1];
        modHitPos[2] = (float)(spin_scale_0 * offset_8) + modHitPos[2];
        modHitDir[0] = scale * *hitDir;
        modHitDir[1] = scale * hitDir[1];
        modHitDir[2] = scale * hitDir[2];
        break;
      case 17:
        scale = 1.0f;
        Phys_ObjGetCenterOfMass(physObjId, modHitPos);
        modHitDir[0] = *hitDir;
        modHitDir[1] = hitDir[1];
        modHitDir[2] = hitDir[2];
        torque[0] = CG_flrand(COERCE_FLOAT(LODWORD(range_1) ^ _mask__NegFloat_), range_1);
        torque[1] = CG_flrand(COERCE_FLOAT(LODWORD(range_1) ^ _mask__NegFloat_), range_1);
        torque[2] = CG_flrand(COERCE_FLOAT(LODWORD(range_1) ^ _mask__NegFloat_), range_1);
        Phys_ObjAddTorque((int)&savedregs, physObjId, torque);
        break;
      default:
        scale = dynEnt_bulletForce->current.value;
        scale = scale * physPreset->bulletForceScale;
        Phys_ObjGetCenterOfMass(physObjId, modHitPos);
        v5 = hitPos[1] - modHitPos[1];
        v6 = hitPos[2] - modHitPos[2];
        modHitPos[0] = (float)(spin_scale * (float)(*hitPos - modHitPos[0])) + modHitPos[0];
        modHitPos[1] = (float)(spin_scale * v5) + modHitPos[1];
        modHitPos[2] = (float)(spin_scale * v6) + modHitPos[2];
        modHitDir[0] = scale * *hitDir;
        modHitDir[1] = scale * hitDir[1];
        modHitDir[2] = scale * hitDir[2];
        break;
    }
    if ( mod )
      Phys_ObjAddForce((int)&savedregs, physObjId, modHitPos, modHitDir, 0);
    else
      Phys_ObjBulletImpact(physObjId, hitPos, modHitDir, scale, physPreset->bulletForceScale);
  }
}

void  Phys_ObjAddForce(int a1@<ebp>, int id, float *worldPos, float *impulse, bool relative)
{
  const phys_vec3 *v5; // eax
  phys_vec3 *torque_mult; // [esp+0h] [ebp-C0h]
  phys_vec3 v7; // [esp+4h] [ebp-BCh] BYREF
  float v8; // [esp+18h] [ebp-A8h]
  float v9; // [esp+1Ch] [ebp-A4h]
  float v10; // [esp+20h] [ebp-A0h]
  phys_vec3 v11; // [esp+24h] [ebp-9Ch] BYREF
  float v12; // [esp+38h] [ebp-88h]
  float v13; // [esp+3Ch] [ebp-84h]
  float v14; // [esp+40h] [ebp-80h]
  phys_vec3 v15; // [esp+44h] [ebp-7Ch] BYREF
  phys_vec3 v16; // [esp+54h] [ebp-6Ch] BYREF
  phys_mat44 *p_m_mat; // [esp+70h] [ebp-50h]
  float x; // [esp+74h] [ebp-4Ch] BYREF
  float v19; // [esp+78h] [ebp-48h]
  float v20; // [esp+7Ch] [ebp-44h]
  phys_vec3 pos; // [esp+80h] [ebp-40h]
  float v22; // [esp+90h] [ebp-30h]
  float w; // [esp+94h] [ebp-2Ch] BYREF
  float z; // [esp+98h] [ebp-28h]
  float y; // [esp+9Ch] [ebp-24h]
  phys_vec3 force; // [esp+A0h] [ebp-20h]
  rigid_body *body; // [esp+B0h] [ebp-10h]
  int v28; // [esp+B4h] [ebp-Ch]
  void *v29; // [esp+B8h] [ebp-8h]
  void *retaddr; // [esp+C0h] [ebp+0h]

  v28 = a1;
  v29 = retaddr;
  if ( id && id != -1 )
  {
    body = Phys_GetUserData(id)->body;
    force.w = *impulse;
    force.z = impulse[1];
    force.y = impulse[2];
    w = force.w;
    z = force.z;
    y = force.y;
    v22 = *worldPos;
    pos.w = worldPos[1];
    pos.z = worldPos[2];
    x = v22;
    v19 = pos.w;
    v20 = pos.z;
    if ( relative )
    {
      p_m_mat = &body->m_mat;
      torque_mult = &body->m_mat.w;
      v5 = phys_multiply(&v16, &body->m_mat, (const phys_vec3 *)&x);
      operator+(&v15, v5, torque_mult);
      x = v15.x;
      v19 = v15.y;
      v20 = v15.z;
    }
    if ( Abs(&w) >= 1000000.0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
            1297,
            0,
            "%s",
            "Abs(force) < 1000000.0f") )
    {
      __debugbreak();
    }
    if ( (float)((float)((float)(x * x) + (float)(v19 * v19)) + (float)(v20 * v20)) == 0.0 )
    {
      v14 = 0.001 * w;
      v13 = 0.001 * z;
      v12 = 0.001 * y;
      v11.x = 0.001 * w;
      v11.y = 0.001 * z;
      v11.z = 0.001 * y;
      rigid_body::add_force(body, &v11);
    }
    else
    {
      v10 = 0.001 * w;
      v9 = 0.001 * z;
      v8 = 0.001 * y;
      v7.x = 0.001 * w;
      v7.y = 0.001 * z;
      v7.z = 0.001 * y;
      rigid_body::add_force(body, &v7, (const phys_vec3 *)&x, 1.0);
    }
  }
}

void  Phys_ObjAddTorque(int a1@<ebp>, int id, float *torque)
{
  phys_vec3 v3; // [esp-2Ch] [ebp-4Ch] BYREF
  float v4; // [esp-18h] [ebp-38h]
  float v5; // [esp-14h] [ebp-34h]
  float v6; // [esp-10h] [ebp-30h]
  float v7; // [esp-Ch] [ebp-2Ch] BYREF
  float v8; // [esp-8h] [ebp-28h]
  float v9; // [esp-4h] [ebp-24h]
  rigid_body *body; // [esp+10h] [ebp-10h]
  int v11; // [esp+14h] [ebp-Ch]
  void *v12; // [esp+18h] [ebp-8h]
  void *retaddr; // [esp+20h] [ebp+0h]

  v11 = a1;
  v12 = retaddr;
  body = Phys_GetUserData(id)->body;
  Phys_Vec3ToNitrousVec(torque, (phys_vec3 *)&v7);
  if ( Abs(&v7) >= 1000000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1309,
          0,
          "%s",
          "Abs(_torque) < 1000000.0f") )
  {
    __debugbreak();
  }
  v6 = 0.001 * v7;
  v5 = 0.001 * v8;
  v4 = 0.001 * v9;
  v3.x = 0.001 * v7;
  v3.y = 0.001 * v8;
  v3.z = 0.001 * v9;
  rigid_body::add_torque(body, &v3);
}

void __thiscall rigid_body::add_torque(rigid_body *this, const phys_vec3 *torque)
{
  this->m_torque_sum.x = this->m_torque_sum.x + torque->x;
  this->m_torque_sum.y = this->m_torque_sum.y + torque->y;
  this->m_torque_sum.z = this->m_torque_sum.z + torque->z;
  if ( fabs(this->m_torque_sum.x) > 100000.0
    || fabs(this->m_torque_sum.y) > 100000.0
    || fabs(this->m_torque_sum.z) > 100000.0 )
  {
    phys_exec_debug_callback(this);
  }
}

void __cdecl Phys_ObjBulletImpact(
        int id,
        const float *worldPosRaw,
        const float *bulletDirRaw,
        float bulletSpeed,
        float scale)
{
  float force[3]; // [esp+8h] [ebp-34h] BYREF
  rigid_body *body; // [esp+14h] [ebp-28h]
  float centerOfMass[3]; // [esp+18h] [ebp-24h] BYREF
  float bulletDir[3]; // [esp+24h] [ebp-18h] BYREF
  float worldPos[3]; // [esp+30h] [ebp-Ch] BYREF
  int savedregs; // [esp+3Ch] [ebp+0h] BYREF

  body = Phys_GetUserData(id)->body;
  Phys_NitrousVecToVec3(&body->m_mat.w, centerOfMass);
  bulletDir[0] = *bulletDirRaw;
  bulletDir[1] = bulletDirRaw[1];
  bulletDir[2] = bulletDirRaw[2];
  worldPos[0] = *worldPosRaw;
  worldPos[1] = worldPosRaw[1];
  worldPos[2] = worldPosRaw[2];
  Phys_TweakBulletImpact(worldPos, bulletDir, centerOfMass);
  force[0] = (float)(scale * bulletSpeed) * bulletDir[0];
  force[1] = (float)(scale * bulletSpeed) * bulletDir[1];
  force[2] = (float)(scale * bulletSpeed) * bulletDir[2];
  Phys_ObjAddForce((int)&savedregs, id, worldPos, force, 0);
}

void __cdecl Phys_TweakBulletImpact(float *worldPos, float *bulletDir, const float *centerOfMass)
{
  float value; // [esp+0h] [ebp-1Ch]
  float offset_4; // [esp+14h] [ebp-8h]
  float offset_8; // [esp+18h] [ebp-4h]

  if ( !phys_bulletUpBias
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1317,
          0,
          "%s",
          "phys_bulletUpBias") )
  {
    __debugbreak();
  }
  if ( !phys_bulletSpinScale
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          1318,
          0,
          "%s",
          "phys_bulletSpinScale") )
  {
    __debugbreak();
  }
  bulletDir[2] = bulletDir[2] + phys_bulletUpBias->current.value;
  Vec3Normalize(bulletDir);
  value = phys_bulletSpinScale->current.value;
  offset_4 = value * (float)(worldPos[1] - centerOfMass[1]);
  offset_8 = value * (float)(worldPos[2] - centerOfMass[2]);
  *worldPos = *worldPos + (float)(value * (float)(*worldPos - *centerOfMass));
  worldPos[1] = worldPos[1] + offset_4;
  worldPos[2] = worldPos[2] + offset_8;
}

int __cdecl Phys_ObjGetSnapshot(int id, float *outPos, float (*outMat)[3])
{
  PhysObjUserData *userData; // [esp+Ch] [ebp-4h]

  userData = Phys_GetUserData(id);
  *outPos = userData->savedPos[0];
  outPos[1] = userData->savedPos[1];
  outPos[2] = userData->savedPos[2];
  memcpy(outMat, userData->savedRot, 0x24u);
  return physGlob.timeLastSnapshot;
}

void __cdecl Phys_RenderBulletMeshInfo(
        float *pos,
        const XModel *model,
        const char *type,
        int numTriangles,
        float avgArea)
{
  bool HasCollmap; // eax
  char lineStr[260]; // [esp+0h] [ebp-108h] BYREF

  XModelRenderString(pos, (char *)model->name);
  pos[2] = pos[2] - s_lineHeight;
  if ( type )
  {
    _snprintf(lineStr, 0xFFu, "  %s, tris(%d), avgarea(%d)", type, numTriangles, (int)(float)(avgArea + 0.5));
    lineStr[255] = 0;
    XModelRenderString(pos, lineStr);
    pos[2] = pos[2] - s_lineHeight;
    _snprintf(lineStr, 0xFFu, "lods(%d) collod(%d), bones(%d)", model->numLods, model->collLod, model->numBones);
    lineStr[255] = 0;
    XModelRenderString(pos, lineStr);
    pos[2] = pos[2] - s_lineHeight;
    HasCollmap = XModelHasCollmap(model);
    _snprintf(lineStr, 0xFFu, "collmaps(%d)", HasCollmap);
    lineStr[255] = 0;
    XModelRenderString(pos, lineStr);
  }
  else
  {
    XModelRenderString(pos, "  No bullet mesh");
  }
  pos[2] = pos[2] - s_lineHeight;
}

void __cdecl Phys_ComputeStringPosition(const float *start, const float *end, float *position)
{
  float v3; // [esp+0h] [ebp-10h]

  *position = *end - *start;
  position[1] = end[1] - start[1];
  position[2] = end[2] - start[2];
  Vec3Normalize(position);
  v3 = s_stringPosScale;
  *position = s_stringPosScale * *position;
  position[1] = v3 * position[1];
  position[2] = v3 * position[2];
  *position = *position + *start;
  position[1] = position[1] + start[1];
  position[2] = position[2] + start[2];
}

void __cdecl Phys_FindAndRenderBulletMesh(const float *start, const float *end)
{
  int baseBoneIndex; // [esp+3Ch] [ebp-154h]
  XModel *model; // [esp+40h] [ebp-150h]
  const centity_s *cent; // [esp+44h] [ebp-14Ch]
  DObjAnimMat *boneMtxList; // [esp+48h] [ebp-148h]
  int partBits[5]; // [esp+4Ch] [ebp-144h] BYREF
  float *origin; // [esp+60h] [ebp-130h]
  DObj *dobj; // [esp+64h] [ebp-12Ch]
  const float (*modelOrigin)[3]; // [esp+68h] [ebp-128h]
  const DynEntityPose *dynEntPose; // [esp+6Ch] [ebp-124h]
  pointtrace_t clip; // [esp+70h] [ebp-120h] BYREF
  float localToWorld[3][3]; // [esp+B4h] [ebp-DCh] BYREF
  trace_t resultsDyn; // [esp+D8h] [ebp-B8h] BYREF
  float viewDirNormalized[3]; // [esp+110h] [ebp-80h] BYREF
  DynEntityDrawType drawType; // [esp+11Ch] [ebp-74h] BYREF
  bool bHitDynEnt; // [esp+123h] [ebp-6Dh]
  const char *dynEntTypeStr; // [esp+124h] [ebp-6Ch]
  int entityHitId; // [esp+128h] [ebp-68h]
  int localClientNum; // [esp+12Ch] [ebp-64h]
  unsigned __int16 dynEntityHitId; // [esp+130h] [ebp-60h]
  const DynEntityDef *dynEntDef; // [esp+134h] [ebp-5Ch]
  int numTriangles; // [esp+138h] [ebp-58h]
  trace_t results; // [esp+13Ch] [ebp-54h] BYREF
  float stringPos[3]; // [esp+178h] [ebp-18h] BYREF
  int modelCount; // [esp+184h] [ebp-Ch]
  int modelIndex; // [esp+188h] [ebp-8h]
  float avgArea; // [esp+18Ch] [ebp-4h]

  memset(&results, 0, 16);
  results.staticModel = 0;
  bHitDynEnt = 0;
  memset(&resultsDyn, 0, 16);
  TraceExtents::TraceExtents(&clip.extents);
  memset((unsigned __int8 *)&resultsDyn, 0, sizeof(resultsDyn));
  resultsDyn.fraction = 1.0f;
  *(_QWORD *)clip.extents.start.vec.v = *(_QWORD *)start;
  clip.extents.start.vec.v[2] = start[2];
  *(_QWORD *)clip.extents.end.vec.v = *(_QWORD *)end;
  clip.extents.end.vec.v[2] = end[2];
  CM_CalcTraceExtents(&clip.extents);
  clip.ignoreEntParams = 0;
  clip.contentmask = (int)&loc_802010 + 3;
  clip.bLocational = 1;
  clip.priorityMap = 0;
  DynEntCl_PointTrace(&clip, &resultsDyn);
  if ( resultsDyn.hitType == TRACE_HITTYPE_DYNENT_MODEL )
  {
    dynEntityHitId = Trace_GetDynEntHitId(&resultsDyn, &drawType);
    dynEntTypeStr = "dynent";
    bHitDynEnt = 1;
  }
  else
  {
    dynEntityHitId = 0;
    drawType = DYNENT_DRAW_MODEL;
    dynEntTypeStr = &toastPopupTitle;
  }
  if ( bHitDynEnt )
  {
    dynEntDef = DynEnt_GetEntityDef(dynEntityHitId, drawType);
    dynEntPose = DynEnt_GetClientPose(dynEntityHitId, drawType);
    modelOrigin = (const float (*)[3])dynEntPose->pose.origin;
    viewDirNormalized[0] = dynEntPose->pose.origin[0] - *start;
    viewDirNormalized[1] = dynEntPose->pose.origin[1] - start[1];
    viewDirNormalized[2] = dynEntPose->pose.origin[2] - start[2];
    Vec3Normalize(viewDirNormalized);
    Phys_ComputeStringPosition(start, dynEntPose->pose.origin, stringPos);
    UnitQuatToAxis(dynEntPose->pose.quat, localToWorld);
    numTriangles = jpeg_mem_init();
    Phys_RenderBulletMeshInfo(stringPos, dynEntDef->xModel, dynEntTypeStr, numTriangles, avgArea);
  }
  else
  {
    CG_LocationalTrace(&results, start, end, 0, (int)&cls.recentServers[7543].countrycode[1], 0, 0);
    if ( results.hitType == TRACE_HITTYPE_ENTITY )
    {
      localClientNum = cgCollWorldLocalClientNum;
      entityHitId = Trace_GetEntityHitId(&results);
      dobj = Com_GetClientDObj(entityHitId, localClientNum);
      if ( dobj )
      {
        cent = CG_GetEntity(localClientNum, entityHitId);
        AnglesToAxis(cent->pose.angles, localToWorld);
        viewDirNormalized[0] = cent->pose.origin[0] - *start;
        viewDirNormalized[1] = cent->pose.origin[1] - start[1];
        viewDirNormalized[2] = cent->pose.origin[2] - start[2];
        Vec3Normalize(viewDirNormalized);
        Phys_ComputeStringPosition(start, cent->pose.origin, stringPos);
        modelCount = DObjGetNumModels(dobj);
        DObjGeomTracelinePartBits(dobj, clip.contentmask, partBits);
        CG_DObjCalcPose(&cent->pose, dobj, partBits);
        boneMtxList = DObjGetRotTransArray(dobj);
        baseBoneIndex = 0;
        modelIndex = 0;
        while ( modelIndex < modelCount )
        {
          model = DObjGetModel(dobj, modelIndex);
          if ( model->collLod < 0 )
          {
            Phys_RenderBulletMeshInfo(stringPos, model, 0, 0, 0.0);
          }
          else
          {
            numTriangles = jpeg_mem_init();
            Phys_RenderBulletMeshInfo(stringPos, model, "dobj", numTriangles, avgArea);
          }
          ++modelIndex;
          boneMtxList += model->numBones;
          baseBoneIndex += model->numBones;
        }
      }
      else if ( results.staticModel )
      {
        origin = results.staticModel->origin;
        viewDirNormalized[0] = results.staticModel->origin[0] - *start;
        viewDirNormalized[1] = results.staticModel->origin[1] - start[1];
        viewDirNormalized[2] = results.staticModel->origin[2] - start[2];
        Vec3Normalize(viewDirNormalized);
        Phys_ComputeStringPosition(start, origin, stringPos);
        MatrixInverse(results.staticModel->invScaledAxis, localToWorld);
        numTriangles = jpeg_mem_init();
        Phys_RenderBulletMeshInfo(stringPos, results.staticModel->xmodel, "static", numTriangles, avgArea);
      }
    }
  }
}

chull_t *__cdecl create_chull(phys_convex_hull *pch)
{
  float *v2; // [esp+1Ch] [ebp-50h]
  float *v3; // [esp+20h] [ebp-4Ch]
  phys_vec3 **m_slot_array; // [esp+44h] [ebp-28h]
  phys_vec3 **v5; // [esp+48h] [ebp-24h]
  phys_convex_hull::ch_triangle *tri_i; // [esp+50h] [ebp-1Ch]
  phys_static_array<phys_convex_hull::ch_triangle,64>::iterator tri_i_end; // [esp+54h] [ebp-18h]
  float **vi; // [esp+58h] [ebp-14h]
  phys_static_array<phys_vec3 *,64>::iterator vi_end; // [esp+5Ch] [ebp-10h]
  int ind_i; // [esp+60h] [ebp-Ch]
  int vert_i; // [esp+64h] [ebp-8h]
  __int16 vert_ia; // [esp+64h] [ebp-8h]
  char *hull; // [esp+68h] [ebp-4h]

  hull = PMM_ALLOC(0x20u, 4u);
  *((unsigned int *)hull + 1) = pch->m_convex_hull_vert_list.m_alloc_count;
  *((unsigned int *)hull + 2) = PMM_ALLOC(16 * *((unsigned int *)hull + 1), 0x10u);
  *((unsigned int *)hull + 3) = 3 * pch->m_convex_hull_triangle_list.m_alloc_count;
  *((unsigned int *)hull + 4) = PMM_ALLOC((2 * *((unsigned int *)hull + 3) + 3) & 0xFFFFFFFC, 4u);
  *((unsigned int *)hull + 5) = 0;
  vert_i = 0;
  vi = (float **)pch->m_convex_hull_vert_list.m_slot_array;
  vi_end.m_ptr = (phys_vec3 **)&vi[pch->m_convex_hull_vert_list.m_alloc_count];
  while ( (float **)vi_end.m_ptr != vi )
  {
    v3 = (float *)(*((unsigned int *)hull + 2) + 16 * vert_i++);
    v2 = *vi;
    *v3 = **vi;
    v3[1] = v2[1];
    v3[2] = v2[2];
    ++vi;
  }
  ind_i = 0;
  tri_i = pch->m_convex_hull_triangle_list.m_slot_array;
  tri_i_end.m_ptr = &tri_i[pch->m_convex_hull_triangle_list.m_alloc_count];
  while ( tri_i_end.m_ptr != tri_i )
  {
    vert_ia = 0;
    m_slot_array = pch->m_convex_hull_vert_list.m_slot_array;
    v5 = &m_slot_array[pch->m_convex_hull_vert_list.m_alloc_count];
    while ( v5 != m_slot_array )
    {
      if ( tri_i->m_verts[0] == *m_slot_array )
        *(_WORD *)(*((unsigned int *)hull + 4) + 2 * ind_i) = vert_ia;
      if ( tri_i->m_verts[1] == *m_slot_array )
        *(_WORD *)(*((unsigned int *)hull + 4) + 2 * ind_i + 2) = vert_ia;
      if ( tri_i->m_verts[2] == *m_slot_array )
        *(_WORD *)(*((unsigned int *)hull + 4) + 2 * ind_i + 4) = vert_ia;
      ++vert_ia;
      ++m_slot_array;
    }
    ind_i += 3;
    ++tri_i;
  }
  return (chull_t *)hull;
}

void __cdecl free_chull(chull_t *first)
{
  chull_t *next; // [esp+Ch] [ebp-4h]

  while ( first )
  {
    next = first->next;
    PMM_FREE((unsigned __int8 *)first->verts, 16 * first->nverts, 0x10u);
    PMM_FREE((unsigned __int8 *)first->inds, (2 * first->ninds + 3) & 0xFFFFFFFC, 4u);
    PMM_FREE((unsigned __int8 *)first, 0x20u, 4u);
    first = next;
  }
}

void __cdecl chull_list_add(chull_t *chull, unsigned int key)
{
  generic_avl_map_add(&entities_map, chull, key);
  chull->key = key;
  chull->next_list = chull_list;
  chull_list = chull;
}

void __cdecl free_chull_lists()
{
  chull_t *next_list; // [esp+0h] [ebp-Ch]
  chull_t *first_list; // [esp+4h] [ebp-8h]
  chull_t **prev_next; // [esp+8h] [ebp-4h]

  first_list = chull_list;
  prev_next = &chull_list;
  while ( first_list )
  {
    next_list = first_list->next_list;
    if ( first_list->touched == 1 )
    {
      first_list->touched = 0;
      prev_next = &first_list->next_list;
    }
    else
    {
      *prev_next = next_list;
      generic_avl_map_destroy(&entities_map, first_list->key);
      free_chull(first_list);
    }
    first_list = next_list;
  }
}

chull_t * generate_brush_chull@<eax>(int a1@<ebp>, const cbrush_t *brush)
{
  phys_vec3 *v3; // eax
  unsigned int v4[3]; // [esp+14h] [ebp-2Ch] BYREF
  phys_vec3 v; // [esp+20h] [ebp-20h]
  int i; // [esp+30h] [ebp-10h]
  int v7; // [esp+34h] [ebp-Ch] BYREF
  unsigned int vi; // [esp+38h] [ebp-8h]
  unsigned int retaddr; // [esp+40h] [ebp+0h]

  v7 = a1;
  vi = retaddr;
  if ( !brush )
    return 0;
  for ( i = 0; i < g_hull.m_vertex_buffer.m_alloc_count; ++i )
    ;
  g_hull.m_vertex_buffer.m_alloc_count = 0;
  for ( v.w = 0.0; LODWORD(v.w) < brush->numverts; ++LODWORD(v.w) )
  {
    Phys_Vec3ToNitrousVec(brush->verts[LODWORD(v.w)], (phys_vec3 *)v4);
    v3 = phys_static_array<phys_vec3,6144>::add(&g_hull.m_vertex_buffer, 0, "phys array add overflow.");
    LODWORD(v3->x) = v4[0];
    LODWORD(v3->y) = v4[1];
    LODWORD(v3->z) = v4[2];
  }
  phys_convex_hull::compute_convex_hull(&g_hull, (int)&v7, 128, 0.0);
  return create_chull(&g_hull);
}

void __userpurge phys_convex_hull::compute_convex_hull(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        int max_verts,
        float min_expansion_volume_percent)
{
  const phys_vec3 *v4; // eax
  phys_convex_hull::ch_triangle *v5; // eax
  const phys_vec3 *v6; // eax
  phys_convex_hull::ch_triangle *v7; // eax
  const phys_convex_hull::ch_triangle *v8; // [esp-14h] [ebp-B0h]
  phys_convex_hull::ch_triangle *list_head; // [esp-Ch] [ebp-A8h]
  phys_convex_hull::ch_edge *v10; // [esp+0h] [ebp-9Ch]
  phys_convex_hull::ch_edge *edge_i_end; // [esp+Ch] [ebp-90h]
  float edge_i; // [esp+18h] [ebp-84h]
  float v13; // [esp+1Ch] [ebp-80h]
  phys_vec3 increase_percent; // [esp+20h] [ebp-7Ch] BYREF
  float v15; // [esp+30h] [ebp-6Ch]
  phys_vec3 **v16; // [esp+34h] [ebp-68h]
  float v17; // [esp+38h] [ebp-64h]
  float v18; // [esp+3Ch] [ebp-60h]
  int v19; // [esp+40h] [ebp-5Ch] BYREF
  float x; // [esp+50h] [ebp-4Ch]
  phys_convex_hull::ch_triangle *v21; // [esp+54h] [ebp-48h]
  phys_convex_hull::ch_triangle *v22; // [esp+58h] [ebp-44h]
  phys_vec3 **v23; // [esp+5Ch] [ebp-40h]
  phys_vec3 **support_vert; // [esp+60h] [ebp-3Ch]
  phys_convex_hull::ch_triangle *tri; // [esp+64h] [ebp-38h]
  phys_vec3 **v26; // [esp+68h] [ebp-34h]
  phys_convex_hull::ch_triangle *m_slot_array; // [esp+6Ch] [ebp-30h]
  float v28; // [esp+70h] [ebp-2Ch]
  phys_static_array<phys_convex_hull::ch_triangle,256>::iterator tri_i; // [esp+74h] [ebp-28h]
  phys_convex_hull::ch_triangle *v30; // [esp+78h] [ebp-24h]
  int best_volume; // [esp+7Ch] [ebp-20h]
  phys_vec3 **best_vert; // [esp+80h] [ebp-1Ch]
  phys_convex_hull::ch_triangle *best_tri; // [esp+84h] [ebp-18h]
  float v34; // [esp+88h] [ebp-14h]
  phys_convex_hull *v35; // [esp+8Ch] [ebp-10h]
  int v36; // [esp+90h] [ebp-Ch] BYREF
  float total_volume; // [esp+94h] [ebp-8h]
  float retaddr; // [esp+9Ch] [ebp+0h]

  v36 = a2;
  total_volume = retaddr;
  v35 = this;
  if ( max_verts < 3
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         331,
         "max_verts >= 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  phys_convex_hull::init_convex_hull(v35);
  v34 = 0.0f;
  while ( 1 )
  {
    best_tri = (phys_convex_hull::ch_triangle *)v35->m_convex_hull_vert_list.m_alloc_count;
    if ( (int)best_tri >= max_verts )
      break;
    best_vert = (phys_vec3 **)v35->m_intermediate_triangle_list.m_alloc_count;
    if ( (int)best_vert <= 0 )
      break;
    best_volume = v35->m_intermediate_vertex_list.m_alloc_count;
    if ( best_volume <= 0 )
      break;
    v30 = 0;
    tri_i.m_ptr = 0;
    v28 = 0.0f;
    m_slot_array = v35->m_intermediate_triangle_list.m_slot_array;
    v26 = (phys_vec3 **)m_slot_array;
    while ( 1 )
    {
      tri = (phys_convex_hull::ch_triangle *)&v35->m_intermediate_triangle_list;
      support_vert = (phys_vec3 **)&v35->m_intermediate_triangle_list.m_slot_array[v35->m_intermediate_triangle_list.m_alloc_count];
      v23 = support_vert;
      if ( support_vert == v26 )
        break;
      v22 = (phys_convex_hull::ch_triangle *)v26;
      v21 = (phys_convex_hull::ch_triangle *)phys_convex_hull::support_intermediate_verts(v35, (const phys_vec3 *)v26);
      x = v21->m_normal.x;
      v4 = operator-((phys_vec3 *)&v19, (const phys_vec3 *)LODWORD(x), v22->m_verts[0]);
      v18 = phys_dot(&v22->m_normal, v4);
      if ( v18 <= 0.034000002 )
      {
        v8 = v22;
        v5 = phys_static_array<phys_convex_hull::ch_triangle,64>::add(
               &v35->m_convex_hull_triangle_list,
               0,
               "phys array add overflow.");
        phys_convex_hull::ch_triangle::operator=(v5, v8);
        v16 = v26;
        phys_static_array<phys_convex_hull::ch_triangle,256>::remove_slow(&v35->m_intermediate_triangle_list, v22);
      }
      else
      {
        v17 = phys_convex_hull::calc_expansion_volume(v35, (const phys_vec3 *)LODWORD(v21->m_normal.x));
        if ( v17 > v28 )
        {
          v30 = v22;
          tri_i.m_ptr = v21;
          v28 = v17;
        }
        v26 += 8;
      }
    }
    if ( !v30 )
      break;
    if ( !tri_i.m_ptr
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
           373,
           "best_vert",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( v28 <= 0.0
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
           374,
           "best_volume > 0.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v15 = tri_i.m_ptr->m_normal.x;
    v6 = operator-(&increase_percent, (const phys_vec3 *)LODWORD(v15), v30->m_verts[0]);
    v13 = phys_dot(&v30->m_normal, v6);
    if ( v13 <= 0.034000002
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
           375,
           "best_tri->get_dist(**best_vert) > CONVEX_HULL_VERT_ADD_DIST_THRESH",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    edge_i = v34 <= 0.000099999997 ? FLOAT_100000_0 : v28 / v34;
    if ( min_expansion_volume_percent > edge_i )
      break;
    v34 = v34 + v28;
    phys_convex_hull::create_edge_list(v35, (const phys_vec3 *)LODWORD(tri_i.m_ptr->m_normal.x));
    edge_i_end = v35->m_intermediate_edge_list.m_slot_array;
    v10 = &edge_i_end[v35->m_intermediate_edge_list.m_alloc_count];
    while ( v10 != edge_i_end )
    {
      phys_convex_hull::create_intermediate_triangle(
        v35,
        (int)&v36,
        (phys_vec3 *)LODWORD(tri_i.m_ptr->m_normal.x),
        edge_i_end->m_verts[0],
        edge_i_end->m_verts[1]);
      ++edge_i_end;
    }
    phys_convex_hull::add_convex_hull_vert(v35, (phys_vec3 **)tri_i.m_ptr);
    phys_convex_hull::remove_inside_verts(v35);
  }
  while ( v35->m_intermediate_triangle_list.m_alloc_count > 0 )
  {
    list_head = phys_static_array<phys_convex_hull::ch_triangle,256>::get_list_head(&v35->m_intermediate_triangle_list);
    v7 = phys_static_array<phys_convex_hull::ch_triangle,64>::add(
           &v35->m_convex_hull_triangle_list,
           0,
           "phys array add overflow.");
    phys_convex_hull::ch_triangle::operator=(v7, list_head);
    phys_static_array<phys_convex_hull::ch_triangle,256>::remove_slow(&v35->m_intermediate_triangle_list, list_head);
  }
}

void __thiscall phys_convex_hull::add_convex_hull_vert(phys_convex_hull *this, phys_vec3 **vert)
{
  phys_vec3 **v3; // [esp+8h] [ebp-10h]
  phys_vec3 **v4; // [esp+14h] [ebp-4h]

  if ( !vert
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         52,
         "vert",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( this->m_convex_hull_vert_list.m_alloc_count < 64 )
  {
    v4 = &this->m_convex_hull_vert_list.m_slot_array[this->m_convex_hull_vert_list.m_alloc_count++];
    v3 = v4;
  }
  else
  {
    tlFatal("phys array add overflow.");
    v3 = 0;
  }
  *v3 = *vert;
  phys_static_array<phys_vec3 *,6144>::remove(&this->m_intermediate_vertex_list, vert);
}

void __userpurge phys_convex_hull::create_intermediate_triangle(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        phys_vec3 *v0,
        phys_vec3 *v1,
        phys_vec3 *v2)
{
  float v5; // [esp-34h] [ebp-98h]
  float v6; // [esp-30h] [ebp-94h]
  float v7; // [esp-Ch] [ebp-70h]
  float v8[2]; // [esp-8h] [ebp-6Ch] BYREF
  float nnormal; // [esp+0h] [ebp-64h]
  phys_vec3 normal; // [esp+4h] [ebp-60h] BYREF
  float v11; // [esp+1Ch] [ebp-48h]
  float v12; // [esp+20h] [ebp-44h]
  float v13; // [esp+24h] [ebp-40h]
  phys_vec3 v14; // [esp+28h] [ebp-3Ch] BYREF
  float v15; // [esp+44h] [ebp-20h]
  float v16; // [esp+48h] [ebp-1Ch]
  float v17; // [esp+4Ch] [ebp-18h]
  phys_convex_hull::ch_triangle *v18; // [esp+50h] [ebp-14h]
  phys_convex_hull *v19; // [esp+54h] [ebp-10h]
  int v20; // [esp+58h] [ebp-Ch]
  phys_convex_hull::ch_triangle *tri; // [esp+5Ch] [ebp-8h]
  phys_convex_hull::ch_triangle *retaddr; // [esp+64h] [ebp+0h]

  v20 = a2;
  tri = retaddr;
  v19 = this;
  v18 = phys_static_array<phys_convex_hull::ch_triangle,256>::add(
          &this->m_intermediate_triangle_list,
          0,
          "phys array add overflow.");
  v18->m_verts[0] = v0;
  v18->m_verts[1] = v1;
  v18->m_verts[2] = v2;
  v17 = v2->x - v0->x;
  v16 = v2->y - v0->y;
  v15 = v2->z - v0->z;
  v14.x = v17;
  v14.y = v16;
  v14.z = v15;
  v13 = v1->x - v0->x;
  v12 = v1->y - v0->y;
  v11 = v1->z - v0->z;
  normal.y = v13;
  normal.z = v12;
  normal.w = v11;
  phys_cross((phys_vec3 *)v8, (phys_vec3 *)&normal.y, &v14);
  v7 = Abs(v8);
  if ( v7 <= 0.0000099999997
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         95,
         "nnormal > 0.00001f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v5 = (float)(1.0 / v7) * v8[1];
  v6 = (float)(1.0 / v7) * nnormal;
  v18->m_normal.x = (float)(1.0 / v7) * v8[0];
  v18->m_normal.y = v5;
  v18->m_normal.z = v6;
}

phys_vec3 **__thiscall phys_convex_hull::support_intermediate_verts(phys_convex_hull *this, const phys_vec3 *dir)
{
  phys_vec3 **vert_i; // [esp+1Ch] [ebp-10h]
  phys_vec3 **best_vert; // [esp+24h] [ebp-8h]
  float best_dotp; // [esp+28h] [ebp-4h]

  best_vert = 0;
  best_dotp = -1000000.0;
  for ( vert_i = this->m_intermediate_vertex_list.m_slot_array;
        &this->m_intermediate_vertex_list.m_slot_array[this->m_intermediate_vertex_list.m_alloc_count] != vert_i;
        ++vert_i )
  {
    if ( (float)((float)((float)(dir->x * (*vert_i)->x) + (float)(dir->y * (*vert_i)->y))
               + (float)(dir->z * (*vert_i)->z)) > best_dotp )
    {
      best_vert = vert_i;
      best_dotp = (float)((float)(dir->x * (*vert_i)->x) + (float)(dir->y * (*vert_i)->y))
                + (float)(dir->z * (*vert_i)->z);
    }
  }
  if ( !best_vert
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         118,
         "best_vert",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return best_vert;
}

void __thiscall phys_convex_hull::init_convex_hull(phys_convex_hull *this)
{
  phys_vec3 **v1; // eax
  phys_vec3 **v2; // eax
  phys_vec3 *v3; // [esp-8h] [ebp-160h]
  phys_vec3 *v4; // [esp-8h] [ebp-160h]
  phys_vec3 *v5; // [esp-4h] [ebp-15Ch]
  phys_vec3 *v6; // [esp-4h] [ebp-15Ch]
  int m; // [esp+118h] [ebp-40h]
  int k; // [esp+120h] [ebp-38h]
  int j; // [esp+128h] [ebp-30h]
  phys_vec3 **v11; // [esp+12Ch] [ebp-2Ch]
  phys_vec3 **v12; // [esp+138h] [ebp-20h]
  int i; // [esp+14Ch] [ebp-Ch]
  phys_vec3 *vert_i; // [esp+150h] [ebp-8h]
  phys_static_array<phys_vec3,6144>::iterator vert_i_end; // [esp+154h] [ebp-4h]
  int savedregs; // [esp+158h] [ebp+0h] BYREF

  for ( i = 0; i < this->m_intermediate_vertex_list.m_alloc_count; ++i )
    ;
  this->m_intermediate_vertex_list.m_alloc_count = 0;
  vert_i = this->m_vertex_buffer.m_slot_array;
  vert_i_end.m_ptr = &vert_i[this->m_vertex_buffer.m_alloc_count];
  while ( vert_i_end.m_ptr != vert_i )
  {
    if ( this->m_intermediate_vertex_list.m_alloc_count < 6144 )
    {
      v12 = &this->m_intermediate_vertex_list.m_slot_array[this->m_intermediate_vertex_list.m_alloc_count++];
      v11 = v12;
    }
    else
    {
      tlFatal("phys array add overflow.");
      v11 = 0;
    }
    *v11 = vert_i++;
  }
  for ( j = 0; j < this->m_convex_hull_vert_list.m_alloc_count; ++j )
    ;
  this->m_convex_hull_vert_list.m_alloc_count = 0;
  for ( k = 0; k < this->m_intermediate_triangle_list.m_alloc_count; ++k )
    ;
  this->m_intermediate_triangle_list.m_alloc_count = 0;
  for ( m = 0; m < this->m_convex_hull_triangle_list.m_alloc_count; ++m )
    ;
  this->m_convex_hull_triangle_list.m_alloc_count = 0;
  phys_convex_hull::calculate_initial_triangle_vertices(this, (int)&savedregs);
  if ( this->m_convex_hull_vert_list.m_alloc_count != 3
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         256,
         "m_convex_hull_vert_list.get_count() == 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v5 = *phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 2);
  v3 = *phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 1);
  v1 = phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 0);
  phys_convex_hull::create_intermediate_triangle(this, (int)&savedregs, *v1, v3, v5);
  v6 = *phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 2);
  v4 = *phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 0);
  v2 = phys_static_array<phys_vec3 *,64>::operator[](&this->m_convex_hull_vert_list, 1);
  phys_convex_hull::create_intermediate_triangle(this, (int)&savedregs, *v2, v4, v6);
}

void  phys_convex_hull::calculate_initial_triangle_vertices(phys_convex_hull *this@<ecx>, int a2@<ebp>)
{
  phys_vec3 *eax15; // eax
  phys_vec3 **v3; // [esp-Ch] [ebp-17Ch]
  int k; // [esp-8h] [ebp-178h]
  char v5; // [esp-1h] [ebp-171h]
  int i; // [esp+4h] [ebp-16Ch]
  phys_vec3 **v7; // [esp+8h] [ebp-168h]
  int v8; // [esp+10h] [ebp-160h]
  phys_vec3 v9; // [esp+14h] [ebp-15Ch] BYREF
  float v10; // [esp+30h] [ebp-140h]
  float v11; // [esp+34h] [ebp-13Ch]
  float v12; // [esp+38h] [ebp-138h]
  float *v13; // [esp+3Ch] [ebp-134h]
  phys_vec3 *v14; // [esp+40h] [ebp-130h]
  phys_vec3 v15; // [esp+44h] [ebp-12Ch] BYREF
  float x; // [esp+54h] [ebp-11Ch] BYREF
  float y; // [esp+58h] [ebp-118h]
  float z; // [esp+5Ch] [ebp-114h]
  phys_vec3 nn; // [esp+60h] [ebp-110h] BYREF
  float v20; // [esp+80h] [ebp-F0h]
  float v21; // [esp+84h] [ebp-ECh]
  float v22; // [esp+88h] [ebp-E8h]
  phys_vec3 *v23; // [esp+8Ch] [ebp-E4h]
  float *v24; // [esp+90h] [ebp-E0h]
  phys_vec3 v25; // [esp+94h] [ebp-DCh] BYREF
  float v26; // [esp+ACh] [ebp-C4h]
  float v27; // [esp+B0h] [ebp-C0h]
  float v28; // [esp+B4h] [ebp-BCh]
  phys_vec3 *v29; // [esp+B8h] [ebp-B8h]
  phys_vec3 *v30; // [esp+BCh] [ebp-B4h]
  phys_vec3 **v31; // [esp+C0h] [ebp-B0h]
  phys_vec3 **v32; // [esp+C4h] [ebp-ACh]
  phys_vec3 **v33; // [esp+C8h] [ebp-A8h]
  phys_vec3 **best_verts[3]; // [esp+CCh] [ebp-A4h] BYREF
  phys_vec3 v35; // [esp+E4h] [ebp-8Ch] BYREF
  float v36; // [esp+100h] [ebp-70h]
  float v37; // [esp+104h] [ebp-6Ch]
  float v38; // [esp+108h] [ebp-68h]
  phys_vec3 *v39; // [esp+10Ch] [ebp-64h]
  float *v40; // [esp+110h] [ebp-60h]
  phys_vec3 v41; // [esp+114h] [ebp-5Ch] BYREF
  float v42; // [esp+12Ch] [ebp-44h]
  float v43; // [esp+130h] [ebp-40h]
  float v44; // [esp+134h] [ebp-3Ch]
  phys_vec3 *v45; // [esp+138h] [ebp-38h]
  float *v46; // [esp+13Ch] [ebp-34h]
  phys_vec3 **v47; // [esp+140h] [ebp-30h]
  int j; // [esp+144h] [ebp-2Ch]
  phys_vec3 **v49; // [esp+148h] [ebp-28h]
  phys_vec3 **z2; // [esp+14Ch] [ebp-24h]
  int v2; // [esp+150h] [ebp-20h]
  phys_vec3 **z1; // [esp+154h] [ebp-1Ch]
  int v1; // [esp+158h] [ebp-18h]
  phys_vec3 **z0; // [esp+15Ch] [ebp-14h]
  int v0; // [esp+160h] [ebp-10h]
  int NUM_SPHERE_VERTS; // [esp+164h] [ebp-Ch]
  float largest_twice_area_sq; // [esp+168h] [ebp-8h]
  float retaddr; // [esp+170h] [ebp+0h]

  NUM_SPHERE_VERTS = a2;
  largest_twice_area_sq = retaddr;
  v0 = (int)this;
  z0 = *(phys_vec3 ***)&FLOAT_0_0;
  v1 = this->m_intermediate_vertex_list.m_alloc_count;
  for ( z1 = 0; (int)z1 < v1 - 2; z1 = (phys_vec3 **)((char *)z1 + 1) )
  {
    v2 = (int)phys_static_array<phys_vec3 *,6144>::operator[](
                (phys_static_array<phys_vec3 *,6144> *)(v0 + 98320),
                (int)z1);
    for ( z2 = (phys_vec3 **)((char *)z1 + 1); (int)z2 < v1 - 1; z2 = (phys_vec3 **)((char *)z2 + 1) )
    {
      v49 = phys_static_array<phys_vec3 *,6144>::operator[](
              (phys_static_array<phys_vec3 *,6144> *)(v0 + 98320),
              (int)z2);
      for ( j = (int)z2 + 1; j < v1; ++j )
      {
        v47 = phys_static_array<phys_vec3 *,6144>::operator[]((phys_static_array<phys_vec3 *,6144> *)(v0 + 98320), j);
        v46 = *(float **)v2;
        v45 = *v47;
        v44 = v45->x - *v46;
        v43 = v45->y - v46[1];
        v42 = v45->z - v46[2];
        v41.x = v44;
        v41.y = v43;
        v41.z = v42;
        v40 = *(float **)v2;
        v39 = *v49;
        v38 = v39->x - *v40;
        v37 = v39->y - v40[1];
        v36 = v39->z - v40[2];
        v35.x = v38;
        v35.y = v37;
        v35.z = v36;
        best_verts[1] = (phys_vec3 **)phys_cross((phys_vec3 *)&best_verts[2], &v35, &v41);
        *(float *)best_verts = (float)((float)(*(float *)best_verts[1] * *(float *)best_verts[1])
                                     + (float)(*((float *)best_verts[1] + 1) * *((float *)best_verts[1] + 1)))
                             + (float)(*((float *)best_verts[1] + 2) * *((float *)best_verts[1] + 2));
        if ( *(float *)best_verts > *(float *)&z0 )
        {
          z0 = best_verts[0];
          v31 = (phys_vec3 **)v2;
          v32 = v49;
          v33 = v47;
        }
      }
    }
  }
  if ( *(float *)&z0 <= 0.0
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         210,
         "largest_twice_area_sq>0.0f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v30 = *v31;
  v29 = *v33;
  v28 = v29->x - v30->x;
  v27 = v29->y - v30->y;
  v26 = v29->z - v30->z;
  v25.x = v28;
  v25.y = v27;
  v25.z = v26;
  v24 = (float *)*v31;
  v23 = *v32;
  v22 = v23->x - *v24;
  v21 = v23->y - v24[1];
  v20 = v23->z - v24[2];
  nn.y = v22;
  nn.z = v21;
  nn.w = v20;
  phys_cross((phys_vec3 *)&x, (phys_vec3 *)&nn.y, &v25);
  v14 = phys_Unitize(&v15, (const phys_vec3 *)&x);
  x = v14->x;
  y = v14->y;
  z = v14->z;
  v13 = (float *)*v31;
  v12 = *v13 + x;
  v11 = v13[1] + y;
  v10 = v13[2] + z;
  v9.x = v12;
  v9.y = v11;
  v9.z = v10;
  eax15 = phys_static_array<phys_vec3,6144>::add((phys_static_array<phys_vec3,6144> *)v0, 0, "phys array add overflow.");
  phys_vec3::operator=(eax15, &v9);
  v8 = *(unsigned int *)(v0 + 98308);
  if ( *(int *)(v0 + 122900) < 6144 )
  {
    i = *(unsigned int *)(v0 + 122896) + 4 * (*(unsigned int *)(v0 + 122900))++;
    v7 = (phys_vec3 **)i;
  }
  else
  {
    tlFatal("phys array add overflow.");
    v7 = 0;
  }
  *v7 = phys_static_array<phys_vec3,6144>::operator[]((phys_static_array<phys_vec3,6144> *)v0, v8 - 1);
  v5 = 1;
  while ( v5 )
  {
    v5 = 0;
    for ( k = 0; k < 2; ++k )
    {
      if ( (&v31)[k] < (&v32)[k] )
      {
        v3 = (&v31)[k];
        (&v31)[k] = (&v32)[k];
        (&v32)[k] = v3;
        v5 = 1;
      }
    }
  }
  phys_convex_hull::add_convex_hull_vert((phys_convex_hull *)v0, v31);
  phys_convex_hull::add_convex_hull_vert((phys_convex_hull *)v0, v32);
  phys_convex_hull::add_convex_hull_vert((phys_convex_hull *)v0, v33);
}

double __thiscall phys_convex_hull::calc_expansion_volume(phys_convex_hull *this, const phys_vec3 *vert)
{
  float vert_dist; // [esp+B0h] [ebp-10h]
  phys_convex_hull::ch_triangle *tri_i; // [esp+B4h] [ebp-Ch]
  phys_static_array<phys_convex_hull::ch_triangle,256>::iterator tri_i_end; // [esp+B8h] [ebp-8h]
  float volume; // [esp+BCh] [ebp-4h]
  int savedregs; // [esp+C0h] [ebp+0h] BYREF

  volume = 0.0f;
  tri_i = this->m_intermediate_triangle_list.m_slot_array;
  tri_i_end.m_ptr = &tri_i[this->m_intermediate_triangle_list.m_alloc_count];
  while ( tri_i_end.m_ptr != tri_i )
  {
    vert_dist = phys_convex_hull::ch_triangle::get_dist(tri_i, vert);
    if ( vert_dist > 0.034000002 )
      volume = phys_convex_hull::tetrahedron_volume(
                 this,
                 (int)&savedregs,
                 vert,
                 tri_i->m_verts[0],
                 tri_i->m_verts[1],
                 tri_i->m_verts[2])
             + volume;
    ++tri_i;
  }
  if ( volume <= 0.0
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         277,
         "volume > 0.0f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return volume;
}

double __thiscall phys_convex_hull::ch_triangle::get_dist(phys_convex_hull::ch_triangle *this, const phys_vec3 *vert)
{
  return this->m_normal.z * (float)(vert->z - this->m_verts[0]->z)
       + this->m_normal.y * (float)(vert->y - this->m_verts[0]->y)
       + this->m_normal.x * (float)(vert->x - this->m_verts[0]->x);
}

double __userpurge phys_convex_hull::tetrahedron_volume@<st0>(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *a,
        const phys_vec3 *b,
        const phys_vec3 *c,
        const phys_vec3 *d)
{
  phys_vec3 *v6; // edx
  phys_vec3 vol; // [esp+0h] [ebp-7Ch] BYREF
  float v9; // [esp+10h] [ebp-6Ch]
  float v10; // [esp+14h] [ebp-68h]
  float v11; // [esp+18h] [ebp-64h]
  float v12; // [esp+24h] [ebp-58h]
  float v13; // [esp+28h] [ebp-54h]
  float v14; // [esp+2Ch] [ebp-50h]
  phys_vec3 v15; // [esp+30h] [ebp-4Ch] BYREF
  float v16; // [esp+44h] [ebp-38h]
  float v17; // [esp+48h] [ebp-34h]
  float v18; // [esp+4Ch] [ebp-30h]
  phys_vec3 v19; // [esp+50h] [ebp-2Ch] BYREF
  float v20; // [esp+60h] [ebp-1Ch]
  float v21; // [esp+64h] [ebp-18h]
  float v22; // [esp+68h] [ebp-14h]
  phys_convex_hull *v23; // [esp+6Ch] [ebp-10h]
  int v24; // [esp+70h] [ebp-Ch]
  void *v25; // [esp+74h] [ebp-8h]
  void *retaddr; // [esp+7Ch] [ebp+0h]

  v24 = a2;
  v25 = retaddr;
  v23 = this;
  v22 = c->x - d->x;
  v21 = c->y - d->y;
  v20 = c->z - d->z;
  v19.x = v22;
  v19.y = v21;
  v19.z = v20;
  v18 = b->x - d->x;
  v17 = b->y - d->y;
  v16 = b->z - d->z;
  v15.x = v18;
  v15.y = v17;
  v15.z = v16;
  v14 = a->x - d->x;
  v13 = a->y - d->y;
  v12 = a->z - d->z;
  v9 = v14;
  v10 = v13;
  v11 = v12;
  v6 = phys_cross(&vol, &v15, &v19);
  return (float)(fabs((float)((float)(v9 * v6->x) + (float)(v10 * v6->y)) + (float)(v11 * v6->z))
               / 6.0);
}

void __thiscall phys_convex_hull::create_edge_list(phys_convex_hull *this, const phys_vec3 *vert)
{
  phys_convex_hull::ch_triangle *m_slot_array; // [esp+34h] [ebp-20h]
  int i; // [esp+3Ch] [ebp-18h]

  for ( i = 0; i < this->m_intermediate_edge_list.m_alloc_count; ++i )
    ;
  this->m_intermediate_edge_list.m_alloc_count = 0;
  m_slot_array = this->m_intermediate_triangle_list.m_slot_array;
  while ( &this->m_intermediate_triangle_list.m_slot_array[this->m_intermediate_triangle_list.m_alloc_count] != m_slot_array )
  {
    if ( (float)((float)((float)(m_slot_array->m_normal.x * (float)(vert->x - m_slot_array->m_verts[0]->x))
                       + (float)(m_slot_array->m_normal.y * (float)(vert->y - m_slot_array->m_verts[0]->y)))
               + (float)(m_slot_array->m_normal.z * (float)(vert->z - m_slot_array->m_verts[0]->z))) <= 0.034000002 )
    {
      ++m_slot_array;
    }
    else
    {
      phys_convex_hull::add_triangle_edges(this, m_slot_array);
      phys_static_array<phys_convex_hull::ch_triangle,256>::remove_slow(
        &this->m_intermediate_triangle_list,
        m_slot_array);
    }
  }
  if ( this->m_intermediate_edge_list.m_alloc_count < 3
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         301,
         "m_intermediate_edge_list.get_count() >= 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
}

void __thiscall phys_convex_hull::add_triangle_edges(phys_convex_hull *this, phys_convex_hull::ch_triangle *tri)
{
  phys_convex_hull::add_intermediate_edge(this, tri->m_verts[0], tri->m_verts[1]);
  phys_convex_hull::add_intermediate_edge(this, tri->m_verts[1], tri->m_verts[2]);
  phys_convex_hull::add_intermediate_edge(this, tri->m_verts[2], tri->m_verts[0]);
}

void __thiscall phys_convex_hull::add_intermediate_edge(phys_convex_hull *this, phys_vec3 *v0, phys_vec3 *v1)
{
  phys_convex_hull::ch_edge *v4; // [esp+Ch] [ebp-24h]
  phys_convex_hull::ch_edge *edge_i; // [esp+28h] [ebp-8h]
  phys_convex_hull::ch_edge *edge; // [esp+2Ch] [ebp-4h]

  if ( (!v0 || !v1 || v0 == v1)
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_convex_hull.h",
         59,
         "v0 && v1 && v0 != v1",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  for ( edge_i = this->m_intermediate_edge_list.m_slot_array;
        &this->m_intermediate_edge_list.m_slot_array[this->m_intermediate_edge_list.m_alloc_count] != edge_i;
        ++edge_i )
  {
    if ( edge_i->m_verts[0] == v0 && edge_i->m_verts[1] == v1 || edge_i->m_verts[0] == v1 && edge_i->m_verts[1] == v0 )
    {
      phys_static_array<phys_convex_hull::ch_edge,128>::remove_slow(&this->m_intermediate_edge_list, edge_i);
      return;
    }
  }
  if ( this->m_intermediate_edge_list.m_alloc_count < 128 )
  {
    v4 = &this->m_intermediate_edge_list.m_slot_array[this->m_intermediate_edge_list.m_alloc_count++];
    edge = v4;
  }
  else
  {
    tlFatal("phys array add overflow.");
    edge = 0;
  }
  edge->m_verts[0] = v0;
  edge->m_verts[1] = v1;
}

void __thiscall phys_convex_hull::remove_inside_verts(phys_convex_hull *this)
{
  float vert_dist; // [esp+50h] [ebp-18h]
  phys_convex_hull::ch_triangle *tri_i; // [esp+54h] [ebp-14h]
  bool inside; // [esp+5Bh] [ebp-Dh]
  phys_static_array<phys_convex_hull::ch_triangle,256>::iterator tri_i_end; // [esp+5Ch] [ebp-Ch]
  phys_vec3 **vert_i; // [esp+64h] [ebp-4h]

  vert_i = this->m_intermediate_vertex_list.m_slot_array;
  while ( &this->m_intermediate_vertex_list.m_slot_array[this->m_intermediate_vertex_list.m_alloc_count] != vert_i )
  {
    inside = 1;
    tri_i = this->m_intermediate_triangle_list.m_slot_array;
    tri_i_end.m_ptr = &tri_i[this->m_intermediate_triangle_list.m_alloc_count];
    while ( tri_i_end.m_ptr != tri_i && inside )
    {
      vert_dist = phys_convex_hull::ch_triangle::get_dist(tri_i, *vert_i);
      inside = vert_dist <= 0.034000002;
      ++tri_i;
    }
    if ( inside )
      phys_static_array<phys_vec3 *,6144>::remove(&this->m_intermediate_vertex_list, vert_i);
    else
      ++vert_i;
  }
}

phys_convex_hull::ch_triangle *__thiscall phys_convex_hull::ch_triangle::operator=(
        phys_convex_hull::ch_triangle *this,
        const phys_convex_hull::ch_triangle *__that)
{
  unsigned int _S2; // [esp+8h] [ebp-4h]

  this->m_normal.x = __that->m_normal.x;
  this->m_normal.y = __that->m_normal.y;
  this->m_normal.z = __that->m_normal.z;
  for ( _S2 = 0; _S2 < 3; ++_S2 )
    this->m_verts[_S2] = __that->m_verts[_S2];
  return this;
}

chull_t * generate_partition_chull@<eax>(const float *a1@<ebp>, const CollisionAabbTree *tree)
{
  phys_vec3 *v3; // eax
  unsigned int v4[3]; // [esp+14h] [ebp-3Ch] BYREF
  phys_vec3 v; // [esp+20h] [ebp-30h]
  int v6; // [esp+30h] [ebp-20h]
  unsigned __int16 *v7; // [esp+34h] [ebp-1Ch]
  int ind_i; // [esp+38h] [ebp-18h]
  int ninds; // [esp+3Ch] [ebp-14h]
  unsigned __int16 *inds; // [esp+40h] [ebp-10h]
  const float (*verts)[3]; // [esp+44h] [ebp-Ch] BYREF
  const CollisionPartition *partition; // [esp+48h] [ebp-8h]
  const CollisionPartition *retaddr; // [esp+50h] [ebp+0h]

  verts = (const float (*)[3])a1;
  partition = retaddr;
  if ( !tree )
    return 0;
  for ( inds = 0; (int)inds < g_hull.m_vertex_buffer.m_alloc_count; inds = (unsigned __int16 *)((char *)inds + 1) )
    ;
  g_hull.m_vertex_buffer.m_alloc_count = 0;
  ninds = (int)&cm.partitions[tree->u.firstChildIndex];
  ind_i = (int)cm.verts;
  v7 = &cm.uinds[*(unsigned int *)(ninds + 12)];
  v6 = *(unsigned int *)(ninds + 8);
  for ( v.w = 0.0; SLODWORD(v.w) < v6; ++LODWORD(v.w) )
  {
    Phys_Vec3ToNitrousVec((const phys_vec3 *)v4, (float *)(ind_i + 12 * v7[LODWORD(v.w)]));
    v3 = phys_static_array<phys_vec3,6144>::add(&g_hull.m_vertex_buffer, 0, "phys array add overflow.");
    LODWORD(v3->x) = v4[0];
    LODWORD(v3->y) = v4[1];
    LODWORD(v3->z) = v4[2];
  }
  phys_convex_hull::compute_convex_hull(&g_hull, (int)&verts, 128, 0.0);
  return create_chull(&g_hull);
}

void __cdecl generate_brushmodel_chull_r(cLeafBrushNode_s *node, chull_t **hull)
{
  chull_t *brush_chull; // eax
  int i; // [esp+Ch] [ebp-4h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  if ( node->leafBrushCount )
  {
    if ( node->leafBrushCount > 0 )
    {
      for ( i = 0; i < node->leafBrushCount; ++i )
      {
        brush_chull = generate_brush_chull((int)&savedregs, &cm.brushes[node->data.leaf.brushes[i]]);
        brush_chull->next = *hull;
        *hull = brush_chull;
      }
      return;
    }
    generate_brushmodel_chull_r(node + 1, hull);
  }
  if ( node->data.children.childOffset[0] || node->data.children.childOffset[1] )
  {
    generate_brushmodel_chull_r(&node[node->data.children.childOffset[0]], hull);
    generate_brushmodel_chull_r(&node[node->data.children.childOffset[1]], hull);
  }
}

chull_t *__cdecl generate_brushmodel_chull(unsigned int brushmodel)
{
  const cmodel_t *model; // [esp+4h] [ebp-8h]
  chull_t *hull; // [esp+8h] [ebp-4h] BYREF

  hull = 0;
  model = CM_ClipHandleToModel(brushmodel);
  generate_brushmodel_chull_r(&cm.leafbrushNodes[model->leaf.leafBrushNode], &hull);
  return hull;
}

chull_t *__cdecl generate_collmap_chull(PhysGeomList *geomList)
{
  chull_t *brush_chull; // eax
  PhysGeomInfo *geom; // [esp+8h] [ebp-Ch]
  chull_t *first; // [esp+Ch] [ebp-8h]
  unsigned int gi; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  first = 0;
  gi = 0;
  geom = geomList->geoms;
  while ( gi < geomList->count )
  {
    if ( geom->brush )
    {
      brush_chull = generate_brush_chull((int)&savedregs, (const cbrush_t *)geom->brush);
      brush_chull->next = first;
      first = brush_chull;
    }
    ++geom;
    ++gi;
  }
  return first;
}

chull_t *__cdecl get_collmap_chull(PhysGeomList *geomList, unsigned int key)
{
  generic_avl_map_node_t *m_tree_root; // [esp+0h] [ebp-Ch]
  chull_t *chull; // [esp+8h] [ebp-4h]

  m_tree_root = entities_map.m_tree_root;
  while ( m_tree_root && key != m_tree_root->m_avl_key )
  {
    if ( key >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
  {
    chull = (chull_t *)m_tree_root->m_data;
  }
  else
  {
    chull = generate_collmap_chull(geomList);
    if ( !chull )
      return 0;
    chull_list_add(chull, key);
  }
  if ( !chull && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1789, 0, "%s", "chull") )
    __debugbreak();
  chull->touched = 1;
  return chull;
}

chull_t *__cdecl get_brushmodel_chull(unsigned int brushmodel, unsigned int key)
{
  generic_avl_map_node_t *m_tree_root; // [esp+0h] [ebp-Ch]
  chull_t *chull; // [esp+8h] [ebp-4h]

  m_tree_root = entities_map.m_tree_root;
  while ( m_tree_root && key != m_tree_root->m_avl_key )
  {
    if ( key >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
  {
    chull = (chull_t *)m_tree_root->m_data;
  }
  else
  {
    chull = generate_brushmodel_chull(brushmodel);
    if ( !chull )
      return 0;
    chull_list_add(chull, key);
  }
  if ( !chull && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1810, 0, "%s", "chull") )
    __debugbreak();
  chull->touched = 1;
  return chull;
}

chull_t *__cdecl get_brush_chull(const cbrush_t *brush)
{
  generic_avl_map_node_t *m_tree_root; // [esp+0h] [ebp-10h]
  chull_t *chull; // [esp+8h] [ebp-8h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  m_tree_root = entities_map.m_tree_root;
  while ( m_tree_root && brush != (const cbrush_t *)m_tree_root->m_avl_key )
  {
    if ( (unsigned int)brush >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
  {
    chull = (chull_t *)m_tree_root->m_data;
  }
  else
  {
    chull = generate_brush_chull((int)&savedregs, brush);
    if ( !chull )
      return 0;
    chull_list_add(chull, (unsigned int)brush);
  }
  if ( !chull && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1832, 0, "%s", "chull") )
    __debugbreak();
  chull->touched = 1;
  return chull;
}

chull_t *__cdecl get_partition_chull(const CollisionAabbTree *tree)
{
  generic_avl_map_node_t *m_tree_root; // [esp+0h] [ebp-10h]
  chull_t *chull; // [esp+8h] [ebp-8h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  m_tree_root = entities_map.m_tree_root;
  while ( m_tree_root && tree != (const CollisionAabbTree *)m_tree_root->m_avl_key )
  {
    if ( (unsigned int)tree >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
  {
    chull = (chull_t *)m_tree_root->m_data;
  }
  else
  {
    chull = generate_partition_chull((const float *)&savedregs, tree);
    if ( !chull )
      return 0;
    chull_list_add(chull, (unsigned int)tree);
  }
  if ( !chull && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1854, 0, "%s", "chull") )
    __debugbreak();
  chull->touched = 1;
  return chull;
}

void  Phys_FindAndRenderEntityBrushes(int a1@<ebp>, const float *pos, int contentmask)
{
  void *v3; // esp
  const XModel *XModel; // eax
  const XModel *v5; // eax
  unsigned int v6; // [esp-38FCh] [ebp-3908h]
  unsigned int v7; // [esp-38FCh] [ebp-3908h]
  const chull_t *collmap_chull; // [esp-38E8h] [ebp-38F4h]
  PhysGeomList *Collmap; // [esp-38E4h] [ebp-38F0h]
  phys_mat44 v10; // [esp-38E0h] [ebp-38ECh] BYREF
  float v11[9]; // [esp-3898h] [ebp-38A4h] BYREF
  DynEntityPose *v12; // [esp-3874h] [ebp-3880h]
  const DynEntityDef *EntityDef; // [esp-3870h] [ebp-387Ch]
  unsigned __int16 *v14; // [esp-386Ch] [ebp-3878h]
  signed int n; // [esp-3868h] [ebp-3874h]
  signed int v16; // [esp-3864h] [ebp-3870h]
  _BYTE v17[8000]; // [esp-3860h] [ebp-386Ch] BYREF
  float v18; // [esp-1920h] [ebp-192Ch]
  DynEntityPose *ClientPose; // [esp-191Ch] [ebp-1928h]
  signed int m; // [esp-1918h] [ebp-1924h]
  float v21[4]; // [esp-1914h] [ebp-1920h] BYREF
  cg_s *LocalClientGlobals; // [esp-1904h] [ebp-1910h]
  unsigned __int16 v23[1000]; // [esp-1900h] [ebp-190Ch] BYREF
  DynEntityDrawType k; // [esp-1130h] [ebp-113Ch]
  const float *v25; // [esp-112Ch] [ebp-1138h]
  const float *v26; // [esp-1128h] [ebp-1134h]
  DObj *ClientDObj; // [esp-1124h] [ebp-1130h]
  phys_mat44 v28; // [esp-1120h] [ebp-112Ch] BYREF
  float v29[9]; // [esp-10D4h] [ebp-10E0h] BYREF
  centity_s *Entity; // [esp-10B0h] [ebp-10BCh]
  signed int j; // [esp-10ACh] [ebp-10B8h]
  int v32; // [esp-10A8h] [ebp-10B4h]
  bool v33; // [esp-10A4h] [ebp-10B0h]
  bool v34; // [esp-10A0h] [ebp-10ACh]
  const float *v35; // [esp-109Ch] [ebp-10A8h]
  const float *v36; // [esp-1098h] [ebp-10A4h]
  DObj *ServerDObj; // [esp-1094h] [ebp-10A0h]
  phys_mat44 v38; // [esp-1090h] [ebp-109Ch] BYREF
  float v39[9]; // [esp-1050h] [ebp-105Ch] BYREF
  gentity_s *v40; // [esp-102Ch] [ebp-1038h]
  signed int i; // [esp-1028h] [ebp-1034h]
  signed int v42; // [esp-1024h] [ebp-1030h]
  int v43[1000]; // [esp-1020h] [ebp-102Ch] BYREF
  unsigned int v44[4]; // [esp-80h] [ebp-8Ch] BYREF
  unsigned int v45[4]; // [esp-70h] [ebp-7Ch] BYREF
  float v46[4]; // [esp-60h] [ebp-6Ch] BYREF
  unsigned int v47[4]; // [esp-50h] [ebp-5Ch] BYREF
  float v48[4]; // [esp-40h] [ebp-4Ch] BYREF
  float v49[3]; // [esp-30h] [ebp-3Ch] BYREF
  float v50[3]; // [esp-24h] [ebp-30h] BYREF
  signed int v51; // [esp-18h] [ebp-24h]
  int v52; // [esp-14h] [ebp-20h]
  float v53; // [esp-10h] [ebp-1Ch]
  float v54; // [esp-Ch] [ebp-18h]
  float v55; // [esp-8h] [ebp-14h]
  unsigned int v56[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v56[0] = a1;
  v56[1] = retaddr;
  v3 = alloca(14576);
  v53 = query_radius;
  v54 = query_radius;
  v55 = query_radius;
  v52 = 1000;
  v51 = 200;
  v50[0] = *pos - query_radius;
  v50[1] = pos[1] - query_radius;
  v50[2] = pos[2] - query_radius;
  v49[0] = *pos + query_radius;
  v49[1] = pos[1] + query_radius;
  v49[2] = pos[2] + query_radius;
  v48[0] = 0.0f;
  v48[1] = 1.0f;
  v48[2] = 1.0f;
  v48[3] = 0.25f;
  *(float *)v47 = 1.0f;
  v47[1] = 0;
  v47[2] = 0;
  *(float *)&v47[3] = 0.25f;
  v46[0] = 0.0f;
  v46[1] = 1.0f;
  v46[2] = 0.0f;
  v46[3] = 0.25f;
  *(float *)v45 = 1.0f;
  *(float *)&v45[1] = 1.0f;
  v45[2] = 0;
  *(float *)&v45[3] = 0.25f;
  *(float *)v44 = 1.0f;
  *(float *)&v44[1] = 1.0f;
  *(float *)&v44[2] = 1.0f;
  *(float *)&v44[3] = 0.25f;
  v42 = CM_AreaEntities(v50, v49, v43, 200, contentmask);
  for ( i = 0; i < v42; ++i )
  {
    v40 = &g_entities[v43[i]];
    AnglesToAxis(v40->r.currentAngles, (float (*)[3])v39);
    Phys_AxisToNitrousMat((float (*)[3])v39, &v38);
    Phys_Vec3ToNitrousVec(v40->r.currentOrigin, &v38.w);
    ServerDObj = Com_GetServerDObj(v43[i]);
    if ( ServerDObj )
    {
      if ( (v40->r.contents & 0x405C0008) != 0 )
        v36 = (const float *)v47;
      else
        v36 = v48;
      v6 = (unsigned int)v40;
      XModel = DObjGetXModel(ServerDObj, 0);
      render_xmodel_chull(XModel, v6, &v38, v36);
    }
    else if ( v40->r.bmodel )
    {
      if ( (v40->r.contents & 0x405C0008) != 0 )
        v35 = (const float *)v47;
      else
        v35 = v48;
      render_brushmodel_chull(v40->s.index.brushmodel, (unsigned int)v40, &v38, v35);
    }
  }
  v34 = (char *)Phys_GetMaskFromDVar() == &cls.recentServers[7734].game[12]
     && (g_bDebugRenderPatches->current.enabled || g_bDebugRenderBrushes->current.enabled);
  v33 = v34;
  if ( !v34 )
  {
    v32 = cgCollWorldLocalClientNum;
    v42 = CG_AreaEntities(v50, v49, v43, 200, contentmask);
    for ( j = 0; j < v42; ++j )
    {
      if ( v43[j] >= 1024 )
      {
        Entity = CG_GetEntity(v32, v43[j]);
        AnglesToAxis(Entity->pose.angles, (float (*)[3])v29);
        Phys_AxisToNitrousMat((float (*)[3])v29, &v28);
        Phys_Vec3ToNitrousVec(Entity->pose.origin, &v28.w);
        ClientDObj = Com_GetClientDObj(v43[j], v32);
        if ( ClientDObj )
        {
          if ( ((*((unsigned int *)Entity + 201) >> 15) & 1) != 0 )
            v26 = (const float *)v44;
          else
            v26 = (const float *)v45;
          v7 = (unsigned int)Entity;
          v5 = DObjGetXModel(ClientDObj, 0);
          render_xmodel_chull(v5, v7, &v28, v26);
        }
        else if ( (char *)Entity->nextState.solid == &cls.rankedServers[711].game[34] )
        {
          if ( ((*((unsigned int *)Entity + 201) >> 15) & 1) != 0 )
            v25 = (const float *)v44;
          else
            v25 = (const float *)v45;
          render_brushmodel_chull(Entity->nextState.index.brushmodel, (unsigned int)Entity, &v28, v25);
        }
      }
    }
    for ( k = DYNENT_DRAW_MODEL; k < DYNENT_DRAW_COUNT; ++k )
    {
      v42 = DynEntCl_AreaEntities(k, v50, v49, contentmask, 0x3E8u, v23);
      LocalClientGlobals = CG_GetLocalClientGlobals(0);
      LODWORD(v21[3]) = LocalClientGlobals->predictedPlayerState.origin;
      v21[0] = LocalClientGlobals->predictedPlayerState.origin[0];
      v21[1] = LocalClientGlobals->predictedPlayerState.origin[1];
      v21[2] = LocalClientGlobals->predictedPlayerState.origin[2];
      for ( m = 0; m < v42; ++m )
      {
        ClientPose = DynEnt_GetClientPose(v23[m], k);
        v18 = Vec3DistanceSq(v21, ClientPose->pose.origin);
        *(_WORD *)&v17[8 * m] = v23[m];
        *(float *)&v17[8 * m + 4] = v18;
      }
      qsort(v17, v42, 8u, (int (__cdecl *)(const void *, const void *))SortSModelsByDist);
      if ( v42 >= v51 )
        v16 = v51;
      else
        v16 = v42;
      v42 = v16;
      for ( n = 0; n < v42; ++n )
      {
        v14 = (unsigned __int16 *)&v17[8 * n];
        EntityDef = DynEnt_GetEntityDef(*v14, k);
        v12 = DynEnt_GetClientPose(*v14, k);
        UnitQuatToAxis(v12->pose.quat, (float (*)[3])v11);
        Phys_AxisToNitrousMat((float (*)[3])v11, &v10);
        Phys_Vec3ToNitrousVec(v12->pose.origin, &v10.w);
        if ( k )
        {
          render_brushmodel_chull(EntityDef->brushModel, (unsigned int)EntityDef, &v10, v46);
        }
        else
        {
          render_xmodel_chull(EntityDef->xModel, (unsigned int)EntityDef, &v10, v48);
          if ( XModelHasCollmap(EntityDef->xModel) )
          {
            Collmap = XModelGetCollmap(EntityDef->xModel, 0);
            collmap_chull = get_collmap_chull(Collmap, (unsigned int)EntityDef);
            render_chull(COERCE_FLOAT(v56), collmap_chull, &v10, v46);
          }
        }
      }
    }
  }
}

char __cdecl Phys_DebugRenderChanged()
{
  if ( bPrevDebugRenderBulletMeshes == g_bDebugRenderBulletMeshes->current.enabled
    && bPrevDebugRenderPatches == g_bDebugRenderPatches->current.enabled
    && bPrevDebugRenderColoredPatches == g_bDebugRenderColoredPatches->current.enabled
    && bPrevDebugRenderBrushes == g_bDebugRenderBrushes->current.enabled
    && prevDebugRenderMask == g_debugRenderMask->current.integer
    && prevDebugRenderCollisionDistance == g_debugRenderCollisionDistance->current.value
    && prevDebugRenderEntityBrushes == g_bDebugRenderEntityBrushes->current.enabled )
  {
    return 0;
  }
  bPrevDebugRenderBulletMeshes = g_bDebugRenderBulletMeshes->current.enabled;
  bPrevDebugRenderPatches = g_bDebugRenderPatches->current.enabled;
  bPrevDebugRenderColoredPatches = g_bDebugRenderColoredPatches->current.enabled;
  bPrevDebugRenderBrushes = g_bDebugRenderBrushes->current.enabled;
  prevDebugRenderMask = g_debugRenderMask->current.integer;
  prevDebugRenderCollisionDistance = g_debugRenderCollisionDistance->current.value;
  prevDebugRenderEntityBrushes = g_bDebugRenderEntityBrushes->current.enabled;
  return 1;
}

int __cdecl Phys_GetMaskFromDVar()
{
  int result; // eax
  unsigned int mask; // [esp+4h] [ebp-4h]

  mask = g_debugRenderMask->current.unsignedInt;
  if ( mask > 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          2090,
          0,
          "%s",
          "mask >= 0 && mask <= 8") )
  {
    __debugbreak();
  }
  switch ( mask )
  {
    case 0u:
      result = -1;
      break;
    case 1u:
      result = (int)&cls.recentServers[7647].hostName[20];
      break;
    case 2u:
      result = 0x10000;
      break;
    case 3u:
      result = 529;
      break;
    case 4u:
      result = (int)&cls.recentServers[7734].game[12];
      break;
    case 5u:
      result = (int)&cls.recentServers[7489].hostName[22];
      break;
    case 6u:
      result = (int)&cls.recentServers[7543].countrycode[1];
      break;
    case 7u:
      result = (int)&cls.rankedServers[711].game[35];
      break;
    case 8u:
      result = (int)&cls.recentServers[7546].city[57];
      break;
    default:
      result = -1;
      break;
  }
  return result;
}

void __cdecl Phys_RenderWorldCollMesh(const float *pos, bool bRenderStaticCollision, bool bRenderBrushes)
{
  float s_expand_h; // [esp+7Ch] [ebp-A4h]
  colgeom_debug_renderer_t colgeomDebugVisitor; // [esp+80h] [ebp-A0h] BYREF
  bool bDebugRenderChanged; // [esp+107h] [ebp-19h]
  float s_expand_v; // [esp+108h] [ebp-18h]
  float expand_vec[3]; // [esp+10Ch] [ebp-14h] BYREF
  int mask; // [esp+118h] [ebp-8h]
  bool bDoWork; // [esp+11Fh] [ebp-1h]

  bDoWork = 0;
  bDebugRenderChanged = Phys_DebugRenderChanged();
  if ( bRenderStaticCollision || bRenderBrushes )
  {
    if ( bDebugRenderChanged )
    {
      bDoWork = 1;
    }
    else if ( previous_pos[0] == 3.4028235e38 )
    {
      bDoWork = 1;
    }
    else if ( (float)((float)((float)((float)(*pos - previous_pos[0]) * (float)(*pos - previous_pos[0]))
                            + (float)((float)(pos[1] - previous_pos[1]) * (float)(pos[1] - previous_pos[1])))
                    + (float)((float)(pos[2] - previous_pos[2]) * (float)(pos[2] - previous_pos[2]))) > (float)((float)(g_debugRenderCollisionDistance->current.value * 0.5) * (float)(g_debugRenderCollisionDistance->current.value * 0.5)) )
    {
      bDoWork = 1;
    }
    if ( bDoWork )
    {
      previous_pos[0] = *pos;
      previous_pos[1] = pos[1];
      previous_pos[2] = pos[2];
      DebugPatchesAndBrushesProlog();
      clear_debug_brushes_and_patches();
      colgeom_visitor_t::colgeom_visitor_t(&colgeomDebugVisitor);
      colgeomDebugVisitor.__vftable = (colgeom_debug_renderer_t_vtbl *)&colgeom_debug_renderer_t::`vftable';
      colgeom_debug_renderer_t::reset(&colgeomDebugVisitor);
      colgeomDebugVisitor.bRenderStaticCollision = bRenderStaticCollision;
      colgeomDebugVisitor.bRenderBrushes = bRenderBrushes;
      colgeomDebugVisitor.lightPos[0] = *pos;
      colgeomDebugVisitor.lightPos[1] = pos[1];
      colgeomDebugVisitor.lightPos[2] = pos[2];
      mask = Phys_GetMaskFromDVar();
      s_expand_h = g_debugRenderCollisionDistance->current.value;
      s_expand_v = 5000.0f;
      expand_vec[0] = s_expand_h;
      expand_vec[1] = s_expand_h;
      expand_vec[2] = 5000.0f;
      colgeom_debug_renderer_t::reset(&colgeomDebugVisitor);
      colgeom_debug_renderer_t::update(&colgeomDebugVisitor, pos, pos, mask, expand_vec);
      DebugPatchesAndBrushesEpilog();
    }
  }
  else if ( bDebugRenderChanged )
  {
    DebugPatchesAndBrushesProlog();
    clear_debug_brushes_and_patches();
    R_ClearPersistentDebugPolygons();
    DebugPatchesAndBrushesEpilog();
  }
}

void __thiscall colgeom_debug_renderer_t::reset(colgeom_debug_renderer_t *this)
{
  this->m_mn.vec.v[0] = FLOAT_9_9999997e37;
  this->m_mn.vec.v[1] = FLOAT_9_9999997e37;
  this->m_mn.vec.v[2] = FLOAT_9_9999997e37;
  this->m_mx.vec.v[0] = FLOAT_N9_9999997e37;
  this->m_mx.vec.v[1] = FLOAT_N9_9999997e37;
  this->m_mx.vec.v[2] = FLOAT_N9_9999997e37;
}

void __thiscall colgeom_debug_renderer_t::visit(colgeom_debug_renderer_t *this, const CollisionAabbTree *tree)
{
  if ( this->bRenderStaticCollision )
    add_debug_patch(tree);
}

void __thiscall colgeom_debug_renderer_t::visit(colgeom_debug_renderer_t *this, const cbrush_t *brush)
{
  if ( this->bRenderBrushes )
    add_debug_brush(brush, 0);
}

void __thiscall colgeom_debug_renderer_t::update(
        colgeom_debug_renderer_t *this,
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec)
{
  bool v5; // [esp+0h] [ebp-58h]
  float result[3]; // [esp+18h] [ebp-40h] BYREF
  float b[3]; // [esp+24h] [ebp-34h] BYREF
  float a[3]; // [esp+30h] [ebp-28h] BYREF
  bool inside; // [esp+3Fh] [ebp-19h]
  float mx[3]; // [esp+40h] [ebp-18h] BYREF
  float mn[3]; // [esp+4Ch] [ebp-Ch] BYREF

  a[0] = this->m_mn.vec.v[0] - *_mn;
  a[1] = this->m_mn.vec.v[1] - _mn[1];
  a[2] = this->m_mn.vec.v[2] - _mn[2];
  b[0] = *_mx - this->m_mx.vec.v[0];
  b[1] = _mx[1] - this->m_mx.vec.v[1];
  b[2] = _mx[2] - this->m_mx.vec.v[2];
  Vec3Max(a, b, result);
  v5 = result[0] < 0.0 && result[1] < 0.0 && result[2] < 0.0;
  inside = v5;
  if ( this->m_mask != mask || !inside )
  {
    mn[0] = *_mn - *expand_vec;
    mn[1] = _mn[1] - expand_vec[1];
    mn[2] = _mn[2] - expand_vec[2];
    mx[0] = *_mx + *expand_vec;
    mx[1] = _mx[1] + expand_vec[1];
    mx[2] = _mx[2] + expand_vec[2];
    colgeom_debug_renderer_t::reset(this);
    colgeom_visitor_t::intersect_box(this, mn, mx, mask);
  }
}

void __cdecl debug_loop()
{
  const char *Name; // [esp+8h] [ebp-844h]
  const DObj *dobj; // [esp+B4h] [ebp-798h]
  unsigned int id; // [esp+B8h] [ebp-794h]
  float endpos[3]; // [esp+BCh] [ebp-790h] BYREF
  DynEntityDef *def; // [esp+C8h] [ebp-784h]
  float hitDir[3]; // [esp+CCh] [ebp-780h]
  float vel[3]; // [esp+D8h] [ebp-774h] BYREF
  float origin[3]; // [esp+E4h] [ebp-768h] BYREF
  float quat[4]; // [esp+F0h] [ebp-75Ch] BYREF
  float hitPos[3]; // [esp+100h] [ebp-74Ch]
  colgeom_visitor_inlined_t<200> visitor; // [esp+10Ch] [ebp-740h] BYREF
  float mins[3]; // [esp+7C8h] [ebp-84h] BYREF
  float maxs[4]; // [esp+7D4h] [ebp-78h] BYREF
  int hitNum; // [esp+7E4h] [ebp-68h] BYREF
  int mask; // [esp+7E8h] [ebp-64h]
  int passEntityNum; // [esp+7ECh] [ebp-60h]
  cg_s *cgameGlob; // [esp+7F0h] [ebp-5Ch]
  float start[3]; // [esp+7F4h] [ebp-58h] BYREF
  float end[3]; // [esp+800h] [ebp-4Ch] BYREF
  bool render_hitpoint; // [esp+80Fh] [ebp-3Dh]
  trace_t results; // [esp+810h] [ebp-3Ch] BYREF
  int savedregs; // [esp+84Ch] [ebp+0h] BYREF

  cgameGlob = CG_GetLocalClientGlobals(0);
  if ( cgameGlob && g_bDebugRenderBulletMeshes && g_bDebugRenderEntityBrushes )
  {
    start[0] = cgameGlob->refdef.vieworg[0];
    start[1] = cgameGlob->refdef.vieworg[1];
    start[2] = cgameGlob->refdef.vieworg[2];
    end[0] = (float)(scale_3 * cgameGlob->refdef.viewaxis[0][0]) + start[0];
    end[1] = (float)(scale_3 * cgameGlob->refdef.viewaxis[0][1]) + start[1];
    end[2] = (float)(scale_3 * cgameGlob->refdef.viewaxis[0][2]) + start[2];
    if ( g_bDebugRenderBulletMeshes->current.enabled )
      Phys_FindAndRenderBulletMesh(start, end);
    if ( g_bDebugRenderEntityBrushes->current.enabled )
    {
      mask = 1121773627;
      if ( g_bDebugRenderPatches->current.enabled || g_bDebugRenderBrushes->current.enabled )
        mask = Phys_GetMaskFromDVar();
      Phys_FindAndRenderEntityBrushes((int)&savedregs, start, mask);
    }
    g_debug_partition = 0;
    Phys_RenderWorldCollMesh(start, g_bDebugRenderPatches->current.enabled, g_bDebugRenderBrushes->current.enabled);
    if ( g_bDebugRenderStaticModelsBounds->current.enabled )
      draw_static_models_bounds();
    if ( g_dumpStaticModels->current.enabled )
    {
      Dvar_SetBool((dvar_s *)g_dumpStaticModels, 0);
      print_static_models_stats();
    }
    if ( debug_trace->current.integer )
    {
      passEntityNum = 0;
      memset((unsigned __int8 *)&results, 0, sizeof(results));
      render_hitpoint = 0;
      switch ( debug_trace->current.integer )
      {
        case 1:
          hitNum = 0;
          CG_SightTracePoint(&hitNum, start, end, (int)&cls.recentServers[7543].countrycode[1], &results);
          render_hitpoint = 1;
          break;
        case 2:
          G_LocationalTrace(
            &results,
            start,
            end,
            passEntityNum,
            (int)&cls.recentServers[7543].countrycode[1],
            bulletPriorityMap,
            0);
          render_hitpoint = 1;
          g_debug_partition = results.hitPartition;
          break;
        case 3:
          CG_LocationalTrace(&results, start, end, passEntityNum, (int)&cls.recentServers[7543].countrycode[1], 0, 0);
          render_hitpoint = 1;
          g_debug_partition = results.hitPartition;
          break;
        case 4:
          colgeom_visitor_t::colgeom_visitor_t(&visitor);
          visitor.__vftable = (colgeom_visitor_inlined_t<200>_vtbl *)&colgeom_visitor_inlined_t<200>::`vftable';
          colgeom_visitor_inlined_t<500>::reset(&visitor);
          mins[0] = -50.0f;
          mins[1] = -50.0f;
          mins[2] = -50.0f;
          maxs[0] = 50.0f;
          maxs[1] = 50.0f;
          maxs[2] = 50.0f;
          maxs[3] = NAN;
          colgeom_visitor_inlined_t<200>::update(&visitor, start, start, mins, maxs, -1);
          render_prims(visitor.prims, visitor.nprims);
          visitor.__vftable = (colgeom_visitor_inlined_t<200>_vtbl *)&visitor_base_t::`vftable';
          break;
        case 5:
          if ( down )
          {
            if ( !Key_IsDown(0, 19) )
            {
              if ( cm.dynEntCount[0] )
              {
                hitPos[0] = start[0];
                hitPos[1] = start[1];
                hitPos[2] = start[2];
                hitDir[0] = 1.0f;
                hitDir[1] = 0.0f;
                hitDir[2] = 0.0f;
                memset(quat, 0, 12);
                quat[3] = 1.0f;
                origin[0] = (float)(sc1 * cgameGlob->refdef.viewaxis[0][0]) + start[0];
                origin[1] = (float)(sc1 * cgameGlob->refdef.viewaxis[0][1]) + start[1];
                origin[2] = (float)(sc1 * cgameGlob->refdef.viewaxis[0][2]) + start[2];
                vel[0] = velScale * cgameGlob->refdef.viewaxis[0][0];
                vel[1] = velScale * cgameGlob->refdef.viewaxis[0][1];
                vel[2] = velScale * cgameGlob->refdef.viewaxis[0][2];
                def = &cm.dynEntDefList[0][G_irand(0, cm.dynEntCount[0] - 1)];
                if ( def->physPreset )
                  DynEntCl_CreateEntityModel(def->xModel, quat, origin, origin, vel, 7, def->physPreset);
              }
            }
          }
          down = Key_IsDown(0, 19);
          break;
        default:
          break;
      }
      if ( render_hitpoint && results.fraction < 1.0 )
      {
        Vec3Lerp(start, end, results.fraction, endpos);
        G_DebugStar(endpos, colorRed, 1);
        if ( results.hitType == TRACE_HITTYPE_ENTITY )
        {
          id = Trace_GetEntityHitId(&results);
          dobj = 0;
          if ( debug_trace->current.integer == 2 )
          {
            dobj = Com_GetServerDObj(id);
          }
          else if ( debug_trace->current.integer == 3 )
          {
            dobj = Com_GetClientDObj(id, 0);
          }
          if ( dobj )
          {
            Name = DObjGetName(dobj);
            Com_Printf(20, "%d, %s\n", id, Name);
          }
        }
      }
    }
  }
}

void __cdecl print_smodel(cStaticModel_s *smodel, int refcount, const float *dims)
{
  XModel *model; // [esp+28h] [ebp-8h]
  int ntris; // [esp+2Ch] [ebp-4h]

  model = smodel->xmodel;
  BLOPS_NULLSUB();
  Com_Printf(
    20,
    "%-42s\t refs( %4d ) lods( %d ) ncol_tris( %4d )   avr dims( %5.0f, %5.0f, %5.0f )\n",
    model->name,
    refcount,
    model->numLods,
    ntris,
    (float)((float)(1.0 / (float)refcount) * *dims),
    (float)((float)(1.0 / (float)refcount) * dims[1]),
    (float)((float)(1.0 / (float)refcount) * dims[2]));
}

void __cdecl print_static_models_stats()
{
  unsigned int numStaticModels; // [esp+0h] [ebp-10050h]
  cStaticModel_s *v1; // [esp+20h] [ebp-10030h]
  signed int j; // [esp+24h] [ebp-1002Ch]
  signed int i; // [esp+28h] [ebp-10028h]
  cStaticModel_s *smodel; // [esp+2Ch] [ebp-10024h]
  unsigned int base[16384]; // [esp+30h] [ebp-10020h] BYREF
  int refcount; // [esp+10030h] [ebp-20h]
  float v7; // [esp+10034h] [ebp-1Ch]
  float v8; // [esp+10038h] [ebp-18h]
  float v9; // [esp+1003Ch] [ebp-14h]
  float dims; // [esp+10040h] [ebp-10h] BYREF
  float v11; // [esp+10044h] [ebp-Ch]
  float v12; // [esp+10048h] [ebp-8h]
  unsigned int num; // [esp+1004Ch] [ebp-4h]

  Com_Printf(20, "Static models %d\n\n", cm.numStaticModels);
  if ( (int)cm.numStaticModels <= 0x4000 )
    numStaticModels = cm.numStaticModels;
  else
    numStaticModels = 0x4000;
  num = numStaticModels;
  for ( i = 0; i < (int)num; ++i )
    base[i] = &cm.staticModelList[i];
  qsort(base, num, 4u, SortModelsFunc);
  refcount = 1;
  smodel = (cStaticModel_s *)base[0];
  dims = *(float *)(base[0] + 68) - *(float *)(base[0] + 56);
  v11 = *(float *)(base[0] + 72) - *(float *)(base[0] + 60);
  v12 = *(float *)(base[0] + 76) - *(float *)(base[0] + 64);
  for ( j = 1; j < (int)num; ++j )
  {
    v1 = (cStaticModel_s *)base[j];
    if ( v1->xmodel == smodel->xmodel )
    {
      ++refcount;
      v7 = smodel->absmax[0] - smodel->absmin[0];
      v8 = smodel->absmax[1] - smodel->absmin[1];
      v9 = smodel->absmax[2] - smodel->absmin[2];
      dims = v7 + dims;
      v11 = v8 + v11;
      v12 = v9 + v12;
    }
    else
    {
      print_smodel(smodel, refcount, &dims);
      smodel = v1;
      dims = v1->absmax[0] - v1->absmin[0];
      v11 = v1->absmax[1] - v1->absmin[1];
      v12 = v1->absmax[2] - v1->absmin[2];
      refcount = 1;
    }
  }
  print_smodel(smodel, refcount, &dims);
}

int __cdecl SortModelsFunc(const void *a, const void *b)
{
  if ( *(unsigned int *)(*(unsigned int *)a + 4) <= *(unsigned int *)(*(unsigned int *)b + 4) )
    return *(unsigned int *)(*(unsigned int *)a + 4) < *(unsigned int *)(*(unsigned int *)b + 4);
  else
    return -1;
}

void __cdecl draw_static_models_bounds()
{
  int v0; // [esp+10h] [ebp-4Ch]
  unsigned int numStaticModels; // [esp+14h] [ebp-48h]
  int i; // [esp+2Ch] [ebp-30h]
  int len2; // [esp+30h] [ebp-2Ch]
  cStaticModel_s *smodel; // [esp+34h] [ebp-28h]
  int j; // [esp+38h] [ebp-24h]
  cg_s *cgameGlob; // [esp+3Ch] [ebp-20h]
  float proj[3]; // [esp+40h] [ebp-1Ch] BYREF
  int nmodels; // [esp+4Ch] [ebp-10h]
  float p[3]; // [esp+50h] [ebp-Ch] BYREF

  if ( (int)cm.numStaticModels <= 0x4000 )
    numStaticModels = cm.numStaticModels;
  else
    numStaticModels = 0x4000;
  nmodels = numStaticModels;
  cgameGlob = CG_GetLocalClientGlobals(0);
  p[0] = cgameGlob->predictedPlayerState.origin[0];
  p[1] = cgameGlob->predictedPlayerState.origin[1];
  p[2] = cgameGlob->predictedPlayerState.origin[2];
  for ( j = 0; j < nmodels; ++j )
  {
    smodel = &cm.staticModelList[j];
    Vec3Min(smodel->absmax, p, proj);
    Vec3Max(proj, smodel->absmin, proj);
    *(float *)&len2 = Vec3DistanceSq(p, proj);
    smodel_debug_infos[j].smodel = smodel;
    dword_9BE612C[2 * j] = len2;
  }
  qsort(smodel_debug_infos, nmodels, 8u, (int (__cdecl *)(const void *, const void *))SortSModelsByDist);
  for ( i = 0; ; ++i )
  {
    v0 = nmodels >= nboxes ? nboxes : nmodels;
    if ( i >= v0 )
      break;
    CG_DebugBox(
      vec3_origin,
      smodel_debug_infos[i].smodel->absmin,
      smodel_debug_infos[i].smodel->absmax,
      0.0,
      colorWhite,
      1,
      1);
  }
}

int __cdecl SortSModelsByDist(float *a, float *b)
{
  if ( a[1] > b[1] )
    return 1;
  if ( b[1] <= a[1] )
    return 0;
  return -1;
}

int __cdecl buoyancy_worker()
{
  phys_free_list<PhysObjUserData>::T_internal_base *i; // [esp+14h] [ebp-4h]

  for ( i = g_pop_iter.m_ptr; g_pop_iter_end.m_ptr != i; i = g_pop_iter.m_ptr )
  {
    if ( (phys_free_list<PhysObjUserData>::T_internal_base *)_InterlockedCompareExchange(
                                                               (volatile signed __int32 *)&g_pop_iter,
                                                               (signed __int32)i->m_next_T_internal,
                                                               (signed __int32)i) == i )
      Phys_BodyGrabSnapshotNitrous((PhysObjUserData *)&i[2], g_delta_t);
  }
  return 0;
}

void __cdecl Phys_BodyGrabSnapshotNitrous(PhysObjUserData *userData, float deltaT)
{
  rigid_body *v2; // [esp+D8h] [ebp-10h] BYREF
  const phys_mat44 *rb2w; // [esp+DCh] [ebp-Ch]
  rigid_body *body; // [esp+E0h] [ebp-8h]
  float drag_scale; // [esp+E4h] [ebp-4h]
  int savedregs; // [esp+E8h] [ebp+0h] BYREF

  body = userData->body;
  v2 = body;
  _mm_prefetch((const char *)&v2, 1);
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 1360, 0, "%s", "body") )
    __debugbreak();
  rb2w = &userData->body->m_mat;
  phys_full_multiply_mat((int)&savedregs, &userData->cg2w, rb2w, &userData->cg2rb);
  if ( (body->m_flags & 0x20) == 0 )
  {
    drag_scale = 1.0f;
    if ( (userData->m_flags & 1) == 0 )
      drag_scale = 2.0f;
    body->m_t_drag_coef = phys_dragLinear->current.value * drag_scale;
    body->m_a_drag_coef = phys_dragAngular->current.value * drag_scale;
    userData->m_time_since_last_event = userData->m_time_since_last_event + deltaT;
    userData->m_time_since_last_reeval = userData->m_time_since_last_reeval + deltaT;
    Phys_ReEvalPriority(userData);
  }
}

void  UpdateRigidBody(float a1@<ebp>, float delta_t)
{
  float t; // xmm0_4
  float v3; // [esp+38h] [ebp-48h]
  float v4; // [esp+3Ch] [ebp-44h]
  float value; // [esp+54h] [ebp-2Ch]
  phys_free_list<rigid_body> *p_m_list_rigid_body; // [esp+58h] [ebp-28h]
  phys_free_list<rigid_body>::T_internal_base *m_next_T_internal; // [esp+5Ch] [ebp-24h]
  phys_free_list<rigid_body>::iterator i_end; // [esp+64h] [ebp-1Ch] BYREF
  phys_free_list<rigid_body>::iterator i; // [esp+68h] [ebp-18h]
  float v10; // [esp+6Ch] [ebp-14h]
  phys_vec3 gravity_dir; // [esp+70h] [ebp-10h] BYREF
  float retaddr; // [esp+80h] [ebp+0h]

  gravity_dir.y = a1;
  gravity_dir.z = retaddr;
  //PIXBeginNamedEvent(-1, "update 1");
  BLOPS_NULLSUB();
  debug_loop();
  free_chull_lists();
  Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
  if ( gravityChange )
  {
    Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, (phys_vec3 *)&i_end);
    m_next_T_internal = g_physics_system->m_list_rigid_body.m_dummy_head.m_next_T_internal;
    p_m_list_rigid_body = &g_physics_system->m_list_rigid_body;
    while ( p_m_list_rigid_body != (phys_free_list<rigid_body> *)m_next_T_internal )
    {
      value = phys_gravity->current.value;
      v3 = value * *(float *)&i.m_ptr;
      v4 = value * v10;
      *(float *)&m_next_T_internal[18].m_prev_T_internal = value * *(float *)&i_end.m_ptr;
      *(float *)&m_next_T_internal[18].m_next_T_internal = v3;
      *(float *)&m_next_T_internal[19].m_prev_T_internal = v4;
      m_next_T_internal = m_next_T_internal->m_next_T_internal;
    }
    gravityChange = 0;
  }
  if ( delta_t >= 0.1 )
    t = 0.1f;
  else
    t = delta_t;
  Phys_AddCacheImpulses((int)&gravity_dir.y);
  Nitrous_ForEachBody<void (__cdecl *)(PhysObjUserData &,float)>(Phys_BodyGrabSnapshotNitrous, t);
  Nitrous_ForEachBody<void (__cdecl *)(PhysObjUserData &,float)>(
    (void (__cdecl *)(PhysObjUserData *, float))Phys_DebugRender,
    t);
  render_debug_draw_gjk_trace_geom();
  NitrousVehicle::frame_prolog_all_systems(t);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  //PIXBeginNamedEvent(-1, "phys_frame_advance");
  phys_sys::phys_frame_advance(t);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  //PIXBeginNamedEvent(-1, "update 2");
  NitrousVehicle::debug_render_all();
  NitrousVehicle::frame_epilog_all_systems(t);
  process_destructible_hits();
  process_dynent_hits();
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
}

void __cdecl Phys_DebugRender(PhysObjUserData *userData)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  if ( phys_drawCollisionObj->current.enabled )
    debug_render((int)&savedregs, userData);
}

void  Phys_CollisionCallback(int a1@<ebp>)
{
  const phys_vec3 *v1; // eax
  float v2; // [esp-2Ch] [ebp-CCh]
  float v3; // [esp-28h] [ebp-C8h]
  float v4; // [esp-Ch] [ebp-ACh] BYREF
  float v5; // [esp-8h] [ebp-A8h]
  float v6; // [esp-4h] [ebp-A4h]
  phys_vec3 closest_point; // [esp+0h] [ebp-A0h] BYREF
  phys_vec3 m_aabb_mx_loc; // [esp+14h] [ebp-8Ch] BYREF
  phys_vec3 m_aabb_mn_loc; // [esp+24h] [ebp-7Ch] BYREF
  float v10; // [esp+34h] [ebp-6Ch] BYREF
  float v11; // [esp+38h] [ebp-68h]
  float v12; // [esp+3Ch] [ebp-64h]
  gjk_base_t *m_gjk_geom; // [esp+50h] [ebp-50h]
  const phys_mat44 *m_cg_to_world_xform; // [esp+54h] [ebp-4Ch]
  broad_phase_info *i; // [esp+58h] [ebp-48h]
  gjk_base_t *geom; // [esp+5Ch] [ebp-44h]
  const phys_mat44 *geom_mat; // [esp+60h] [ebp-40h]
  broad_phase_info *bpi_env; // [esp+64h] [ebp-3Ch] BYREF
  phys_vec3 player_origin; // [esp+70h] [ebp-30h]
  phys_free_list<rigid_body_constraint_contact> *p_m_list_rbc_contact; // [esp+80h] [ebp-20h]
  centity_s *v21; // [esp+84h] [ebp-1Ch]
  centity_s *player_cent; // [esp+88h] [ebp-18h]
  phys_free_list<rigid_body_constraint_contact>::iterator rbc_i_end; // [esp+8Ch] [ebp-14h]
  phys_free_list<rigid_body_constraint_contact>::iterator rbc_i; // [esp+90h] [ebp-10h]
  unsigned int v25[3]; // [esp+94h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+A0h] [ebp+0h]

  v25[0] = a1;
  v25[1] = retaddr;
  rbc_i.m_ptr = (phys_free_list<rigid_body_constraint_contact>::T_internal_base *)physGlob.objects_by_world[0].m_alloc_count;
  if ( physGlob.objects_by_world[0].m_alloc_count >= 10
    || (rbc_i_end.m_ptr = (phys_free_list<rigid_body_constraint_contact>::T_internal_base *)physGlob.objects_by_world[1].m_alloc_count,
        physGlob.objects_by_world[1].m_alloc_count >= 4) )
  {
    cdl_proftimer::start_capture(&proftimer_physics_frame_advance);
  }
  prop_system_collision_process();
  broad_phase_process(COERCE_FLOAT(v25));
  Phys_EffectsProcess();
  if ( phys_drawcontacts->current.enabled )
  {
    player_cent = (centity_s *)g_physics_system->m_list_rbc_contact.m_dummy_head.m_next_T_internal;
    v21 = player_cent;
    p_m_list_rbc_contact = &g_physics_system->m_list_rbc_contact;
    while ( p_m_list_rbc_contact != (phys_free_list<rigid_body_constraint_contact> *)v21 )
    {
      render_contact((contact_point_info *)v25, (rigid_body_constraint_contact *)&v21->pose.cullIn);
      v21 = *(centity_s **)&v21->pose.localClientNum;
    }
  }
  if ( render_bpi_env_collision->current.integer > 0 )
  {
    LODWORD(player_origin.w) = CG_GetEntity(0, 0);
    Phys_Vec3ToNitrousVec((const phys_vec3 *)&bpi_env, (float *)(LODWORD(player_origin.w) + 48));
    *(float *)&geom_mat = (float)render_bpi_env_collision->current.integer;
    *(float *)&geom = *(float *)&geom_mat * *(float *)&geom_mat;
    for ( i = G_BPM->m_list_bpi_env; i; i = (broad_phase_info *)i->m_list_bpb_cluster_next )
    {
      m_cg_to_world_xform = i->m_cg_to_world_xform;
      m_gjk_geom = (gjk_base_t *)i->m_gjk_geom;
      ((void (__thiscall *)(gjk_base_t *, unsigned int, unsigned int))m_gjk_geom->comp_aabb_loc)(
        m_gjk_geom,
        LODWORD(v2),
        LODWORD(v3));
      phys_full_inv_multiply((int)v25, (phys_vec3 *)&v10, m_cg_to_world_xform, (const phys_vec3 *)&bpi_env);
      if ( (m_gjk_geom->m_flags & 2) == 0
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
             82,
             "get_flag(FLAG_AABB_LOC_VALID)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      m_aabb_mn_loc = m_gjk_geom->m_aabb_mn_loc;
      if ( (m_gjk_geom->m_flags & 2) == 0
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
             83,
             "get_flag(FLAG_AABB_LOC_VALID)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      m_aabb_mx_loc = m_gjk_geom->m_aabb_mx_loc;
      v1 = phys_max((phys_vec3 *)&closest_point.y, &m_aabb_mn_loc, (const phys_vec3 *)&v10);
      phys_min((const phys_vec3 *)&v4, &m_aabb_mx_loc, v1);
      v2 = v4 - v10;
      v3 = v5 - v11;
      if ( *(float *)&geom >= (float)((float)((float)((float)(v4 - v10) * (float)(v4 - v10))
                                            + (float)((float)(v5 - v11) * (float)(v5 - v11)))
                                    + (float)((float)(v6 - v12) * (float)(v6 - v12))) )
        render_gjk_geom(COERCE_FLOAT(v25), m_gjk_geom, m_cg_to_world_xform);
    }
  }
  broad_phase_reset_buffer();
}

void __thiscall cdl_proftimer::start_capture(cdl_proftimer *this)
{
  int i; // [esp+4h] [ebp-4h]

  if ( !this->capture )
  {
    this->capture = 1;
    this->capture_count = 0;
    this->tot = 0;
    this->avr = 0;
    for ( i = 0; i < 5; ++i )
    {
      LODWORD(this->mx[i]) = 0;
      HIDWORD(this->mx[i]) = 0;
    }
  }
}

char __cdecl Phys_ShouldCollideCallback(const broad_phase_base *bpi1, const broad_phase_base *bpi2)
{
  if ( (bpi1->m_my_collision_type_flags & 0x40) != 0 )
  {
    if ( (bpi2->m_my_collision_type_flags & 4) != 0 )
      return 0;
  }
  else if ( (bpi1->m_my_collision_type_flags & 4) != 0 )
  {
    if ( (bpi2->m_my_collision_type_flags & 0x40) != 0 )
      return 0;
  }
  else if ( (bpi1->m_my_collision_type_flags & 0x400) != 0 && (bpi2->m_my_collision_type_flags & 0x400) != 0 )
  {
    return 0;
  }
  return 1;
}

void __cdecl PhysicsSystem_Update()
{
  cg_s *cgameGlob; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  cg_s *cg; // [esp+8h] [ebp-4h]

  cg = CG_GetLocalClientGlobals(0);
  if ( cg && cg->snap )
  {
    for ( i = 0; i < 1; ++i )
    {
      if ( CL_LocalClient_IsActive(i) && CL_GetLocalClientConnectionState(i) >= 4 )
      {
        cgameGlob = CG_GetLocalClientGlobals(i);
        if ( cgameGlob )
          Phys_RunToTime(cgameGlob->physicsTime);
        return;
      }
    }
  }
}

void __cdecl Phys_RunToTime(int timeNow)
{
  const char *v1; // eax
  int v2; // [esp+4h] [ebp-34h]
  int v3; // [esp+20h] [ebp-18h]
  int time_msec; // [esp+34h] [ebp-4h]
  int savedregs; // [esp+38h] [ebp+0h] BYREF

  proftimer_physics_frame_advance.stamp = tlPcGetTick().QuadPart;
  ++proftimer_physics_frame_advance.calls;
  v1 = va("Physics update (obj=%d)", physGlob.objects.m_list_count);
  //PIXBeginNamedEvent(-1, v1);
  if ( !physInited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 2680, 0, "%s", "physInited") )
  {
    __debugbreak();
  }
  physGlob.timeLastSnapshot = physGlob.timeLastUpdate;
  time_msec = timeNow - physGlob.timeLastUpdate;
  if ( timeNow != physGlob.timeLastUpdate )
  {
    //PIXBeginNamedEvent(-1, "auto_rigid_body");
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    auto_rigid_body::update((auto_rigid_body *)&savedregs);
    if ( !Demo_IsPlaying() || !Demo_IsPaused() && time_msec > 0 && time_msec < 100 )
    {
      if ( time_msec < 100 )
        v3 = time_msec;
      else
        v3 = 100;
      if ( v3 > 1 )
        v2 = v3;
      else
        v2 = 1;
      time_msec = v2;
      UpdateRigidBody(COERCE_FLOAT(&savedregs), (float)v2 * 0.001);
    }
    physGlob.timeLastUpdate = timeNow;
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  proftimer_physics_frame_advance.value += *(_QWORD *)&tlPcGetTick() - proftimer_physics_frame_advance.stamp;
  //PIXBeginNamedEvent(-1, "Ragdoll_Update/XDoll_Update");
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  Ragdoll_Update(time_msec);
  XDoll_Update(time_msec);
}

void __cdecl Phys_ObjGetInterpolatedState(int id, float *outPos, float *outQuat)
{
  float oldMat[3][3]; // [esp+10h] [ebp-84h] BYREF
  float newPos[3]; // [esp+34h] [ebp-60h] BYREF
  float frac; // [esp+40h] [ebp-54h]
  float newMat[3][3]; // [esp+44h] [ebp-50h] BYREF
  float oldPos[3]; // [esp+68h] [ebp-2Ch] BYREF
  float newQuat[4]; // [esp+74h] [ebp-20h] BYREF
  float oldQuat[4]; // [esp+84h] [ebp-10h] BYREF
  int savedregs; // [esp+94h] [ebp+0h] BYREF

  frac = 1.0f;
  if ( phys_msecStep->current.integer <= 0 )
  {
    Phys_ObjGetPosition((int)&savedregs, id, outPos, newMat);
    AxisToQuat(newMat, outQuat);
  }
  else
  {
    Phys_ObjGetSnapshot(id, oldPos, oldMat);
    Phys_ObjGetPosition((int)&savedregs, id, newPos, newMat);
    Vec3Lerp(oldPos, newPos, frac, outPos);
    AxisToQuat(oldMat, oldQuat);
    AxisToQuat(newMat, newQuat);
    QuatLerp(oldQuat, newQuat, frac, outQuat);
    Vec4Normalize(outQuat);
  }
}

void  Phys_SetUserBody(int a1@<ebp>, int id, float *position)
{
  unsigned int v3[3]; // [esp-Ch] [ebp-7Ch] BYREF
  phys_mat44 dictator; // [esp+0h] [ebp-70h]
  float v5; // [esp+44h] [ebp-2Ch] BYREF
  float v6; // [esp+48h] [ebp-28h]
  float v7; // [esp+4Ch] [ebp-24h]
  phys_vec3 pos; // [esp+50h] [ebp-20h]
  float *UserData; // [esp+60h] [ebp-10h]
  int v10; // [esp+64h] [ebp-Ch]
  user_rigid_body *body; // [esp+68h] [ebp-8h]
  user_rigid_body *retaddr; // [esp+70h] [ebp+0h]

  v10 = a1;
  body = retaddr;
  UserData = (float *)Phys_GetUserData(id);
  pos.w = *UserData;
  Phys_Vec3ToNitrousVec(position, (phys_vec3 *)&v5);
  dictator.w.y = 1.0f;
  dictator.w.z = 0.0f;
  dictator.w.w = 0.0f;
  *(float *)v3 = 1.0f;
  v3[1] = 0;
  v3[2] = 0;
  dictator.x.y = 0.0f;
  dictator.x.z = 1.0f;
  dictator.x.w = 0.0f;
  dictator.y.y = 0.0f;
  dictator.y.z = 0.0f;
  dictator.y.w = 1.0f;
  dictator.z.y = v5;
  dictator.z.z = v6;
  dictator.z.w = v7;
  dictator.z.y = v5 - UserData[20];
  dictator.z.z = v6 - UserData[21];
  dictator.z.w = v7 - UserData[22];
  user_rigid_body::setPosition((user_rigid_body *)LODWORD(pos.w), (const phys_mat44 *const)v3);
}

bool __cdecl Phys_ObjIsAsleep(int id)
{
  const char *v1; // eax
  float _p1[3]; // [esp+2Ch] [ebp-1Ch] BYREF
  float _p0[3]; // [esp+38h] [ebp-10h] BYREF
  rigid_body *body; // [esp+44h] [ebp-4h]

  body = Phys_GetUserData(id)->body;
  if ( phys_debugDangerousRigidBodies->current.enabled && rigid_body::is_dangerous(body) )
  {
    minspec_mutex::Lock(&g_render_mutex);
    Phys_NitrousVecToVec3(&body->m_last_position, _p0);
    _p1[0] = _p0[0];
    _p1[1] = _p0[1];
    _p1[2] = _p0[2] + 10000.0;
    CG_DebugLine(_p0, _p1, colorYellow, 1, 2000);
    v1 = va(
           "The rigid body at (%6.2f, %6.2f, %6.2f) is about to freakout! Check if it was improperly placed in solid.",
           _p0[0],
           _p0[1],
           _p0[2]);
    tlWarning(v1);
    minspec_mutex::Unlock(&g_render_mutex);
  }
  return rigid_body::is_group_stable(body) || rigid_body::is_dangerous(body) || body->m_mat.w.z < -10000.0;
}

unsigned int __thiscall rigid_body::is_dangerous(rigid_body *this)
{
  return this->m_flags & 0x80;
}

bool __cdecl Phys_ObjIsAsleepSingle(int id)
{
  const char *v1; // eax
  float _p1[3]; // [esp+2Ch] [ebp-1Ch] BYREF
  float _p0[3]; // [esp+38h] [ebp-10h] BYREF
  rigid_body *body; // [esp+44h] [ebp-4h]

  body = Phys_GetUserData(id)->body;
  if ( phys_debugDangerousRigidBodies->current.enabled && rigid_body::is_dangerous(body) )
  {
    minspec_mutex::Lock(&g_render_mutex);
    Phys_NitrousVecToVec3(&body->m_last_position, _p0);
    _p1[0] = _p0[0];
    _p1[1] = _p0[1];
    _p1[2] = _p0[2] + 10000.0;
    CG_DebugLine(_p0, _p1, colorYellow, 1, 2000);
    v1 = va(
           "The rigid body at (%6.2f, %6.2f, %6.2f) is about to freakout! Check if it was improperly placed in solid.",
           _p0[0],
           _p0[1],
           _p0[2]);
    tlWarning(v1);
    minspec_mutex::Unlock(&g_render_mutex);
  }
  return rigid_body::is_stable(body) || rigid_body::is_dangerous(body) || body->m_mat.w.z < -10000.0;
}

unsigned int __thiscall rigid_body::is_stable(rigid_body *this)
{
  return this->m_flags & 4;
}

void __cdecl Phys_SetHingeParams(rigid_body_constraint_ragdoll *id, float motorSpeed, float damp)
{
  if ( !id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 2900, 0, "%s", "id") )
    __debugbreak();
  rigid_body_constraint_ragdoll::set_damp_k(id, damp * phys_ragdoll_joint_damp_scale->current.value);
}

rigid_body_constraint_ragdoll * Phys_CreateHinge@<eax>(
        rigid_body_constraint_ragdoll *a1@<ebp>,
        int obj1,
        int obj2,
        const float *anchor,
        float *axis,
        float motorSpeed,
        float damp,
        float lowStop,
        float highStop)
{
  unsigned int v10[3]; // [esp+8h] [ebp-16Ch] BYREF
  phys_vec3 b2_ref_loc; // [esp+14h] [ebp-160h]
  unsigned int v12[3]; // [esp+28h] [ebp-14Ch] BYREF
  phys_vec3 b1_ref_loc; // [esp+34h] [ebp-140h]
  unsigned int v14[3]; // [esp+48h] [ebp-12Ch] BYREF
  const phys_vec3 *v15; // [esp+64h] [ebp-110h]
  phys_vec3 v16; // [esp+68h] [ebp-10Ch] BYREF
  unsigned int v17[3]; // [esp+78h] [ebp-FCh] BYREF
  const phys_vec3 *v18; // [esp+94h] [ebp-E0h]
  phys_vec3 v19; // [esp+98h] [ebp-DCh] BYREF
  unsigned int v20[3]; // [esp+A8h] [ebp-CCh] BYREF
  phys_vec3 *v21; // [esp+C4h] [ebp-B0h]
  phys_vec3 v22; // [esp+C8h] [ebp-ACh] BYREF
  unsigned int v23[3]; // [esp+D8h] [ebp-9Ch] BYREF
  phys_vec3 *v24; // [esp+F4h] [ebp-80h]
  phys_vec3 v25; // [esp+F8h] [ebp-7Ch] BYREF
  float v26; // [esp+108h] [ebp-6Ch]
  int v27; // [esp+10Ch] [ebp-68h]
  int v28; // [esp+110h] [ebp-64h]
  phys_vec3 ref_abs; // [esp+114h] [ebp-60h] BYREF
  phys_vec3 axis_abs; // [esp+124h] [ebp-50h]
  float v31; // [esp+134h] [ebp-40h]
  unsigned int v32[3]; // [esp+138h] [ebp-3Ch] BYREF
  phys_vec3 anchor_abs; // [esp+144h] [ebp-30h]
  int v34; // [esp+154h] [ebp-20h]
  int v35; // [esp+158h] [ebp-1Ch]
  rigid_body_constraint_ragdoll *rbc_ragdoll; // [esp+15Ch] [ebp-18h]
  environment_rigid_body *v37; // [esp+160h] [ebp-14h]
  environment_rigid_body *body; // [esp+164h] [ebp-10h]
  rigid_body_constraint_ragdoll *joint; // [esp+168h] [ebp-Ch] BYREF
  rigid_body *body2; // [esp+16Ch] [ebp-8h]
  rigid_body *retaddr; // [esp+174h] [ebp+0h]

  joint = a1;
  body2 = retaddr;
  body = (environment_rigid_body *)Phys_GetUserData(obj1)->body;
  v37 = (environment_rigid_body *)Phys_GetUserData(obj2)->body;
  rbc_ragdoll = phys_sys::create_rbc_ragdoll(body, v37, 1);
  if ( rbc_ragdoll )
  {
    v35 = *(unsigned int *)anchor;
    v34 = *((unsigned int *)anchor + 1);
    anchor_abs.w = anchor[2];
    v32[0] = v35;
    v32[1] = v34;
    v32[2] = LODWORD(anchor_abs.w);
    v31 = *axis;
    axis_abs.w = axis[1];
    axis_abs.z = axis[2];
    ref_abs.y = v31;
    ref_abs.z = axis_abs.w;
    ref_abs.w = axis_abs.z;
    v26 = 1.0f;
    v27 = 0;
    v28 = 0;
    v24 = phys_full_inv_multiply((int)&joint, &v25, &body->m_mat, (const phys_vec3 *)v32);
    v23[0] = LODWORD(v24->x);
    v23[1] = LODWORD(v24->y);
    v23[2] = LODWORD(v24->z);
    v21 = phys_full_inv_multiply((int)&joint, &v22, &v37->m_mat, (const phys_vec3 *)v32);
    v20[0] = LODWORD(v21->x);
    v20[1] = LODWORD(v21->y);
    v20[2] = LODWORD(v21->z);
    v18 = phys_inv_multiply(&v19, &body->m_mat, (phys_vec3 *)&ref_abs.y);
    v17[0] = LODWORD(v18->x);
    v17[1] = LODWORD(v18->y);
    v17[2] = LODWORD(v18->z);
    v15 = phys_inv_multiply(&v16, &v37->m_mat, (phys_vec3 *)&ref_abs.y);
    v14[0] = LODWORD(v15->x);
    v14[1] = LODWORD(v15->y);
    v14[2] = LODWORD(v15->z);
    b1_ref_loc.y = 1.0f;
    b1_ref_loc.z = 0.0f;
    b1_ref_loc.w = 0.0f;
    *(float *)v12 = 1.0f;
    v12[1] = 0;
    v12[2] = 0;
    b2_ref_loc.y = 1.0f;
    b2_ref_loc.z = 0.0f;
    b2_ref_loc.w = 0.0f;
    *(float *)v10 = 1.0f;
    v10[1] = 0;
    v10[2] = 0;
    rigid_body_constraint_ragdoll::set(rbc_ragdoll, (const phys_vec3 *)v23, (const phys_vec3 *)v20);
    rigid_body_constraint_ragdoll::set_hinge(
      rbc_ragdoll,
      (int)&joint,
      (const phys_vec3 *)v17,
      (const phys_vec3 *)v14,
      (const phys_vec3 *)v12,
      (const phys_vec3 *)v10,
      lowStop,
      highStop);
    rigid_body_constraint_ragdoll::set_damp_k(rbc_ragdoll, damp);
    return rbc_ragdoll;
  }
  else
  {
    Com_PrintWarning(20, "Physics: Out of hinge joints (%d max)\n", 256);
    return 0;
  }
}

void __cdecl Phys_SetAngularMotorParams(rigid_body_constraint_ragdoll *id, const float *motorSpeeds, float *damp)
{
  float v3; // [esp+4h] [ebp-Ch]

  if ( !id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 2946, 0, "%s", "id") )
    __debugbreak();
  if ( *damp <= damp[1] )
    v3 = damp[1];
  else
    v3 = *damp;
  rigid_body_constraint_ragdoll::set_damp_k(id, v3 * phys_ragdoll_joint_damp_scale->current.value);
}

rigid_body_constraint_ragdoll * Phys_CreateSwivel@<eax>(
        rigid_body_constraint_ragdoll *a1@<ebp>,
        int obj1,
        int obj2,
        const float *anchor,
        unsigned int numAxes,
        float (*axes)[3],
        const float *motorSpeeds,
        const float *motorFMaxs,
        float *lowStops,
        float *highStops)
{
  const phys_vec3 *v11; // eax
  const phys_vec3 *v12; // eax
  float v13; // xmm0_4
  float theta_max; // xmm0_4
  float v15; // xmm0_4
  float v16; // [esp+18h] [ebp-2BCh] BYREF
  float v17; // [esp+1Ch] [ebp-2B8h]
  float v18; // [esp+20h] [ebp-2B4h]
  float *p_y; // [esp+34h] [ebp-2A0h]
  _BYTE v20[12]; // [esp+38h] [ebp-29Ch] BYREF
  phys_mat44 axes_loc; // [esp+44h] [ebp-290h] BYREF
  phys_vec3 b1_ud_loc; // [esp+84h] [ebp-250h]
  float v23; // [esp+94h] [ebp-240h]
  float w; // [esp+98h] [ebp-23Ch] BYREF
  float angle_rotate; // [esp+9Ch] [ebp-238h]
  float angle_extents; // [esp+A0h] [ebp-234h]
  phys_vec3 b1_ud_abs; // [esp+A4h] [ebp-230h]
  int j; // [esp+B4h] [ebp-220h]
  unsigned int v29[2]; // [esp+B8h] [ebp-21Ch] BYREF
  int i; // [esp+C0h] [ebp-214h]
  int v31; // [esp+CCh] [ebp-208h]
  int v32; // [esp+D0h] [ebp-204h]
  int v33; // [esp+D4h] [ebp-200h]
  phys_vec3 v34; // [esp+D8h] [ebp-1FCh] BYREF
  int v35; // [esp+F4h] [ebp-1E0h]
  int v36; // [esp+F8h] [ebp-1DCh]
  int v37; // [esp+FCh] [ebp-1D8h]
  float v38; // [esp+100h] [ebp-1D4h]
  float v39; // [esp+104h] [ebp-1D0h]
  float theta_min; // [esp+108h] [ebp-1CCh]
  float max_angle; // [esp+10Ch] [ebp-1C8h]
  float v42; // [esp+110h] [ebp-1C4h]
  float min_angle; // [esp+114h] [ebp-1C0h]
  float v44; // [esp+118h] [ebp-1BCh]
  float v45; // [esp+11Ch] [ebp-1B8h]
  float limit; // [esp+120h] [ebp-1B4h]
  const phys_vec3 *v47; // [esp+134h] [ebp-1A0h]
  phys_vec3 v48; // [esp+138h] [ebp-19Ch] BYREF
  float x; // [esp+148h] [ebp-18Ch]
  float y; // [esp+14Ch] [ebp-188h]
  float z; // [esp+150h] [ebp-184h]
  const phys_vec3 *v52; // [esp+164h] [ebp-170h]
  phys_vec3 v53; // [esp+168h] [ebp-16Ch] BYREF
  unsigned int v54[3]; // [esp+178h] [ebp-15Ch] BYREF
  const phys_vec3 *v55; // [esp+194h] [ebp-140h]
  phys_vec3 v56; // [esp+198h] [ebp-13Ch] BYREF
  unsigned int v57[3]; // [esp+1A8h] [ebp-12Ch] BYREF
  const phys_vec3 *v58; // [esp+1C4h] [ebp-110h]
  phys_vec3 v59; // [esp+1C8h] [ebp-10Ch] BYREF
  unsigned int v60[3]; // [esp+1D8h] [ebp-FCh] BYREF
  phys_vec3 b2_anchor_loc; // [esp+1E4h] [ebp-F0h] BYREF
  phys_vec3 v62; // [esp+1F8h] [ebp-DCh] BYREF
  const phys_mat44 *v63; // [esp+214h] [ebp-C0h]
  unsigned int v64[3]; // [esp+218h] [ebp-BCh] BYREF
  phys_vec3 b1_anchor_loc; // [esp+224h] [ebp-B0h] BYREF
  phys_vec3 v66; // [esp+238h] [ebp-9Ch] BYREF
  const phys_mat44 *p_m_mat; // [esp+254h] [ebp-80h]
  unsigned int v68[3]; // [esp+258h] [ebp-7Ch] BYREF
  phys_vec3 ref_abs; // [esp+264h] [ebp-70h]
  float v70; // [esp+274h] [ebp-60h]
  unsigned int v71[3]; // [esp+278h] [ebp-5Ch] BYREF
  phys_vec3 axis_abs; // [esp+284h] [ebp-50h]
  float v73; // [esp+294h] [ebp-40h]
  unsigned int v74[3]; // [esp+298h] [ebp-3Ch] BYREF
  phys_vec3 anchor_abs; // [esp+2A4h] [ebp-30h]
  int v76; // [esp+2B4h] [ebp-20h]
  int v77; // [esp+2B8h] [ebp-1Ch]
  rigid_body_constraint_ragdoll *rbc_ragdoll; // [esp+2BCh] [ebp-18h]
  environment_rigid_body *v79; // [esp+2C0h] [ebp-14h]
  environment_rigid_body *body; // [esp+2C4h] [ebp-10h]
  rigid_body_constraint_ragdoll *joint; // [esp+2C8h] [ebp-Ch] BYREF
  rigid_body *body2; // [esp+2CCh] [ebp-8h]
  rigid_body *retaddr; // [esp+2D4h] [ebp+0h]

  joint = a1;
  body2 = retaddr;
  body = (environment_rigid_body *)Phys_GetUserData(obj1)->body;
  v79 = (environment_rigid_body *)Phys_GetUserData(obj2)->body;
  if ( numAxes >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
          2958,
          0,
          "%s",
          "numAxes >= 0 && numAxes <= 3") )
  {
    __debugbreak();
  }
  rbc_ragdoll = phys_sys::create_rbc_ragdoll(body, v79, 1);
  if ( rbc_ragdoll )
  {
    v77 = *(unsigned int *)anchor;
    v76 = *((unsigned int *)anchor + 1);
    anchor_abs.w = anchor[2];
    v74[0] = v77;
    v74[1] = v76;
    v74[2] = LODWORD(anchor_abs.w);
    v73 = (*axes)[0];
    axis_abs.w = (*axes)[1];
    axis_abs.z = (*axes)[2];
    *(float *)v71 = v73;
    v71[1] = LODWORD(axis_abs.w);
    v71[2] = LODWORD(axis_abs.z);
    v70 = (*axes)[3];
    ref_abs.w = (*axes)[4];
    ref_abs.z = (*axes)[5];
    *(float *)v68 = v70;
    v68[1] = LODWORD(ref_abs.w);
    v68[2] = LODWORD(ref_abs.z);
    p_m_mat = &body->m_mat;
    v11 = operator-(&v66, (const phys_vec3 *)v74, &body->m_mat.w);
    phys_inv_multiply((phys_vec3 *)&b1_anchor_loc.y, p_m_mat, v11);
    v64[0] = LODWORD(b1_anchor_loc.y);
    v64[1] = LODWORD(b1_anchor_loc.z);
    v64[2] = LODWORD(b1_anchor_loc.w);
    v63 = &v79->m_mat;
    v12 = operator-(&v62, (const phys_vec3 *)v74, &v79->m_mat.w);
    phys_inv_multiply((phys_vec3 *)&b2_anchor_loc.y, v63, v12);
    v60[0] = LODWORD(b2_anchor_loc.y);
    v60[1] = LODWORD(b2_anchor_loc.z);
    v60[2] = LODWORD(b2_anchor_loc.w);
    v58 = phys_inv_multiply(&v59, &body->m_mat, (const phys_vec3 *)v71);
    v57[0] = LODWORD(v58->x);
    v57[1] = LODWORD(v58->y);
    v57[2] = LODWORD(v58->z);
    v55 = phys_inv_multiply(&v56, &v79->m_mat, (const phys_vec3 *)v71);
    v54[0] = LODWORD(v55->x);
    v54[1] = LODWORD(v55->y);
    v54[2] = LODWORD(v55->z);
    v52 = phys_inv_multiply(&v53, &body->m_mat, (const phys_vec3 *)v68);
    x = v52->x;
    y = v52->y;
    z = v52->z;
    v47 = phys_inv_multiply(&v48, &v79->m_mat, (const phys_vec3 *)v68);
    v44 = v47->x;
    v45 = v47->y;
    limit = v47->z;
    rigid_body_constraint_ragdoll::set(rbc_ragdoll, (const phys_vec3 *)v64, (const phys_vec3 *)v60);
    min_angle = FLOAT_1_2;
    LODWORD(v42) = LODWORD(FLOAT_1_2) ^ _mask__NegFloat_;
    if ( *lowStops <= COERCE_FLOAT(LODWORD(FLOAT_1_2) ^ _mask__NegFloat_) )
      v13 = v42;
    else
      v13 = *lowStops;
    max_angle = v13;
    theta_min = v13;
    if ( min_angle <= *highStops )
      theta_max = min_angle;
    else
      theta_max = *highStops;
    v39 = theta_max;
    v38 = theta_max;
    v37 = LODWORD(v44) ^ _mask__NegFloat_;
    v36 = LODWORD(v45) ^ _mask__NegFloat_;
    v35 = LODWORD(limit) ^ _mask__NegFloat_;
    LODWORD(v34.x) = LODWORD(v44) ^ _mask__NegFloat_;
    LODWORD(v34.y) = LODWORD(v45) ^ _mask__NegFloat_;
    LODWORD(v34.z) = LODWORD(limit) ^ _mask__NegFloat_;
    v33 = LODWORD(x) ^ _mask__NegFloat_;
    v32 = LODWORD(y) ^ _mask__NegFloat_;
    v31 = LODWORD(z) ^ _mask__NegFloat_;
    v29[0] = LODWORD(x) ^ _mask__NegFloat_;
    v29[1] = LODWORD(y) ^ _mask__NegFloat_;
    i = LODWORD(z) ^ _mask__NegFloat_;
    rigid_body_constraint_ragdoll::set_swivel(
      rbc_ragdoll,
      (int)&joint,
      (const phys_vec3 *)v57,
      (const phys_vec3 *)v54,
      (const phys_vec3 *)v29,
      &v34,
      theta_min,
      theta_max);
    for ( j = 1; j < (int)numAxes; ++j )
    {
      b1_ud_abs.w = (*axes)[0];
      b1_ud_abs.z = (*axes)[1];
      b1_ud_abs.y = (*axes)[2];
      w = b1_ud_abs.w;
      angle_rotate = b1_ud_abs.z;
      angle_extents = b1_ud_abs.y;
      v23 = (float)(highStops[j] - lowStops[j]) * 0.5;
      b1_ud_loc.w = highStops[j] + lowStops[j];
      phys_inv_multiply((const phys_vec3 *)&axes_loc.w.y, &body->m_mat, (const phys_vec3 *)&w);
      Phys_AxisToNitrousMat(axes, (phys_mat44 *)v20);
      phys_inv_multiply_mat((int)&joint, (phys_mat44 *)v20, &body->m_mat, (const phys_mat44 *)v20);
      if ( j == 1 )
      {
        p_y = &axes_loc.x.y;
        v16 = axes_loc.x.y;
        v17 = axes_loc.x.z;
        v15 = axes_loc.x.w;
      }
      else
      {
        v16 = axes_loc.y.y;
        v17 = axes_loc.y.z;
        v15 = axes_loc.y.w;
      }
      v18 = v15;
      make_rotate((int)&joint, (phys_mat44 *)v20, (const phys_vec3 *)&v16, b1_ud_loc.w, 1000.0);
      rigid_body_constraint_ragdoll::add_joint_limit(rbc_ragdoll, (const phys_vec3 *)&axes_loc.w.y, v23);
    }
    return rbc_ragdoll;
  }
  else
  {
    Com_PrintWarning(20, "Physics: Out of hinge joints (%d max)\n", 256);
    return 0;
  }
}

// local variable allocation has failed, the output may be wrong!
void  phys_inv_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right)
{
  _BYTE v4[76]; // [esp-Ch] [ebp-ACh] OVERLAPPED BYREF
  const phys_vec3 *v5; // [esp+40h] [ebp-60h]
  phys_vec3 v6; // [esp+44h] [ebp-5Ch] BYREF
  phys_vec3 *p_y; // [esp+5Ch] [ebp-44h]
  const phys_vec3 *v8; // [esp+60h] [ebp-40h]
  phys_vec3 v9; // [esp+64h] [ebp-3Ch] BYREF
  phys_mat44 *v10; // [esp+7Ch] [ebp-24h]
  const phys_vec3 *v11; // [esp+80h] [ebp-20h]
  phys_vec3 v12; // [esp+84h] [ebp-1Ch] BYREF
  int v13; // [esp+94h] [ebp-Ch]
  void *v14; // [esp+98h] [ebp-8h]
  void *retaddr; // [esp+A0h] [ebp+0h]

  v13 = a1;
  v14 = retaddr;
  if ( dest == left )
  {
    memcpy(v4, left, 0x40u);
    phys_inv_multiply_mat(dest, (const phys_mat44 *)v4, right);
  }
  else
  {
    v11 = phys_inv_multiply(&v12, left, &right->x);
    v10 = dest;
    dest->x.x = v11->x;
    v10->x.y = v11->y;
    v10->x.z = v11->z;
    v8 = phys_inv_multiply(&v9, left, &right->y);
    p_y = &dest->y;
    dest->y.x = v8->x;
    p_y->y = v8->y;
    p_y->z = v8->z;
    v5 = phys_inv_multiply(&v6, left, &right->z);
    *(unsigned int *)&v4[72] = &dest->z;
    dest->z.x = v5->x;
    *(float *)(*(unsigned int *)&v4[72] + 4) = v5->y;
    *(float *)(*(unsigned int *)&v4[72] + 8) = v5->z;
  }
}

void __cdecl Phys_CalcPreset(PhysPreset *physPreset, float *dims, int surfaceType)
{
  float v3; // [esp+0h] [ebp-24h]
  float mass; // [esp+10h] [ebp-14h]
  float v5; // [esp+14h] [ebp-10h]
  float size; // [esp+18h] [ebp-Ch]
  bool floats; // [esp+1Fh] [ebp-5h]
  float density; // [esp+20h] [ebp-4h]

  if ( !physPreset
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 3019, 0, "%s", "physPreset") )
  {
    __debugbreak();
  }
  density = Com_SurfaceDensity(surfaceType);
  size = (float)(*dims + dims[1]) + dims[2];
  floats = Com_SurfaceFloats(surfaceType);
  if ( physPreset->bounce == 0.0 )
    physPreset->bounce = 0.25f;
  if ( physPreset->friction == 0.0 )
    physPreset->friction = 1.0f;
  physPreset->bulletForceScale = 1.0f;
  physPreset->explosiveForceScale = 1.0f;
  physPreset->canFloat = floats;
  if ( surfaceType == 8 )
    physPreset->gravityScale = 0.3f;
  else
    physPreset->gravityScale = 1.0f;
  if ( physPreset->mass == 0.0 )
  {
    physPreset->mass = size * density;
    mass = physPreset->mass;
    if ( (float)(mass - 1000.0) < 0.0 )
      v5 = physPreset->mass;
    else
      v5 = 1000.0f;
    if ( (float)(1.0 - mass) < 0.0 )
      v3 = v5;
    else
      v3 = 1.0f;
    physPreset->mass = v3;
  }
  physPreset->mass = physPreset->mass * 0.001;
  physPreset->buoyancyBoxMin[0] = 0.0f;
  physPreset->buoyancyBoxMin[1] = 0.0f;
  physPreset->buoyancyBoxMin[2] = 0.0f;
  physPreset->buoyancyBoxMax[0] = 0.0f;
  physPreset->buoyancyBoxMax[1] = 0.0f;
  physPreset->buoyancyBoxMax[2] = 0.0f;
  physPreset->centerOfMassOffset[0] = 0.0f;
  physPreset->centerOfMassOffset[1] = 0.0f;
  physPreset->centerOfMassOffset[2] = 0.0f;
  physPreset->flags |= 2u;
}

int __cdecl Phys_GetCurrentTime()
{
  return physGlob.timeLastUpdate;
}

PhysObjUserData *__thiscall phys_free_list<PhysObjUserData>::add(
        phys_free_list<PhysObjUserData> *this,
        int no_error,
        const char *error_msg)
{
  phys_free_list<PhysObjUserData>::T_internal *ptr; // [esp+28h] [ebp-4h]

  ptr = (phys_free_list<PhysObjUserData>::T_internal *)PMM_ALLOC(0x150u, 0x10u);
  if ( ptr )
  {
    ptr->m_data.m_gjk_geom_list.m_first_geom = 0;
    ptr->m_data.m_gjk_geom_list.m_geom_count = 0;
    ptr->m_data.m_bpb = 0;
    ptr->m_prev_T_internal = &this->m_dummy_head;
    ptr->m_next_T_internal = this->m_dummy_head.m_next_T_internal;
    this->m_dummy_head.m_next_T_internal->m_prev_T_internal = ptr;
    this->m_dummy_head.m_next_T_internal = ptr;
    ++this->m_list_count;
    phys_free_list<PhysObjUserData>::debug_add(this, ptr);
    return &ptr->m_data;
  }
  else
  {
    if ( !no_error )
    {
      if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 470, "no_error", error_msg) )
        __debugbreak();
    }
    return 0;
  }
}

void __thiscall phys_free_list<PhysObjUserData>::remove(phys_free_list<PhysObjUserData> *this, PhysObjUserData *data_)
{
  if ( data_ )
  {
    PMM_VALIDATE((char *)data_[-1].centerOfMassOffset, 0x150u, 0x10u);
    phys_free_list<PhysObjUserData>::remove(
      this,
      (phys_free_list<PhysObjUserData>::T_internal *)data_[-1].centerOfMassOffset);
  }
}

void __thiscall phys_link_list1<PhysObjUserData>::add(phys_link_list1<PhysObjUserData> *this, PhysObjUserData *p)
{
  PhysObjUserData *i; // [esp+Ch] [ebp-4h]

  for ( i = this->m_first; i; i = i->m_next_link )
  {
    if ( i == p )
    {
      if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h", 135, "i != p", &toastPopupTitle) )
        __debugbreak();
    }
  }
  ++this->m_alloc_count;
  if ( this->m_last )
    this->m_last->m_next_link = p;
  else
    this->m_first = p;
  this->m_last = p;
  this->m_last->m_next_link = 0;
}

void __thiscall phys_link_list1<PhysObjUserData>::remove(phys_link_list1<PhysObjUserData> *this, PhysObjUserData *p)
{
  PhysObjUserData *i; // [esp+10h] [ebp-8h]
  PhysObjUserData *last_i; // [esp+14h] [ebp-4h]

  i = this->m_first;
  last_i = 0;
  while ( i )
  {
    if ( p == i )
    {
      --this->m_alloc_count;
      if ( last_i )
        last_i->m_next_link = i->m_next_link;
      else
        this->m_first = i->m_next_link;
      if ( i == this->m_last )
      {
        this->m_last = last_i;
        if ( last_i )
        {
          if ( last_i->m_next_link )
          {
            if ( _tlAssert(
                   "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h",
                   160,
                   "!last_i || last_i->get_next_link() == NULL",
                   &toastPopupTitle) )
            {
              __debugbreak();
            }
          }
        }
      }
      return;
    }
    last_i = i;
    i = i->m_next_link;
  }
  if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_local.h", 165, "0", &toastPopupTitle) )
    __debugbreak();
}

phys_vec3 *__thiscall phys_static_array<phys_vec3,6144>::add(
        phys_static_array<phys_vec3,6144> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 6144 )
  {
    return &this->m_slot_array[this->m_alloc_count++];
  }
  else
  {
    if ( !no_error )
      tlFatal(error_msg);
    return 0;
  }
}

phys_vec3 *__thiscall phys_static_array<phys_vec3,6144>::operator[](phys_static_array<phys_vec3,6144> *this, int i)
{
  if ( i < 0 || i >= this->m_alloc_count )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
           118,
           "i >= 0 && i < m_alloc_count",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return &this->m_slot_array[i];
}

phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::add(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 256 )
  {
    return &this->m_slot_array[this->m_alloc_count++];
  }
  else
  {
    if ( !no_error )
      tlFatal(error_msg);
    return 0;
  }
}

void __thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::remove_slow(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this,
        phys_convex_hull::ch_triangle *data)
{
  bool v3; // [esp+Bh] [ebp-1h]

  if ( (unsigned int)((char *)data - (char *)this->m_slot_array) % 0x20 )
    v3 = 0;
  else
    v3 = data >= this->m_slot_array && data < &this->m_slot_array[this->m_alloc_count];
  if ( !v3
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
         73,
         "is_member(data)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  phys_convex_hull::ch_triangle::operator=(data, &this->m_slot_array[--this->m_alloc_count]);
}

phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::get_list_head(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this)
{
  if ( this->m_alloc_count <= 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
         131,
         "m_alloc_count > 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return this->m_slot_array;
}

void __thiscall phys_static_array<phys_convex_hull::ch_edge,128>::remove_slow(
        phys_static_array<phys_convex_hull::ch_edge,128> *this,
        phys_convex_hull::ch_edge *data)
{
  int v2; // eax
  phys_convex_hull::ch_edge *m_slot_array; // edx
  phys_vec3 *v4; // ecx
  phys_vec3 *v5; // edx
  bool v7; // [esp+7h] [ebp-1h]

  if ( (unsigned int)((char *)data - (char *)this->m_slot_array) % 8 )
    v7 = 0;
  else
    v7 = data >= this->m_slot_array && data < &this->m_slot_array[this->m_alloc_count];
  if ( !v7
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
         73,
         "is_member(data)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v2 = --this->m_alloc_count;
  m_slot_array = this->m_slot_array;
  v4 = m_slot_array[v2].m_verts[0];
  v5 = m_slot_array[v2].m_verts[1];
  data->m_verts[0] = v4;
  data->m_verts[1] = v5;
}

phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,64>::add(
        phys_static_array<phys_convex_hull::ch_triangle,64> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 64 )
  {
    return &this->m_slot_array[this->m_alloc_count++];
  }
  else
  {
    if ( !no_error )
      tlFatal(error_msg);
    return 0;
  }
}

void __thiscall phys_free_list<PhysObjUserData>::debug_add(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *T_i)
{
  int m_list_count; // [esp+0h] [ebp-10h]

  if ( this->m_list_count_high_water <= this->m_list_count )
    m_list_count = this->m_list_count;
  else
    m_list_count = this->m_list_count_high_water;
  this->m_list_count_high_water = m_list_count;
  if ( this->m_ptr_list_count >= 256 )
  {
    T_i->m_ptr_list_index = -1;
  }
  else
  {
    T_i->m_ptr_list_index = this->m_ptr_list_count;
    this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
  }
}

void __thiscall phys_free_list<PhysObjUserData>::remove(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *data)
{
  phys_free_list<PhysObjUserData>::T_internal_base *next; // [esp+14h] [ebp-8h]
  phys_free_list<PhysObjUserData>::T_internal_base *prev; // [esp+18h] [ebp-4h]

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<PhysObjUserData>::debug_remove(this, data);
  next = data->m_next_T_internal;
  prev = data->m_prev_T_internal;
  prev->m_next_T_internal = next;
  next->m_prev_T_internal = prev;
  PMM_FREE((unsigned __int8 *)data, 0x150u, 0x10u);
}

void __thiscall phys_free_list<PhysObjUserData>::debug_remove(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *T_i)
{
  if ( T_i->m_ptr_list_index != -1 )
  {
    if ( T_i->m_ptr_list_index >= 0x100u
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    this->m_ptr_list[--this->m_ptr_list_count][1].body = (rigid_body *)T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

cdl_proftimer *__thiscall cdl_proftimer::cdl_proftimer(cdl_proftimer *this)
{
  int i; // [esp+4h] [ebp-4h]

  this->value = 0;
  this->avr = 0;
  this->capture_count = 0;
  for ( i = 0; i < 5; ++i )
  {
    LODWORD(this->mx[i]) = 0;
    HIDWORD(this->mx[i]) = 0;
  }
  this->capture = 0;
  return this;
}

PhysGlob *__thiscall PhysGlob::PhysGlob(PhysGlob *this)
{
  int v2; // [esp+4h] [ebp-8h]
  phys_link_list1<PhysObjUserData> *i; // [esp+8h] [ebp-4h]

  this->objects.m_dummy_head.m_next_T_internal = &this->objects.m_dummy_head;
  this->objects.m_dummy_head.m_prev_T_internal = &this->objects.m_dummy_head;
  this->objects.m_list_count = 0;
  this->objects.m_list_count_high_water = 0;
  this->objects.m_ptr_list_count = 0;
  v2 = 3;
  for ( i = this->objects_by_world; --v2 >= 0; ++i )
  {
    i->m_first = 0;
    i->m_last = 0;
    i->m_alloc_count = 0;
  }
  return this;
}

phys_convex_hull *__thiscall phys_convex_hull::phys_convex_hull(phys_convex_hull *this)
{
  this->m_vertex_buffer.m_slot_array = (phys_vec3 *const)this;
  this->m_vertex_buffer.m_alloc_count = 0;
  this->m_intermediate_vertex_list.m_slot_array = (phys_vec3 **const)&this->m_intermediate_vertex_list;
  this->m_intermediate_vertex_list.m_alloc_count = 0;
  this->m_intermediate_triangle_list.m_slot_array = (phys_convex_hull::ch_triangle *const)&this->m_intermediate_triangle_list;
  this->m_intermediate_triangle_list.m_alloc_count = 0;
  this->m_intermediate_edge_list.m_slot_array = (phys_convex_hull::ch_edge *const)&this->m_intermediate_edge_list;
  this->m_intermediate_edge_list.m_alloc_count = 0;
  this->m_convex_hull_vert_list.m_slot_array = (phys_vec3 **const)&this->m_convex_hull_vert_list;
  this->m_convex_hull_vert_list.m_alloc_count = 0;
  this->m_convex_hull_triangle_list.m_slot_array = (phys_convex_hull::ch_triangle *const)&this->m_convex_hull_triangle_list;
  this->m_convex_hull_triangle_list.m_alloc_count = 0;
  return this;
}

void __thiscall phys_free_list<PhysObjUserData>::~phys_free_list<PhysObjUserData>(
        phys_free_list<PhysObjUserData> *this)
{
  while ( (phys_free_list<PhysObjUserData> *)this->m_dummy_head.m_next_T_internal != this )
    phys_free_list<PhysObjUserData>::remove(
      this,
      (phys_free_list<PhysObjUserData>::T_internal *)this->m_dummy_head.m_next_T_internal);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<PhysObjUserData> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<PhysObjUserData> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<VehicleParameter>::~phys_free_list<VehicleParameter>(
        phys_free_list<VehicleParameter> *this)
{
  while ( (phys_free_list<VehicleParameter> *)this->m_dummy_head.m_next_T_internal != this )
    phys_free_list<VehicleParameter>::remove(
      this,
      (phys_free_list<VehicleParameter>::T_internal *)this->m_dummy_head.m_next_T_internal);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<VehicleParameter> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<VehicleParameter> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<VehicleParameter>::remove(
        phys_free_list<VehicleParameter> *this,
        phys_free_list<VehicleParameter>::T_internal *data)
{
  phys_free_list<VehicleParameter>::T_internal_base *next; // [esp+14h] [ebp-8h]
  phys_free_list<VehicleParameter>::T_internal_base *prev; // [esp+18h] [ebp-4h]

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<VehicleParameter>::debug_remove(this, data);
  next = data->m_next_T_internal;
  prev = data->m_prev_T_internal;
  prev->m_next_T_internal = next;
  next->m_prev_T_internal = prev;
  PMM_FREE((unsigned __int8 *)data, 0x130u, 4u);
}

void __thiscall phys_free_list<VehicleParameter>::debug_remove(
        phys_free_list<VehicleParameter> *this,
        phys_free_list<VehicleParameter>::T_internal *T_i)
{
  if ( T_i->m_ptr_list_index != -1 )
  {
    if ( T_i->m_ptr_list_index >= 0x100u
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    LODWORD(this->m_ptr_list[--this->m_ptr_list_count][1].m_speed_max) = T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void __thiscall phys_free_list<NitrousVehicle>::~phys_free_list<NitrousVehicle>(phys_free_list<NitrousVehicle> *this)
{
  while ( (phys_free_list<NitrousVehicle> *)this->m_dummy_head.m_next_T_internal != this )
    phys_free_list<NitrousVehicle>::remove(
      this,
      (phys_free_list<NitrousVehicle>::T_internal *)this->m_dummy_head.m_next_T_internal);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<NitrousVehicle> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<NitrousVehicle> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<RagdollBody>::~phys_free_list<RagdollBody>(phys_free_list<RagdollBody> *this)
{
  while ( (phys_free_list<RagdollBody> *)this->m_dummy_head.m_next_T_internal != this )
    phys_free_list<RagdollBody>::remove(
      this,
      (phys_free_list<RagdollBody>::T_internal *)this->m_dummy_head.m_next_T_internal);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<RagdollBody> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<RagdollBody> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<RagdollBody>::remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *data)
{
  phys_free_list<RagdollBody>::T_internal_base *next; // [esp+14h] [ebp-8h]
  phys_free_list<RagdollBody>::T_internal_base *prev; // [esp+18h] [ebp-4h]

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<RagdollBody>::debug_remove(this, data);
  next = data->m_next_T_internal;
  prev = data->m_prev_T_internal;
  prev->m_next_T_internal = next;
  next->m_prev_T_internal = prev;
  PMM_FREE((unsigned __int8 *)data, 0xA34u, 4u);
}

void __thiscall phys_free_list<RagdollBody>::debug_remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *T_i)
{
  if ( T_i->m_ptr_list_index != -1 )
  {
    if ( T_i->m_ptr_list_index >= 0x100u
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    this->m_ptr_list[--this->m_ptr_list_count][1].references = T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void __thiscall phys_convex_hull::~phys_convex_hull(phys_convex_hull *this)
{
  int ii; // [esp+4h] [ebp-18h]
  int n; // [esp+8h] [ebp-14h]
  int m; // [esp+Ch] [ebp-10h]
  int k; // [esp+10h] [ebp-Ch]
  int j; // [esp+14h] [ebp-8h]
  int i; // [esp+18h] [ebp-4h]

  for ( i = 0; i < this->m_convex_hull_triangle_list.m_alloc_count; ++i )
    ;
  for ( j = 0; j < this->m_convex_hull_vert_list.m_alloc_count; ++j )
    ;
  for ( k = 0; k < this->m_intermediate_edge_list.m_alloc_count; ++k )
    ;
  for ( m = 0; m < this->m_intermediate_triangle_list.m_alloc_count; ++m )
    ;
  for ( n = 0; n < this->m_intermediate_vertex_list.m_alloc_count; ++n )
    ;
  for ( ii = 0; ii < this->m_vertex_buffer.m_alloc_count; ++ii )
    ;
}

void __thiscall phys_free_list<RagdollBody>::remove(phys_free_list<RagdollBody> *this, RagdollBody *data_)
{
  if ( data_ )
  {
    PMM_VALIDATE((char *)&data_[-1].rope_id, 0xA34u, 4u);
    phys_free_list<RagdollBody>::remove(this, (phys_free_list<RagdollBody>::T_internal *)&data_[-1].rope_id);
  }
}

broad_phase_info *__cdecl create_broad_phase_info()
{
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // edi
  phys_free_list<broad_phase_info>::T_internal *v1; // eax
  phys_free_list<broad_phase_info>::T_internal *v2; // esi

  p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
  v1 = (phys_free_list<broad_phase_info>::T_internal *)PMM_ALLOC(0x90u, 0x10u);
  v2 = v1;
  if ( v1 )
  {
    v1->m_prev_T_internal = &p_g_list_broad_phase_info->m_dummy_head;
    v1->m_next_T_internal = p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal;
    p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal->m_prev_T_internal = v1;
    ++p_g_list_broad_phase_info->m_list_count;
    p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal = v1;
    phys_free_list<broad_phase_info>::debug_add(p_g_list_broad_phase_info, v1);
    return &v2->m_data;
  }
  else
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           470,
           "no_error",
           "phys_free_list error: out of memory.") )
    {
      __debugbreak();
    }
    return 0;
  }
}

void __cdecl destroy_broad_phase_info(broad_phase_info *bpi)
{
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // edi

  environment_collision_list_remove(bpi);
  axis_aligned_sweep_and_prune::destroy_sap_node(g_axis_aligned_sweep_and_prune, bpi);
  p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
  if ( bpi )
  {
    PMM_VALIDATE((char *)&bpi[-1].m_gjk_geom, 0x90u, 0x10u);
    phys_free_list<broad_phase_info>::remove(
      p_g_list_broad_phase_info,
      (phys_free_list<broad_phase_info>::T_internal *)&bpi[-1].m_gjk_geom);
  }
}

