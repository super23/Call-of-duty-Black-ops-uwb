#include "recorder.h"

#include <cstring>
#include <qcommon/cmd.h>
#include <qcommon/common.h>
#include <qcommon/threads.h>
#include <win32/win_common.h>
#include <universal/com_memory.h>
#include <universal/dvar.h>
#include <universal/physicalmemory.h>
#include <clientscript/cscr_vm.h>

const dvar_s *recorder_enableRec;
const dvar_s *recorder_debugMemory;
#ifdef KISAK_SP
extern const dvar_s *cg_paused;
#else
const dvar_s *cg_paused;
#endif

namespace Recorder
{

struct ReplayMgr;

struct ReplayMemBuffer
{
    unsigned __int8 *buffer;
    unsigned int capacity;
    unsigned int used;
    unsigned int frameCount;

    static void Reset(int result);
    static int Alloc(int a1, int a2, int size);
};

struct CameraInfo
{
    float origin[3];
    float angles[3];

    static void Reset();
    static int LoadCamera(int a1, float *viewAngles);
    static int SaveCamera(float *origin, float *viewAngles);
};

struct PrimitiveMgr
{
    bool recordingEnabled;
    bool pad[3];

    static PrimitiveMgr *Inst();
    static PrimitiveMgr *CreateInst();
    static void *DeleteInst();
    static void Reset(int a1);
    static ReplayMgr *Render(int a1, int a2, int a3, int a4, int a5, int a6);
    static void PostPrimitive(int a1, int a2, int a3, int a4, int a5);
};

struct ReplayMgr
{
    int mode;
    int pad0;
    ReplayMemBuffer memBuffer;
    CameraInfo cameraInfo;
    float recordedCamPos[3];

