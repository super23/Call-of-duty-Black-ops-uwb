#pragma once

void __cdecl R_CheckDxCaps(const _D3DCAPS9 *caps);
void __cdecl R_RespondToMissingCaps(DxCapsResponse response, const char *msg);
