#include "nitrous_vehicle_constraint.h"
#include <cgame/cg_drawtools.h>
#include <universal/com_math_anglevectors.h>
#include <physics/physics_system.h>
#include <game/g_scr_vehicle.h>
#include <physics/phys_main.h>

// Decomp: CoDMPServer.c:659635  (RVA 0082A290)
void    path_constraint_update(rigid_body_constraint_custom_path *vpc, gentity_s *veh)
{
    float axis[9];
    phys_mat44 *p_m_path_mat;
    scr_vehicle_s *scr_vehicle;
    if ( vpc && veh && veh->scr_vehicle && veh->scr_vehicle->nitrousVehicle )
    {
        scr_vehicle = veh->scr_vehicle;
        p_m_path_mat = &vpc->m_path_mat;
        AnglesToAxis(scr_vehicle->pathPos.angles, (float (*)[3])axis);
        Phys_AxisToNitrousMat((float (*)[3])axis, p_m_path_mat);
        Phys_Vec3ToNitrousVec(scr_vehicle->pathPos.origin, &p_m_path_mat->w);
        if ( ((LODWORD(scr_vehicle->pathPos.origin[0]) & 0x7F800000) == 0x7F800000
             || (LODWORD(scr_vehicle->pathPos.origin[1]) & 0x7F800000) == 0x7F800000
             || (LODWORD(scr_vehicle->pathPos.origin[2]) & 0x7F800000) == 0x7F800000)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                        22,
                        0,
                        "%s",
                        "!IS_NAN((vpp.origin)[0]) && !IS_NAN((vpp.origin)[1]) && !IS_NAN((vpp.origin)[2])") )
        {
            __debugbreak();
        }
        if ( ((LODWORD(scr_vehicle->pathPos.lookPos[0]) & 0x7F800000) == 0x7F800000
             || (LODWORD(scr_vehicle->pathPos.lookPos[1]) & 0x7F800000) == 0x7F800000
             || (LODWORD(scr_vehicle->pathPos.lookPos[2]) & 0x7F800000) == 0x7F800000)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                        23,
                        0,
                        "%s",
                        "!IS_NAN((vpp.lookPos)[0]) && !IS_NAN((vpp.lookPos)[1]) && !IS_NAN((vpp.lookPos)[2])") )
        {
            __debugbreak();
        }
        if ( phys_drawNitrousVehicle->current.enabled )
            CG_DebugSphere(scr_vehicle->pathPos.origin, 15.0, colorGreen, 10, 0, 1);
        vpc->b1_r_loc.x = 0.0f;
        vpc->b1_r_loc.y = 0.0f;
        vpc->b1_r_loc.z = 0.0f;
        if ( veh->scr_vehicle->nitrousVehicle->m_num_colliding_wheels >= 3
            || veh->scr_vehicle->nitrousVehicle->m_vehicle_info->type == 4 )
        {
            vpc->b1_r_loc.z = -6.0f;
        }
    }
}

// Decomp: CoDMPServer.c:659698  (RVA 0082A520)
// Notes:   IDA casts m_phys_user_data to rigid_body_constraint* to read b1; offset 0 is body (CoDMPServer.h PhysObjUserData::body == rigid_body_constraint::b1)
rigid_body_constraint_custom_path * path_constraint_create(gentity_s *veh)
{
    float axis[9];
    phys_mat44 *p_m_path_mat;
    phys_vec3 *p_b1_r_loc;
    float b1_r_loc_y;
    float b1_r_loc_z;
    rigid_body_constraint_custom_path *path_constraint;
    user_rigid_body *user_rb;
    environment_rigid_body *b1;
    PhysObjUserData *phys_user_data;
    rigid_body *rb;
    if ( !veh
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 38,
                 "veh",
                 "") )
    {
        __debugbreak();
    }
    if ( !veh->scr_vehicle
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 39,
                 "veh->scr_vehicle",
                 "") )
    {
        __debugbreak();
    }
    if ( !veh->scr_vehicle->nitrousVehicle
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 40,
                 "veh->scr_vehicle->nitrousVehicle",
                 "") )
    {
        __debugbreak();
    }
    phys_user_data = veh->scr_vehicle->nitrousVehicle->m_phys_user_data;
    if ( !phys_user_data
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 41,
                 "veh->scr_vehicle->nitrousVehicle->get_phys_user_data()",
                 "") )
    {
        __debugbreak();
    }
    rb = phys_user_data->body;
    if ( !rb
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 42,
                 "veh->scr_vehicle->nitrousVehicle->get_phys_user_data()->body",
                 "") )
    {
        __debugbreak();
    }
    b1 = (environment_rigid_body *)rb;
    user_rb = phys_sys::create_user_rigid_body(0);
    user_rb->set(0);
    path_constraint = phys_sys::create_rbc_custom_path(b1, (environment_rigid_body *)user_rb, 0);
    if ( !path_constraint
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 49,
                 "rbc_custom_path",
                 "") )
    {
        __debugbreak();
    }
    b1_r_loc_y = 0.0f;
    b1_r_loc_z = 0.0f;
    p_b1_r_loc = &path_constraint->b1_r_loc;
    path_constraint->b1_r_loc.x = 0.0f;
    p_b1_r_loc->y = b1_r_loc_y;
    p_b1_r_loc->z = b1_r_loc_z;
    p_m_path_mat = &path_constraint->m_path_mat;
    AnglesToAxis(veh->r.currentAngles, (float (*)[3])axis);
    Phys_AxisToNitrousMat((float (*)[3])axis, p_m_path_mat);
    Phys_Vec3ToNitrousVec(veh->r.currentOrigin, &p_m_path_mat->w);
    path_constraint->m_urb = user_rb;
    //user_rb::set(path_constraint->m_urb, &path_constraint->m_path_mat);
    path_constraint->m_urb->set(&path_constraint->m_path_mat);
    return path_constraint;
}

// Decomp: CoDMPServer.c:659797  (RVA 0082A780)
void __cdecl path_constraint_destroy(rigid_body_constraint_custom_path *vpc)
{
    if ( !vpc->m_urb
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 60,
                 "vpc->m_urb",
                 "") )
    {
        __debugbreak();
    }
    if ( vpc->b2 != vpc->m_urb
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_constraint.cpp",
                 61,
                 "vpc->get_b2() == vpc->m_urb",
                 "") )
    {
        __debugbreak();
    }
    phys_sys::destroy(vpc->m_urb);
}
