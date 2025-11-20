#pragma once

int __cdecl BuildAabbTree(const GenericAabbTreeOptions *options);
void __cdecl BuildAabbTree_r(GenericAabbTree *tree, const GenericAabbTreeOptions *options, int *remap);
int __cdecl SplitAabbTree(int count, const GenericAabbTreeOptions *options, int *remap, int *midStart, int *lastStart);
bool __cdecl PickAabbSplitPlane(
        float (*mins)[3],
        float (*maxs)[3],
        int *remap,
        int count,
        int *chosenAxis,
        float *chosenDist);
int __cdecl compare_floats(float *e0, float *e1);
double __cdecl AddedVolume(const float *addedmins, const float *addedmaxs, const float *mins, const float *maxs);
void __cdecl CreateAabbSubTrees(
        GenericAabbTree *tree,
        const GenericAabbTreeOptions *options,
        int *remap,
        int firstIndex,
        int count);
GenericAabbTree *__cdecl AllocAabbTreeNode(const GenericAabbTreeOptions *options);
