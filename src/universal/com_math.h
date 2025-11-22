#pragma once

struct cplane_s // sizeof=0x14
{                                       // XREF: cplane_t/r
                                        // CM_TraceThroughBrush/r ...
    float normal[3];                    // XREF: CM_TraceThroughBrush+629/w
                                        // CM_TraceThroughBrush+636/w ...
    float dist;
    unsigned __int8 type;
    unsigned __int8 signbits;
    unsigned __int8 pad[2];
};

double __cdecl random();
double __cdecl crandom();
void __cdecl GaussianRandom(float *f0, float *f1);
unsigned int __cdecl RandWithSeed(int *seed);
double __cdecl randomhash(int value);
void __cdecl PointInCircleFromUniformDeviates(float radiusDeviate, float yawDeviate, float *point);
double __cdecl LinearTrack(float tgt, float cur, float rate, float deltaTime);
double __cdecl LinearTrackAngle(float tgt, float cur, float rate, float deltaTime);
double __cdecl DiffTrack(float tgt, float cur, float rate, float deltaTime);
double __cdecl DiffTrackAngle(float tgt, float cur, float rate, float deltaTime);
double __cdecl GraphGetValueFromFraction(int knotCount, const float (*knots)[2], float fraction);
double __cdecl Q_acos(float c);
char __cdecl ClampChar(int i);
__int16 __cdecl ClampShort(int i);
unsigned __int8 __cdecl DirToByte(const float *dir);
void __cdecl ByteToDir(unsigned int b, float *dir);
int __cdecl VecNCompareCustomEpsilon(const float *v0, const float *v1, float epsilon, int coordCount);
void __cdecl Vec3ProjectionCoords(const float *dir, int *xCoord, int *yCoord);
void __cdecl RotatePointAroundVector(float *dst, const float *dir, const float *point, float degrees);
void __cdecl Vec3Basis_RightHanded(const float *forward, float *left, float *up);
double __cdecl vectoyaw(const float *vec);
double __cdecl vectosignedyaw(const float *vec);
double __cdecl vectopitch(const float *vec);
void __cdecl vectoangles(const float *vec, float *angles);
void __cdecl UnitQuatToAngles(const float *quat, float *angles);
void __cdecl YawVectors(float yaw, float *forward, float *right);
void __cdecl YawVectors2D(float yaw, float *forward, float *right);
void __cdecl PerpendicularVector(const float *src, float *dst);
double __cdecl PointToLineDistSq(const float *point, const float *start, const float *end);
double __cdecl PointToLineDistSq2D(const float *point, const float *start, const float *end);
int __cdecl BoxInPlanes(const float (*planes)[4], unsigned int numPlanes, const float *mins, const float *maxs);
int __cdecl SphereInPlanes(const float (*planes)[4], unsigned int numPlanes, const float *center, float radius);
void __cdecl MatrixIdentity33(float (*out)[3]);
void __cdecl MatrixIdentity44(float (*out)[4]);
void __cdecl MatrixMultiply(const float (*in1)[3], const float (*in2)[3], float (*out)[3]);
void __cdecl MatrixVecMultiply(const float (*mulMat)[3], const float *mulVec, float *solution);
void __cdecl MatrixVecMultiply(const float (*mulMat)[4], const float *mulVec, float *solution);
void __cdecl MatrixVecMultiplyProject(const float (*mulMat)[4], const float *mulVec, float *solution);
void __cdecl MatrixMultiplyEquals(const float (*in)[3], float (*out)[3]);
void __cdecl MatrixMultiply43(const float (*in1)[3], const float (*in2)[3], float (*out)[3]);
void __cdecl MatrixMultiply44(const float (*in1)[4], const float (*in2)[4], float (*out)[4]);
void __cdecl MatrixTranspose(const float (*in)[3], float (*out)[3]);
void __cdecl MatrixTranspose44(const float *in, float *out);
void __cdecl MatrixInverse(const float (*in)[3], float (*out)[3]);
void __cdecl MatrixInverseOrthogonal43(const float (*in)[3], float (*out)[3]);
void __cdecl MatrixInverse44(const float *mat, float *dst);
void __cdecl MatrixTransformVector44(const float *vec, const float (*mat)[4], float *out);
void __cdecl MatrixTransformVector43(const float *in1, const float (*in2)[3], float *out);
void __cdecl MatrixTransposeTransformVector43(const float *in1, const float (*in2)[3], float *out);
void __cdecl MatrixTransformVector43Equals(float *out, const float *in);
void __cdecl VectorAngleMultiply(float *vec, float angle);
void __cdecl UnitQuatToAxis(const float *quat, float (*axis)[3]);
void __cdecl UnitQuatToForward(const float *quat, float *forward);
void __cdecl QuatSlerp(const float *from, const float *to, float frac, float *result);
double __cdecl RotationToYaw(const float *rot);
void __cdecl MatrixRotationZ(float (*mat)[3], float degree);
void __cdecl FinitePerspectiveMatrix(float tanHalfFovX, float tanHalfFovY, float zNear, float zFar, float (*mtx)[4]);

void  SpotLightViewMatrix(const float *direction, float rotation, float (*mtx)[4]);
void  SpotLightViewMatrixDir3(
        const float *dirx,
        const float *diry,
        const float *dirz,
        float (*mtx)[4]);
void __cdecl SpotLightProjectionMatrix(float cosFov, float zNear, float zFar, float (*mtx)[4]);
void __cdecl InfinitePerspectiveMatrix(float tanHalfFovX, float tanHalfFovY, float zNear, float (*mtx)[4]);

