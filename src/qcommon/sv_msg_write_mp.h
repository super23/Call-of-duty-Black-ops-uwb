#pragma once

bool __cdecl MSG_EntityIsLinked(const entityState_s *ent);
void __cdecl MSG_WriteGroundEntityNum(int clientNum, msg_t *msg, int groundEntityNum);
void __cdecl MSG_WriteDeltaField_Default(
        const SnapshotInfo_s *snapInfo,
        msg_t *msg,
        const NetField *field,
        const int *fromF,
        const int *toF);
int __cdecl MSG_WriteDeltaStruct(
        SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        unsigned __int8 *from,
        unsigned __int8 *to,
        int force,
        int numFields,
        int indexBits,
        const NetField *stateFields,
        int bChangeBit,
        int skippedFieldBits);
int __cdecl MSG_WriteAppendedDeltaStruct(
        SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        unsigned __int8 *from,
        unsigned __int8 *to,
        int force,
        int numFields,
        int indexBits,
        const NetField *stateFields,
        int bChangeBit,
        int skippedFieldBits);
int __cdecl MSG_WriteEntityDelta(
        const SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        const entityState_s *fromES,
        const entityState_s *toES,
        DeltaFlags flags,
        int numFields,
        int indexBits,
        const NetField *stateFields);
void __cdecl MSG_WriteEntityDeltaFields(
        const SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        const entityState_s *fromES,
        const entityState_s *toES,
        int force,
        int lastChanged,
        int numFields,
        const NetField *stateFields,
        int skippedFieldBits,
        int *fieldsChanged);
bool __cdecl MSG_WriteDeltaArchivedEntity(
        SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        const archivedEntity_s *from,
        const archivedEntity_s *to,
        DeltaFlags flags);
void __cdecl MSG_WriteDeltaHudElems(
        SnapshotInfo_s *snapInfo,
        msg_t *msg,
        int time,
        const hudelem_s *from,
        const hudelem_s *to,
        int count);
