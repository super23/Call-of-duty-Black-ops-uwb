#pragma once

persistentStats *__cdecl LiveStorage_GetStatsBuffer(
        int controllerIndex,
        statsLocation playerStatsLocation,
        bool verifyLocation);
persistentStats *__cdecl LiveStorage_GetPersStatsBuffer(
        int controllerIndex,
        statsLocation playerStatsLocation,
        bool verifyLocation);
void __cdecl LiveStorage_VerifyCorrectStats(persistentStats *stats, statsLocation location);
void __cdecl LiveStorage_CorrectStatsError(char *msg);
int __cdecl LiveStorage_GetStatsBufferSize();
unsigned __int8 __cdecl LiveStorage_GetStatsChecksumValid(int controllerIndex, statsLocation playerStatsLocation);
void __cdecl LiveStorage_SetStatsChecksumValid(int controllerIndex, statsLocation playerStatsLocation, bool isValid);
bool __cdecl LiveStorage_GetStatsWriteNeeded(int controllerIndex, statsLocation location);
void __cdecl LiveStorage_SetStatsWriteNeeded(int controllerIndex, bool isWriteNeeded, statsLocation location);
int __cdecl LiveStorage_ValidateWithDDL(int controllerIndex, statsLocation location);
unsigned __int8 __cdecl LiveStorage_AreStatsDDLValidated(int controllerIndex, statsLocation playerStatsLocation);
void __cdecl LiveStorage_SetStatsDDLValidated(
        int controllerIndex,
        statsLocation playerStatsLocation,
        bool statsValidatedWithDDL);
void __cdecl LiveStorage_StatsBackupFetchCompleted(int controllerIndex);
unsigned __int8 __cdecl LiveStorage_DoWeHaveStats(int controllerIndex, statsLocation playerStatsLocation);
unsigned __int8 __cdecl LiveStorage_DoWeHaveCurrentStats(int controllerIndex);
bool __cdecl LiveStorage_DoWeHaveAllStats(int controllerIndex);
void __cdecl LiveStorage_SetStatsFetched(int localControllerIndex, statsLocation playerStatsLocation, bool isFetched);
fileSharePrivateData *__cdecl LiveStorage_FileShare_GetFileShareData(fileShareBufferLocation bufferLocation);
void __cdecl LiveStorage_FetchRequiredFiles(int controllerIndex);
TaskRecord *__cdecl LiveStorage_SetupNestedTask(
        const TaskDefinition *taskdef,
        int controllerIndex,
        TaskRecord *nestedTask,
        void *payload);
void __cdecl LiveStorage_ResetFileInfo(dwFileOperationInfo *fileInfo);
void __cdecl LiveStorage_DeleteUserFileFailure(TaskRecord *task);
void __cdecl LiveStorage_DeleteUserFileSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_DeleteDWUserFile(int controllerIndex, dwFileOperationInfo *fileInfo);
void __cdecl LiveStorage_WriteUserFileFailure(TaskRecord *task);
void __cdecl LiveStorage_WriteUserFileSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_WriteDWUserFile(
        int controllerIndex,
        dwFileOperationInfo *fileInfo,
        unsigned __int64 uid);
void __cdecl LiveStorage_GetUserFileFailure(TaskRecord *task);
void __cdecl LiveStorage_GetUserFileSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_ReadDWFile(int controllerIndex, dwFileOperationInfo *fileInfo);
char __cdecl LiveStorage_TryFile(fileRetryInfo *retryInfo);
void __cdecl LiveStorage_UpdateFileTryData(fileRetryInfo *retryInfo);
TaskRecord *__cdecl LiveStorage_ReadDWFileByUserID(
        int controllerIndex,
        dwFileOperationInfo *fileInfo,
        unsigned __int64 xuidLocal);
void __cdecl LiveStorage_RestoreStatsFromBackup(int localControllerIndex);
TaskRecord *__cdecl LiveStorage_ReadStatsBackup(int controllerIndex);
void __cdecl LiveStorage_StatsBackupReadSuccessful(int controllerIndex);
int __cdecl LiveStorage_StatsBackupFileNotFound(int controllerIndex, const char **data);
TaskRecord *__cdecl LiveStorage_SyncTime(int controllerIndex);
void __cdecl LiveStorage_GetServerTimeComplete();
void __cdecl LiveStorage_GetServerTimeFailed();
__int64 __cdecl LiveStorage_GetUTC();
int __cdecl LiveStorage_GetUTCOffset();
bool __cdecl LiveStorage_IsTimeSynced();
TaskRecord *__cdecl LiveStorage_ReadMetPlayerList(
        int controllerIndex,
        char *fileName,
        unsigned __int8 *buffer,
        int sizeOfBuffer);
