#include "fx_graph.h"

void __cdecl Vec3Scale(const float *v, float scale, float *result)
{
  *result = scale * *v;
  result[1] = scale * v[1];
  result[2] = scale * v[2];
}

