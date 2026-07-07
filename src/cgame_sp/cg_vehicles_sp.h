#pragma once

struct DObj;
struct centity_s;
struct clientInfo_t;
struct vehicle_info_t;

DObj *GetVehicleEntDObj(int localClientNum, centity_s *centVeh);
bool CG_VehEntityUsingVehicle(int localClientNum, int entNum);
clientInfo_t *ClientInfoForEntity(int localClientNum, int entNum);
bool CG_VehEntityUsingRemoteControlVehicle(int localClientNum, int entNum);
const vehicle_info_t *CG_GetVehicleInfo(int index);
int CG_VehPlayerVehicleSeat(int localClientNum, int entNum);
void CG_VehSeatTransformForPlayer(int localClientNum, int entNum, float *resultOrigin, float *resultAngles);
void SeatTransformForClientInfo(int localClientNum, clientInfo_t *ci, float *resultOrigin, float *resultAngles);
void SeatTransformForSlot(
                int localClientNum,
                int vehEntNum,
                int vehSlotIdx,
                float *resultOrigin,
                float *resultAngles);
void GetTagMatrix(
                int localClientNum,
                int vehEntNum,
                unsigned __int16 tagName,
                float (*resultTagMat)[3],
                float *resultOrigin);
double CG_VehGetHealthPercentageEntity(const centity_s *cent);
double CG_VehGetHealthPercentageLeftTread(const centity_s *cent);
double CG_VehGetHealthPercentageRightTread(const centity_s *cent);
int CG_VehGetSeatOccupancyFlags(const centity_s *cent);
bool CG_VehGetSeatGunTurningYaw(const centity_s *cent, unsigned int seatIndex);
bool CG_VehGetSeatGunTurningPitch(const centity_s *cent, unsigned int seatIndex);
bool CG_VehGetSeatGunOverheating(const centity_s *cent, unsigned int seatIndex);
