#include "phys_render.h"

float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i)
{
  if ( i > 2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
         34,
         "i >= 0 && i < 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return (float *)this + i;
}

void  make_rotate(
        int a1@<ebp>,
        phys_mat44 *mat,
        const phys_vec3 *v,
        float theta_factor,
        float max_rotation_radians)
{
  long double x; // [esp+4h] [ebp-50h]
  float xa; // [esp+4h] [ebp-50h]
  long double x_4; // [esp+8h] [ebp-4Ch]
  unsigned int v8[3]; // [esp+18h] [ebp-3Ch] BYREF
  float v9; // [esp+34h] [ebp-20h]
  float v10; // [esp+38h] [ebp-1Ch]
  float v11; // [esp+3Ch] [ebp-18h]
  float v12; // [esp+40h] [ebp-14h]
  float ca; // [esp+44h] [ebp-10h]
  int v14; // [esp+48h] [ebp-Ch]
  void *v15; // [esp+4Ch] [ebp-8h]
  void *retaddr; // [esp+54h] [ebp+0h]

  v14 = a1;
  v15 = retaddr;
  ca = Abs(&v->x);
  if ( ca >= 0.0000099999997 )
  {
    v12 = 1.0 / ca;
    v11 = (float)(1.0 / ca) * v->x;
    v10 = (float)(1.0 / ca) * v->y;
    v9 = (float)(1.0 / ca) * v->z;
    *(float *)v8 = v11;
    *(float *)&v8[1] = v10;
    *(float *)&v8[2] = v9;
    ca = ca * theta_factor;
    if ( ca > max_rotation_radians )
      ca = max_rotation_radians;
    __libm_sse2_sin(x_4);
    *(float *)&x = ca;
    __libm_sse2_cos(x);
    make_rotate(mat, (const phys_vec3 *)v8, ca, xa);
  }
  else
  {
    phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_34);
  }
}

void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa)
{
  float yy; // [esp+1Ch] [ebp-28h]
  float xsa; // [esp+20h] [ebp-24h]
  float xy; // [esp+24h] [ebp-20h]
  float zsa; // [esp+28h] [ebp-1Ch]
  float zz; // [esp+2Ch] [ebp-18h]
  float ysa; // [esp+34h] [ebp-10h]
  float xz; // [esp+38h] [ebp-Ch]
  float yz; // [esp+3Ch] [ebp-8h]

  xy = (float)(u->x * u->y) * (float)(1.0 - ca);
  xz = (float)(u->x * u->z) * (float)(1.0 - ca);
  yy = (float)(u->y * u->y) * (float)(1.0 - ca);
  yz = (float)(u->y * u->z) * (float)(1.0 - ca);
  zz = (float)(u->z * u->z) * (float)(1.0 - ca);
  xsa = u->x * sa;
  ysa = u->y * sa;
  zsa = u->z * sa;
  m->x.x = (float)((float)(u->x * u->x) * (float)(1.0 - ca)) + ca;
  m->y.x = xy - zsa;
  m->z.x = xz + ysa;
  m->w.x = *(float *)&FLOAT_0_0;
  *phys_vec3::operator[]<int>(&m->x, 1u) = xy + zsa;
  *phys_vec3::operator[]<int>(&m->y, 1u) = yy + ca;
  *phys_vec3::operator[]<int>(&m->z, 1u) = yz - xsa;
  *phys_vec3::operator[]<int>(&m->w, 1u) = *(float *)&FLOAT_0_0;
  *phys_vec3::operator[]<int>(&m->x, 2u) = xz - ysa;
  *phys_vec3::operator[]<int>(&m->y, 2u) = yz + xsa;
  *phys_vec3::operator[]<int>(&m->z, 2u) = zz + ca;
  *phys_vec3::operator[]<int>(&m->w, 2u) = *(float *)&FLOAT_0_0;
  phys_mat44::fix_w_column(m);
}

void __cdecl DebugPatchesAndBrushesProlog()
{
  Sys_EnterCriticalSection(CRTISECT_DEBUG_BRUSHES_AND_PATCHES);
}

void __cdecl DebugPatchesAndBrushesEpilog()
{
  Sys_LeaveCriticalSection(CRTISECT_DEBUG_BRUSHES_AND_PATCHES);
}

void __cdecl clear_debug_brushes_and_patches()
{
  debugCollisionAabbTreeCount = 0;
  if ( !debug_brush_info )
  {
    R_DebugAlloc((void **)&debug_brush_info, 490012, "(debug_brush_info)");
    if ( !debug_brush_info )
    {
      Com_Printf(0, "Could not allocate memory for debug_brush_info\n");
      return;
    }
    memset((unsigned __int8 *)debug_brush_info, 0, sizeof(debug_brush_info_t));
  }
  debug_brush_info->num_brushes = 0;
  debug_brush_info->num_windings = 0;
  debug_brush_info->num_indices = 0;
  if ( !debug_patch_info )
  {
    R_DebugAlloc((void **)&debug_patch_info, 460008, "(debug_patch_info)");
    if ( !debug_patch_info )
    {
      Com_Printf(0, "Could not allocate memory for debug_patch_info\n");
      return;
    }
    memset((unsigned __int8 *)debug_patch_info, 0, sizeof(debug_patch_info_t));
  }
  debug_patch_info->num_patches = 0;
  debug_patch_info->num_indices = 0;
}

void __cdecl add_debug_brush(const cbrush_t *brush, const phys_mat44 *mat)
{
  int savedregs; // [esp+6224h] [ebp+0h] BYREF

  if ( debug_brush_info )
    debug_brush_info_t::add_brush(debug_brush_info, (int)&savedregs, brush, mat);
}

void __userpurge debug_brush_info_t::add_brush(
        debug_brush_info_t *this@<ecx>,
        int a2@<ebp>,
        const cbrush_t *brush,
        const phys_mat44 *mat)
{
  void *v4; // esp
  phys_vec3 *v5; // eax
  int jj; // [esp-62E0h] [ebp-62ECh]
  int ii; // [esp-62DCh] [ebp-62E8h]
  unsigned int index_in_brush; // [esp-62D8h] [ebp-62E4h]
  float v9[3]; // [esp-62D4h] [ebp-62E0h] BYREF
  int n; // [esp-62C8h] [ebp-62D4h]
  int m; // [esp-62C4h] [ebp-62D0h]
  int k; // [esp-62C0h] [ebp-62CCh]
  unsigned __int8 v13; // [esp-62B9h] [ebp-62C5h]
  int v14; // [esp-62B8h] [ebp-62C4h]
  int v15; // [esp-62B4h] [ebp-62C0h]
  phys_static_array<phys_vec3,512> v16; // [esp-62B0h] [ebp-62BCh] BYREF
  unsigned int j; // [esp-42A0h] [ebp-42ACh]
  int num_windings; // [esp-429Ch] [ebp-42A8h]
  int kk; // [esp-4298h] [ebp-42A4h]
  debug_brush_t *v20; // [esp-4294h] [ebp-42A0h]
  unsigned __int16 v21; // [esp-4290h] [ebp-429Ch]
  int v22; // [esp-428Ch] [ebp-4298h]
  int m_alloc_count; // [esp-4288h] [ebp-4294h]
  plane_lt *v24; // [esp-4284h] [ebp-4290h]
  phys_vec3 v25; // [esp-4280h] [ebp-428Ch] BYREF
  float v26; // [esp-4270h] [ebp-427Ch]
  float z; // [esp-4254h] [ebp-4260h]
  int v28; // [esp-4250h] [ebp-425Ch]
  int v29; // [esp-424Ch] [ebp-4258h]
  float v30; // [esp-4248h] [ebp-4254h]
  float v31; // [esp-4244h] [ebp-4250h]
  plane_lt *v32; // [esp-4234h] [ebp-4240h]
  phys_vec3 v33; // [esp-4230h] [ebp-423Ch] BYREF
  float v34; // [esp-4220h] [ebp-422Ch]
  float y; // [esp-4204h] [ebp-4210h]
  int v36; // [esp-4200h] [ebp-420Ch]
  float v37; // [esp-41FCh] [ebp-4208h]
  int v38; // [esp-41F8h] [ebp-4204h]
  float v39; // [esp-41F4h] [ebp-4200h]
  plane_lt *v40; // [esp-41E4h] [ebp-41F0h]
  phys_vec3 v41; // [esp-41E0h] [ebp-41ECh] BYREF
  float v42; // [esp-41D0h] [ebp-41DCh]
  float x; // [esp-41B4h] [ebp-41C0h]
  float v44; // [esp-41B0h] [ebp-41BCh]
  int v45; // [esp-41ACh] [ebp-41B8h]
  int v46; // [esp-41A8h] [ebp-41B4h]
  float v47; // [esp-41A4h] [ebp-41B0h]
  plane_lt *v48; // [esp-4194h] [ebp-41A0h]
  phys_vec3 v49; // [esp-4190h] [ebp-419Ch] BYREF
  float v50; // [esp-4180h] [ebp-418Ch]
  int v51; // [esp-4164h] [ebp-4170h]
  int v52; // [esp-4160h] [ebp-416Ch]
  int v53; // [esp-415Ch] [ebp-4168h]
  float v54; // [esp-4158h] [ebp-4164h]
  float v55; // [esp-4154h] [ebp-4160h]
  plane_lt *v56; // [esp-4144h] [ebp-4150h]
  phys_vec3 v57; // [esp-4140h] [ebp-414Ch] BYREF
  float v58; // [esp-4130h] [ebp-413Ch]
  int v59; // [esp-4114h] [ebp-4120h]
  int v60; // [esp-4110h] [ebp-411Ch]
  float v61; // [esp-410Ch] [ebp-4118h]
  int v62; // [esp-4108h] [ebp-4114h]
  float v63; // [esp-4104h] [ebp-4110h]
  plane_lt *v64; // [esp-40F4h] [ebp-4100h]
  phys_vec3 v65; // [esp-40F0h] [ebp-40FCh] BYREF
  float v66; // [esp-40E0h] [ebp-40ECh]
  int v67; // [esp-40C4h] [ebp-40D0h]
  float v68; // [esp-40C0h] [ebp-40CCh]
  int v69; // [esp-40BCh] [ebp-40C8h]
  int v70; // [esp-40B8h] [ebp-40C4h]
  float v71; // [esp-40B4h] [ebp-40C0h]
  plane_lt *v72; // [esp-40A4h] [ebp-40B0h]
  phys_vec3 v73; // [esp-40A0h] [ebp-40ACh] BYREF
  float v74; // [esp-4090h] [ebp-409Ch]
  float dist; // [esp-4074h] [ebp-4080h]
  phys_vec3 v76; // [esp-4070h] [ebp-407Ch] BYREF
  cplane_s *plane; // [esp-4058h] [ebp-4064h]
  unsigned int i; // [esp-4054h] [ebp-4060h]
  phys_static_array<plane_lt,512> v79; // [esp-4050h] [ebp-405Ch] BYREF
  unsigned int numsides; // [esp-34h] [ebp-40h]
  phys_vec3 v81; // [esp-30h] [ebp-3Ch] BYREF
  phys_vec3 v82; // [esp-20h] [ebp-2Ch] BYREF
  debug_brush_info_t *v83; // [esp-8h] [ebp-14h]
  unsigned int v84[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v84[0] = a2;
  v84[1] = retaddr;
  v4 = alloca(25312);
  v83 = this;
  if ( this->num_brushes == 5000 )
  {
    Com_Printf(0, "Debug brush limit exceeded, limit=%d\n", 5000);
  }
  else if ( mat )
  {
    Com_Printf(0, "Debug rendering for xformed brushes is not supported at the moment\n");
  }
  else
  {
    Phys_Vec3ToNitrousVec(brush->mins, &v82);
    Phys_Vec3ToNitrousVec(brush->maxs, &v81);
    numsides = brush->numsides;
    v79.m_slot_array = (plane_lt *const)&v79;
    v79.m_alloc_count = 0;
    for ( i = 0; i < numsides; ++i )
    {
      plane = brush->sides[i].plane;
      Phys_Vec3ToNitrousVec(plane->normal, &v76);
      dist = plane->dist;
      v73 = v76;
      v74 = dist;
      v72 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
      phys_vec3::operator=(&v72->n, &v73);
      v72->d = v74;
    }
    v68 = FLOAT_N1_0;
    v69 = *(unsigned int *)&FLOAT_0_0;
    v70 = *(unsigned int *)&FLOAT_0_0;
    v67 = LODWORD(v82.x) ^ _mask__NegFloat_;
    v65.x = FLOAT_N1_0;
    v65.y = *(float *)&FLOAT_0_0;
    v65.z = *(float *)&FLOAT_0_0;
    v65.w = v71;
    LODWORD(v66) = LODWORD(v82.x) ^ _mask__NegFloat_;
    v64 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v64->n, &v65);
    v64->d = v66;
    v60 = *(unsigned int *)&FLOAT_0_0;
    v61 = FLOAT_N1_0;
    v62 = *(unsigned int *)&FLOAT_0_0;
    v59 = LODWORD(v82.y) ^ _mask__NegFloat_;
    v57.x = *(float *)&FLOAT_0_0;
    v57.y = FLOAT_N1_0;
    v57.z = *(float *)&FLOAT_0_0;
    v57.w = v63;
    LODWORD(v58) = LODWORD(v82.y) ^ _mask__NegFloat_;
    v56 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v56->n, &v57);
    v56->d = v58;
    v52 = *(unsigned int *)&FLOAT_0_0;
    v53 = *(unsigned int *)&FLOAT_0_0;
    v54 = FLOAT_N1_0;
    v51 = LODWORD(v82.z) ^ _mask__NegFloat_;
    v49.x = *(float *)&FLOAT_0_0;
    v49.y = *(float *)&FLOAT_0_0;
    v49.z = FLOAT_N1_0;
    v49.w = v55;
    LODWORD(v50) = LODWORD(v82.z) ^ _mask__NegFloat_;
    v48 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v48->n, &v49);
    v48->d = v50;
    v44 = FLOAT_1_0;
    v45 = *(unsigned int *)&FLOAT_0_0;
    v46 = *(unsigned int *)&FLOAT_0_0;
    x = v81.x;
    v41.x = FLOAT_1_0;
    v41.y = *(float *)&FLOAT_0_0;
    v41.z = *(float *)&FLOAT_0_0;
    v41.w = v47;
    v42 = v81.x;
    v40 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v40->n, &v41);
    v40->d = v42;
    v36 = *(unsigned int *)&FLOAT_0_0;
    v37 = FLOAT_1_0;
    v38 = *(unsigned int *)&FLOAT_0_0;
    y = v81.y;
    v33.x = *(float *)&FLOAT_0_0;
    v33.y = FLOAT_1_0;
    v33.z = *(float *)&FLOAT_0_0;
    v33.w = v39;
    v34 = v81.y;
    v32 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v32->n, &v33);
    v32->d = v34;
    v28 = *(unsigned int *)&FLOAT_0_0;
    v29 = *(unsigned int *)&FLOAT_0_0;
    v30 = FLOAT_1_0;
    z = v81.z;
    v25.x = *(float *)&FLOAT_0_0;
    v25.y = *(float *)&FLOAT_0_0;
    v25.z = FLOAT_1_0;
    v25.w = v31;
    v26 = v81.z;
    v24 = phys_static_array<plane_lt,512>::add(&v79, 0, "phys array add overflow.");
    phys_vec3::operator=(&v24->n, &v25);
    v24->d = v26;
    m_alloc_count = v79.m_alloc_count;
    if ( v79.m_alloc_count >= 0x10000
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
            146,
            0,
            "%s",
            "planes.get_count() < 65536") )
    {
      __debugbreak();
    }
    v22 = v79.m_alloc_count;
    v21 = v79.m_alloc_count;
    v20 = &v83->brushes[v83->num_brushes++];
    v20->brush = brush;
    v20->first_winding_index = v83->num_windings;
    v20->first_vertex_index = v83->num_indices;
    v20->num_windings = v21;
    if ( v83->num_windings + v21 < 50000 )
    {
      num_windings = v83->num_windings;
      for ( j = 0; j < v21; ++j )
      {
        v16.m_slot_array = (phys_vec3 *const)&v16;
        v16.m_alloc_count = 0;
        calc_winding((int)v84, &v79, j, &v16);
        v15 = v16.m_alloc_count;
        if ( v16.m_alloc_count >= 0x10000
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
                167,
                0,
                "%s",
                "winding.get_count() < 65536") )
        {
          __debugbreak();
        }
        v14 = v16.m_alloc_count;
        v13 = v16.m_alloc_count;
        v83->windings[v83->num_windings++] = v16.m_alloc_count;
        if ( v83->num_indices + v13 >= 360000 )
        {
          Com_Printf(0, "Debug brush windings limit exceeded, limit=%d\n", 360000);
          --v83->num_brushes;
          v83->num_windings = num_windings;
          for ( k = 0; k < v16.m_alloc_count; ++k )
            ;
          for ( m = 0; m < v79.m_alloc_count; ++m )
            ;
          return;
        }
        for ( n = 0; n < v13; ++n )
        {
          v5 = phys_static_array<phys_vec3,512>::operator[](&v16, n);
          Phys_NitrousVecToVec3(v5, v9);
          index_in_brush = (unsigned __int16)debug_brush_info_t::find_index_in_brush(v83, brush, v9);
          if ( index_in_brush >= 0x100
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
                  184,
                  0,
                  "%s",
                  "ind < 256") )
          {
            __debugbreak();
          }
          v83->indices[v83->num_indices++] = index_in_brush;
        }
        for ( ii = 0; ii < v16.m_alloc_count; ++ii )
          ;
      }
      v20->num_indices = LOWORD(v83->num_indices) - LOWORD(v20->first_vertex_index);
      for ( jj = 0; jj < v79.m_alloc_count; ++jj )
        ;
    }
    else
    {
      Com_Printf(0, "Debug brush windings limit exceeded, limit=%d\n", 50000);
      --v83->num_brushes;
      for ( kk = 0; kk < v79.m_alloc_count; ++kk )
        ;
    }
  }
}