void __cdecl MatrixForViewer(const float *origin, const float (*axis)[3], float (*mtx)[4]);
void __cdecl AnglesSubtract(const float *v1, const float *v2, float *v3);
double __cdecl AngleNormalize360(float angle);
double __cdecl AngleDelta(float angle1, float angle2);
double __cdecl RadiusFromBounds(const float *mins, const float *maxs);
double __cdecl RadiusFromBounds2D(const float *mins, const float *maxs);
double __cdecl RadiusFromBoundsSq(const float *mins, const float *maxs);
double __cdecl RadiusFromBounds2DSq(const float *mins, const float *maxs);
void __cdecl ExtendBounds(float *mins, float *maxs, const float *offset);
void __cdecl ExpandBoundsToWidth(float *mins, float *maxs);
void __cdecl ClearBounds(float *mins, float *maxs);
bool __cdecl IsClearedBounds(const float *mins, const float *maxs);
void __cdecl ClearBounds2D(float *mins, float *maxs);
void __cdecl AddPointToBounds(const float *v, float *mins, float *maxs);
void __cdecl AddPointToBounds2D(const float *v, float *mins, float *maxs);
bool __cdecl PointInBounds(const float *v, const float *mins, const float *maxs);
bool __cdecl PointInBounds2D(const float *v, const float *mins, const float *maxs);
bool __cdecl BoundsOverlap(const float *mins0, const float *maxs0, const float *mins1, const float *maxs1);
void __cdecl ExpandBounds(const float *addedmins, const float *addedmaxs, float *mins, float *maxs);
void __cdecl ExpandBounds2D(const float *addedmins, const float *addedmaxs, float *mins, float *maxs);
void __cdecl GetRotatedBounds(
        const float (*baseBounds)[3],
        const float *origin,
        const float (*axis)[3],
        float (*rotatedBounds)[3]);
void __cdecl AxisClear(float (*axis)[3]);
void __cdecl AxisTranspose(const float (*in)[3], float (*out)[3]);
void __cdecl AxisTransformVec3(const float (*axes)[3], const float *vec, float *out);
void __cdecl YawToAxis(float yaw, float (*axis)[3]);
void __cdecl AxisToAngles(const float (*axis)[3], float *angles);
void __cdecl Axis4ToAngles(const float (*axis)[4], float *angles);
int __cdecl IntersectPlanes(const float **plane, float *xyz);
void __cdecl SnapPointToIntersectingPlanes(const float **planes, float *xyz, float snapGrid, float snapEpsilon);
int __cdecl ProjectedWindingContainsCoplanarPoint(
        const float (*verts)[3],
        int vertCount,
        int x,
        int y,
        const float *point);
int __cdecl PlaneFromPoints(float *plane, const float *v0, const float *v1, const float *v2);
void __cdecl ProjectPointOnPlane(const float *p, const float *normal, float *dst);
void __cdecl SetPlaneSignbits(cplane_s *out);
int __cdecl BoxOnPlaneSide(
    const float *emins,
    const float *emaxs,
    const cplane_s *p,
    const cplane_s *pa);
int __cdecl IsPosInsideArc(
        const float *pos,
        float posRadius,
        const float *arcOrigin,
        float arcRadius,
        float arcAngle0,
        float arcAngle1,
        float arcHalfHeight);
bool __cdecl BoxDistSqrdExceeds(const float *absmin, const float *absmax, const float *org, float fogOpaqueDistSqrd);
double __cdecl Q_rint(float in);
double __cdecl ColorNormalize(float *in, float *out);
void __cdecl ColorSRGBtoLinear(const float *in, float *out);
double __cdecl PitchForYawOnNormal(float fYaw, const float *normal);
void __cdecl NearestPitchAndYawOnPlane(const float *angles, const float *normal, float *result);
void __cdecl Rand_Init(int seed);
double __cdecl flrand(float min, float max);
int __cdecl irand(int min, int max);
void __cdecl AxisToQuat(const float (*mat)[3], float *out);
void __cdecl QuatLerp(const float *qa, const float *qb, float frac, float *out);
bool __cdecl CullBoxFromCone(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        const float *boxCenter,
        const float *boxHalfSize);
bool __cdecl CullBoxFromSphere(const float *sphereOrg, float radius, const float *boxCenter, const float *boxHalfSize);
bool __cdecl CullBoxFromConicSectionOfSphere(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        float radius,
        const float *boxCenter,
        const float *boxHalfSize);
bool __cdecl CullSphereFromCone(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        const float *sphereCenter,
        float radius);
void __cdecl colorTempMatrix(float (*finalMatrix)[4], float colorTemp);
void __cdecl colorTempToXYZ(float colorTemp, float *XYZ);
void __cdecl colorHueMatrix(float (*finalMatrix)[4], float hue);
void __cdecl colorSaturationMatrix(float (*finalMatrix)[4], float saturation);
void __cdecl AxisCopy(const float (*in)[3], float (*out)[3]);

void __cdecl Vec3Lerp(const float *start, const float *end, float fraction, float *endpos);
float __cdecl Vec3DistanceSq(const float *p1, const float *p2);


constexpr float vec2_origin[2] = { 0.0, 0.0 };
constexpr float vec3_origin[3] = { 0.0, 0.0, 0.0 };
constexpr float vec4_origin[4] = { 0.0, 0.0, 0.0, 0.0 };