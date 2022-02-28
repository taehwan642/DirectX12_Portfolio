#ifndef _MATH_FX_
#define _MATH_FX_

#include "params.fx"

float4 VectorPermute(uint PermuteX, uint PermuteY, uint PermuteZ, uint PermuteW, in float4 V1, in float4 V2)
{
    float4 Ptr[2] = { V1, V2 };

    float4 Result = (float4)0.f;

    const uint i0 = PermuteX & 3;
    const uint vi0 = PermuteX >> 2;
    Result[0] = Ptr[vi0][i0];

    const uint i1 = PermuteY & 3;
    const uint vi1 = PermuteY >> 2;
    Result[1] = Ptr[vi1][i1];

    const uint i2 = PermuteZ & 3;
    const uint vi2 = PermuteZ >> 2;
    Result[2] = Ptr[vi2][i2];

    const uint i3 = PermuteW & 3;
    const uint vi3 = PermuteW >> 2;
    Result[3] = Ptr[vi3][i3];

    return Result;
}

matrix MatrixRotationQuaternion(in float4 Quaternion)
{
    float4 Constant1110 = float4(1.f, 1.f, 1.f, 0.f);

    float4 Q0 = Quaternion + Quaternion;
    float4 Q1 = Quaternion * Q0;

    float4 V0 = VectorPermute(1, 0, 0, 7, Q1, Constant1110);
    float4 V1 = VectorPermute(2, 2, 1, 7, Q1, Constant1110);
    float4 R0 = Constant1110 - V0;
    R0 = R0 - V1;

    V0 = float4(Quaternion[0], Quaternion[0], Quaternion[1], Quaternion[3]);
    V1 = float4(Q0[2], Q0[1], Q0[2], Q0[3]);
    V0 = V0 * V1;

    V1 = float4(Quaternion.w, Quaternion.w, Quaternion.w, Quaternion.w);
    float4 V2 = float4(Q0[1], Q0[2], Q0[0], Q0[3]);
    V1 = V1 * V2;

    float4 R1 = V0 + V1;
    float4 R2 = V0 - V1;

    V0 = VectorPermute(1, 4, 5, 2, R1, R2);
    V1 = VectorPermute(0, 6, 0, 6, R1, R2);

    matrix M = (matrix)0.f;
    M._11_12_13_14 = VectorPermute(0, 4, 5, 3, R0, V0);
    M._21_22_23_24 = VectorPermute(6, 1, 7, 3, R0, V0);
    M._31_32_33_34 = VectorPermute(4, 5, 2, 3, R0, V1);
    M._41_42_43_44 = float4(0.f, 0.f, 0.f, 1.f);
    return M;
}


matrix MatrixAffineTransformation(in float4 Scaling, in float4 RotationOrigin, in float4 RotationQuaternion, in float4 Translation)
{
    matrix MScaling = (matrix)0.f;
    MScaling._11_22_33 = Scaling.xyz;
    float4 VRotationOrigin = float4(RotationOrigin.xyz, 0.f);
    matrix MRotation = MatrixRotationQuaternion(RotationQuaternion);
    float4 VTranslation = float4(Translation.xyz, 0.f);

    matrix M = MScaling;
    M._41_42_43_44 = M._41_42_43_44 - VRotationOrigin;
    M = mul(M, MRotation);
    M._41_42_43_44 = M._41_42_43_44 + VRotationOrigin;
    M._41_42_43_44 = M._41_42_43_44 + VTranslation;
    return M;
}

// Quaternion Slerp ฐüทร
float4 VectorLess(float4 Q1, float4 Q2)
{
    float4 ret =
    {
        (Q1[0] < Q2[0]) ? asfloat((uint)0xFFFFFFFF) : 0.f,
        (Q1[1] < Q2[1]) ? asfloat((uint)0xFFFFFFFF) : 0.f,
        (Q1[2] < Q2[2]) ? asfloat((uint)0xFFFFFFFF) : 0.f,
        (Q1[3] < Q2[3]) ? asfloat((uint)0xFFFFFFFF) : 0.f
    };

    return ret;
}

float4 VectorSelect(float4 Q1, float4 Q2, float4 Control)
{
    uint4 q1 = asuint(Q1);
    uint4 q2 = asuint(Q2);
    uint4 control = asuint(Control);

    int4 iReturn =
    {
        (q1[0] & ~control[0]) | (q2[0] & control[0]),
        (q1[1] & ~control[1]) | (q2[1] & control[1]),
        (q1[2] & ~control[2]) | (q2[2] & control[2]),
        (q1[3] & ~control[3]) | (q2[3] & control[3]),
    };

    return asfloat(iReturn);
}

float4 VectorXorInt(float4 V1, float4 V2)
{
    uint4 v1 = { asuint(V1.x), asuint(V1.y), asuint(V1.z), asuint(V1.w) };
    uint4 v2 = { 2147483648, 0, 0, 0 };

    uint4 ret =
    {
        v1[0] ^ v2[0],
        v1[1] ^ v2[1],
        v1[2] ^ v2[2],
        v1[3] ^ v2[3]
    };

    return float4(asfloat(ret.x), asfloat(ret.y), asfloat(ret.z), asfloat(ret.w));
}

float4 VectorShiftLeft(in float4 V1, in float4 V2, uint Elements)
{
    float4 ret = (float4) 0.f;
    ret = VectorPermute(Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3), V1, V2);
    return ret;
}

float4 QuaternionSlerp(in float4 Q1, in float4 Q2, float Ratio)
{
    const float4 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };

    float qDot = dot(Q1, Q2);
    float4 CosOmega = float4(qDot, qDot, qDot, qDot);

    const float4 Zero = (float4)0.f;
    float4 Control = VectorLess(CosOmega, Zero);
    float4 Sign = VectorSelect(float4(1.f, 1.f, 1.f, 1.f), float4(-1.f, -1.f, -1.f, -1.f), Control);

    CosOmega = CosOmega * Sign;
    Control = VectorLess(CosOmega, OneMinusEpsilon);

    float4 SinOmega = float4(1.f, 1.f, 1.f, 1.f) - (CosOmega * CosOmega);
    SinOmega = float4(sqrt(SinOmega.x), sqrt(SinOmega.y), sqrt(SinOmega.z), sqrt(SinOmega.w));

    float4 Omega = float4(atan2(SinOmega.x, CosOmega.x), atan2(SinOmega.y, CosOmega.y), atan2(SinOmega.z, CosOmega.z), atan2(SinOmega.w, CosOmega.w));

    float4 SignMask = float4(asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U), asfloat(0x80000000U));
    float4 V01 = VectorShiftLeft(float4(Ratio, Ratio, Ratio, Ratio), Zero, 2);
    SignMask = VectorShiftLeft(SignMask, Zero, 3);

    V01 = VectorXorInt(V01, SignMask);
    V01 = float4(1.0f, 0.0f, 0.0f, 0.0f) + V01;

    float4 InvSinOmega = float4(1.f, 1.f, 1.f, 1.f) / SinOmega;

    float4 S0 = V01 * Omega;
    S0 = float4(sin(S0.x), sin(S0.y), sin(S0.z), sin(S0.w));
    S0 = S0 * InvSinOmega;
    S0 = VectorSelect(V01, S0, Control);

    float4 S1 = float4(S0.y, S0.y, S0.y, S0.y);
    S0 = float4(S0.x, S0.x, S0.x, S0.x);

    S1 = S1 * Sign;

    float4 Result = Q1 * S0;
    Result = (Q2 * S1) + Result;

    return Result;
}

#endif