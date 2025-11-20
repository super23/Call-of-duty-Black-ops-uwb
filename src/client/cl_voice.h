#pragma once

void __cdecl CL_WriteVoicePacket(int localClientNum);
void __cdecl CL_VoicePacket(int localClientNum, msg_t *msg);
bool __cdecl CL_IsPlayerTalking(int localClientNum, unsigned int talkingClientIndex);
