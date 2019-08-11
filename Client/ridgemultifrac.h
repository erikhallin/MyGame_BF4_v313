#ifndef RIDGEMULTIFRAC_H
#define RIDGEMULTIFRAC_H


class ridgemultifrac
{
    public:
        ridgemultifrac();
        float ridgedMF(float,float);

    private:

        float lacunarity;
        float gain;
        float offset;
        int numberOfOctaves;
        float hScale;
        float vScale;

        float ridge(float,float);
        float noise(int,int);
        float interpolatedNoise(float,float);
        float interpolate(float,float,float);
};

#endif // RIDGEMULTIFRAC_H
