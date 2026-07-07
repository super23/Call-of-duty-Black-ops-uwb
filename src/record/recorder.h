#pragma once

#include <universal/q_shared.h>

struct dvar_s;
struct gentity_s;

enum RecorderEReplayMode : int
{
    RECORDER_MODE_RECORD = 0,
    RECORDER_MODE_PLAYBACK = 1,
};

namespace Recorder
{

struct ReplayMgr;
struct PrimitiveMgr;

PrimitiveMgr *Init();
void *Free(int a1, int a2);
void Reset();
void ResetPrims();
void Update(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10,
            int a11, int a12, int a13, int a14);
void Record();
void Render();
int ChangeCamera(int result);
void TogglePlayback();
int IsInPlayback();
int IsEnableRec();
void RegisterRecorder(int a1, int a2);
void UnregisterRecorder(int a1);
void PostPrimitive(int a1, int a2, int a3, int a4);
void PostPrimitive(int a1, int a2, int a3);
void MemResize();

} // namespace Recorder

void Recorder_TogglePlayback_f();
void Recorder_MemResize_f();
void MatchRecorder_ClearDWOverlappedTasks();
int Recorder_PlayingBack();
char *Recorder_GetRecordedCamPos();

void GScr_Record3DText();
void GScr_RecordEntText();
void GScr_RecordLine();
void GScr_RecordEnt();
void GScr_ReportFilm();
