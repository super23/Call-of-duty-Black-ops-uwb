#pragma once

void __cdecl RMsg_DropAllPacketsForClient(unsigned int clientSlot);
int __cdecl RMsg_FindSlotForAddr(netadr_t *to);
void __cdecl RMsg_Init();
void __cdecl RMsg_SendMessages();
void __cdecl RMsg_AddRMsgsToPacket(unsigned int clientSlot, msg_t *outMsg);
int __cdecl RMsg_FindMessageSlot(unsigned int clientSlot, unsigned __int8 sequenceNum);
int __cdecl RMsg_FindOldestSeq(unsigned int clientSlot);
void __cdecl RMsg_AckSequence(netadr_t *from, unsigned __int8 sequenceNum);
void __cdecl RMsg_MarkPacketReceived(netadr_t *from, unsigned __int8 sequenceNum);
void __cdecl RMsg_AckMsgSlot(reliableClient *client, unsigned int clientSlot, unsigned int msgSlot);
int __cdecl RMsg_CountOutstandingMessages(unsigned int clientSlot);
