#pragma once

void __cdecl Live_FileShareSearch_LoadSummary(int controllerIndex, int itemIndex);
char __cdecl Live_FileShareSearch_IsTaskPermitted();
int __cdecl Live_FileShareSearch_GetNextTaskDelay();
int __cdecl Live_FileShareSearch_GetTaskGap();
void __cdecl Live_FileShareSearch_LoadSummarySuccess();
void __cdecl Live_FileShareSearch_ReportSuccess();
void __cdecl Live_FileShareSearch_LoadSummaryFailure();
void __cdecl Live_FileShareSearch_ReportFailure();
void __cdecl Live_FileShareSearch_SetIndexOverride(int controllerIndex, int itemIndex);
int __cdecl Live_FileShareSearch_GetTotalFiles();
void __cdecl Live_FileShareSearch_SetContext(
        int controllerIndex,
        bdTag *tags,
        int numTags,
        fileShareLocation location,
        bool sortHighestRated,
        int numPastDays);
int Live_FileShareSearch_ClearSearch();
char __cdecl Live_FileShareSearch_NeedToPerformSearch(
        bdTag *tags,
        int numTags,
        fileShareLocation location,
        int fileType,
        bool sortHighestRated,
        int numPastDays);
void __cdecl Live_FileShareSearch_LoadBook(int controllerIndex, int itemIndex, bool forceTask);
int __cdecl Live_FileShareSearch_GetFirstBookItem(int fileIndex);
bool __cdecl Live_FileShareSearch_IsBookLoading(int controllerIndex);
void __cdecl Live_FileShareSearch_LoadBookSuccess();
void __cdecl Live_FileShareSearch_LoadRatingsFailure();
bool __cdecl Live_FileShareSearch_LoadDescriptor(int controllerIndex, int startIndex, int selectedIndex, int itemIndex);
char __cdecl Live_FileShareSearch_LoadItem(int controllerIndex, int startIndex, int selectedIndex, int itemIndex);
char __cdecl Live_FileShareSearch_IsPageLoaded(int fileIndex);
void __cdecl Live_FileShareSearch_LoadPage(int controllerIndex, int startIndex, int itemIndex);
int __cdecl Live_FileShareSearch_GetFirstPageItem(int fileIndex);
bool __cdecl Live_FileShareSearch_IsPageLoading(int controllerIndex);
fshSearchPage_t *__cdecl Live_FileShareSearch_AllocatePage(int startIndex);
bool __cdecl Live_FileShareSearch_IsBookLoaded(int itemIndex);
bool __cdecl Live_FileShareSearch_AreRatingsLoaded();
void __cdecl Live_FileShareSearch_LoadRatings(int controllerIndex);
bool __cdecl Live_FileShareSearch_AreRatingsLoading(int controllerIndex);
void __cdecl Live_FileShareSearch_LoadRatingsSuccess();
char __cdecl Live_FileShareSearch_GetDescriptor(int controllerIndex, int itemIndex, bdFileMetaData *outDescriptor);
char __cdecl Live_FileShareSearch_FindDescriptor(
        int controllerIndex,
        int itemIndex,
        bdFileMetaData *outDescriptor,
        fshSearchPage_t **page);
bdFileMetaData *__thiscall bdFileMetaData::operator=(bdFileMetaData *this, const bdFileMetaData *__that);
char __cdecl Live_FileShareSearch_GetDesriptorByID(
        int controllerIndex,
        unsigned __int64 fileID,
        fileShareLocation location,
        bdFileMetaData *outDescriptor);
char __cdecl Live_FileShareSearch_FindDescriptorByID(
        int controllerIndex,
        unsigned __int64 fileID,
        fileShareLocation location,
        bdFileMetaData *outDescriptor);
void __cdecl Live_FileShareSearch_GetRatings(bdVoteRankStatsInfo **ratings, int *numRatings);
void __cdecl Live_FileShareSearch_MarkItemCorrupt(int controllerIndex, int itemIndex);
bool __cdecl Live_FileShareSearch_IsSummaryLoaded();
char __cdecl Live_FileShareSearch_FeederItemEnabled(int index);
char __cdecl Live_FileShareSearch_FeederColor(
        listBoxDef_s *listPtr,
        itemDef_s *item,
        int contextIndex,
        int index,
        int column,
        DvarValue *color);
fshSearch_t *__thiscall fshSearch_t::fshSearch_t(fshSearch_t *this);
fshSearchBookFiles_t *__thiscall fshSearchBookFiles_t::fshSearchBookFiles_t(fshSearchBookFiles_t *this);
void __thiscall fshSearch_t::~fshSearch_t(fshSearch_t *this);
void __thiscall fshSearchBookFiles_t::~fshSearchBookFiles_t(fshSearchBookFiles_t *this);
void __thiscall fshSearchContext_t::~fshSearchContext_t(fshSearchContext_t *this);
