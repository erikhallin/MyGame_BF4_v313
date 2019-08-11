#include "ridgemultifrac.h"
#include <math.h>

ridgemultifrac::ridgemultifrac()
{
    lacunarity=1;
    gain=2;
    offset=1;
    numberOfOctaves=4;
    hScale=0.005f;
    vScale=20;
}

float ridgemultifrac::ridgedMF(float x, float z)
{
    float sum = 0;
    float amplitude = 0.5;
    float frequency = 1.0f;
    float prev = 1.0f;
    int octaves=numberOfOctaves;

    x*=hScale;
    z*=hScale;

    for (int i = 0; i < octaves; i++)
    {
      float n = ridge(interpolatedNoise((float)(x*frequency),(float)(z*frequency)), offset);
      sum += n * amplitude * prev;
      prev = n;
      frequency *= lacunarity;
      amplitude *= gain;
    }

    return (float)-sum*vScale;
}

float ridgemultifrac::ridge(float h, float offset)
{
     h = fabs(h);
     h = offset - h;
     h = h * h;
     return h;
}

float ridgemultifrac::noise(int x, int y)
{
    int n=x+y*57;
    n=(n<<13)^n;
    return (1.0-((n*(n*n*15731+789221)+1376312589)&0x7fffffff)
            /1073741824);
}

float ridgemultifrac::interpolatedNoise(float x, float y)
{
    int integer_X=(int)x;
    float fractional_X = x-integer_X;

    int integer_Y=(int)y;
    float fractional_Y=y-integer_Y;

    float v1 = noise(integer_X,integer_Y);
    float v2 = noise(integer_X+1,integer_Y);
    float v3 = noise(integer_X,integer_Y+1);
    float v4 = noise(integer_X+1,integer_Y+1);

    float i1=interpolate(v1,v2,fractional_X);
    float i2=interpolate(v3,v4,fractional_X);

    return interpolate(i1,i2,fractional_Y);
}

float ridgemultifrac::interpolate(float a, float b, float x)
{
    // linear
//        return  a*(1-x) + b*x;

    // cosine
    float ft=x*3.1415927f;
    float f=(float)((1-cosf(ft))*0.5f);
    return  a*(1-f)+b*f;

}
