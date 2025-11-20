#pragma once

void __thiscall phys_transient_allocator::reset(phys_transient_allocator *this);
void __thiscall phys_transient_allocator::reset_to_state(
        phys_transient_allocator *this,
        const phys_transient_allocator::allocator_state *as);
void __thiscall phys_transient_allocator::resize(phys_transient_allocator *this);