__int16 __thiscall debug_brush_info_t::find_index_in_brush(
        debug_brush_info_t *this,
        const cbrush_t *brush,
        const float *v)
{
  float d; // [esp+1Ch] [ebp-10h]
  unsigned int i; // [esp+20h] [ebp-Ch]
  __int16 best_i; // [esp+24h] [ebp-8h]
  float best_d; // [esp+28h] [ebp-4h]

  best_d = Vec3DistanceSq(v, (const float *)brush->verts);
  best_i = 0;
  if ( brush->numverts >= 0x10000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
          98,
          0,
          "%s",
          "brush->numverts < 65536") )
  {
    __debugbreak();
  }
  for ( i = 1; i < brush->numverts; ++i )
  {
    d = Vec3DistanceSq(v, brush->verts[i]);
    if ( best_d > d )
    {
      best_d = d;
      best_i = i;
    }
  }
  return best_i;
}

void __cdecl add_debug_patch(const CollisionAabbTree *tree)
{
  if ( debug_patch_info )
    debug_patch_info_t::add_patch(debug_patch_info, tree);
}

void __thiscall debug_patch_info_t::add_patch(debug_patch_info_t *this, const CollisionAabbTree *tree)
{
  float v[3]; // [esp+48h] [ebp-20h] BYREF
  unsigned int ind; // [esp+54h] [ebp-14h]
  int ind_i; // [esp+58h] [ebp-10h]
  unsigned int ntris; // [esp+5Ch] [ebp-Ch]
  debug_patch_t *debug_patch; // [esp+60h] [ebp-8h]
  chull_t *chull; // [esp+64h] [ebp-4h]

  if ( this->num_patches == 5000 )
  {
    Com_Printf(0, "Debug patches limit exceeded, limit=%d\n", 5000);
  }
  else
  {
    chull = get_partition_chull(tree);
    if ( chull && chull->ninds > 0 )
    {
      if ( chull->ninds % 3
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
              258,
              0,
              "%s",
              "chull->ninds % 3 == 0") )
      {
        __debugbreak();
      }
      ntris = chull->ninds / 3;
      debug_patch = &this->patches[this->num_patches++];
      debug_patch->tree = tree;
      debug_patch->first_vertex_index = this->num_indices;
      debug_patch->num_indices = 3 * ntris;
      if ( this->num_indices + 3 * ntris < 0x186A0 )
      {
        for ( ind_i = 0; ind_i < chull->ninds; ++ind_i )
        {
          Phys_NitrousVecToVec3(&chull->verts[chull->inds[ind_i]], v);
          ind = debug_patch_info_t::find_index_in_clipmap(this, tree, v);
          this->indices[this->num_indices++] = ind;
        }
      }
      else
      {
        Com_Printf(0, "Debug patches indices limit exceeded, limit=%d\n", 100000);
        --this->num_patches;
      }
    }
  }
}

unsigned int __thiscall debug_patch_info_t::find_index_in_clipmap(
        debug_patch_info_t *this,
        const CollisionAabbTree *tree,
        const float *v)
{
  int ti; // [esp+34h] [ebp-14h]
  unsigned int best_i; // [esp+38h] [ebp-10h]
  float d; // [esp+3Ch] [ebp-Ch]
  float da; // [esp+3Ch] [ebp-Ch]
  float db; // [esp+3Ch] [ebp-Ch]
  float best_d; // [esp+40h] [ebp-8h]
  const CollisionPartition *partition; // [esp+44h] [ebp-4h]

  partition = &cm.partitions[tree->u.firstChildIndex];
  best_d = Vec3DistanceSq(v, cm.verts[cm.triIndices[3 * partition->firstTri]]);
  best_i = cm.triIndices[3 * partition->firstTri];
  for ( ti = partition->firstTri; ti < partition->firstTri + partition->triCount; ++ti )
  {
    d = Vec3DistanceSq(v, cm.verts[cm.triIndices[3 * ti]]);
    if ( best_d > d )
    {
      best_d = d;
      best_i = cm.triIndices[3 * ti];
    }
    da = Vec3DistanceSq(v, cm.verts[cm.triIndices[3 * ti + 1]]);
    if ( best_d > da )
    {
      best_d = da;
      best_i = cm.triIndices[3 * ti + 1];
    }
    db = Vec3DistanceSq(v, cm.verts[cm.triIndices[3 * ti + 2]]);
    if ( best_d > db )
    {
      best_d = db;
      best_i = cm.triIndices[3 * ti + 2];
    }
  }
  return best_i;
}

void __cdecl render_debug_brushes()
{
  float *v0; // [esp+4h] [ebp-B8h]
  float *v1; // [esp+Ch] [ebp-B0h]
  int v2; // [esp+10h] [ebp-ACh]
  unsigned __int16 i; // [esp+14h] [ebp-A8h]
  int ii; // [esp+1Ch] [ebp-A0h]
  unsigned __int16 ibase; // [esp+20h] [ebp-9Ch]
  unsigned int num_indices; // [esp+24h] [ebp-98h]
  int wi; // [esp+28h] [ebp-94h]
  int first_winding_index; // [esp+2Ch] [ebp-90h]
  debug_brush_t *debug_brush; // [esp+30h] [ebp-8Ch]
  const cbrush_t *brush; // [esp+34h] [ebp-88h]
  int first_vertex_index; // [esp+38h] [ebp-84h]
  int bi; // [esp+3Ch] [ebp-80h]
  float lit_color[4]; // [esp+40h] [ebp-7Ch] BYREF
  GfxColor gfxColor; // [esp+50h] [ebp-6Ch] BYREF
  float verts[3][3]; // [esp+54h] [ebp-68h] BYREF
  materialCommands_t *tess; // [esp+78h] [ebp-44h]
  float player_clip_color[4]; // [esp+7Ch] [ebp-40h]
  float color[4]; // [esp+8Ch] [ebp-30h] BYREF
  float default_color[4]; // [esp+9Ch] [ebp-20h]
  float mantle_color[4]; // [esp+ACh] [ebp-10h]

  default_color[0] = *(float *)&FLOAT_0_0;
  default_color[1] = FLOAT_0_75;
  default_color[2] = FLOAT_0_75;
  default_color[3] = FLOAT_1_0;
  player_clip_color[0] = FLOAT_0_75;
  player_clip_color[1] = FLOAT_0_75;
  player_clip_color[2] = *(float *)&FLOAT_0_0;
  player_clip_color[3] = FLOAT_1_0;
  mantle_color[0] = FLOAT_0_5;
  mantle_color[1] = *(float *)&FLOAT_0_0;
  mantle_color[2] = *(float *)&FLOAT_0_0;
  mantle_color[3] = FLOAT_1_0;
  DebugPatchesAndBrushesProlog();
  if ( debug_brush_info->num_brushes )
  {
    RB_BeginCollisionPolygons(1, 0);
    tess = RB_GetCmd();
    for ( bi = 0; bi < debug_brush_info->num_brushes; ++bi )
    {
      debug_brush = &debug_brush_info->brushes[bi];
      brush = debug_brush->brush;
      first_vertex_index = debug_brush->first_vertex_index;
      first_winding_index = debug_brush->first_winding_index;
      for ( wi = first_winding_index; wi < first_winding_index + debug_brush->num_windings; ++wi )
      {
        num_indices = debug_brush_info->windings[wi];
        if ( num_indices >= 3 )
        {
          RB_CheckTessOverflow(num_indices, 3 * (num_indices - 2));
          v2 = (int)brush->verts[debug_brush_info->indices[first_vertex_index]];
          *(_QWORD *)&verts[0][0] = *(_QWORD *)v2;
          verts[0][2] = *(float *)(v2 + 8);
          v1 = brush->verts[debug_brush_info->indices[first_vertex_index + 1]];
          verts[1][0] = *v1;
          verts[1][1] = v1[1];
          verts[1][2] = v1[2];
          v0 = brush->verts[debug_brush_info->indices[first_vertex_index + 2]];
          verts[2][0] = *v0;
          verts[2][1] = v0[1];
          verts[2][2] = v0[2];
          if ( (brush->contents & 0x10000) != 0 )
          {
            color[0] = player_clip_color[0];
            color[1] = player_clip_color[1];
            color[2] = player_clip_color[2];
            color[3] = player_clip_color[3];
          }
          else if ( ((unsigned int)&cls.rankedServers[711].game[35] & brush->contents) != 0 )
          {
            color[0] = mantle_color[0];
            color[1] = mantle_color[1];
            color[2] = mantle_color[2];
            color[3] = mantle_color[3];
          }
          else
          {
            color[0] = default_color[0];
            color[1] = default_color[1];
            color[2] = default_color[2];
            color[3] = default_color[3];
          }
          RB_ApplySunLight(verts, color, lit_color);
          R_ConvertColorToBytes(lit_color, (unsigned __int8 *)&gfxColor);
          ibase = tess->vertexCount;
          for ( ii = first_vertex_index; ii < (int)(num_indices + first_vertex_index); ++ii )
          {
            RB_SetPolyVert(brush->verts[debug_brush_info->indices[ii]], gfxColor, tess->vertexCount);
            ++tess->vertexCount;
          }
          for ( i = 1; i < (int)(num_indices - 1); ++i )
          {
            tess->indices[tess->indexCount] = ibase;
            tess->indices[tess->indexCount + 1] = i + ibase;
            tess->indices[tess->indexCount + 2] = ibase + i + 1;
            tess->indexCount += 3;
          }
          first_vertex_index += num_indices;
        }
      }
    }
    RB_EndCollisionPolygons();
    DebugPatchesAndBrushesEpilog();
  }
  else
  {
    DebugPatchesAndBrushesEpilog();
  }
}

void __cdecl RB_CheckTessOverflow(int vertexCount, int indexCount)
{
  if ( vertexCount > 5450
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../gfx_d3d/rb_backend.h",
          175,
          0,
          "%s\n\t(vertexCount) = %i",
          "(vertexCount <= 5450)",
          vertexCount) )
  {
    __debugbreak();
  }
  if ( indexCount > 0x100000
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../gfx_d3d/rb_backend.h",
          176,
          0,
          "%s\n\t(indexCount) = %i",
          "(indexCount <= ((2 * 1024 * 1024) / 2))",
          indexCount) )
  {
    __debugbreak();
  }
  if ( vertexCount + tess.vertexCount > 5450 || indexCount + tess.indexCount > 0x100000 )
    RB_TessOverflow();
}

