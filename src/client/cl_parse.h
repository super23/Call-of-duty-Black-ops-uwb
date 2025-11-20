#pragma once

void __cdecl CL_ParsePacketMatchState(
        clientActive_t *cl,
        msg_t *msg,
        int time,
        clSnapshot_t *oldframe,
        clSnapshot_t *newframe);
