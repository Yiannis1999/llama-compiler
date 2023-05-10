#include <math.h>

int abs_10(int x)
{
    return x >= 0 ? x : -x;
}

float fabs_11(float x)
{
    return x >= 0.0 ? x : -x;
}

float sqrt_12(float x)
{
    return sqrtf(x);
}

float sin_13(float x)
{
    return sinf(x);
}

float cos_14(float x)
{
    return cosf(x);
}

float tan_15(float x)
{
    return tanf(x);
}

float atan_16(float x)
{
    return atanf(x);
}

float exp_17(float x)
{
    return expf(x);
}

float ln_18(float x)
{
    return logf(x);
}

float pi_19()
{
    return M_PI;
}