void __cdecl render_debug_patches_full()
{
  float *v0; // [esp+0h] [ebp-78h]
  float *v1; // [esp+4h] [ebp-74h]
  float *v2; // [esp+8h] [ebp-70h]
  float *v3; // [esp+Ch] [ebp-6Ch]
  float *v4; // [esp+10h] [ebp-68h]
  unsigned int vi; // [esp+14h] [ebp-64h]
  debug_patch_t *debug_patch; // [esp+38h] [ebp-40h]
  unsigned int i; // [esp+3Ch] [ebp-3Ch]
  const CollisionAabbTree *tree; // [esp+40h] [ebp-38h]
  float polygonVerts[3][3]; // [esp+44h] [ebp-34h] BYREF
  float faceColor[4]; // [esp+68h] [ebp-10h] BYREF

  faceColor[0] = FLOAT_0_1;
  faceColor[1] = FLOAT_0_85000002;
  faceColor[2] = FLOAT_0_40000001;
  faceColor[3] = FLOAT_1_0;
  DebugPatchesAndBrushesProlog();
  if ( debug_patch_info && debug_patch_info->num_patches )
  {
    RB_BeginCollisionPolygons(1, 0);
    for ( i = 0; i < debug_patch_info->num_patches; ++i )
    {
      debug_patch = &debug_patch_info->patches[i];
      tree = debug_patch->tree;
      faceColor[0] = (float)(0.5 * (float)((float)((((unsigned int)debug_patch->tree >> 4) & 0xF00) >> 4) / 256.0))
                   + 0.5;
      faceColor[1] = (float)(0.5 * (float)((float)(((unsigned int)tree >> 4) & 0xF0) / 256.0)) + 0.5;
      faceColor[2] = (float)(0.5 * (float)((float)(unsigned __int8)(16 * (((unsigned int)tree >> 4) & 0xF)) / 256.0))
                   + 0.5;
      if ( tree->u.firstChildIndex == g_debug_partition )
      {
        faceColor[0] = colorWhite[0];
        faceColor[1] = 1.0;
        faceColor[2] = 1.0;
        faceColor[3] = 1.0;
      }
      for ( vi = debug_patch->first_vertex_index; vi < debug_patch->first_vertex_index + debug_patch->num_indices; vi += 3 )
      {
        v4 = cm.verts[debug_patch_info->indices[vi]];
        *(_QWORD *)&polygonVerts[0][0] = *(_QWORD *)v4;
        polygonVerts[0][2] = v4[2];
        v2 = polygonVerts[(flip != 0) + 1];
        v3 = cm.verts[debug_patch_info->indices[vi + 1]];
        *v2 = *v3;
        v2[1] = v3[1];
        v2[2] = v3[2];
        v0 = polygonVerts[2 - (flip != 0)];
        v1 = cm.verts[debug_patch_info->indices[vi + 2]];
        *v0 = *v1;
        v0[1] = v1[1];
        v0[2] = v1[2];
        RB_DrawCollisionPolygon(3, polygonVerts, faceColor, 1, g_debug_partition);
      }
    }
    RB_EndCollisionPolygons();
    DebugPatchesAndBrushesEpilog();
  }
  else
  {
    DebugPatchesAndBrushesEpilog();
  }
}

void __cdecl render_debug_brushes_and_patches()
{
  if ( debug_brush_info )
  {
    render_debug_patches_full();
    render_debug_brushes();
  }
}

void  render_brush(
        float a1@<ebp>,
        const cbrush_t *brush,
        const phys_mat44 *xform,
        const float *color,
        bool bLinesOnly,
        const float *lightPos,
        bool persistent,
        bool ztest,
        const float *edgeColor)
{
  const phys_vec3 *v9; // eax
  float *v10; // [esp-14h] [ebp-580h]
  float v11; // [esp-Ch] [ebp-578h]
  float v12[2]; // [esp-8h] [ebp-574h] BYREF
  float normal[3]; // [esp+4h] [ebp-568h] BYREF
  float p02[4]; // [esp+10h] [ebp-55Ch] BYREF
  float p01[4]; // [esp+20h] [ebp-54Ch] BYREF
  float lightDir[3]; // [esp+30h] [ebp-53Ch]
  float v17; // [esp+3Ch] [ebp-530h]
  float center[3]; // [esp+40h] [ebp-52Ch]
  float *v19; // [esp+4Ch] [ebp-520h]
  float *v20; // [esp+50h] [ebp-51Ch]
  float v21[3]; // [esp+54h] [ebp-518h] BYREF
  float verts[3][3]; // [esp+60h] [ebp-50Ch] BYREF
  unsigned int v23[3]; // [esp+98h] [ebp-4D4h] BYREF
  float vert_list[100][3]; // [esp+A4h] [ebp-4C8h] BYREF
  int vi; // [esp+554h] [ebp-18h]
  chull_t *chull; // [esp+558h] [ebp-14h]
  float clr[4]; // [esp+55Ch] [ebp-10h] BYREF
  float retaddr; // [esp+56Ch] [ebp+0h]

  clr[1] = a1;
  clr[2] = retaddr;
  vert_list[99][2] = *(float *)&FLOAT_0_0;
  *(float *)&vi = FLOAT_1_0;
  *(float *)&chull = FLOAT_1_0;
  clr[0] = FLOAT_0_25;
  if ( color )
  {
    vert_list[99][2] = *color;
    vi = *((int *)color + 1);
    chull = *((chull_t **)color + 2);
    clr[0] = color[3];
  }
  if ( !xform )
    xform = &PHYS_IDENTITY_MATRIX_44;
  LODWORD(vert_list[99][1]) = get_brush_chull(brush);
  if ( LODWORD(vert_list[99][1]) )
  {
    if ( *(int *)(LODWORD(vert_list[99][1]) + 4) >= 100
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
            664,
            0,
            "%s",
            "chull->nverts < 100") )
    {
      __debugbreak();
    }
    for ( vert_list[99][0] = 0.0;
          SLODWORD(vert_list[99][0]) < *(unsigned int *)(LODWORD(vert_list[99][1]) + 4);
          ++LODWORD(vert_list[99][0]) )
    {
      v10 = (float *)&v23[3 * LODWORD(vert_list[99][0])];
      v9 = phys_full_multiply(
             (int)&clr[1],
             (const phys_vec3 *)&verts[2][2],
             xform,
             (const phys_vec3 *)(*(unsigned int *)(LODWORD(vert_list[99][1]) + 8) + 16 * LODWORD(vert_list[99][0])));
      Phys_NitrousVecToVec3(v9, v10);
    }
    for ( verts[2][1] = 0.0; ; LODWORD(verts[2][1]) += 3 )
    {
      if ( SLODWORD(verts[2][1]) >= *(unsigned int *)(LODWORD(vert_list[99][1]) + 12) )
        return;
      LODWORD(verts[2][0]) = &v23[3
                                * *(unsigned __int16 *)(*(unsigned int *)(LODWORD(vert_list[99][1]) + 16)
                                                      + 2 * LODWORD(verts[2][1]))];
      v21[0] = *(float *)LODWORD(verts[2][0]);
      v21[1] = *(float *)(LODWORD(verts[2][0]) + 4);
      v21[2] = *(float *)(LODWORD(verts[2][0]) + 8);
      v20 = verts[0];
      v19 = (float *)&v23[3
                        * *(unsigned __int16 *)(*(unsigned int *)(LODWORD(vert_list[99][1]) + 16)
                                              + 2 * LODWORD(verts[2][1])
                                              + 2)];
      verts[0][0] = *v19;
      verts[0][1] = v19[1];
      verts[0][2] = v19[2];
      LODWORD(center[2]) = verts[1];
      LODWORD(center[1]) = &v23[3
                              * *(unsigned __int16 *)(*(unsigned int *)(LODWORD(vert_list[99][1]) + 16)
                                                    + 2 * LODWORD(verts[2][1])
                                                    + 4)];
      verts[1][0] = *(float *)LODWORD(center[1]);
      verts[1][1] = *(float *)(LODWORD(center[1]) + 4);
      verts[1][2] = *(float *)(LODWORD(center[1]) + 8);
      if ( bLinesOnly )
      {
        render_line(v21, verts[0], colorLtGreen, 0, ztest);
        render_line(v21, verts[1], colorLtGreen, 0, ztest);
        render_line(verts[0], verts[1], colorLtGreen, 0, ztest);
        continue;
      }
      if ( lightPos )
      {
        LODWORD(center[0]) = verts;
        LODWORD(lightDir[0]) = verts[1];
        lightDir[1] = (float)(v21[0] + verts[0][0]) + verts[1][0];
        lightDir[2] = (float)(v21[1] + verts[0][1]) + verts[1][1];
        v17 = (float)(v21[2] + verts[0][2]) + verts[1][2];
        lightDir[1] = 0.33333334 * lightDir[1];
        lightDir[2] = 0.33333334 * lightDir[2];
        v17 = 0.33333334 * v17;
        p01[1] = lightDir[1] - *lightPos;
        p01[2] = lightDir[2] - lightPos[1];
        p01[3] = v17 - lightPos[2];
        Vec3Normalize(&p01[1]);
        LODWORD(p01[0]) = verts;
        p02[1] = verts[0][0] - v21[0];
        p02[2] = verts[0][1] - v21[1];
        p02[3] = verts[0][2] - v21[2];
        LODWORD(p02[0]) = verts[1];
        normal[0] = verts[1][0] - v21[0];
        normal[1] = verts[1][1] - v21[1];
        normal[2] = verts[1][2] - v21[2];
        Vec3Cross(&p02[1], normal, v12);
        Vec3Normalize(v12);
        v11 = FLOAT_1_0;
        if ( 1.0 > 0.0 )
        {
          if ( 1.0 < 0.60000002 )
            v11 = FLOAT_0_60000002;
          vert_list[99][2] = v11 * vert_list[99][2];
          *(float *)&vi = v11 * *(float *)&vi;
          *(float *)&chull = v11 * *(float *)&chull;
          clr[0] = FLOAT_0_25;
          if ( persistent )
          {
LABEL_22:
            R_AddPersistentDebugPolygon(
              &frontEndDataOut->debugGlobals,
              3,
              (float (*)[3])v21,
              &vert_list[99][2],
              0,
              &vert_list[99][2],
              0);
            continue;
          }
          R_AddDebugPolygon(
            &frontEndDataOut->debugGlobals,
            3,
            (float (*)[3])v21,
            &vert_list[99][2],
            ztest,
            &vert_list[99][2],
            ztest);
        }
      }
      else
      {
        if ( persistent )
          goto LABEL_22;
        R_AddDebugPolygon(&frontEndDataOut->debugGlobals, 3, (float (*)[3])v21, color, ztest, edgeColor, ztest);
      }
    }
  }
}

void __cdecl calc_color(unsigned int id, float alpha, float *color)
{
  *color = FLOAT_0_1;
  color[1] = FLOAT_0_85000002;
  color[2] = FLOAT_0_40000001;
  color[3] = FLOAT_1_0;
  *color = (float)(0.5 * (float)((float)(((id >> 4) & 0xF00) >> 4) / 256.0)) + 0.5;
  color[1] = (float)(0.5 * (float)((float)((id >> 4) & 0xF0) / 256.0)) + 0.5;
  color[2] = (float)(0.5 * (float)((float)(unsigned __int8)(16 * ((unsigned __int8)id >> 4)) / 256.0)) + 0.5;
  color[3] = alpha;
}

void __cdecl render_convex_partition(const CollisionAabbTree *tree)
{
  int ind_i; // [esp+8h] [ebp-4Ch]
  float edgeColor[4]; // [esp+Ch] [ebp-48h] BYREF
  chull_t *chull; // [esp+1Ch] [ebp-38h]
  float polygonVerts[3][3]; // [esp+20h] [ebp-34h] BYREF
  float faceColor[4]; // [esp+44h] [ebp-10h] BYREF

  calc_color((unsigned int)tree, alpha, faceColor);
  memset(edgeColor, 0, 12);
  edgeColor[3] = FLOAT_1_0;
  chull = get_partition_chull(tree);
  if ( chull )
  {
    for ( ind_i = 0; ind_i < chull->ninds; ind_i += 3 )
    {
      Phys_NitrousVecToVec3(&chull->verts[chull->inds[ind_i]], polygonVerts[0]);
      Phys_NitrousVecToVec3(&chull->verts[chull->inds[ind_i + 1]], polygonVerts[1]);
      Phys_NitrousVecToVec3(&chull->verts[chull->inds[ind_i + 2]], polygonVerts[2]);
      R_AddDebugPolygon(&frontEndDataOut->debugGlobals, 3, polygonVerts, faceColor, 1, edgeColor, 1);
    }
  }
}

void  render_chull(float a1@<ebp>, const chull_t *first, const phys_mat44 *xform, const float *color)
{
  const phys_vec3 *v4; // eax
  float *v5; // [esp-24h] [ebp-530h]
  float *v6; // [esp-1Ch] [ebp-528h]
  float *v7; // [esp-14h] [ebp-520h]
  float v8[3]; // [esp-Ch] [ebp-518h] BYREF
  float tverts[3][3]; // [esp+0h] [ebp-50Ch] BYREF
  unsigned int v10[3]; // [esp+38h] [ebp-4D4h] BYREF
  float verts[100][3]; // [esp+44h] [ebp-4C8h] BYREF
  float v12; // [esp+4F4h] [ebp-18h]
  int vi; // [esp+4F8h] [ebp-14h]
  float clr[4]; // [esp+4FCh] [ebp-10h] BYREF
  float retaddr; // [esp+50Ch] [ebp+0h]

  clr[1] = a1;
  clr[2] = retaddr;
  if ( first )
  {
    verts[99][2] = *(float *)&FLOAT_0_0;
    v12 = FLOAT_1_0;
    vi = LODWORD(FLOAT_1_0);
    clr[0] = FLOAT_0_25;
    if ( color )
    {
      verts[99][2] = *color;
      v12 = color[1];
      vi = *((unsigned int *)color + 2);
      clr[0] = color[3];
    }
    if ( !xform )
      xform = &PHYS_IDENTITY_MATRIX_44;
    while ( first )
    {
      if ( first->nverts >= 100
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
              804,
              0,
              "%s",
              "first->nverts < 100") )
      {
        __debugbreak();
      }
      for ( verts[99][1] = 0.0; SLODWORD(verts[99][1]) < first->nverts; ++LODWORD(verts[99][1]) )
      {
        v5 = (float *)&v10[3 * LODWORD(verts[99][1])];
        v4 = phys_full_multiply(
               (int)&clr[1],
               (const phys_vec3 *)&tverts[2][2],
               xform,
               &first->verts[LODWORD(verts[99][1])]);
        Phys_NitrousVecToVec3(v4, v5);
      }
      for ( tverts[2][1] = 0.0; SLODWORD(tverts[2][1]) < first->ninds; LODWORD(tverts[2][1]) += 3 )
      {
        LODWORD(tverts[2][0]) = &v10[3 * first->inds[LODWORD(tverts[2][1])]];
        v8[0] = *(float *)LODWORD(tverts[2][0]);
        v8[1] = *(float *)(LODWORD(tverts[2][0]) + 4);
        v8[2] = *(float *)(LODWORD(tverts[2][0]) + 8);
        v7 = (float *)&v10[3 * first->inds[LODWORD(tverts[2][1]) + 1]];
        tverts[0][0] = *v7;
        tverts[0][1] = v7[1];
        tverts[0][2] = v7[2];
        v6 = (float *)&v10[3 * first->inds[LODWORD(tverts[2][1]) + 2]];
        tverts[1][0] = *v6;
        tverts[1][1] = v6[1];
        tverts[1][2] = v6[2];
        R_AddDebugPolygon(&frontEndDataOut->debugGlobals, 3, (float (*)[3])v8, &verts[99][2], 1, &verts[99][2], 1);
      }
      first = first->next;
    }
  }
}

