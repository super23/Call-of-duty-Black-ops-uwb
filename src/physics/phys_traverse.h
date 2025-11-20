#pragma once

void __cdecl Vec3Abs(const float *a, float *res);
void __thiscall colgeom_visitor_t::intersect_box(colgeom_visitor_t *this, float *mn, float *mx, int mask);
void __thiscall colgeom_visitor_t::intersect_box_brushes(colgeom_visitor_t *this, cLeaf_s *leaf);
void __thiscall colgeom_visitor_t::intersect_box_partitions(colgeom_visitor_t *this, cLeaf_s *leaf);
void __thiscall colgeom_visitor_t::intersect_box_brushnode(colgeom_visitor_t *this, cLeafBrushNode_s *node);
void __thiscall colgeom_visitor_t::intersect_box_partitions_r(colgeom_visitor_t *this, CollisionAabbTree *aabbTree);
void __thiscall static_colgeom_visitor_t::visit(static_colgeom_visitor_t *this, const CollisionAabbTree *tree);
void __thiscall static_colgeom_visitor_t::visit(static_colgeom_visitor_t *this, const cbrush_t *brush);
void __thiscall static_colgeom_visitor_t::update(
        static_colgeom_visitor_t *this,
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec);
