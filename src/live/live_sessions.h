#pragma once

int __cdecl Session_BuildQoSPayload();
unsigned __int8 *__cdecl Session_GetQosPayloadBuffer();
bool __cdecl Session_IsHost();
int __cdecl Session_HostNum(SessionData_s *session);
