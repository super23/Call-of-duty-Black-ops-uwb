#pragma once

void __cdecl CG_Laser_Add(centity_s *cent, DObj *obj, cpose_t *pose, const float *viewerPos, LaserOwnerEnum laserOwner);
void __cdecl CG_Laser_Add_Core(
        centity_s *cent,
        DObj *obj,
        orientation_t *orient,
        const float *viewerPos,
        LaserOwnerEnum laserOwner);