    static ReplayMgr *Inst();
    static ReplayMgr *CreateInst();
    static int DeleteInst(int a1, int a2, int a3, int a4, int a5);
    static void Reset(int a1);
    static void SetMode(int a1, int mode);
    static void Update(int **a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10,
        int a11, int a12, int a13, int a14, int a15, int a16);
    static void Record(unsigned __int8 *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9,
        int a10, int a11, int a12, int a13, int a14);
    static void Render(ReplayMgr *self, int a2, int a3);
    static void RegisterRecorder(unsigned int *a1, int a2, int a3);
    static void UnregisterRecorder(int **a1, int a2);
};

static ReplayMgr *s_replayMgrInstance;
static PrimitiveMgr *s_primitiveMgrInstance;
static void *s_recorderMemPool;
static bool s_recorderScriptsLoaded;
static bool s_skipNextRecord;
static float s_tanHalfFovX;
static float s_tanHalfFovY;

static void RegisterRecorderDvars()
{
    if (!recorder_enableRec)
        recorder_enableRec = _Dvar_RegisterBool("recorder_enableRec", true, 0, "Enable theater recorder");
    if (!recorder_debugMemory)
        recorder_debugMemory = _Dvar_RegisterBool("recorder_debugMemory", false, 0, "Debug recorder memory");
#ifndef KISAK_SP
    if (!cg_paused)
        cg_paused = _Dvar_RegisterBool("cg_paused", false, 0, "Game paused");
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c:7021669
int MemInit()
{
    RegisterRecorderDvars();
    if (s_recorderMemPool)
        return 1;
    PMem_BeginAlloc("recorder", 1u, TRACK_RECORDER);
    s_recorderMemPool = Hunk_Alloc(0x400000, "MemInit", 0);
    return s_recorderMemPool != nullptr;
}

// Decomp: CoDSP_rdBlackOps.map.c:7021669
void *MemDelete()
{
    s_recorderMemPool = nullptr;
    return nullptr;
}

// Decomp: CoDSP_rdBlackOps.map.c — stub (empty body in SP port)
void MemResize()
{
}

// Decomp: CoDSP_rdBlackOps.map.c
int MemForReplay()
{
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c
void *MemAlloc(unsigned int size)
{
    if (!s_recorderMemPool)
        MemInit();
    return Hunk_Alloc(size, "Recorder::MemAlloc", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
int MemFree(int result)
{
    return result;
}

// Decomp: CoDSP_rdBlackOps.map.c
void MemDebug()
{
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMemBuffer::Reset(int result)
{
    ReplayMemBuffer *self = (ReplayMemBuffer *)result;
    if (self)
    {
        self->used = 0;
        self->frameCount = 0;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c
int ReplayMemBuffer::Alloc(int a1, int a2, int size)
{
    ReplayMemBuffer *self = (ReplayMemBuffer *)a1;
    unsigned int newUsed;
    (void)a2;
    if (!self || size <= 0)
        return 0;
    newUsed = self->used + (unsigned int)size;
    if (newUsed > self->capacity)
        return 0;
    self->used = newUsed;
    return (int)(self->buffer + self->used - size);
}

// Decomp: CoDSP_rdBlackOps.map.c
ReplayMgr *ReplayMgr::Inst()
{
    return s_replayMgrInstance;
}

// Decomp: CoDSP_rdBlackOps.map.c
ReplayMgr *ReplayMgr::CreateInst()
{
    if (!s_replayMgrInstance)
    {
        s_replayMgrInstance = (ReplayMgr *)MemAlloc(sizeof(ReplayMgr));
        if (s_replayMgrInstance)
        {
            memset(s_replayMgrInstance, 0, sizeof(ReplayMgr));
            s_replayMgrInstance->memBuffer.buffer = (unsigned __int8 *)MemAlloc(0x200000);
            s_replayMgrInstance->memBuffer.capacity = 0x200000;
            s_replayMgrInstance->memBuffer.used = 0;
            s_replayMgrInstance->memBuffer.frameCount = 0;
            s_replayMgrInstance->mode = RECORDER_MODE_RECORD;
        }
    }
    return s_replayMgrInstance;
}

// Decomp: CoDSP_rdBlackOps.map.c
int ReplayMgr::DeleteInst(int a1, int a2, int a3, int a4, int a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    s_replayMgrInstance = nullptr;
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::Reset(int a1)
{
    ReplayMgr *self = (ReplayMgr *)a1;
    if (!self)
        return;
    self->mode = RECORDER_MODE_RECORD;
    ReplayMemBuffer::Reset((int)&self->memBuffer);
    CameraInfo::Reset();
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::SetMode(int a1, int mode)
{
    ReplayMgr *self = (ReplayMgr *)a1;
    if (self)
        self->mode = mode;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::Update(int **a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10,
    int a11, int a12, int a13, int a14, int a15, int a16)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    (void)a9; (void)a10; (void)a11; (void)a12; (void)a13; (void)a14; (void)a15; (void)a16;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::Record(unsigned __int8 *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9,
    int a10, int a11, int a12, int a13, int a14)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8; (void)a9;
    (void)a10; (void)a11; (void)a12; (void)a13; (void)a14;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::Render(ReplayMgr *self, int a2, int a3)
{
    (void)self; (void)a2; (void)a3;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::RegisterRecorder(unsigned int *a1, int a2, int a3)
{
    (void)a1; (void)a2; (void)a3;
}

// Decomp: CoDSP_rdBlackOps.map.c
void ReplayMgr::UnregisterRecorder(int **a1, int a2)
{
    (void)a1; (void)a2;
}

// Decomp: CoDSP_rdBlackOps.map.c
PrimitiveMgr *PrimitiveMgr::Inst()
{
    return s_primitiveMgrInstance;
}

// Decomp: CoDSP_rdBlackOps.map.c
PrimitiveMgr *PrimitiveMgr::CreateInst()
{
    if (!s_primitiveMgrInstance)
    {
        s_primitiveMgrInstance = (PrimitiveMgr *)MemAlloc(sizeof(PrimitiveMgr));
        if (s_primitiveMgrInstance)
            memset(s_primitiveMgrInstance, 0, sizeof(PrimitiveMgr));
    }
    return s_primitiveMgrInstance;
}

// Decomp: CoDSP_rdBlackOps.map.c
void *PrimitiveMgr::DeleteInst()
{
    s_primitiveMgrInstance = nullptr;
    return nullptr;
}

// Decomp: CoDSP_rdBlackOps.map.c
void PrimitiveMgr::Reset(int a1)
{
    PrimitiveMgr *self = (PrimitiveMgr *)a1;
    if (self)
        self->recordingEnabled = false;
}

// Decomp: CoDSP_rdBlackOps.map.c
ReplayMgr *PrimitiveMgr::Render(int a1, int a2, int a3, int a4, int a5, int a6)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6;
    return ReplayMgr::Inst();
}

// Decomp: CoDSP_rdBlackOps.map.c
void PrimitiveMgr::PostPrimitive(int a1, int a2, int a3, int a4, int a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
}

// Decomp: CoDSP_rdBlackOps.map.c
void CameraInfo::Reset()
{
}

// Decomp: CoDSP_rdBlackOps.map.c
PrimitiveMgr *Init()
{
    if (!MemInit())
        return nullptr;
    ReplayMgr::CreateInst();
    return PrimitiveMgr::CreateInst();
}

// Decomp: CoDSP_rdBlackOps.map.c
void *Free(int a1, int a2)
{
    (void)a1; (void)a2;
    if (s_replayMgrInstance)
        ReplayMgr::DeleteInst(0, 0, 0, 0, 0);
    if (s_primitiveMgrInstance)
        PrimitiveMgr::DeleteInst();
    return MemDelete();
}

// Decomp: CoDSP_rdBlackOps.map.c
void Reset()
{
    if (s_replayMgrInstance)
        ReplayMgr::Reset((int)s_replayMgrInstance);
    ResetPrims();
}

// Decomp: CoDSP_rdBlackOps.map.c
void ResetPrims()
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (s_primitiveMgrInstance)
        PrimitiveMgr::Reset((int)s_primitiveMgrInstance);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDSP_rdBlackOps.map.c
void Update(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10,
    int a11, int a12, int a13, int a14)
{
    s_tanHalfFovX = (float)a9;
    s_tanHalfFovY = (float)a10;
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    (void)a11; (void)a12; (void)a13; (void)a14;
}

// Decomp: CoDSP_rdBlackOps.map.c:7021741
void Record()
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (!s_recorderScriptsLoaded)
    {
        Cbuf_InsertText(0, "exec devgui_recorder\n");
        Cbuf_InsertText(0, "exec recorder\n");
        s_recorderScriptsLoaded = true;
    }
    if (s_replayMgrInstance && !s_skipNextRecord && cg_paused && !cg_paused->current.enabled)
    {
        ReplayMgr::Update((int **)s_replayMgrInstance, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
        ReplayMgr::Record((unsigned __int8 *)s_replayMgrInstance, 0, 0, 0, 0, 0, 0, 0, 0.0, 0, 0, 0, 0, 0);
    }
    s_skipNextRecord = false;
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDSP_rdBlackOps.map.c:7021741
void Render()
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (s_replayMgrInstance)
    {
        if (IsEnableRec())
        {
            const int prevMode = s_replayMgrInstance->mode;
            ReplayMgr::Update((int **)s_replayMgrInstance, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
            s_skipNextRecord = prevMode != s_replayMgrInstance->mode;
            if (!s_replayMgrInstance->mode || !s_skipNextRecord)
                ReplayMgr::Render(s_replayMgrInstance, 0, 0);
        }
    }
    if (s_primitiveMgrInstance)
        PrimitiveMgr::Render((int)s_primitiveMgrInstance, 0, 0, 0, 0, 0);
    if (recorder_debugMemory && recorder_debugMemory->current.enabled)
        MemDebug();
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDSP_rdBlackOps.map.c:7021669
int ChangeCamera(int result)
{
    float *viewAngles = (float *)result;
    if (!s_replayMgrInstance)
        return result;
    if (s_replayMgrInstance->mode == RECORDER_MODE_PLAYBACK)
        return CameraInfo::LoadCamera((int)&s_replayMgrInstance->cameraInfo, viewAngles);
    return CameraInfo::SaveCamera(s_replayMgrInstance->cameraInfo.origin, viewAngles);
}

// Decomp: CoDSP_rdBlackOps.map.c
void TogglePlayback()
{
    if (!s_replayMgrInstance)
        return;
    if (s_replayMgrInstance->mode == RECORDER_MODE_PLAYBACK)
        ReplayMgr::SetMode((int)s_replayMgrInstance, RECORDER_MODE_RECORD);
    else
        ReplayMgr::SetMode((int)s_replayMgrInstance, RECORDER_MODE_PLAYBACK);
}

// Decomp: CoDSP_rdBlackOps.map.c
int IsInPlayback()
{
    return s_replayMgrInstance && s_replayMgrInstance->mode == RECORDER_MODE_PLAYBACK;
}

// Decomp: CoDSP_rdBlackOps.map.c
int IsEnableRec()
{
    return s_replayMgrInstance && recorder_enableRec && recorder_enableRec->current.enabled;
}

// Decomp: CoDSP_rdBlackOps.map.c
void RegisterRecorder(int a1, int a2)
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (s_replayMgrInstance)
        ReplayMgr::RegisterRecorder((unsigned int *)s_replayMgrInstance, a1, a2);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDSP_rdBlackOps.map.c
void UnregisterRecorder(int a1)
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (s_replayMgrInstance)
        ReplayMgr::UnregisterRecorder((int **)s_replayMgrInstance, a1);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDSP_rdBlackOps.map.c
void PostPrimitive(int a1, int a2, int a3, int a4)
{
    if (!a2 || !s_primitiveMgrInstance)
        return;
    if (s_primitiveMgrInstance->recordingEnabled)
        PrimitiveMgr::PostPrimitive((int)s_primitiveMgrInstance, a1, a2, a3, a4);
}

// Decomp: CoDSP_rdBlackOps.map.c
void PostPrimitive(int a1, int a2, int a3)
{
    PostPrimitive(a1, a2, a3, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
int CameraInfo::LoadCamera(int a1, float *viewAngles)
{
    CameraInfo *self = (CameraInfo *)a1;
    if (self && viewAngles)
    {
        viewAngles[0] = self->angles[0];
        viewAngles[1] = self->angles[1];
        viewAngles[2] = self->angles[2];
    }
    return a1;
}

// Decomp: CoDSP_rdBlackOps.map.c
int CameraInfo::SaveCamera(float *origin, float *viewAngles)
{
    if (s_replayMgrInstance && viewAngles)
    {
        s_replayMgrInstance->cameraInfo.angles[0] = viewAngles[0];
        s_replayMgrInstance->cameraInfo.angles[1] = viewAngles[1];
        s_replayMgrInstance->cameraInfo.angles[2] = viewAngles[2];
        if (origin)
        {
            s_replayMgrInstance->cameraInfo.origin[0] = origin[0];
            s_replayMgrInstance->cameraInfo.origin[1] = origin[1];
            s_replayMgrInstance->cameraInfo.origin[2] = origin[2];
        }
    }
    return 0;
}

} // namespace Recorder

// Decomp: CoDSP_rdBlackOps.map.c
BOOL Recorder_PlayingBack()
{
    return Recorder::IsInPlayback();
}

// Decomp: CoDSP_rdBlackOps.map.c
char *Recorder_GetRecordedCamPos()
{
    if (!Recorder::ReplayMgr::Inst())
        return nullptr;
    return (char *)&Recorder::ReplayMgr::Inst()->recordedCamPos[0];
}

// Decomp: CoDSP_rdBlackOps.map.c
void Recorder_TogglePlayback_f()
{
    Recorder::TogglePlayback();
}

// Decomp: CoDSP_rdBlackOps.map.c
void Recorder_MemResize_f()
{
    Recorder::MemResize();
}

// Decomp: CoDSP_rdBlackOps.map.c
void GScr_Record3DText()
{
    if (!Recorder::IsEnableRec())
        return;
    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3)
        Scr_Error(SCRIPTINSTANCE_SERVER, "illegal call to Record3DText()", 0);
    Recorder::PostPrimitive(0, 1, 0, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
void GScr_RecordEntText()
{
    if (!Recorder::IsEnableRec())
        return;
    Recorder::PostPrimitive(0, 1, 0, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
void GScr_RecordLine()
{
    if (!Recorder::IsEnableRec())
        return;
    Recorder::PostPrimitive(0, 1, 0, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
void GScr_RecordEnt()
{
    if (!Recorder::IsEnableRec())
        return;
    Recorder::PostPrimitive(0, 1, 0, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c
void GScr_ReportFilm()
{
    Scr_AddBool(Recorder::IsInPlayback(), SCRIPTINSTANCE_SERVER);
}
