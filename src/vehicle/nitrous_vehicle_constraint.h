#pragma once

void  path_constraint_update(int a1@<ebp>, rigid_body_constraint_custom_path *vpc, gentity_s *veh);
rigid_body_constraint_custom_path * path_constraint_create@<eax>(int a1@<ebp>, gentity_s *veh);
void __cdecl path_constraint_destroy(rigid_body_constraint_custom_path *vpc);
