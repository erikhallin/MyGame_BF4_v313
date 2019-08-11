#ifndef CHUNK_H
#define CHUNK_H

#include "perlinnoise.h"

#define chunk_size 25 //25
#define chunk_offset 7 //6
#define chunk_min -7 //-6
#define chunk_max 8 //7
#define chunk_full 15 //13

class chunk
{
    public:
        int m_x; int m_z;
        float m_heights[chunk_size+1][chunk_size+1]; //temp innan perlin

        chunk(); //måste få info om perlin noise
        chunk(int,int);
        chunk(int,int,PerlinNoise*);
        chunk(int,int,bool);
        bool saveChunk(void); //till HDD
        void addRowToChunk(int,PerlinNoise*);
        void finishLoading(int,PerlinNoise*);
        float getHeightFromModule(int,int,PerlinNoise*);

    protected:
    private:
};

#endif // CHUNK_H
