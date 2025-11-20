#include "phys_traverse.h"

void __cdecl Vec3Abs(const float *a, float *res)
{
  *(unsigned int *)res = *(unsigned int *)a & _mask__AbsFloat_;
  *((unsigned int *)res + 1) = (unsigned int)a[1] & _mask__AbsFloat_;
  *((unsigned int *)res + 2) = (unsigned int)a[2] & _mask__AbsFloat_;
}

void __thiscall colgeom_visitor_t::intersect_box(colgeom_visitor_t *this, float *mn, float *mx, int mask)
{
  cLeaf_s *leaf; // [esp+18h] [ebp-1050h]
  int i; // [esp+1Ch] [ebp-104Ch]
  leafList_s ll; // [esp+20h] [ebp-1048h] BYREF
  char v8; // [esp+4Ch] [ebp-101Ch] BYREF
  _WORD v9[2048]; // [esp+60h] [ebp-1008h] BYREF
  int v10; // [esp+1064h] [ebp-4h]

  v10 = 2048;
  this->m_mn.vec.v[0] = *mn;
  this->m_mn.vec.v[1] = mn[1];
  this->m_mn.vec.v[2] = mn[2];
  this->m_mx.vec.v[0] = *mx;
  this->m_mx.vec.v[1] = mx[1];
  this->m_mx.vec.v[2] = mx[2];
  this->m_mask = mask;
  *(_QWORD *)&ll.bounds[0][0] = *(_QWORD *)this->m_mn.vec.v;
  LODWORD(ll.bounds[0][2]) = this->m_mn.vec.u[2];
  ll.bounds[1][0] = this->m_mx.vec.v[0];
  ll.bounds[1][1] = this->m_mx.vec.v[1];
  ll.bounds[1][2] = this->m_mx.vec.v[2];
  ll.count = 0;
  ll.maxcount = 2048;
  ll.list = v9;
  ll.lastLeaf = 0;
  ll.overflowed = 0;
  CM_BoxLeafnums_r(&ll, 0);
  if ( ll.count )
  {
    if ( ll.overflowed )
      Com_Printf(0, "colgeom_visitor_t::intersect_box: leafList overflow (max %d)\n", 2048);
    this->m_threadInfo = (TraceThreadInfo *)&v8;
    CM_GetTraceThreadInfo(this->m_threadInfo);
    if ( this->m_threadInfo->checkcount.partitions || !cm.partitionCount )
    {
      for ( i = 0; i < ll.count; ++i )
      {
        leaf = &cm.leafs[(unsigned __int16)v9[i]];
        if ( (this->m_mask & leaf->brushContents) != 0 )
          colgeom_visitor_t::intersect_box_brushes(this, leaf);
        if ( (this->m_mask & leaf->terrainContents) != 0 )
          colgeom_visitor_t::intersect_box_partitions(this, leaf);
      }
    }
  }
}

void __thiscall colgeom_visitor_t::intersect_box_brushes(colgeom_visitor_t *this, cLeaf_s *leaf)
{
  if ( leaf->leafBrushNode )
  {
    if ( this->m_mx.vec.v[0] >= leaf->mins[0]
      && this->m_mx.vec.v[1] >= leaf->mins[1]
      && this->m_mx.vec.v[2] >= leaf->mins[2]
      && leaf->maxs[0] >= this->m_mn.vec.v[0]
      && leaf->maxs[1] >= this->m_mn.vec.v[1]
      && leaf->maxs[2] >= this->m_mn.vec.v[2] )
    {
      colgeom_visitor_t::intersect_box_brushnode(this, &cm.leafbrushNodes[leaf->leafBrushNode]);
    }
  }
}

void __thiscall colgeom_visitor_t::intersect_box_partitions(colgeom_visitor_t *this, cLeaf_s *leaf)
{
  CollisionAabbTree *aabbTree; // [esp+8h] [ebp-8h]
  int k; // [esp+Ch] [ebp-4h]

  for ( k = 0; k < leaf->collAabbCount; ++k )
  {
    aabbTree = &cm.aabbTrees[k + leaf->firstCollAabbIndex];
    if ( (cm.materials[aabbTree->materialIndex].contentFlags & this->m_mask) != 0 )
      colgeom_visitor_t::intersect_box_partitions_r(this, aabbTree);
  }
}

void __thiscall colgeom_visitor_t::intersect_box_brushnode(colgeom_visitor_t *this, cLeafBrushNode_s *node)
{
  cbrush_t *b; // [esp+14h] [ebp-Ch]
  int i; // [esp+1Ch] [ebp-4h]

  while ( (this->m_mask & node->contents) != 0 )
  {
    if ( node->leafBrushCount )
    {
      if ( node->leafBrushCount > 0 )
      {
        for ( i = 0; i < node->leafBrushCount; ++i )
        {
          b = &cm.brushes[node->data.leaf.brushes[i]];
          if ( (this->m_mask & b->contents) != 0
            && this->m_mx.vec.v[0] >= b->mins[0]
            && this->m_mx.vec.v[1] >= b->mins[1]
            && this->m_mx.vec.v[2] >= b->mins[2]
            && b->maxs[0] >= this->m_mn.vec.v[0]
            && b->maxs[1] >= this->m_mn.vec.v[1]
            && b->maxs[2] >= this->m_mn.vec.v[2] )
          {
            this->visit(this, b);
          }
        }
        return;
      }
      colgeom_visitor_t::intersect_box_brushnode(this, node + 1);
    }
    if ( this->m_mn.vec.v[node->axis] <= node->data.children.dist )
    {
      if ( this->m_mx.vec.v[node->axis] >= node->data.children.dist )
        colgeom_visitor_t::intersect_box_brushnode(this, &node[node->data.children.childOffset[0]]);
      node += node->data.children.childOffset[1];
    }
    else
    {
      node += node->data.children.childOffset[0];
    }
  }
}

