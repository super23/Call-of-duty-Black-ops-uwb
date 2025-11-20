#pragma once

void __cdecl Flame_Class_Stream_Init();
flameStream_s *__cdecl Flame_Class_Stream_Alloc(bool is_server);
void __cdecl Flame_Class_Stream_Free(bool is_server, flameStream_s *flameStream);
void __cdecl Flame_Class_Stream_Light_Chunks(const flameStream_s *stream);
void __cdecl Flame_Class_Stream_Age(bool is_server);
void __cdecl Flame_Class_Stream_Fire_Chunks(
        flameSource_t *source,
        flameWeaponConfig_t *weaponConfig,
        int inTime,
        flameRender_s *flameRend);
void __cdecl Flame_Class_Stream_Render_Item(
        int localClientNum,
        const flameStream_s *stream,
        Material *material,
        Material *material2,
        float (*verts)[3],
        bool isFire);
void __cdecl Flame_Class_Stream_Render_All(int localClientNum);
void __cdecl CG_Flame_Render();
