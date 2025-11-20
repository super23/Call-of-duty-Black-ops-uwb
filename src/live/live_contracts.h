#pragma once

void __cdecl LiveContracts_ParseContracts(char *buffer);
char *__cdecl LiveContracts_GetTokenString(const char *token);
char __cdecl LiveContracts_ValidateContracts();
void LiveContracts_CalculateCurrentContractCycle();
int __cdecl LiveContracts_GetTimeUntilNextCycle();
unsigned int __cdecl LiveContracts_GetContractsCountForList();
int __cdecl LiveContracts_GetContractIndexFromListIndex(int controllerIndex, int listIndex);
char *__cdecl LiveContracts_GetContractName(int index);
bool __cdecl LiveContracts_ValidateContractIndex(int contractIndex);
char *__cdecl LiveContracts_GetContractDesc(int index);
char *__cdecl LiveContracts_GetContractStatType(int index);
char *__cdecl LiveContracts_GetContractStatName(int index);
int __cdecl LiveContracts_GetContractCost(int index);
char *__cdecl LiveContracts_GetRequirementType(int contractIndex, unsigned int requirementIndex);
char *__cdecl LiveContracts_GetRequirementData(int contractIndex, unsigned int requirementIndex);
char *__cdecl LiveContracts_GetResetConditions(int contractIndex);
int __cdecl LiveContracts_GetContractRewardXP(int contractIndex);
int __cdecl LiveContracts_GetContractRewardCP(int contractIndex);
int __cdecl LiveContracts_GetContractRequiredCount(int index);
char *__cdecl LiveContracts_GetExpirationType(int index);
int __cdecl LiveContracts_GetExpirationData(int index);
int __cdecl LiveContracts_GetTimesPurchasable(int contractIndex);
int __cdecl LiveContracts_GetCooldownTime(int contractIndex);
int __cdecl LiveContracts_GetUnlockLevel(int contractIndex);
bool __cdecl LiveContracts_IsContractLocked(int controllerIndex, int contractIndex);
unsigned int __cdecl LiveContracts_GetTimesPurchased(int controllerIndex, int contractIndex);
persistentStats *__cdecl LiveContracts_GetStatBuffer(int controllerIndex);
unsigned int __cdecl LiveContracts_GetLastTimeDeactivated(int controllerIndex, int contractIndex);
int __cdecl LiveContracts_GetTimeUntilCooledDown(int controllerIndex, int contractIndex);
void __cdecl LiveContracts_ActivateContract(int controllerIndex, int index, unsigned int activeContractIndex);
void __cdecl LiveContracts_DeactivateContract(int controllerIndex, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetNumActiveContracts(int controllerIndex);
int __cdecl LiveContracts_GetNumContractsInProgress(int controllerIndex);
int __cdecl LiveContracts_GetActiveContractIndex(int controllerIndex, int contractIndex);
unsigned int __cdecl LiveContracts_GetIndexForActiveContract(int controllerIndex, unsigned int activeContractIndex);
unsigned int __cdecl LiveContracts_GetIndexForActiveContract_Internal(char *buffer, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetActiveContractProgress(int controllerIndex, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetActiveContractProgress_Internal(
        char *buffer,
        unsigned int activeContractIndex,
        bool getInternalValue);
int __cdecl LiveContracts_GetProgressInterval(int contractIndex);
int __cdecl LiveContracts_GetCombatTimeLeft(int controllerIndex, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetCombatTimeLeft_Internal(char *buffer, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetCombatTimePassed_Internal(char *buffer, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetActiveContractStatus_Internal(char *buffer, unsigned int activeContractIndex);
bool __cdecl LiveContracts_IsActiveContractExpired_Internal(char *buffer, unsigned int activeContractIndex);
int __cdecl LiveContracts_GetActiveContractStatus(int controllerIndex, unsigned int activeContractIndex);
unsigned int __cdecl LiveContracts_SV_GetIndexForActiveContract(
        unsigned int clientNum,
        unsigned int activeContractIndex);
char *__cdecl LiveContracts_SV_GetStatBuffer(unsigned int clientNum);
int __cdecl LiveContracts_SV_GetActiveContractProgress(unsigned int clientNum, unsigned int activeContractIndex);
void __cdecl LiveContracts_SV_IncrementActiveContractProgress(
        unsigned int clientNum,
        unsigned int activeContractIndex,
        int increment);
void __cdecl LiveContracts_SV_ResetActiveContractProgress(unsigned int clientNum, unsigned int activeContractIndex);
void __cdecl LiveContracts_SV_IncrementActiveContractTime(
        unsigned int clientNum,
        unsigned int activeContractIndex,
        int increment);
int __cdecl LiveContracts_SV_GetActiveContractStatus(unsigned int clientNum, unsigned int activeContractIndex);
int __cdecl LiveContracts_SV_GetCombatTimePassed(unsigned int clientNum, unsigned int activeContractIndex);
void __cdecl LiveContracts_SVMergeBuffers(unsigned __int8 *cacbuffer, unsigned __int8 *globalbuffer);
void __cdecl LiveContracts_CLMergeBuffers(unsigned __int8 *cacbuffer, unsigned __int8 *globalbuffer);
unsigned int __cdecl SV_CACValidate_GetIndexForActiveContract(unsigned __int8 *cacbuffer, unsigned int activeindex);
void __cdecl LiveContracts_SetFilterCmd();
void __cdecl LiveContracts_RebuildFilteredList(int controllerIndex);
int __cdecl FilteredContractListSort(int *a, int *b);