bool __cdecl LiveStorage_NeedToDownloadMetPlayerList(int controllerIndex);
int __cdecl LiveStorage_ReadMetPlayerListFileNotFound(int controllerIndex);
void __cdecl LiveStorage_SetLastMetPlayerListDownload(int controllerIndex);
void __cdecl LiveStorage_ReadMetPlayerListSuccessful(int controllerIndex);
TaskRecord *__cdecl LiveStorage_WriteMetPlayerList(
        int controllerIndex,
        char *fileName,
        unsigned __int8 *buffer,
        int sizeOfBuffer);
void __cdecl LiveStorage_WriteMetPlayerListSuccessful(int controllerIndex);
TaskRecord *__cdecl LiveStorage_ReadOtherPlayerStats(int controllerIndex, unsigned __int64 playerXuid);
void __cdecl LiveStorage_ReadOtherPlayerStatsSuccessful(int controllerIndex);
int __cdecl LiveStorage_OtherPlayerStatsFileNotFound(int controllerIndex, const char **data);
TaskRecord *__cdecl LiveStorage_ReadPlayerStats(int controllerIndex, bool validate, bool silent);
int __cdecl LiveStorage_PlayerStatsFileNotFound(int controllerIndex, const char **data);
void __cdecl LiveStorage_ReadPlayerStatsSuccessful(int controllerIndex);
TaskRecord *__cdecl LiveStorage_ReadCommonStats(
        int controllerIndex,
        bool silent,
        statsLocation location,
        const TaskDefinition *taskDef,
        dwFileOperationInfo *fileInfo);