void __cdecl render_xmodel_chull(const XModel *model, unsigned int key, const phys_mat44 *xform, const float *color)
{
  chull_t *chull; // [esp+0h] [ebp-8h]
  PhysGeomList *geomList; // [esp+4h] [ebp-4h]
  int savedregs; // [esp+8h] [ebp+0h] BYREF

  if ( model && XModelHasCollmap(model) )
  {
    geomList = XModelGetCollmap(model, 0);
    if ( geomList )
    {
      chull = get_collmap_chull(geomList, key);
      render_chull(COERCE_FLOAT(&savedregs), chull, xform, color);
    }
  }
}

void __cdecl render_brushmodel_chull(
        unsigned int brushmodel,
        unsigned int key,
        const phys_mat44 *xform,
        const float *color)
{
  chull_t *brushmodel_chull; // eax
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  brushmodel_chull = get_brushmodel_chull(brushmodel, key);
  render_chull(COERCE_FLOAT(&savedregs), brushmodel_chull, xform, color);
}

void __cdecl render_line(const float *p0, const float *p1, const float *color, int duration, int ztest)
{
  CG_DebugLine(p0, p1, color, ztest, duration);
}

void __cdecl render_line(const phys_vec3 *p0, const phys_vec3 *p1, const float *color, int duration, int ztest)
{
  float _p1[3]; // [esp+0h] [ebp-18h] BYREF
  float _p0[3]; // [esp+Ch] [ebp-Ch] BYREF

  Phys_NitrousVecToVec3(p0, _p0);
  Phys_NitrousVecToVec3(p1, _p1);
  render_line(_p0, _p1, color, ztest, duration);
}

