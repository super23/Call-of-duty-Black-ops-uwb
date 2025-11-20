#pragma once

bool __cdecl ProfLoad_IsActive();
void __cdecl ProfLoad_BeginTrackedValue(MapProfileTrackedValue type);
void __cdecl ProfLoad_BeginTrackedValueTicks(MapProfileElement *value, unsigned __int64 ticks);
void __cdecl ProfLoad_EndTrackedValue(MapProfileTrackedValue type);
void __cdecl ProfLoad_EndTrackedValueTicks(MapProfileElement *value, unsigned __int64 ticks);
void __cdecl ProfLoad_Init();
void __cdecl ProfLoad_Activate();
void __cdecl ProfLoad_Deactivate();
void  ProfLoad_Print(MapProfileEntry *a1@<eax>);
MapProfileEntry * ProfLoad_CalculateSelfTicks@<eax>(MapProfileEntry *result@<eax>);
int ProfLoad_PrintTree();
void __cdecl ProfLoad_GetEntryRowText(const MapProfileEntry *entry, char *rowText, unsigned int sizeofRowText);
void ProfLoad_PrintHotSpots();
bool __cdecl ProfLoad_CompareHotSpotNames(const MapProfileHotSpot *hotSpot0, const MapProfileHotSpot *hotSpot1);
bool __cdecl ProfLoad_CompareHotSpotTicks(const MapProfileHotSpot *hotSpot0, const MapProfileHotSpot *hotSpot1);
void __cdecl ProfLoad_Begin(const char *label);
MapProfileEntry *__cdecl Com_GetEntryForNewLabel(const char *label);
void __cdecl ProfLoad_End();
void __cdecl ProfLoad_DrawOverlay(rectDef_s *rect);
int  ProfLoad_DrawTree@<eax>(MapProfileEntry *a1@<eax>);