TaskRecord *__cdecl LiveStorage_ReadBasicTrainingStats(int controllerIndex, bool validate, bool silent);
int __cdecl LiveStorage_BasicTrainingStatsFileNotFound(int controllerIndex, const char **data);
void __cdecl LiveStorage_ReadBasicTrainingStatsSuccessful(int controllerIndex);
void __cdecl LiveStorage_WritePlayerStatsFailure(TaskRecord *task);
void __cdecl LiveStorage_WriteBasicTrainingStatsFailure(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_WriteStats(
        int controllerIndex,
        bool backup,
        statsLocation location,
        const TaskDefinition *taskDef,
        dwFileOperationInfo *fileInfo);
TaskRecord *__cdecl LiveStorage_WriteBasicTrainingStats(int controllerIndex);
void __cdecl LiveStorage_WriteBackupStats(int controllerIndex);
TaskRecord *__cdecl LiveStorage_WriteBackupStatsInternal(
        int controllerIndex,
        dwFileOperationInfo *fileInfo,
        const TaskDefinition *taskdef);
void __cdecl LiveStorage_BackupCorruptedStats(int controllerIndex, char *filename);
void __cdecl LiveStorage_SetAllStatsNotFetched(int controllerIndex);
void __cdecl LiveStorage_FileShare_UpdateMetaData(char *metaData);
bool __cdecl LiveStorage_FileShare_IsValidHTTPCode(int httpCode);
fileShareLocation __cdecl LiveStorage_FileShare_GetCurrentHTTPLocation(int controllerIndex);
void __cdecl LiveStorage_FileShare_GetDownloadProgress(int controllerIndex, unsigned int *bytes, float *dataRate);
void __cdecl LiveStorage_FileShare_GetUploadProgress(int controllerIndex, unsigned int *bytes, float *dataRate);
void __cdecl LiveStorage_FileShare_AbortOperation(int controllerIndex, fileShareLocation location);
TaskRecord *__cdecl LiveStorage_FileShare_ReadListing(
        int controllerIndex,
        unsigned __int64 playerXuid,
        fileShareBufferLocation bufferLocation);
void __cdecl LiveStorage_FileShare_ReadListingFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_KickOffRatingsTask(
        int controllerIndex,
        fileSharePrivateData *fileShareData,
        int category,
        dwFileShareGetRatingTask *ratingTask,
        const TaskDefinition *sym,
        int *resultOffset);
void __cdecl LiveStorage_FileShare_ReadListingSuccess(TaskRecord *task);
bool __cdecl LiveStorage_FileShare_IsPageLoading(int controllerIndex);
void __cdecl LiveStorage_FileShare_PublicMetaToTags(bdFileMetaData *descriptors, int descriptorCount);
void __cdecl LiveStorage_FileShare_LoadBlockSuccessAction(dwFileShareDescriptorsTask *fsTask);
int __cdecl LiveStorage_FileShareSortComparator_MostRecent(unsigned int *arg0, unsigned int *arg1);
int __cdecl LiveStorage_FileShareSortComparator_MostPopular(const bdFileMetaData *arg0, const bdFileMetaData *arg1);
TaskRecord *__cdecl LiveStorage_FileShare_LoadPage(
        int controllerIndex,
        int numFiles,
        bdFileID *fileIDs,
        fileShareLocation location,
        bdFileMetaData *outDescriptors,
        int *outDescriptorCount,
        void (__cdecl *successCallback)(),
        void (__cdecl *failureCallback)());
void __cdecl LiveStorage_FileShare_LoadBlockFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_LoadBlockSuccess(TaskRecord *task);
void __cdecl LiveStorage_FileShare_DownloadFile_f();
bool __cdecl LiveStorage_FileShare_IsPerformingSearch(int controllerIndex);
TaskRecord *__cdecl LiveStorage_FileShare_PerformSearch(int controllerIndex, fileShareSearchInfo_t *taskInfo);
void __cdecl LiveStorage_FileShare_SearchFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_SearchSuccess(TaskRecord *task);
unsigned int __cdecl LiveStorage_FileShare_ReadFileData(
        void *data,
        unsigned int dataSize,
        dwFileShareReadFileTask *fileTask);
unsigned int __cdecl LiveStorage_FileShare_GetDownloadingFileTotalSize(int controllerIndex);
unsigned int __cdecl LiveStorage_FileShare_GetUploadingFileTotalSize(int controllerIndex);
TaskRecord *__cdecl LiveStorage_FileShare_ReadFile(int controllerIndex, fileShareReadFileInfo *fileInfo);
unsigned int __thiscall fileShareDownloadInterceptor::handleDownload(
        fileShareDownloadInterceptor *this,
        void *data,
        unsigned int dataSize);
void __cdecl LiveStorage_FileShare_ReadFileFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadFileSuccess(TaskRecord *task);
char *__cdecl LiveStorage_GetMatchRecordBuffer();
int __cdecl LiveStorage_GetMatchRecordBufferSize();
unsigned int __cdecl LiveStorage_FileShare_WriteFileData(
        void *data,
        unsigned int dataSize,
        unsigned int bytesUploaded,
        dwFileShareWriteFileTask *fileTask);
TaskRecord *__cdecl LiveStorage_FileShare_WriteFile(int controllerIndex, fileShareWriteFileInfo *writeFileInfo);
unsigned int __thiscall fileShareUploadInterceptor::handleUpload(
        fileShareUploadInterceptor *this,
        void *data,
        unsigned int dataSize,
        unsigned int bytesUploaded);
void __cdecl LiveStorage_FileShare_WriteFileFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_WriteFileSuccess(TaskRecord *task);
void __cdecl LiveStorage_FileShare_GenerateHeatmap(
        int controllerIndex,
        unsigned __int64 fileID,
        unsigned __int8 *buffer,
        unsigned int bufferSize);
TaskRecord *__cdecl LiveStorage_FileShare_WriteSummary(
        int controllerIndex,
        unsigned __int64 fileID,
        fileShareLocation location,
        void *summaryData,
        unsigned int summaryDataSize,
        const void *metaData,
        unsigned int metaDataSize,
        bdTag *tags,
        unsigned int numTags,
        bool showSuccess);
void __cdecl LiveStorage_FileShare_WriteSummaryFailure();
void __cdecl LiveStorage_FileShare_WriteSummarySuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_FileShare_ReadSummary(
        int controllerIndex,
        bdFileMetaData *descriptor,
        void (__cdecl *successCallback)(),
        void (__cdecl *failureCallback)());
void __cdecl LiveStorage_FileShare_ReadSummaryFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadSummarySuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_FileShare_ReadMetaDataByID(
        int controllerIndex,
        bdFileID *fileIDs,
        unsigned int numFiles,
        bdFileMetaData *outDescriptors,
        fileShareLocation location);
void __cdecl LiveStorage_FileShare_ReadMetaDataByIDFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadMetaDataByIDSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_FileShare_RemoveFile(int controllerIndex, unsigned int fileSlot);
void __cdecl LiveStorage_FileShare_RemoveFileFailure();
void __cdecl LiveStorage_FileShare_RemoveFileSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_FileShare_TransferLastUploaded(int controllerIndex, unsigned int userSlot);
TaskRecord *__cdecl LiveStorage_FileShare_TransferFromPooled(
        int controllerIndex,
        unsigned __int64 fileID,
        unsigned int userSlot);
TaskRecord *__cdecl LiveStorage_FileShare_TransferFromUser(
        int controllerIndex,
        unsigned __int64 fileID,
        unsigned int userSlot,
        bool isCommunity);
TaskRecord *__cdecl LiveStorage_FileShare_TransferFile(
        int controllerIndex,
        unsigned __int64 fileID,
        unsigned int userSlot,
        fileShareLocation location,
        bool isCommunityFile);
void __cdecl LiveStorage_FileShare_TransferFileFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_TransferFileSuccess(TaskRecord *task);
TaskRecord *__cdecl LiveStorage_FileShare_WriteRating(
        int controllerIndex,
        unsigned __int64 fileID,
        unsigned __int8 rating);
void __cdecl LiveStorage_FileShare_WriteRatingFailure();
void __cdecl LiveStorage_FileShare_WriteRatingSuccess();
bool __cdecl LiveStorage_FileShare_IsReadingTopRated(int controllerIndex);
TaskRecord *__cdecl LiveStorage_FileShare_ReadTopRated(
        int controllerIndex,
        int fileType,
        int startIndex,
        int maxFiles,
        bdFileID *fileIDs,
        int *numFiles,
        int *totalNumFiles,
        void (__cdecl *successCallback)(),
        void (__cdecl *failureCallback)());
void __cdecl LiveStorage_FileShare_ReadTopRatedFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadTopRatedSuccess(TaskRecord *task);
bool __cdecl LiveStorage_FileShare_IsReadingRatings(int controllerIndex);
int __cdecl LiveStorage_FileShare_VoteRankComparator(unsigned int *arg0, unsigned int *arg1);
void __cdecl LiveStorage_FileShare_ReadRatingSuccessAction(dwFileShareGetRatingTask *ratingTask);
TaskRecord *__cdecl LiveStorage_FileShare_ReadRatingsCommunity(
        int controllerIndex,
        bdFileID *fileIDs,
        int numFileIDs,
        fileShareLocation location,
        bdVoteRankStatsInfo *outRatings,
        int *outNumRatings,
        void (__cdecl *successCallback)(),
        void (__cdecl *failureCallback)());
TaskRecord *__cdecl LiveStorage_FileShare_ReadRating(
        int controllerIndex,
        dwFileShareGetRatingTask *ratingTask,
        const TaskDefinition *sym);
void __cdecl LiveStorage_FileShare_ReadRatingFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadRatingSuccess(TaskRecord *task);
void __cdecl LiveStorage_FileShare_ReadFileRatingSuccess(TaskRecord *task);
void __cdecl LiveStorage_FileShare_WriteTagsFailure(TaskRecord *task);
void __cdecl LiveStorage_FileShare_WriteTagsSuccess(TaskRecord *task);
void __cdecl LiveStorage_ListCustomGameTypesForUser_f();
void __cdecl LiveStorage_NewUser(int controllerIndex);
void __cdecl LiveStorage_ClearPlayerStats(int controllerIndex);
int LiveStorage_ResetAllFileOps();
char __cdecl LiveStorage_Init();
void __cdecl LiveStorage_WriteBackupStatsCmd();
void __cdecl LiveStorage_ReadStatsBackupCmd();
void __cdecl LiveStorage_GetFriendStatsCmd();
void __cdecl LiveStorage_RestoreStatsFromBackupCmd();
void __cdecl LiveStorage_FakeComErrorCmd();
void __cdecl LiveStorage_CheckOngoingSessionTasks();
fileSharePrivateData *__thiscall fileSharePrivateData::fileSharePrivateData(fileSharePrivateData *this);
playerFileOperations *__thiscall playerFileOperations::playerFileOperations(playerFileOperations *this);
void __thiscall fileSharePrivateData::~fileSharePrivateData(fileSharePrivateData *this);
void __thiscall playerFileOperations::~playerFileOperations(playerFileOperations *this);