void  render_box(
        int a1@<ebp>,
        const phys_vec3 *mins,
        const phys_vec3 *maxs,
        const phys_mat44 *xform,
        const float *color,
        int duration)
{
  const phys_vec3 *v6; // eax
  int n; // [esp-Ch] [ebp-C8h]
  float *v8; // [esp-8h] [ebp-C4h]
  phys_vec3 v9; // [esp+0h] [ebp-BCh] BYREF
  int v10; // [esp+10h] [ebp-ACh]
  int m; // [esp+14h] [ebp-A8h]
  int k; // [esp+18h] [ebp-A4h]
  int *i; // [esp+1Ch] [ebp-A0h]
  int j; // [esp+20h] [ebp-9Ch] BYREF
  int v15; // [esp+ACh] [ebp-10h]
  unsigned int v16[3]; // [esp+B0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+BCh] [ebp+0h]

  v16[0] = a1;
  v16[1] = retaddr;
  v15 = 8;
  for ( i = &j; --v15 >= 0; i += 4 )
    ;
  for ( k = 0; k < 8; ++k )
  {
    for ( m = 0; m < 3; ++m )
    {
      if ( (k & (1 << m)) != 0 )
      {
        if ( m < 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
               32,
               "i >= 0 && i < 3",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v10 = *((unsigned int *)&maxs->x + m);
      }
      else
      {
        if ( m < 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
               32,
               "i >= 0 && i < 3",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v10 = *((unsigned int *)&mins->x + m);
      }
      if ( (unsigned int)m > 2 )
      {
        if ( _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
               34,
               "i >= 0 && i < 3",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      *(&j + 4 * k + m) = v10;
    }
    v6 = phys_full_multiply((int)v16, &v9, xform, (const phys_vec3 *)&j + k);
    v8 = (float *)(&j + 4 * k);
    *v8 = v6->x;
    v8[1] = v6->y;
    v8[2] = v6->z;
  }
  for ( n = 0; n < 12; ++n )
    render_line(
      (const phys_vec3 *)&j + iEdgePairs[n][0],
      (const phys_vec3 *)&j + dword_D7F394[2 * n],
      color,
      duration,
      0);
}

void  render_box(float a1@<ebp>, float *_mn, float *_mx, const float *color, int duration)
{
  _BYTE v5[12]; // [esp-Ch] [ebp-2Ch] BYREF
  phys_vec3 mx; // [esp+0h] [ebp-20h] BYREF
  phys_vec3 mn; // [esp+10h] [ebp-10h] BYREF
  float retaddr; // [esp+20h] [ebp+0h]

  mn.y = a1;
  mn.z = retaddr;
  Phys_Vec3ToNitrousVec(_mn, (phys_vec3 *)&mx.y);
  Phys_Vec3ToNitrousVec(_mx, (phys_vec3 *)v5);
  render_box((int)&mn.y, (phys_vec3 *)&mx.y, (const phys_vec3 *)v5, &PHYS_IDENTITY_MATRIX_44, color, duration);
}

void __cdecl render_box(const phys_vec3 *mins, const phys_vec3 *maxs, const float *color, int duration)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  render_box((int)&savedregs, mins, maxs, &PHYS_IDENTITY_MATRIX_44, color, duration);
}

// bad sp value at call has been detected, the output may be wrong!
void  render_gjk_geom(float a1@<ebp>, gjk_base_t *geom, const phys_mat44 *cg2w)
{
  phys_vec3 v3; // [esp+8h] [ebp-11Ch] BYREF
  phys_vec3 v4; // [esp+18h] [ebp-10Ch] BYREF
  float v5[4]; // [esp+34h] [ebp-F0h] BYREF
  float v6[4]; // [esp+44h] [ebp-E0h] BYREF
  const cbrush_t *v7; // [esp+54h] [ebp-D0h]
  phys_vec3 v8; // [esp+58h] [ebp-CCh] BYREF
  float v9; // [esp+68h] [ebp-BCh]
  float v10; // [esp+6Ch] [ebp-B8h]
  float v11; // [esp+70h] [ebp-B4h]
  gjk_base_t *v12; // [esp+74h] [ebp-B0h]
  _BYTE v13[12]; // [esp+78h] [ebp-ACh] BYREF
  gjk_base_t *v14; // [esp+C4h] [ebp-60h]
  unsigned int v15[2]; // [esp+C8h] [ebp-5Ch] BYREF
  const gjk_obb_t *obb; // [esp+D0h] [ebp-54h]
  float w; // [esp+D4h] [ebp-50h]
  phys_vec3 m_aabb_mx_loc; // [esp+D8h] [ebp-4Ch] BYREF
  unsigned int v19; // [esp+ECh] [ebp-38h]
  unsigned int v20; // [esp+F0h] [ebp-34h]
  float v21; // [esp+F4h] [ebp-30h] BYREF
  float edgeColor[4]; // [esp+100h] [ebp-24h] BYREF
  float faceColor[4]; // [esp+110h] [ebp-14h] BYREF
  float retaddr; // [esp+124h] [ebp+0h]

  faceColor[2] = a1;
  faceColor[3] = retaddr;
  if ( geom->get_brush(geom) )
  {
    LODWORD(faceColor[1]) = geom->get_brush(geom);
    calc_color(LODWORD(faceColor[1]), alpha_1, &edgeColor[1]);
    render_brush(
      COERCE_FLOAT(&faceColor[2]),
      (const cbrush_t *)LODWORD(faceColor[1]),
      cg2w,
      &edgeColor[1],
      0,
      0,
      0,
      1,
      &v21);
  }
  else
  {
    v20 = geom->get_type(geom);
    v19 = v20 - 1;
    switch ( v20 )
    {
      case 1u:
      case 4u:
      case 5u:
        if ( (geom->m_flags & 2) == 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
               83,
               "get_flag(FLAG_AABB_LOC_VALID)",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        m_aabb_mx_loc = geom->m_aabb_mx_loc;
        if ( (geom->m_flags & 2) == 0 )
        {
          if ( _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                 82,
                 "get_flag(FLAG_AABB_LOC_VALID)",
                 &toastPopupTitle) )
          {
            __debugbreak();
          }
        }
        v15[0] = LODWORD(geom->m_aabb_mn_loc.x);
        v15[1] = LODWORD(geom->m_aabb_mn_loc.y);
        obb = (const gjk_obb_t *)LODWORD(geom->m_aabb_mn_loc.z);
        w = geom->m_aabb_mn_loc.w;
        render_box((int)&faceColor[2], (const phys_vec3 *)v15, &m_aabb_mx_loc, cg2w, colorMdRed, 0);
        break;
      case 2u:
        v7 = geom->get_brush(geom);
        calc_color((unsigned int)v7, alpha_0, v6);
        memset(v5, 0, 12);
        v5[3] = FLOAT_1_0;
        render_brush(COERCE_FLOAT(&faceColor[2]), v7, cg2w, v6, 0, 0, 0, 1, v5);
        break;
      case 3u:
        render_convex_partition((const CollisionAabbTree *)LODWORD(geom[1].m_aabb_mn_loc.x));
        break;
      case 6u:
        v14 = geom;
        phys_full_multiply_mat((int)&faceColor[2], (phys_mat44 *)v13, cg2w, (const phys_mat44 *)&geom[1].m_aabb_mn_loc);
        v12 = v14 + 1;
        LODWORD(v11) = (__int128)v14[1].__vftable ^ _mask__NegFloat_;
        LODWORD(v10) = *((unsigned int *)&v14[1].phys_gjk_geom + 1) ^ _mask__NegFloat_;
        LODWORD(v9) = *((unsigned int *)&v14[1].phys_gjk_geom + 2) ^ _mask__NegFloat_;
        v8.x = v11;
        v8.y = v10;
        v8.z = v9;
        render_box((int)&faceColor[2], &v8, (const phys_vec3 *)&v14[1], (const phys_mat44 *)v13, colorMdRed, 0);
        break;
      default:
        if ( (geom->m_flags & 2) == 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
               83,
               "get_flag(FLAG_AABB_LOC_VALID)",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v4 = geom->m_aabb_mx_loc;
        if ( (geom->m_flags & 2) == 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
               82,
               "get_flag(FLAG_AABB_LOC_VALID)",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        render_box((int)&faceColor[2], &v3, &v4, cg2w, colorLtOrange, 0);
        break;
    }
  }
}

void  debug_render(int a1@<ebp>, PhysObjUserData *userData)
{
  gjk_base_t *i; // [esp-Ch] [ebp-70h]
  float v3; // [esp-4h] [ebp-68h] BYREF
  float pt[3]; // [esp+8h] [ebp-5Ch] BYREF
  phys_mat44 cg2w; // [esp+14h] [ebp-50h]
  rigid_body *body; // [esp+54h] [ebp-10h]
  int v7; // [esp+58h] [ebp-Ch] BYREF
  const phys_mat44 *rb2w; // [esp+5Ch] [ebp-8h]
  const phys_mat44 *retaddr; // [esp+64h] [ebp+0h]

  v7 = a1;
  rb2w = retaddr;
  body = userData->body;
  LODWORD(cg2w.w.w) = &body->m_mat;
  phys_full_multiply_mat((int)&v7, (phys_mat44 *)pt, &body->m_mat, &userData->cg2rb);
  Phys_NitrousVecToVec3((const phys_vec3 *)(LODWORD(cg2w.w.w) + 48), &v3);
  G_DebugStar(&v3, colorYellow, 0);
  if ( userData->m_gjk_geom_list.m_geom_count < 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
         1035,
         "m_geom_count >= 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  for ( i = userData->m_gjk_geom_list.m_first_geom; i; i = i->m_next_geom )
    render_gjk_geom(COERCE_FLOAT(&v7), i, (const phys_mat44 *)pt);
  BG_EvalVehicleName();
}

void  clip_winding(int a1@<ebp>, phys_static_array<phys_vec3,512> *winding, const plane_lt *clip)
{
  void *v3; // esp
  int *v4; // esi
  int *v5; // esi
  int *v6; // edi
  float *v7; // eax
  const float *v8; // esi
  const float *v9; // edi
  const float *v10; // eax
  phys_vec3 *v11; // eax
  phys_vec3 *v12; // eax
  phys_vec3 *v13; // [esp-3104h] [ebp-3110h]
  int jj; // [esp-30F8h] [ebp-3104h]
  int ii; // [esp-30F4h] [ebp-3100h]
  int v16; // [esp-30F0h] [ebp-30FCh]
  int m; // [esp-30ECh] [ebp-30F8h]
  float v18; // [esp-30E8h] [ebp-30F4h]
  int v19; // [esp-30E4h] [ebp-30F0h]
  phys_vec3 v20; // [esp-30E0h] [ebp-30ECh] BYREF
  signed int k; // [esp-30D0h] [ebp-30DCh]
  float v22; // [esp-30CCh] [ebp-30D8h]
  phys_vec3 *v23; // [esp-30C8h] [ebp-30D4h]
  phys_vec3 *v24; // [esp-30C4h] [ebp-30D0h]
  phys_vec3 *v25; // [esp-30C0h] [ebp-30CCh]
  phys_vec3 *v26; // [esp-30BCh] [ebp-30C8h]
  int v27; // [esp-30B8h] [ebp-30C4h]
  int j; // [esp-30B4h] [ebp-30C0h]
  phys_static_array<phys_vec3,512> v29; // [esp-30B0h] [ebp-30BCh] BYREF
  int kk; // [esp-1098h] [ebp-10A4h]
  int *v31; // [esp-1094h] [ebp-10A0h]
  int *v32; // [esp-1090h] [ebp-109Ch]
  float *v33; // [esp-108Ch] [ebp-1098h]
  int *v34; // [esp-1088h] [ebp-1094h]
  int *v35; // [esp-1084h] [ebp-1090h]
  int *v36; // [esp-1080h] [ebp-108Ch]
  int *v37; // [esp-107Ch] [ebp-1088h]
  float v38; // [esp-1078h] [ebp-1084h]
  float v39; // [esp-1074h] [ebp-1080h]
  phys_vec3 *v40; // [esp-1070h] [ebp-107Ch]
  int *v41; // [esp-106Ch] [ebp-1078h]
  int *v42; // [esp-1068h] [ebp-1074h]
  int *v43; // [esp-1064h] [ebp-1070h]
  int *v44; // [esp-1060h] [ebp-106Ch]
  int i; // [esp-105Ch] [ebp-1068h]
  int m_alloc_count; // [esp-1058h] [ebp-1064h]
  float d; // [esp-1054h] [ebp-1060h]
  phys_vec3 n; // [esp-1050h] [ebp-105Ch] BYREF
  int v49; // [esp-103Ch] [ebp-1048h] BYREF
  int v50; // [esp-1038h] [ebp-1044h]
  int v51; // [esp-1034h] [ebp-1040h]
  phys_static_array<int,512> v52; // [esp-1030h] [ebp-103Ch] BYREF
  phys_static_array<int,512> v53; // [esp-820h] [ebp-82Ch] BYREF
  int v54; // [esp+0h] [ebp-Ch]
  void *v55; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v54 = a1;
  v55 = retaddr;
  v3 = alloca(12536);
  v53.m_slot_array = (int *const)&v53;
  v53.m_alloc_count = 0;
  v52.m_slot_array = (int *const)&v52;
  v52.m_alloc_count = 0;
  v49 = 0;
  v50 = 0;
  v51 = 0;
  n = clip->n;
  d = clip->d;
  m_alloc_count = winding->m_alloc_count;
  phys_static_array<float,512>::call_destructors(&v53);
  v53.m_alloc_count = 0;
  phys_static_array<float,512>::call_destructors(&v52);
  v52.m_alloc_count = 0;
  for ( i = 0; i < m_alloc_count; ++i )
  {
    if ( v53.m_alloc_count < 512 )
    {
      v44 = &v53.m_slot_array[v53.m_alloc_count++];
      v43 = v44;
    }
    else
    {
      tlFatal("phys array add overflow.");
    }
    if ( v52.m_alloc_count < 512 )
    {
      v42 = &v52.m_slot_array[v52.m_alloc_count++];
      v41 = v42;
    }
    else
    {
      tlFatal("phys array add overflow.");
    }
    v40 = phys_static_array<phys_vec3,512>::operator[](winding, i);
    v39 = (float)((float)(v40->x * n.x) + (float)(v40->y * n.y)) + (float)(v40->z * n.z);
    v38 = v39 - d;
    *(float *)phys_static_array<float,512>::operator[](&v53, i) = v39 - d;
    if ( *(float *)phys_static_array<float,512>::operator[](&v53, i) <= 0.0099999998 )
    {
      if ( *(float *)phys_static_array<float,512>::operator[](&v53, i) >= -0.0099999998 )
        *phys_static_array<float,512>::operator[](&v52, i) = 2;
      else
        *phys_static_array<float,512>::operator[](&v52, i) = 1;
    }
    else
    {
      *phys_static_array<float,512>::operator[](&v52, i) = 0;
    }
    v37 = &v49 + *phys_static_array<float,512>::operator[](&v52, i);
    ++*v37;
  }
  if ( v52.m_alloc_count < 512 )
  {
    v35 = &v52.m_slot_array[v52.m_alloc_count++];
    v34 = v35;
    v36 = v35;
  }
  else
  {
    tlFatal("phys array add overflow.");
    v36 = 0;
  }
  *v36 = *phys_static_array<float,512>::operator[](&v52, 0);
  if ( v53.m_alloc_count < 512 )
  {
    v32 = &v53.m_slot_array[v53.m_alloc_count++];
    v31 = v32;
    v33 = (float *)v32;
  }
  else
  {
    tlFatal("phys array add overflow.");
    v33 = 0;
  }
  *v33 = *(float *)phys_static_array<float,512>::operator[](&v53, 0);
  if ( v49 )
  {
    if ( v50 )
    {
      v29.m_slot_array = (phys_vec3 *const)&v29;
      v29.m_alloc_count = 0;
      for ( j = 0; ; ++j )
      {
        v27 = winding->m_alloc_count;
        if ( j >= v27 )
          break;
        v26 = phys_static_array<phys_vec3,512>::operator[](winding, j);
        if ( *phys_static_array<float,512>::operator[](&v52, j) == 2 )
        {
          v25 = phys_static_array<phys_vec3,512>::add(&v29, 0, "phys array add overflow.");
          v25->x = v26->x;
          v25->y = v26->y;
          v25->z = v26->z;
        }
        else
        {
          if ( !*phys_static_array<float,512>::operator[](&v52, j) )
          {
            v24 = phys_static_array<phys_vec3,512>::add(&v29, 0, "phys array add overflow.");
            v24->x = v26->x;
            v24->y = v26->y;
            v24->z = v26->z;
          }
          if ( *phys_static_array<float,512>::operator[](&v52, j + 1) != 2 )
          {
            v4 = phys_static_array<float,512>::operator[](&v52, j + 1);
            if ( *v4 != *phys_static_array<float,512>::operator[](&v52, j) )
            {
              v23 = phys_static_array<phys_vec3,512>::operator[](winding, (j + 1) % m_alloc_count);
              v5 = phys_static_array<float,512>::operator[](&v53, j);
              v6 = phys_static_array<float,512>::operator[](&v53, j);
              v22 = *(float *)v5
                  / (float)(*(float *)v6 - *(float *)phys_static_array<float,512>::operator[](&v53, j + 1));
              for ( k = 0; k < 3; ++k )
              {
                if ( k < 0
                  && _tlAssert(
                       "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                       32,
                       "i >= 0 && i < 3",
                       &toastPopupTitle) )
                {
                  __debugbreak();
                }
                if ( *(&n.x + k) == 1.0 )
                {
                  v7 = phys_vec3::operator[]<int>(&v20, k);
                  *v7 = d;
                }
                else if ( *phys_vec3::operator[]<int>(&n, k) == -1.0 )
                {
                  v19 = LODWORD(d) ^ _mask__NegFloat_;
                  *(unsigned int *)phys_vec3::operator[]<int>(&v20, k) = v19;
                }
                else
                {
                  v8 = phys_vec3::operator[]<int>(v26, k);
                  v9 = phys_vec3::operator[]<int>(v23, k);
                  v10 = phys_vec3::operator[]<int>(v26, k);
                  v18 = (float)((float)(*v9 - *v10) * v22) + *v8;
                  *phys_vec3::operator[]<int>(&v20, k) = v18;
                }
              }
              v11 = phys_static_array<phys_vec3,512>::add(&v29, 0, "phys array add overflow.");
              phys_vec3::operator=(v11, &v20);
            }
          }
        }
      }
      for ( m = 0; m < winding->m_alloc_count; ++m )
        ;
      winding->m_alloc_count = 0;
      v16 = v29.m_alloc_count;
      for ( ii = 0; ii < v16; ++ii )
      {
        v13 = phys_static_array<phys_vec3,512>::operator[](&v29, ii);
        v12 = phys_static_array<phys_vec3,512>::add(winding, 0, "phys array add overflow.");
        phys_vec3::operator=(v12, v13);
      }
      for ( jj = 0; jj < v29.m_alloc_count; ++jj )
        ;
      phys_static_array<float,512>::call_destructors(&v52);
      phys_static_array<float,512>::call_destructors(&v53);
    }
    else
    {
      phys_static_array<float,512>::call_destructors(&v52);
      phys_static_array<float,512>::call_destructors(&v53);
    }
  }
  else
  {
    for ( kk = 0; kk < winding->m_alloc_count; ++kk )
      ;
    winding->m_alloc_count = 0;
    phys_static_array<float,512>::call_destructors(&v52);
    phys_static_array<float,512>::call_destructors(&v53);
  }
}

void  init_winding(float a1@<ebp>, const plane_lt *plane, phys_static_array<phys_vec3,512> *winding)
{
  float v3; // xmm0_4
  phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  const phys_vec3 *v6; // eax
  phys_vec3 *v7; // eax
  const phys_vec3 *v8; // eax
  const phys_vec3 *v9; // eax
  phys_vec3 *v10; // eax
  const phys_vec3 *v11; // eax
  const phys_vec3 *v12; // eax
  phys_vec3 *v13; // eax
  phys_vec3 v14; // [esp-6Ch] [ebp-19Ch] BYREF
  phys_vec3 v15; // [esp-5Ch] [ebp-18Ch] BYREF
  phys_vec3 v16; // [esp-4Ch] [ebp-17Ch] BYREF
  phys_vec3 v17; // [esp-3Ch] [ebp-16Ch] BYREF
  phys_vec3 v18; // [esp-2Ch] [ebp-15Ch] BYREF
  phys_vec3 v19; // [esp-1Ch] [ebp-14Ch] BYREF
  float v20[3]; // [esp-Ch] [ebp-13Ch] BYREF
  phys_vec3 p; // [esp+0h] [ebp-130h]
  float v22; // [esp+18h] [ebp-118h]
  float v23; // [esp+1Ch] [ebp-114h]
  float v24; // [esp+20h] [ebp-110h]
  float v25; // [esp+24h] [ebp-10Ch]
  float v26; // [esp+28h] [ebp-108h]
  float v27; // [esp+2Ch] [ebp-104h]
  float v28; // [esp+38h] [ebp-F8h]
  float v29; // [esp+3Ch] [ebp-F4h]
  float v30; // [esp+40h] [ebp-F0h]
  float v31; // [esp+44h] [ebp-ECh] BYREF
  float v32; // [esp+48h] [ebp-E8h]
  float v33; // [esp+4Ch] [ebp-E4h]
  phys_vec3 right; // [esp+50h] [ebp-E0h] BYREF
  float v35; // [esp+70h] [ebp-C0h]
  float v36; // [esp+74h] [ebp-BCh]
  float v37; // [esp+78h] [ebp-B8h]
  float v38; // [esp+7Ch] [ebp-B4h]
  float v39; // [esp+80h] [ebp-B0h]
  float v40; // [esp+84h] [ebp-ACh]
  float v41; // [esp+88h] [ebp-A8h]
  float len; // [esp+8Ch] [ebp-A4h]
  float v43; // [esp+94h] [ebp-9Ch]
  float v44; // [esp+98h] [ebp-98h]
  float v45; // [esp+9Ch] [ebp-94h]
  float v46; // [esp+A0h] [ebp-90h]
  int v47; // [esp+A4h] [ebp-8Ch]
  int v48; // [esp+A8h] [ebp-88h]
  float proj; // [esp+ACh] [ebp-84h]
  float v50; // [esp+B4h] [ebp-7Ch] BYREF
  float v51; // [esp+B8h] [ebp-78h]
  float v52; // [esp+BCh] [ebp-74h]
  phys_vec3 up; // [esp+C0h] [ebp-70h]
  int v54; // [esp+DCh] [ebp-54h]
  bool v55; // [esp+E0h] [ebp-50h]
  int v56; // [esp+E4h] [ebp-4Ch]
  int axis; // [esp+E8h] [ebp-48h]
  int v58; // [esp+ECh] [ebp-44h]
  phys_vec3 abs_normal; // [esp+F0h] [ebp-40h]
  int v60; // [esp+100h] [ebp-30h]
  float v61; // [esp+104h] [ebp-2Ch]
  int v62; // [esp+108h] [ebp-28h]
  float v63; // [esp+10Ch] [ebp-24h]
  float d; // [esp+110h] [ebp-20h]
  float x; // [esp+114h] [ebp-1Ch] BYREF
  float y; // [esp+118h] [ebp-18h]
  float dist; // [esp+11Ch] [ebp-14h]
  phys_vec3 normal; // [esp+120h] [ebp-10h]
  float retaddr; // [esp+130h] [ebp+0h]

  normal.y = a1;
  normal.z = retaddr;
  x = plane->n.x;
  y = plane->n.y;
  dist = plane->n.z;
  normal.x = plane->n.w;
  d = plane->d;
  v63 = dist;
  v62 = LODWORD(dist) & _mask__AbsFloat_;
  v61 = y;
  v60 = LODWORD(y) & _mask__AbsFloat_;
  abs_normal.w = x;
  LODWORD(abs_normal.z) = LODWORD(x) & _mask__AbsFloat_;
  v56 = LODWORD(x) & _mask__AbsFloat_;
  axis = LODWORD(y) & _mask__AbsFloat_;
  v58 = LODWORD(dist) & _mask__AbsFloat_;
  v55 = COERCE_FLOAT(LODWORD(y) & _mask__AbsFloat_) > COERCE_FLOAT(LODWORD(x) & _mask__AbsFloat_);
  v54 = v55;
  if ( COERCE_FLOAT(LODWORD(dist) & _mask__AbsFloat_) <= *((float *)&v56 + v55) )
  {
    v47 = *(unsigned int *)&FLOAT_0_0;
    v48 = *(unsigned int *)&FLOAT_0_0;
    proj = FLOAT_1_0;
    v50 = *(float *)&FLOAT_0_0;
    v51 = *(float *)&FLOAT_0_0;
    v3 = FLOAT_1_0;
  }
  else
  {
    v54 = 2;
    up.y = FLOAT_1_0;
    up.z = *(float *)&FLOAT_0_0;
    up.w = *(float *)&FLOAT_0_0;
    v50 = FLOAT_1_0;
    v51 = *(float *)&FLOAT_0_0;
    v3 = *(float *)&FLOAT_0_0;
  }
  v46 = (float)((float)(v50 * x) + (float)(v51 * y)) + (float)(v3 * dist);
  v45 = v46 * x;
  v44 = v46 * y;
  v43 = v46 * dist;
  v40 = v46 * x;
  v41 = v46 * y;
  len = v46 * dist;
  v50 = v50 - (float)(v46 * x);
  v51 = v51 - (float)(v46 * y);
  v52 = v3 - (float)(v46 * dist);
  v39 = Abs(&v50);
  if ( v39 != 0.0 )
  {
    v38 = 1.0 / v39;
    v50 = v50 * (float)(1.0 / v39);
    v51 = v51 * (float)(1.0 / v39);
    v52 = v52 * (float)(1.0 / v39);
  }
  v37 = d * x;
  v36 = d * y;
  v35 = d * dist;
  right.y = d * x;
  right.z = d * y;
  right.w = d * dist;
  phys_cross((phys_vec3 *)&v31, (const phys_vec3 *)&v50, (const phys_vec3 *)&x);
  v50 = v50 * 131072.0;
  v51 = v51 * 131072.0;
  v52 = v52 * 131072.0;
  v31 = v31 * 131072.0;
  v32 = v32 * 131072.0;
  v33 = v33 * 131072.0;
  v30 = right.y - v31;
  v29 = right.z - v32;
  v28 = right.w - v33;
  v25 = right.y - v31;
  v26 = right.z - v32;
  v27 = right.w - v33;
  v24 = (float)(right.y - v31) + v50;
  v23 = (float)(right.z - v32) + v51;
  v22 = (float)(right.w - v33) + v52;
  p.y = v24;
  p.z = v23;
  p.w = v22;
  v20[0] = v24;
  v20[1] = v23;
  v20[2] = v22;
  v4 = phys_static_array<phys_vec3,512>::add(winding, 0, "phys array add overflow.");
  phys_vec3::operator=(v4, (const phys_vec3 *)v20);
  v5 = operator+(&v19, (phys_vec3 *)&right.y, (const phys_vec3 *)&v31);
  v6 = operator+(&v18, v5, (const phys_vec3 *)&v50);
  phys_vec3::operator=((phys_vec3 *)v20, v6);
  v7 = phys_static_array<phys_vec3,512>::add(winding, 0, "phys array add overflow.");
  phys_vec3::operator=(v7, (const phys_vec3 *)v20);
  v8 = operator+(&v17, (phys_vec3 *)&right.y, (const phys_vec3 *)&v31);
  v9 = operator-(&v16, v8, (const phys_vec3 *)&v50);
  phys_vec3::operator=((phys_vec3 *)v20, v9);
  v10 = phys_static_array<phys_vec3,512>::add(winding, 0, "phys array add overflow.");
  phys_vec3::operator=(v10, (const phys_vec3 *)v20);
  v11 = operator-(&v15, (phys_vec3 *)&right.y, (const phys_vec3 *)&v31);
  v12 = operator-(&v14, v11, (const phys_vec3 *)&v50);
  phys_vec3::operator=((phys_vec3 *)v20, v12);
  v13 = phys_static_array<phys_vec3,512>::add(winding, 0, "phys array add overflow.");
  phys_vec3::operator=(v13, (const phys_vec3 *)v20);
}

// local variable allocation has failed, the output may be wrong!
void  calc_winding(
        int a1@<ebp>,
        phys_static_array<plane_lt,512> *planes,
        int plane_index,
        phys_static_array<phys_vec3,512> *winding)
{
  int j; // [esp-38h] [ebp-68h]
  _BYTE v5[44]; // [esp-Ch] [ebp-3Ch] OVERLAPPED BYREF
  int i; // [esp+20h] [ebp-10h]
  int nplanes; // [esp+24h] [ebp-Ch] BYREF
  void *v8; // [esp+28h] [ebp-8h] OVERLAPPED
  void *retaddr; // [esp+30h] [ebp+0h]

  nplanes = a1;
  v8 = retaddr;
  i = (int)phys_static_array<plane_lt,512>::operator[](planes, plane_index);
  init_winding(COERCE_FLOAT(&nplanes), (const plane_lt *)i, winding);
  v5[43] = 0;
  *(unsigned int *)&v5[36] = planes->m_alloc_count;
  *(unsigned int *)&v5[32] = 0;
  while ( *(int *)&v5[32] < *(int *)&v5[36] )
  {
    if ( *(unsigned int *)&v5[32] == plane_index )
    {
      v5[43] = 1;
    }
    else
    {
      memcpy(v5, phys_static_array<plane_lt,512>::operator[](planes, *(int *)&v5[32]), 0x20u);
      if ( (float)((float)((float)(*(float *)i * *(float *)v5) + (float)(*(float *)(i + 4) * *(float *)&v5[4]))
                 + (float)(*(float *)(i + 8) * *(float *)&v5[8])) <= 0.99989998
        || COERCE_FLOAT(COERCE_UNSIGNED_INT(*(float *)(i + 16) - *(float *)&v5[16]) & _mask__AbsFloat_) >= 0.001 )
      {
        *(unsigned int *)v5 ^= _mask__NegFloat_;
        *(unsigned int *)&v5[4] ^= _mask__NegFloat_;
        *(unsigned int *)&v5[8] ^= _mask__NegFloat_;
        *(unsigned int *)&v5[16] ^= _mask__NegFloat_;
        clip_winding((int)&nplanes, winding, (const plane_lt *)v5);
        if ( winding->m_alloc_count < 3 )
          return;
      }
      else if ( v5[43] )
      {
        for ( j = 0; j < winding->m_alloc_count; ++j )
          ;
        winding->m_alloc_count = 0;
        return;
      }
    }
    ++*(unsigned int *)&v5[32];
  }
}

void __cdecl Phys_DebugPoint(const phys_vec3 *pos, float radius, const float *color)
{
  float point[3]; // [esp+0h] [ebp-Ch] BYREF

  Phys_NitrousVecToVec3(pos, point);
  G_DebugStar(point, color, 0);
}

void __cdecl Phys_DebugBox(const phys_mat44 *mat, const float *color, float scale)
{
  float origin[3]; // [esp+24h] [ebp-4Ch] BYREF
  float mins[3]; // [esp+30h] [ebp-40h] BYREF
  float rotation[3][3]; // [esp+3Ch] [ebp-34h] BYREF
  float size; // [esp+60h] [ebp-10h]
  float maxs[3]; // [esp+64h] [ebp-Ch] BYREF

  size = FLOAT_7_0;
  mins[0] = scale * COERCE_FLOAT(LODWORD(FLOAT_7_0) ^ _mask__NegFloat_);
  mins[1] = scale * -3.0;
  mins[2] = scale * -3.0;
  maxs[0] = scale * 7.0;
  maxs[1] = scale * 3.0;
  maxs[2] = scale * 3.0;
  Phys_NitrousVecToVec3(&mat->w, origin);
  Phys_NitrousMat44ToVec33(mat, rotation);
  CG_DebugBoxOriented(origin, mins, maxs, rotation, color, 0, 0);
}

void __cdecl Phys_DebugBox(const phys_mat44 *mat, const phys_vec3 *dims, const float *color)
{
  float origin[3]; // [esp+24h] [ebp-54h] BYREF
  float mins[3]; // [esp+30h] [ebp-48h] BYREF
  float _dims[3]; // [esp+3Ch] [ebp-3Ch] BYREF
  float rotation[3][3]; // [esp+48h] [ebp-30h] BYREF
  float maxs[3]; // [esp+6Ch] [ebp-Ch] BYREF

  Phys_NitrousVecToVec3(dims, _dims);
  maxs[0] = _dims[0];
  maxs[1] = _dims[1];
  maxs[2] = _dims[2];
  mins[0] = -1.0 * _dims[0];
  mins[1] = -1.0 * _dims[1];
  mins[2] = -1.0 * _dims[2];
  Phys_NitrousVecToVec3(&mat->w, origin);
  Phys_NitrousMat44ToVec33(mat, rotation);
  CG_DebugBoxOriented(origin, mins, maxs, rotation, color, 0, 0);
}

void  Phys_RenderBase(float a1@<ebp>, const phys_mat44 *mat, float scale)
{
  phys_vec3 v3; // [esp-CCh] [ebp-DCh] BYREF
  float v4; // [esp-B8h] [ebp-C8h]
  float v5; // [esp-B4h] [ebp-C4h]
  float v6; // [esp-B0h] [ebp-C0h]
  float v7; // [esp-ACh] [ebp-BCh]
  float v8; // [esp-A8h] [ebp-B8h]
  float v9; // [esp-A4h] [ebp-B4h]
  float v10; // [esp-9Ch] [ebp-ACh]
  float v11; // [esp-98h] [ebp-A8h]
  float v12; // [esp-94h] [ebp-A4h]
  phys_vec3 *p_z; // [esp-90h] [ebp-A0h]
  phys_vec3 v14; // [esp-8Ch] [ebp-9Ch] BYREF
  float v15; // [esp-78h] [ebp-88h]
  float v16; // [esp-74h] [ebp-84h]
  float v17; // [esp-70h] [ebp-80h]
  float v18; // [esp-6Ch] [ebp-7Ch]
  float v19; // [esp-68h] [ebp-78h]
  float v20; // [esp-64h] [ebp-74h]
  float v21; // [esp-5Ch] [ebp-6Ch]
  float v22; // [esp-58h] [ebp-68h]
  float v23; // [esp-54h] [ebp-64h]
  phys_vec3 *p_y; // [esp-50h] [ebp-60h]
  phys_vec3 v25; // [esp-4Ch] [ebp-5Ch] BYREF
  float v26; // [esp-38h] [ebp-48h]
  float v27; // [esp-34h] [ebp-44h]
  float v28; // [esp-30h] [ebp-40h]
  float v29; // [esp-2Ch] [ebp-3Ch]
  float v30; // [esp-28h] [ebp-38h]
  float v31; // [esp-24h] [ebp-34h]
  float v32; // [esp-18h] [ebp-28h]
  float v33; // [esp-14h] [ebp-24h]
  float v34; // [esp-10h] [ebp-20h]
  float x; // [esp-Ch] [ebp-1Ch] BYREF
  float y; // [esp-8h] [ebp-18h]
  float z; // [esp-4h] [ebp-14h]
  phys_vec3 trans; // [esp+0h] [ebp-10h]
  float retaddr; // [esp+10h] [ebp+0h]

  trans.y = a1;
  trans.z = retaddr;
  x = mat->w.x;
  y = mat->w.y;
  z = mat->w.z;
  trans.x = mat->w.w;
  v34 = scale * mat->x.x;
  v33 = scale * mat->x.y;
  v32 = scale * mat->x.z;
  v29 = v34;
  v30 = v33;
  v31 = v32;
  v28 = x + v34;
  v27 = y + v33;
  v26 = z + v32;
  v25.x = x + v34;
  v25.y = y + v33;
  v25.z = z + v32;
  render_line((const phys_vec3 *)&x, &v25, colorMdRed, 0, 0);
  p_y = &mat->y;
  v23 = scale * mat->y.x;
  v22 = scale * mat->y.y;
  v21 = scale * mat->y.z;
  v18 = v23;
  v19 = v22;
  v20 = v21;
  v17 = x + v23;
  v16 = y + v22;
  v15 = z + v21;
  v14.x = x + v23;
  v14.y = y + v22;
  v14.z = z + v21;
  render_line((const phys_vec3 *)&x, &v14, colorMdGreen, 0, 0);
  p_z = &mat->z;
  v12 = scale * mat->z.x;
  v11 = scale * mat->z.y;
  v10 = scale * mat->z.z;
  v7 = v12;
  v8 = v11;
  v9 = v10;
  v6 = x + v12;
  v5 = y + v11;
  v4 = z + v10;
  v3.x = x + v12;
  v3.y = y + v11;
  v3.z = z + v10;
  render_line((const phys_vec3 *)&x, &v3, colorMdBlue, 0, 0);
}

void  Phys_JointDebugRender(rigid_body *a1@<ebp>, int id)
{
  const phys_vec3 *v2; // eax
  const phys_vec3 *v3; // eax
  const phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  phys_vec3 *v6; // eax
  const phys_vec3 *v7; // eax
  phys_vec3 *v8; // eax
  const phys_vec3 *v9; // eax
  phys_vec3 *v10; // eax
  const phys_vec3 *v11; // eax
  const phys_vec3 *v12; // eax
  phys_vec3 *v13; // eax
  const phys_vec3 *v14; // eax
  const phys_vec3 *v15; // eax
  phys_vec3 *v16; // eax
  const phys_vec3 *v17; // eax
  const phys_vec3 *scale; // [esp+Ch] [ebp-3C0h]
  const phys_vec3 *scalea; // [esp+Ch] [ebp-3C0h]
  const phys_vec3 *scaleb; // [esp+Ch] [ebp-3C0h]
  phys_vec3 v21; // [esp+10h] [ebp-3BCh] BYREF
  phys_vec3 v22; // [esp+20h] [ebp-3ACh] BYREF
  phys_vec3 v23; // [esp+30h] [ebp-39Ch] BYREF
  phys_vec3 v24; // [esp+40h] [ebp-38Ch] BYREF
  phys_vec3 v25; // [esp+50h] [ebp-37Ch] BYREF
  phys_vec3 v26; // [esp+60h] [ebp-36Ch] BYREF
  phys_vec3 v27; // [esp+70h] [ebp-35Ch] BYREF
  phys_vec3 v28; // [esp+80h] [ebp-34Ch] BYREF
  _BYTE v29[12]; // [esp+90h] [ebp-33Ch] BYREF
  phys_vec3 pt2; // [esp+9Ch] [ebp-330h] BYREF
  phys_vec3 pt1; // [esp+ACh] [ebp-320h] BYREF
  phys_vec3 v32; // [esp+C0h] [ebp-30Ch] BYREF
  phys_vec3 v33; // [esp+D0h] [ebp-2FCh] BYREF
  phys_vec3 v34; // [esp+E0h] [ebp-2ECh] BYREF
  _BYTE v35[12]; // [esp+F0h] [ebp-2DCh] BYREF
  phys_vec3 p2; // [esp+FCh] [ebp-2D0h] BYREF
  phys_vec3 p1; // [esp+10Ch] [ebp-2C0h] BYREF
  int v38; // [esp+12Ch] [ebp-2A0h]
  phys_vec3 v39; // [esp+130h] [ebp-29Ch] BYREF
  phys_vec3 v40; // [esp+140h] [ebp-28Ch] BYREF
  float v41; // [esp+150h] [ebp-27Ch]
  float v42; // [esp+154h] [ebp-278h]
  float v43; // [esp+164h] [ebp-268h]
  float v44; // [esp+168h] [ebp-264h]
  float v45; // [esp+16Ch] [ebp-260h]
  float v46; // [esp+170h] [ebp-25Ch]
  float v47; // [esp+174h] [ebp-258h]
  float v48; // [esp+178h] [ebp-254h]
  float v49; // [esp+184h] [ebp-248h]
  float v50; // [esp+188h] [ebp-244h]
  float v51; // [esp+18Ch] [ebp-240h]
  float v52; // [esp+190h] [ebp-23Ch]
  float v53; // [esp+194h] [ebp-238h]
  float v54; // [esp+198h] [ebp-234h]
  float v55; // [esp+1A4h] [ebp-228h]
  float v56; // [esp+1A8h] [ebp-224h]
  float v57; // [esp+1ACh] [ebp-220h]
  float v58; // [esp+1B0h] [ebp-21Ch]
  float v59; // [esp+1B4h] [ebp-218h]
  float v60; // [esp+1B8h] [ebp-214h]
  float v61; // [esp+1C4h] [ebp-208h]
  float v62; // [esp+1C8h] [ebp-204h]
  float v63; // [esp+1CCh] [ebp-200h]
  float v64; // [esp+1D0h] [ebp-1FCh]
  float v65; // [esp+1D4h] [ebp-1F8h]
  float v66; // [esp+1D8h] [ebp-1F4h]
  float v67; // [esp+1E4h] [ebp-1E8h]
  float v68; // [esp+1E8h] [ebp-1E4h]
  float v69; // [esp+1ECh] [ebp-1E0h]
  float v70; // [esp+1F0h] [ebp-1DCh]
  float v71; // [esp+1F4h] [ebp-1D8h]
  float v72; // [esp+1F8h] [ebp-1D4h]
  float v73; // [esp+204h] [ebp-1C8h]
  float v74; // [esp+208h] [ebp-1C4h]
  float v75; // [esp+20Ch] [ebp-1C0h]
  float v76; // [esp+210h] [ebp-1BCh]
  float v77; // [esp+214h] [ebp-1B8h]
  float v78; // [esp+218h] [ebp-1B4h]
  float v79; // [esp+224h] [ebp-1A8h]
  float v80; // [esp+228h] [ebp-1A4h]
  float v81; // [esp+22Ch] [ebp-1A0h]
  float v82; // [esp+230h] [ebp-19Ch]
  float v83; // [esp+234h] [ebp-198h]
  float v84; // [esp+238h] [ebp-194h]
  float v85; // [esp+244h] [ebp-188h]
  float v86; // [esp+248h] [ebp-184h]
  float v87; // [esp+24Ch] [ebp-180h]
  float v88; // [esp+250h] [ebp-17Ch] BYREF
  float v89; // [esp+254h] [ebp-178h]
  float v90; // [esp+258h] [ebp-174h]
  phys_vec3 phys_vec_end; // [esp+25Ch] [ebp-170h]
  float v92; // [esp+274h] [ebp-158h]
  float v93; // [esp+278h] [ebp-154h]
  float v94; // [esp+27Ch] [ebp-150h]
  float v95; // [esp+280h] [ebp-14Ch]
  float v96; // [esp+284h] [ebp-148h]
  float v97; // [esp+288h] [ebp-144h]
  float v98; // [esp+294h] [ebp-138h]
  float v99; // [esp+298h] [ebp-134h]
  float v100; // [esp+29Ch] [ebp-130h]
  float v101; // [esp+2A0h] [ebp-12Ch] BYREF
  float v102; // [esp+2A4h] [ebp-128h]
  float v103; // [esp+2A8h] [ebp-124h]
  phys_vec3 b2_ref_max; // [esp+2ACh] [ebp-120h] BYREF
  phys_vec3 b2_ref_min; // [esp+2BCh] [ebp-110h] BYREF
  phys_vec3 b1_ref; // [esp+2CCh] [ebp-100h] BYREF
  _BYTE v107[12]; // [esp+2E0h] [ebp-ECh] BYREF
  phys_mat44 rv; // [esp+2ECh] [ebp-E0h] BYREF
  float v109[3]; // [esp+330h] [ebp-9Ch] BYREF
  phys_vec3 phys_vec_start2; // [esp+33Ch] [ebp-90h] BYREF
  int v111; // [esp+35Ch] [ebp-70h]
  float v112; // [esp+360h] [ebp-6Ch] BYREF
  float v113; // [esp+364h] [ebp-68h]
  float v114; // [esp+368h] [ebp-64h]
  phys_vec3 phys_vec_start; // [esp+36Ch] [ebp-60h] BYREF
  int v116; // [esp+38Ch] [ebp-40h]
  float v117; // [esp+390h] [ebp-3Ch] BYREF
  float v118; // [esp+394h] [ebp-38h]
  float v119; // [esp+398h] [ebp-34h]
  phys_vec3 b2_axis; // [esp+39Ch] [ebp-30h] BYREF
  phys_vec3 b1_axis; // [esp+3ACh] [ebp-20h]
  rigid_body *b2; // [esp+3BCh] [ebp-10h]
  rigid_body *b1; // [esp+3C0h] [ebp-Ch] BYREF
  rigid_body_constraint_ragdoll *joint; // [esp+3C4h] [ebp-8h]
  rigid_body_constraint_ragdoll *retaddr; // [esp+3CCh] [ebp+0h]

  b1 = a1;
  joint = retaddr;
  *(float *)&b2 = FLOAT_15_0;
  if ( !id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp", 1266, 0, "%s", "id") )
    __debugbreak();
  LODWORD(b1_axis.w) = id;
  b1_axis.z = *(float *)id;
  b1_axis.y = *(float *)(id + 4);
  if ( (*(unsigned int *)(id + 48) & 8) != 0 )
  {
    phys_multiply(
      (phys_vec3 *)&b2_axis.y,
      (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 96));
    phys_multiply(
      (const phys_vec3 *)&v117,
      (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 112));
    v116 = LODWORD(b1_axis.z) + 48;
    scale = (const phys_vec3 *)(LODWORD(b1_axis.z) + 96);
    v2 = phys_multiply(
           (phys_vec3 *)&phys_vec_start.y,
           (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
           (const phys_vec3 *)(LODWORD(b1_axis.w) + 16));
    operator+((phys_vec3 *)&v112, v2, scale);
    v111 = LODWORD(b1_axis.y) + 48;
    scalea = (const phys_vec3 *)(LODWORD(b1_axis.y) + 96);
    v3 = phys_multiply(
           (phys_vec3 *)&phys_vec_start2.y,
           (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
           (const phys_vec3 *)(LODWORD(b1_axis.w) + 32));
    operator+((phys_vec3 *)v109, v3, scalea);
    Phys_NitrousVecToVec3((const phys_vec3 *)v109, &rv.w.z);
    G_DebugStar(&rv.w.z, colorLtGreen, 0);
    make_rotate(COERCE_FLOAT(&b1), (phys_mat44 *)v107, (phys_vec3 *)&b2_axis.y, (const phys_vec3 *)&v117);
    v4 = phys_multiply(
           (phys_vec3 *)&b1_ref.y,
           (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
           (const phys_vec3 *)(LODWORD(b1_axis.w) + 160));
    phys_multiply((phys_vec3 *)&b2_ref_min.y, (const phys_mat44 *)v107, v4);
    phys_multiply(
      (phys_vec3 *)&b2_ref_max.y,
      (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 176));
    phys_multiply(
      (const phys_vec3 *)&v101,
      (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 192));
    v100 = *(float *)&b2 * b2_ref_min.y;
    v99 = *(float *)&b2 * b2_ref_min.z;
    v98 = *(float *)&b2 * b2_ref_min.w;
    v95 = *(float *)&b2 * b2_ref_min.y;
    v96 = *(float *)&b2 * b2_ref_min.z;
    v97 = *(float *)&b2 * b2_ref_min.w;
    v94 = v112 + (float)(*(float *)&b2 * b2_ref_min.y);
    v93 = v113 + (float)(*(float *)&b2 * b2_ref_min.z);
    v92 = v114 + (float)(*(float *)&b2 * b2_ref_min.w);
    phys_vec_end.y = v94;
    phys_vec_end.z = v93;
    phys_vec_end.w = v92;
    v88 = v94;
    v89 = v93;
    v90 = v92;
    render_line((const phys_vec3 *)&v112, (const phys_vec3 *)&v88, colorMdGreen, 0, 0);
    v87 = *(float *)&b2 * b2_ref_max.y;
    v86 = *(float *)&b2 * b2_ref_max.z;
    v85 = *(float *)&b2 * b2_ref_max.w;
    v82 = *(float *)&b2 * b2_ref_max.y;
    v83 = *(float *)&b2 * b2_ref_max.z;
    v84 = *(float *)&b2 * b2_ref_max.w;
    v81 = v112 + (float)(*(float *)&b2 * b2_ref_max.y);
    v80 = v113 + (float)(*(float *)&b2 * b2_ref_max.z);
    v79 = v114 + (float)(*(float *)&b2 * b2_ref_max.w);
    v76 = v81;
    v77 = v80;
    v78 = v79;
    v75 = *(float *)&b2 * v101;
    v74 = *(float *)&b2 * v102;
    v73 = *(float *)&b2 * v103;
    v70 = *(float *)&b2 * v101;
    v71 = *(float *)&b2 * v102;
    v72 = *(float *)&b2 * v103;
    v69 = v112 + (float)(*(float *)&b2 * v101);
    v68 = v113 + (float)(*(float *)&b2 * v102);
    v67 = v114 + (float)(*(float *)&b2 * v103);
    v64 = v69;
    v65 = v68;
    v66 = v67;
    v63 = *(float *)&b2 * b2_axis.y;
    v62 = *(float *)&b2 * b2_axis.z;
    v61 = *(float *)&b2 * b2_axis.w;
    v58 = *(float *)&b2 * b2_axis.y;
    v59 = *(float *)&b2 * b2_axis.z;
    v60 = *(float *)&b2 * b2_axis.w;
    v57 = v112 + (float)(*(float *)&b2 * b2_axis.y);
    v56 = v113 + (float)(*(float *)&b2 * b2_axis.z);
    v55 = v114 + (float)(*(float *)&b2 * b2_axis.w);
    v52 = v57;
    v53 = v56;
    v54 = v55;
    v88 = v57;
    v89 = v56;
    v90 = v55;
    render_line((const phys_vec3 *)&v112, (const phys_vec3 *)&v88, colorLtGrey, 0, 0);
    v51 = *(float *)&b2 * v117;
    v50 = *(float *)&b2 * v118;
    v49 = *(float *)&b2 * v119;
    v46 = *(float *)&b2 * v117;
    v47 = *(float *)&b2 * v118;
    v48 = *(float *)&b2 * v119;
    v45 = v112 + (float)(*(float *)&b2 * v117);
    v44 = v113 + (float)(*(float *)&b2 * v118);
    v43 = v114 + (float)(*(float *)&b2 * v119);
    v41 = v45;
    v42 = v44;
    v88 = v45;
    v89 = v44;
    v90 = v43;
    render_line((const phys_vec3 *)&v112, (const phys_vec3 *)&v88, colorOrange, 0, 0);
    Phys_DebugBox((const phys_mat44 *)(LODWORD(b1_axis.z) + 48), colorWhiteFaded, 1.0);
    Phys_DebugBox((const phys_mat44 *)(LODWORD(b1_axis.y) + 48), colorWhiteFaded, 1.0);
  }
  else if ( (*(unsigned int *)(LODWORD(b1_axis.w) + 48) & 4) != 0 )
  {
    if ( render_pivots )
    {
      phys_multiply(&v40, (const phys_mat44 *)(LODWORD(b1_axis.z) + 48), (const phys_vec3 *)(LODWORD(b1_axis.w) + 96));
      phys_multiply(&v39, (const phys_mat44 *)(LODWORD(b1_axis.y) + 48), (const phys_vec3 *)(LODWORD(b1_axis.w) + 112));
      v38 = LODWORD(b1_axis.z) + 48;
      scaleb = (const phys_vec3 *)(LODWORD(b1_axis.z) + 96);
      v5 = phys_multiply(
             (phys_vec3 *)&p1.y,
             (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
             (const phys_vec3 *)(LODWORD(b1_axis.w) + 16));
      operator+((phys_vec3 *)&p2.y, v5, scaleb);
      phys_full_multiply(
        (int)&b1,
        (const phys_vec3 *)v35,
        (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
        (const phys_vec3 *)(LODWORD(b1_axis.w) + 32));
      v6 = operator*(&v34, &v40, l1);
      v7 = operator+(&v33, (phys_vec3 *)&p2.y, v6);
      render_line((phys_vec3 *)&p2.y, v7, colorMdGreen, 0, 0);
      v8 = operator*(&v32, &v39, l2);
      v9 = operator+((phys_vec3 *)&pt1.y, (const phys_vec3 *)v35, v8);
      render_line((const phys_vec3 *)v35, v9, colorMdGreen, 0, 0);
      Phys_RenderBase(COERCE_FLOAT(&b1), (const phys_mat44 *)(LODWORD(b1_axis.z) + 48), l1);
      Phys_RenderBase(COERCE_FLOAT(&b1), (const phys_mat44 *)(LODWORD(b1_axis.y) + 48), l2);
    }
    phys_full_multiply(
      (int)&b1,
      (phys_vec3 *)&pt2.y,
      (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 16));
    phys_multiply(
      (const phys_vec3 *)v29,
      (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
      (const phys_vec3 *)(LODWORD(b1_axis.w) + 176));
    v10 = operator*(&v28, (const phys_vec3 *)v29, line_length);
    v11 = operator+(&v27, (phys_vec3 *)&pt2.y, v10);
    phys_vec3::operator=((phys_vec3 *)v29, v11);
    v12 = phys_multiply(
            &v26,
            (const phys_mat44 *)(LODWORD(b1_axis.y) + 48),
            (const phys_vec3 *)(LODWORD(b1_axis.w) + 192));
    phys_vec3::operator=((phys_vec3 *)v29, v12);
    v13 = operator*(&v25, (const phys_vec3 *)v29, line_length);
    v14 = operator+(&v24, (phys_vec3 *)&pt2.y, v13);
    phys_vec3::operator=((phys_vec3 *)v29, v14);
    v15 = phys_multiply(
            &v23,
            (const phys_mat44 *)(LODWORD(b1_axis.z) + 48),
            (const phys_vec3 *)(LODWORD(b1_axis.w) + 160));
    phys_vec3::operator=((phys_vec3 *)v29, v15);
    v16 = operator*(&v22, (const phys_vec3 *)v29, line_length);
    v17 = operator+(&v21, (phys_vec3 *)&pt2.y, v16);
    phys_vec3::operator=((phys_vec3 *)v29, v17);
    render_line((phys_vec3 *)&pt2.y, (const phys_vec3 *)v29, colorYellow, 0, 0);
  }
}

void  make_rotate(float a1@<ebp>, phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2)
{
  float v4; // [esp+8h] [ebp-2Ch]
  float v5; // [esp+Ch] [ebp-28h]
  float len; // [esp+14h] [ebp-20h]
  float lena; // [esp+14h] [ebp-20h]
  float co_; // [esp+18h] [ebp-1Ch] BYREF
  float v9; // [esp+1Ch] [ebp-18h]
  float si_; // [esp+20h] [ebp-14h]
  phys_vec3 ud; // [esp+24h] [ebp-10h]
  float retaddr; // [esp+34h] [ebp+0h]

  ud.y = a1;
  ud.z = retaddr;
  phys_cross((phys_vec3 *)&co_, v1, v2);
  len = (float)((float)(co_ * co_) + (float)(v9 * v9)) + (float)(si_ * si_);
  if ( len >= 0.0000099999997 )
  {
    lena = fsqrt(len);
    co_ = co_ * (float)(1.0 / lena);
    v9 = v9 * (float)(1.0 / lena);
    si_ = si_ * (float)(1.0 / lena);
    v5 = (float)((float)(v1->x * v2->x) + (float)(v1->y * v2->y)) + (float)(v1->z * v2->z);
    v4 = fsqrt((float)(v5 * v5) + (float)(lena * lena));
    make_rotate(mat, (const phys_vec3 *)&co_, v5 / v4, lena / v4);
  }
  else
  {
    phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_44);
  }
}

void  render_contact(contact_point_info *a1@<ebp>, rigid_body_constraint_contact *rbc)
{
  const phys_vec3 *v2; // eax
  const phys_vec3 *p_w; // [esp-50h] [ebp-130h]
  phys_vec3 v4; // [esp-4Ch] [ebp-12Ch] BYREF
  float v5; // [esp-38h] [ebp-118h]
  float v6; // [esp-34h] [ebp-114h]
  float v7; // [esp-30h] [ebp-110h]
  phys_vec3 v8; // [esp-2Ch] [ebp-10Ch] BYREF
  float v9; // [esp-18h] [ebp-F8h]
  float v10; // [esp-14h] [ebp-F4h]
  float v11; // [esp-10h] [ebp-F0h]
  float v12; // [esp-Ch] [ebp-ECh]
  float v13; // [esp-8h] [ebp-E8h]
  float v14; // [esp-4h] [ebp-E4h]
  phys_vec3 dims; // [esp+0h] [ebp-E0h]
  float v16; // [esp+10h] [ebp-D0h]
  float v17; // [esp+14h] [ebp-CCh] BYREF
  float w; // [esp+18h] [ebp-C8h]
  float v19; // [esp+1Ch] [ebp-C4h]
  phys_vec3 p1; // [esp+20h] [ebp-C0h]
  float v21; // [esp+30h] [ebp-B0h]
  float v22; // [esp+34h] [ebp-ACh]
  float v23; // [esp+38h] [ebp-A8h]
  float v24; // [esp+3Ch] [ebp-A4h]
  float v25; // [esp+44h] [ebp-9Ch]
  float v26; // [esp+48h] [ebp-98h]
  float v27; // [esp+4Ch] [ebp-94h]
  float v28; // [esp+50h] [ebp-90h]
  int v29; // [esp+54h] [ebp-8Ch]
  int v30; // [esp+58h] [ebp-88h]
  int v31; // [esp+5Ch] [ebp-84h]
  int v32; // [esp+64h] [ebp-7Ch]
  int v33; // [esp+68h] [ebp-78h]
  int v34; // [esp+6Ch] [ebp-74h]
  environment_rigid_body *v35; // [esp+70h] [ebp-70h]
  float x; // [esp+74h] [ebp-6Ch]
  float y; // [esp+78h] [ebp-68h]
  float z; // [esp+7Ch] [ebp-64h]
  phys_vec3 hitn; // [esp+80h] [ebp-60h] BYREF
  phys_vec3 p0; // [esp+90h] [ebp-50h] BYREF
  phys_mat44 *p_m_mat; // [esp+ACh] [ebp-34h]
  const phys_vec3 *v42; // [esp+B0h] [ebp-30h]
  rigid_body *b2; // [esp+B4h] [ebp-2Ch]
  int j; // [esp+B8h] [ebp-28h]
  contact_point_info *v45; // [esp+BCh] [ebp-24h]
  contact_point_info *m_next_link; // [esp+C0h] [ebp-20h]
  int i; // [esp+C4h] [ebp-1Ch]
  contact_point_info *cpi; // [esp+C8h] [ebp-18h]
  contact_point_info *m_first; // [esp+CCh] [ebp-14h]
  phys_simple_link_list<contact_point_info>::iterator cpi_i_end; // [esp+D0h] [ebp-10h]
  phys_simple_link_list<contact_point_info>::iterator cpi_i; // [esp+D4h] [ebp-Ch]
  void *v52; // [esp+D8h] [ebp-8h]
  void *retaddr; // [esp+E0h] [ebp+0h]

  cpi_i.m_ptr = a1;
  v52 = retaddr;
  if ( rbc )
  {
    cpi_i_end.m_ptr = (contact_point_info *)&rbc->m_list_contact_point_info_buffer_1;
    m_first = rbc->m_list_contact_point_info_buffer_1.m_first;
    cpi = m_first;
    i = 0;
    while ( (contact_point_info *)i != cpi )
    {
      v45 = cpi;
      if ( cpi->m_point_pair_count <= 0
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_render.cpp",
             1361,
             "cpi.m_point_pair_count > 0",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      for ( j = 0; j < v45->m_point_pair_count; ++j )
      {
        b2 = rbc->b2;
        v42 = &v45->m_list_b2_r_loc[j];
        p_m_mat = &b2->m_mat;
        p_w = &b2->m_mat.w;
        v2 = phys_multiply((phys_vec3 *)&p0.y, &b2->m_mat, v42);
        operator+((phys_vec3 *)&hitn.y, v2, p_w);
        x = v45->m_normal.x;
        y = v45->m_normal.y;
        z = v45->m_normal.z;
        hitn.x = v45->m_normal.w;
        v35 = (environment_rigid_body *)rbc->b2;
        if ( v35 == phys_sys::get_environment_rigid_body() )
        {
          v34 = LODWORD(x) ^ _mask__NegFloat_;
          v33 = LODWORD(y) ^ _mask__NegFloat_;
          v32 = LODWORD(z) ^ _mask__NegFloat_;
          v29 = LODWORD(x) ^ _mask__NegFloat_;
          v30 = LODWORD(y) ^ _mask__NegFloat_;
          v31 = LODWORD(z) ^ _mask__NegFloat_;
          LODWORD(x) ^= _mask__NegFloat_;
          LODWORD(y) ^= _mask__NegFloat_;
          LODWORD(z) ^= _mask__NegFloat_;
        }
        v28 = scale_4;
        v27 = scale_4 * x;
        v26 = scale_4 * y;
        v25 = scale_4 * z;
        v22 = scale_4 * x;
        v23 = scale_4 * y;
        v24 = scale_4 * z;
        v21 = hitn.y + (float)(scale_4 * x);
        p1.w = hitn.z + (float)(scale_4 * y);
        p1.z = hitn.w + (float)(scale_4 * z);
        v17 = v21;
        w = p1.w;
        v19 = p1.z;
        render_line((phys_vec3 *)&hitn.y, (const phys_vec3 *)&v17, colorWhite, 0, 0);
        v16 = box_size;
        dims.w = box_size;
        dims.z = box_size;
        v12 = box_size;
        v13 = box_size;
        v14 = box_size;
        v11 = v17 + box_size;
        v10 = w + box_size;
        v9 = v19 + box_size;
        v8.x = v17 + box_size;
        v8.y = w + box_size;
        v8.z = v19 + box_size;
        v7 = v17 - box_size;
        v6 = w - box_size;
        v5 = v19 - box_size;
        v4.x = v17 - box_size;
        v4.y = w - box_size;
        v4.z = v19 - box_size;
        render_box(&v4, &v8, colorWhite, 0);
      }
      m_next_link = cpi->m_next_link;
      cpi = m_next_link;
    }
  }
}

void __cdecl render_collision_tree(const CollisionAabbTree *tree, const float *color)
{
  float *var6C; // [esp+10h] [ebp-6Ch]
  float *v3; // [esp+14h] [ebp-68h]
  float *v4; // [esp+18h] [ebp-64h]
  float mins[3]; // [esp+1Ch] [ebp-60h] BYREF
  float maxs[3]; // [esp+28h] [ebp-54h] BYREF
  float v7[4]; // [esp+34h] [ebp-48h] BYREF
  int triIndex; // [esp+44h] [ebp-38h]
  float v0[3]; // [esp+48h] [ebp-34h] BYREF
  float v2[3]; // [esp+54h] [ebp-28h] BYREF
  float v1[3]; // [esp+60h] [ebp-1Ch] BYREF
  const CollisionPartition *partition; // [esp+6Ch] [ebp-10h]
  float center[3]; // [esp+70h] [ebp-Ch] BYREF

  partition = &cm.partitions[tree->u.firstChildIndex];
  for ( triIndex = partition->firstTri; triIndex < partition->firstTri + partition->triCount; ++triIndex )
  {
    v4 = cm.verts[cm.triIndices[3 * triIndex]];
    v0[0] = *v4;
    v0[1] = v4[1];
    v0[2] = v4[2];
    v3 = cm.verts[cm.triIndices[3 * triIndex + 1]];
    v1[0] = *v3;
    v1[1] = v3[1];
    v1[2] = v3[2];
    var6C = cm.verts[cm.triIndices[3 * triIndex + 2]];
    v2[0] = *var6C;
    v2[1] = var6C[1];
    v2[2] = var6C[2];
    CG_DebugLine(v0, v1, color, 1, 0);
    CG_DebugLine(v0, v2, color, 1, 0);
    CG_DebugLine(v1, v2, color, 1, 0);
    center[0] = (float)(v0[0] + v1[0]) + v2[0];
    center[1] = (float)(v0[1] + v1[1]) + v2[1];
    center[2] = (float)(v0[2] + v1[2]) + v2[2];
    center[0] = 0.33333334 * center[0];
    center[1] = 0.33333334 * center[1];
    center[2] = 0.33333334 * center[2];
    LODWORD(mins[0]) = LODWORD(rad) ^ _mask__NegFloat_;
    LODWORD(mins[1]) = LODWORD(rad) ^ _mask__NegFloat_;
    LODWORD(mins[2]) = LODWORD(rad) ^ _mask__NegFloat_;
    maxs[0] = rad;
    maxs[1] = rad;
    maxs[2] = rad;
    v7[0] = FLOAT_1_0;
    v7[1] = *(float *)&FLOAT_0_0;
    v7[2] = *(float *)&FLOAT_0_0;
    v7[3] = FLOAT_1_0;
    CG_DebugBox(center, mins, maxs, 0.0, v7, 1, 0);
  }
}

void __cdecl render_prims(col_prim_t *prims, int nprims)
{
  col_prim_t *prim; // [esp+0h] [ebp-8h]
  int pi; // [esp+4h] [ebp-4h]
  int savedregs; // [esp+8h] [ebp+0h] BYREF

  for ( pi = 0; pi < nprims; ++pi )
  {
    prim = &prims[pi];
    if ( prim->type )
      render_brush(COERCE_FLOAT(&savedregs), prim->brush, 0, 0, 1, 0, 0, 0, 0);
    else
      render_collision_tree(prim->tree, colorWhite);
  }
}

void __cdecl render_debug_draw_gjk_trace_geom()
{
  GjkTraceGeom::Render();
}

void __cdecl GjkTraceGeom::Render()
{
  int i; // [esp+0h] [ebp-4h]
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  if ( g_debugRenderGjkTraceGeom->current.enabled )
  {
    for ( i = 0; i < GjkTraceGeom::nGeoms; ++i )
      render_brush(
        COERCE_FLOAT(&savedregs),
        GjkTraceGeom::geoms[i].brush,
        (const phys_mat44 *)((char *)&unk_9C2C160 + 80 * i),
        0,
        1,
        0,
        0,
        0,
        0);
  }
  GjkTraceGeom::nGeoms = 0;
}

plane_lt *__thiscall phys_static_array<plane_lt,512>::add(
        phys_static_array<plane_lt,512> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 512 )
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

const plane_lt *__thiscall phys_static_array<plane_lt,512>::operator[](phys_static_array<plane_lt,512> *this, int i)
{
  if ( i < 0 || i >= this->m_alloc_count )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
           124,
           "i >= 0 && i < m_alloc_count",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return &this->m_slot_array[i];
}

phys_vec3 *__thiscall phys_static_array<phys_vec3,512>::add(
        phys_static_array<phys_vec3,512> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 512 )
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

phys_vec3 *__thiscall phys_static_array<phys_vec3,512>::operator[](phys_static_array<phys_vec3,512> *this, int i)
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

int *__thiscall phys_static_array<float,512>::operator[](phys_static_array<int,512> *this, int i)
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

void __thiscall phys_static_array<float,512>::call_destructors(phys_static_array<int,512> *this)
{
  int i; // [esp+4h] [ebp-4h]

  for ( i = 0; i < this->m_alloc_count; ++i )
    ;
}

const float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i)
{
  if ( i > 2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
         32,
         "i >= 0 && i < 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return (const float *)this + i;
}

