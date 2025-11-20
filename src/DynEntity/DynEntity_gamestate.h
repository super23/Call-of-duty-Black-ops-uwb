#pragma once

void __cdecl DynEnt_WriteGameState(msg_t *msg);
void __cdecl MSG_WritePosition(msg_t *msg, float *origin);
void __cdecl MSG_WriteQuat(msg_t *msg, float *quat);
void __cdecl DynEnt_ReadGameState(msg_t *msg, int localClientNum);
void __cdecl MSG_ReadPosition(msg_t *msg, float *origin);
void __cdecl MSG_ReadQuat(msg_t *msg, float *quat);
double __cdecl Vec4Normalize(float *v);