void __thiscall colgeom_visitor_t::intersect_box_partitions_r(colgeom_visitor_t *this, CollisionAabbTree *aabbTree)
{
  bool v3; // [esp+7h] [ebp-25h]
  int childIndex; // [esp+18h] [ebp-14h]
  int partitionIndex; // [esp+1Ch] [ebp-10h]
  CollisionAabbTree *child; // [esp+20h] [ebp-Ch]
  int checkStamp; // [esp+24h] [ebp-8h]

  if ( this->m_mn.vec.v[0] <= (float)(aabbTree->origin[0] + aabbTree->halfSize[0]) )
  {
    if ( this->m_mn.vec.v[1] <= (float)(aabbTree->origin[1] + aabbTree->halfSize[1]) )
    {
      if ( this->m_mn.vec.v[2] <= (float)(aabbTree->origin[2] + aabbTree->halfSize[2]) )
        v3 = (float)(aabbTree->origin[0] - aabbTree->halfSize[0]) <= this->m_mx.vec.v[0]
          && (float)(aabbTree->origin[1] - aabbTree->halfSize[1]) <= this->m_mx.vec.v[1]
          && (float)(aabbTree->origin[2] - aabbTree->halfSize[2]) <= this->m_mx.vec.v[2];
      else
        v3 = 0;
    }
    else
    {
      v3 = 0;
    }
  }
  else
  {
    v3 = 0;
  }
  if ( v3 )
  {
    if ( aabbTree->childCount )
    {
      childIndex = 0;
      child = &cm.aabbTrees[aabbTree->u.firstChildIndex];
      while ( childIndex < aabbTree->childCount )
      {
        colgeom_visitor_t::intersect_box_partitions_r(this, child);
        ++childIndex;
        ++child;
      }
    }
    else
    {
      partitionIndex = aabbTree->u.firstChildIndex;
      checkStamp = this->m_threadInfo->checkcount.global;
      if ( this->m_threadInfo->checkcount.partitions[partitionIndex] != checkStamp )
      {
        this->m_threadInfo->checkcount.partitions[partitionIndex] = checkStamp;
        this->visit(this, aabbTree);
      }
    }
  }
}

void __thiscall static_colgeom_visitor_t::visit(static_colgeom_visitor_t *this, const CollisionAabbTree *tree)
{
  if ( this->ntrees < 512 )
    this->trees[this->ntrees++] = tree;
}

void __thiscall static_colgeom_visitor_t::visit(static_colgeom_visitor_t *this, const cbrush_t *brush)
{
  int i; // [esp+4h] [ebp-4h]

  if ( this->nbrushes < 512 )
  {
    for ( i = 0; i < this->nbrushes; ++i )
    {
      if ( brush == this->brushes[i] )
        return;
    }
    this->brushes[this->nbrushes++] = brush;
  }
}

void __thiscall static_colgeom_visitor_t::update(
        static_colgeom_visitor_t *this,
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec)
{
  bool v5; // [esp+0h] [ebp-50h]
  float result[3]; // [esp+10h] [ebp-40h] BYREF
  float b[3]; // [esp+1Ch] [ebp-34h] BYREF
  float a[3]; // [esp+28h] [ebp-28h] BYREF
  bool inside; // [esp+37h] [ebp-19h]
  float mx[3]; // [esp+38h] [ebp-18h] BYREF
  float mn[3]; // [esp+44h] [ebp-Ch] BYREF

  a[0] = this->m_mn.vec.v[0] - *_mn;
  a[1] = this->m_mn.vec.v[1] - _mn[1];
  a[2] = this->m_mn.vec.v[2] - _mn[2];
  b[0] = *_mx - this->m_mx.vec.v[0];
  b[1] = _mx[1] - this->m_mx.vec.v[1];
  b[2] = _mx[2] - this->m_mx.vec.v[2];
  Vec3Max(a, b, result);
  v5 = result[0] < 0.0 && result[1] < 0.0 && result[2] < 0.0;
  inside = v5;
  if ( this->m_mask != mask || !inside )
  {
    mn[0] = *_mn - *expand_vec;
    mn[1] = _mn[1] - expand_vec[1];
    mn[2] = _mn[2] - expand_vec[2];
    mx[0] = *_mx + *expand_vec;
    mx[1] = _mx[1] + expand_vec[1];
    mx[2] = _mx[2] + expand_vec[2];
    this->prolog(this);
    colgeom_visitor_t::intersect_box(this, mn, mx, mask);
  }
}

