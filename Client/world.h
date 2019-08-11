#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "perlinnoise.h"
#include <vector>

using namespace std;

struct stHeightChange
{
    int hc_x; int hc_z;
    float hc_height;
};

struct stChunkID
{
    int chunk_x; int chunk_z;
    vector<stHeightChange> chunk_height_change;
};

struct stBuildblock
{
    float xpos,zpos;
    int type;
};

struct stObjectHeight
{
    float xpos,zpos,object_height;
};

struct stChunkLoad
{
    int chunk_x; int chunk_z;
    chunk* pChunk;
    int current_row;
    bool done;
};

class world
{
    public:
        PerlinNoise*           m_pNoise;
        chunk                  live_chunk[chunk_full][chunk_full];
        vector<stChunkID>      ChunkIDs;
        vector<stBuildblock>   Buildblocks;
        vector<stObjectHeight> ObjectHeights;
        vector<stChunkLoad>    extraLoadedChunks;
        int                    m_textureID;

        world();
        void  setPerlinNoise(int); //seed perlin noise generator
        void  firstChunks(float,float); //fill first chunks at startup
        bool  updateMyChunks(float,float); //check chunk border crossing near you
        void  updateOtherChunks(float,float); //check chunk border crossing near others
        bool  checkAbandonedChunks(float player_pos[8][2]); //if chunk have height changes and not populated, store heightchages
        float getHeight(float,float);
        void  setHeight(float,float,float);
        bool  drawSurface(bool draw[chunk_full][chunk_full],float player_pos[3]);
        float getHeightFromFile(int,int);
        float getHeightFromModule(int,int);
        int   getLiveChunkXval(int,int);
        int   getLiveChunkZval(int,int);
        void  setBuildblockType(int,int,int);
        int   getBuildblockType(int,int);
        void  removeBuildblock(float,float);
        void  setObjectHeight(float,float,float);
        float getObjectHeight(float,float);
        float getAverageHeight(float,float);

    private:
        float m_textureOffset[chunk_size+1][chunk_size+1];

        void  setOffChunkHeight(float,float,float);
};

#endif // WORLD_H
