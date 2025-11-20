#pragma once

void __cdecl auto_rigid_body::add(const centity_s *cent, gjk_physics_collision_visitor *collision_visitor, int mask);
void __thiscall tlAtomicReadWriteMutex::WriteLock(tlAtomicReadWriteMutex *this);
void __thiscall tlAtomicReadWriteMutex::ReadLock(tlAtomicReadWriteMutex *this);
void __thiscall tlAtomicReadWriteMutex::WriteUnlock(tlAtomicReadWriteMutex *this);
// local variable allocation has failed, the output may be wrong!
void  auto_rigid_body::update(auto_rigid_body *a1@<ebp>);
void __cdecl fixup_wheel_constraints_arb(auto_rigid_body *arb);
void __cdecl auto_rigid_body::remove_ent(const centity_s *cent);
user_rigid_body *__cdecl auto_rigid_body::ent_has_auto_rigid_body(const centity_s *cent);
auto_rigid_body *__thiscall phys_simple_allocator<auto_rigid_body>::allocate(
        phys_simple_allocator<auto_rigid_body> *this);
void __thiscall phys_simple_allocator<auto_rigid_body>::free(
        phys_simple_allocator<auto_rigid_body> *this,
        auto_rigid_body *slot);
