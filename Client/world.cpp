#include "world.h"
#include "perlinnoise.h"
#include "vec3f.h"
#include <gl\gl.h>
#include <vector>
#include <math.h>
#include <stdlib.h>

#include <iostream>

using namespace std;


world::world()
{

}

void world::setPerlinNoise(int seed)
{
    //0.5,0.03,40,4,seed
    m_pNoise=new PerlinNoise(0.1,0.03,50,4,seed);
    //set texture offset
    for (int x=0;x<chunk_size+1;x++)
    {
        for (int z=0;z<chunk_size+1;z++)
        {
            m_textureOffset[x][z]=float(rand()%50)/1000;
        }
    }
}

void world::firstChunks(float x,float z)
{
    int center_chunk_x=x/chunk_size; int center_chunk_z=z/chunk_size; //world coord to chunk coord

    for (int shift_x=chunk_min;shift_x<chunk_max;shift_x++)
    for (int shift_z=chunk_min;shift_z<chunk_max;shift_z++)
    {
        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset]=chunk(center_chunk_x+shift_x,center_chunk_z+shift_z,m_pNoise);
        stChunkID newStruct_chunk; //temp chunk ID struct
        newStruct_chunk.chunk_x=center_chunk_x+shift_x;
        newStruct_chunk.chunk_z=center_chunk_z+shift_z; //fill chunkID struct
        ChunkIDs.push_back(newStruct_chunk); //put new chunk in stored chunk vector
        cout << "New chunk put in list: " << newStruct_chunk.chunk_x << '_'
             << newStruct_chunk.chunk_z << endl;
        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].saveChunk(); //save to HDD
    }
    //Load extra chunks in memory
    for (int chunk_x=center_chunk_x+chunk_min-1;chunk_x<center_chunk_x+chunk_max+1;chunk_x++) //top row
    {
        int chunk_z=center_chunk_z+chunk_max+1;
        stChunkLoad temp_chunk_load_st;
        temp_chunk_load_st.chunk_x=chunk_x;
        temp_chunk_load_st.chunk_z=chunk_z;
        temp_chunk_load_st.current_row=chunk_size+1;
        temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z,m_pNoise);
        temp_chunk_load_st.done=true;
        extraLoadedChunks.push_back(temp_chunk_load_st);
    }
    for (int chunk_x=center_chunk_x+chunk_min-1;chunk_x<center_chunk_x+chunk_max+1;chunk_x++) //bottom row
    {
        int chunk_z=center_chunk_z+chunk_min-1;
        stChunkLoad temp_chunk_load_st;
        temp_chunk_load_st.chunk_x=chunk_x;
        temp_chunk_load_st.chunk_z=chunk_z;
        temp_chunk_load_st.current_row=chunk_size+1;
        temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z,m_pNoise);
        temp_chunk_load_st.done=true;
        extraLoadedChunks.push_back(temp_chunk_load_st);
    }
    for (int chunk_z=center_chunk_z+chunk_min;chunk_z<center_chunk_z+chunk_max;chunk_z++) //left row
    {
        int chunk_x=center_chunk_x+chunk_min-1;
        stChunkLoad temp_chunk_load_st;
        temp_chunk_load_st.chunk_x=chunk_x;
        temp_chunk_load_st.chunk_z=chunk_z;
        temp_chunk_load_st.current_row=chunk_size+1;
        temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z,m_pNoise);
        temp_chunk_load_st.done=true;
        extraLoadedChunks.push_back(temp_chunk_load_st);
    }
    for (int chunk_z=center_chunk_z+chunk_min;chunk_z<center_chunk_z+chunk_max;chunk_z++) //right row
    {
        int chunk_x=center_chunk_x+chunk_max+1;
        stChunkLoad temp_chunk_load_st;
        temp_chunk_load_st.chunk_x=chunk_x;
        temp_chunk_load_st.chunk_z=chunk_z;
        temp_chunk_load_st.current_row=chunk_size+1;
        temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z,m_pNoise);
        temp_chunk_load_st.done=true;
        extraLoadedChunks.push_back(temp_chunk_load_st);
    }
}

bool world::updateMyChunks(float x, float z)
{
    int center_chunk_x=x/chunk_size; int center_chunk_z=z/chunk_size; //world coord to chunk coord

    //Load noise values in extra chunks in extraLoadedChunks vector
    for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
    {
        if (!extraLoadedChunks[chunk_index].done)
        {
            //add extra row
            extraLoadedChunks[chunk_index].pChunk->addRowToChunk(extraLoadedChunks[chunk_index].current_row,m_pNoise);
            //increment row counter
            extraLoadedChunks[chunk_index].current_row++;
            //if row counter is greater that size of chunk, chunk is done
            if (extraLoadedChunks[chunk_index].current_row>chunk_size) extraLoadedChunks[chunk_index].done=true;
            //cout<<"ExtraChunk Updated: "<<extraLoadedChunks[chunk_index].chunk_x<<' '<<extraLoadedChunks[chunk_index].chunk_z<<endl;
        }
    }

    if (live_chunk[chunk_offset][chunk_offset].m_x==center_chunk_x &&
        live_chunk[chunk_offset][chunk_offset].m_z==center_chunk_z)
    {
        return false; //No chunkborder crossed since last update -> do nothing
    }
    //Chunk border was crossed, analysis required

    //if extra chunk is too far away, delete
    for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
    {
        if (extraLoadedChunks[chunk_index].chunk_x<center_chunk_x+chunk_min-1 ||
            extraLoadedChunks[chunk_index].chunk_x>center_chunk_x+chunk_max+1 ||
            extraLoadedChunks[chunk_index].chunk_z<center_chunk_z+chunk_min-1 ||
            extraLoadedChunks[chunk_index].chunk_z<center_chunk_z+chunk_max+1) //too far away
        {
            cout<<"ExtraChunk deleted: "<<extraLoadedChunks[chunk_index].chunk_x<<' '<<extraLoadedChunks[chunk_index].chunk_z<<endl;
            //delete extra chunk
            delete extraLoadedChunks[chunk_index].pChunk;
            //empty vector
            extraLoadedChunks.erase(extraLoadedChunks.begin()+chunk_index);
        }
    }
    //start new extraChunks
    for (int chunk_x=center_chunk_x+chunk_min-1;chunk_x<center_chunk_x+chunk_max+1;chunk_x++) //top row
    {
        bool already_in_vector=false;
        int chunk_z=center_chunk_z+chunk_max+1;
        for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
        {
            if (extraLoadedChunks[chunk_index].chunk_x==chunk_x &&
                extraLoadedChunks[chunk_index].chunk_z==chunk_z)
            {
                already_in_vector=true; break;
            }
        }
        if (!already_in_vector) //create new extraChunk
        {
            stChunkLoad temp_chunk_load_st;
            temp_chunk_load_st.chunk_x=chunk_x;
            temp_chunk_load_st.chunk_z=chunk_z;
            temp_chunk_load_st.current_row=0;
            temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z);
            temp_chunk_load_st.done=false;
            extraLoadedChunks.push_back(temp_chunk_load_st);
        }
    }
    for (int chunk_x=center_chunk_x+chunk_min-1;chunk_x<center_chunk_x+chunk_max+1;chunk_x++) //bottom row
    {
        bool already_in_vector=false;
        int chunk_z=center_chunk_z+chunk_min-1;
        for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
        {
            if (extraLoadedChunks[chunk_index].chunk_x==chunk_x &&
                extraLoadedChunks[chunk_index].chunk_z==chunk_z)
            {
                already_in_vector=true; break;
            }

        }
        if (!already_in_vector) //create new extraChunk
        {
            stChunkLoad temp_chunk_load_st;
            temp_chunk_load_st.chunk_x=chunk_x;
            temp_chunk_load_st.chunk_z=chunk_z;
            temp_chunk_load_st.current_row=0;
            temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z);
            temp_chunk_load_st.done=false;
            extraLoadedChunks.push_back(temp_chunk_load_st);
        }
    }
    for (int chunk_z=center_chunk_z+chunk_min;chunk_z<center_chunk_z+chunk_max;chunk_z++) //left row
    {
        bool already_in_vector=false;
        int chunk_x=center_chunk_x+chunk_min-1;
        for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
        {
            if (extraLoadedChunks[chunk_index].chunk_x==chunk_x &&
                extraLoadedChunks[chunk_index].chunk_z==chunk_z)
            {
                already_in_vector=true; break;
            }
        }
        if (!already_in_vector) //create new extraChunk
        {
            stChunkLoad temp_chunk_load_st;
            temp_chunk_load_st.chunk_x=chunk_x;
            temp_chunk_load_st.chunk_z=chunk_z;
            temp_chunk_load_st.current_row=0;
            temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z);
            temp_chunk_load_st.done=false;
            extraLoadedChunks.push_back(temp_chunk_load_st);
        }
    }
    for (int chunk_z=center_chunk_z+chunk_min;chunk_z<center_chunk_z+chunk_max;chunk_z++) //right row
    {
        bool already_in_vector=false;
        int chunk_x=center_chunk_x+chunk_max+1;
        for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
        {
            if (extraLoadedChunks[chunk_index].chunk_x==chunk_x &&
                extraLoadedChunks[chunk_index].chunk_z==chunk_z)
            {
                already_in_vector=true; break;
            }

        }
        if (!already_in_vector) //create new extraChunk
        {
            stChunkLoad temp_chunk_load_st;
            temp_chunk_load_st.chunk_x=chunk_x;
            temp_chunk_load_st.chunk_z=chunk_z;
            temp_chunk_load_st.current_row=0;
            temp_chunk_load_st.pChunk=new chunk(chunk_x,chunk_z);
            temp_chunk_load_st.done=false;
            extraLoadedChunks.push_back(temp_chunk_load_st);
        }
    }

    chunk old_live_chunk[chunk_full][chunk_full]; //backup for rearrangements
    int curr_chunk_x,curr_chunk_z;
    enum chunk_status{status_not_found,status_found_live,status_found_in_list};
//    int chunk_found_status=status_not_found;
    int near_chunk_status[chunk_full][chunk_full]; //status for near chunks
    int chunk_found_in_list_index[chunk_full][chunk_full];
    for (int a=0;a<chunk_full;a++)
    for (int b=0;b<chunk_full;b++)
    {
        old_live_chunk[a][b]=live_chunk[a][b];
        near_chunk_status[a][b]=status_not_found; //reset status
        chunk_found_in_list_index[a][b]=-1; //reset stored chunk index flag
    }

    //Check the old live chunks near position x,z
    for (int shift_x=chunk_min;shift_x<chunk_max;shift_x++)
    for (int shift_z=chunk_min;shift_z<chunk_max;shift_z++)
    {
        curr_chunk_x=center_chunk_x+shift_x; curr_chunk_z=center_chunk_z+shift_z;
        //Check live chunks
        for (int x_axis=0;x_axis<chunk_full;x_axis++)
        for (int z_axis=0;z_axis<chunk_full;z_axis++)
        {
            if (near_chunk_status[shift_x+chunk_offset][shift_z+chunk_offset]==status_not_found)
            {
                if (old_live_chunk[x_axis][z_axis].m_x==curr_chunk_x &&
                    old_live_chunk[x_axis][z_axis].m_z==curr_chunk_z) //is live
                {
                    //if live = already created -> do nothing for this chunk, but needs to be moved in live_chunk array
                    near_chunk_status[shift_x+chunk_offset][shift_z+chunk_offset]=status_found_live;
                    break;
                }
            }
        }
        //Check stored chunks
        if (near_chunk_status[shift_x+chunk_offset][shift_z+chunk_offset]==status_not_found)
        {
            int max_chunks=ChunkIDs.size();
            for (int i=0;i<max_chunks;i++)
            {
                if (curr_chunk_x==ChunkIDs[i].chunk_x && curr_chunk_z==ChunkIDs[i].chunk_z) //is stored
                {
                    //if not live but stored -> should be read and put in live_chunk
                    chunk_found_in_list_index[shift_x+chunk_offset][shift_z+chunk_offset]=i;
                    near_chunk_status[shift_x+chunk_offset][shift_z+chunk_offset]=status_found_in_list;
                    break;
                }
            }
        }
    }

    //Outcome - new(not found), nothing(is live), delete(was live)
    for (int shift_x=chunk_min;shift_x<chunk_max;shift_x++)
    for (int shift_z=chunk_min;shift_z<chunk_max;shift_z++)
    {
        switch (near_chunk_status[shift_x+chunk_offset][shift_z+chunk_offset])
        {
            case status_not_found: //new chunk required
            {
                bool found_in_extra_chunks=false;
                //check if in extraChunks
                for (int chunk_index=0;chunk_index<extraLoadedChunks.size();chunk_index++)
                {
                    if (extraLoadedChunks[chunk_index].chunk_x==center_chunk_x+shift_x &&
                        extraLoadedChunks[chunk_index].chunk_z==center_chunk_z+shift_z) //chunk found
                    {
                        if (!extraLoadedChunks[chunk_index].done)
                        {
                            //finish loading
                            extraLoadedChunks[chunk_index].pChunk->finishLoading(extraLoadedChunks[chunk_index].current_row,m_pNoise);
                        }
                        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset]=*extraLoadedChunks[chunk_index].pChunk;
                        //delete extra chunk
                        delete extraLoadedChunks[chunk_index].pChunk;
                        //empty vector
                        extraLoadedChunks.erase(extraLoadedChunks.begin()+chunk_index);
                        found_in_extra_chunks=true;
                        break;
                    }
                }
                //create new chunk
                if (!found_in_extra_chunks) live_chunk[shift_x+chunk_offset][shift_z+chunk_offset]=chunk(center_chunk_x+shift_x,center_chunk_z+shift_z,m_pNoise); //new chunk object
                stChunkID newStruct_chunk; //temp chunk ID struct
                newStruct_chunk.chunk_x=center_chunk_x+shift_x; newStruct_chunk.chunk_z=center_chunk_z+shift_z; //fill chunkID struct
                ChunkIDs.push_back(newStruct_chunk); //put new chunk in stored chunk vector
                std::cout << "New chunk put in list: " << newStruct_chunk.chunk_x << '_'
                          << newStruct_chunk.chunk_z << std::endl;
                live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].saveChunk();

            }break;
            case status_found_live:
            {
                //move inside live_chunk array, no saving or creating
                //kolla igenom alla old_live och hitta om chunk.x/z stämmer med center+shift
                int new_x=center_chunk_x+shift_x;
                int new_z=center_chunk_z+shift_z;
                for (int a=0;a<chunk_full;a++)
                for (int b=0;b<chunk_full;b++)
                {
                    if (old_live_chunk[a][b].m_x==new_x && old_live_chunk[a][b].m_z==new_z) //a,b was the old pos
                    {
                        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset]=old_live_chunk[a][b];
                        break;
                    }
                }
            }break;
            case status_found_in_list: //should be stored
            {
                //load stored chunk
                live_chunk[shift_x+chunk_offset][shift_z+chunk_offset]=chunk(center_chunk_x+shift_x,center_chunk_z+shift_z,true);
            }break;
        }
    }
    return true;
}

void world::updateOtherChunks(float x,float z)
{
    int chunk_x=x/chunk_size; int chunk_z=z/chunk_size; //world coord to chunk coord
    //Check if stored, if not store
    for (int shift_x=chunk_min;shift_x<chunk_max;shift_x++)
    for (int shift_z=chunk_min;shift_z<chunk_max;shift_z++)
    {
        bool chunk_in_list=false;
        int max_chunks=ChunkIDs.size();
        for (int chunk=0;chunk<max_chunks;chunk++)
        {
            if (ChunkIDs[chunk].chunk_x==chunk_x+shift_x && ChunkIDs[chunk].chunk_z==chunk_z+shift_z) //was stored
            {
                chunk_in_list=true; break;
            }
        }
        if (!chunk_in_list) //was not stored, store
        {
            chunk temp_chunk(chunk_x+shift_x,chunk_z+shift_z,m_pNoise);
            temp_chunk.saveChunk();
            stChunkID temp_id;
            temp_id.chunk_x=chunk_x+shift_x;
            temp_id.chunk_z=chunk_z+shift_z;
            ChunkIDs.push_back(temp_id);
        }
    }
}

bool world::checkAbandonedChunks(float player_pos[8][2]) //[player ID][x(0) z(1)]
{
    int max_chunks=ChunkIDs.size();
    for (int cnk=0;cnk<max_chunks;cnk++)
    {
        if (!ChunkIDs[cnk].chunk_height_change.empty()) //contain height changes
        {
            bool chunk_abandoned=true;
            for (int player=0;player<8;player++) //check all players pos
            {
                if (int(player_pos[player][0]/chunk_size)+chunk_offset+1<ChunkIDs[cnk].chunk_x &&
                    int(player_pos[player][0]/chunk_size)-chunk_offset-1>ChunkIDs[cnk].chunk_x &&
                    int(player_pos[player][1]/chunk_size)+chunk_offset+1<ChunkIDs[cnk].chunk_z &&
                    int(player_pos[player][1]/chunk_size)-chunk_offset-1>ChunkIDs[cnk].chunk_z) //player is near chunk
                {
                    chunk_abandoned=false;
                }
            }
            if (chunk_abandoned)
            {
                chunk temp_chunk(ChunkIDs[cnk].chunk_x,ChunkIDs[cnk].chunk_z,true); //load chunk from file
                //lägg till heightchanges, töm vektor
                int max_hcs=ChunkIDs[cnk].chunk_height_change.size();
                for (int hc_i=0;hc_i<max_hcs;hc_i++)
                {
                    int x=ChunkIDs[cnk].chunk_height_change[hc_i].hc_x;
                    int z=ChunkIDs[cnk].chunk_height_change[hc_i].hc_z;
                    float height=ChunkIDs[cnk].chunk_height_change[hc_i].hc_height;
                    temp_chunk.m_heights[x][z]=height;
                }
                ChunkIDs[cnk].chunk_height_change.clear();
                temp_chunk.saveChunk(); //spara
            }
        }
    }
    return true;
}

float world::getHeight(float x,float z)
{
    int chunk_x=x/chunk_size; int chunk_z=z/chunk_size; //world coord to chunk coord
    int local_x=x-(int(x/chunk_size))*chunk_size; int local_z=z-(int(z/chunk_size))*chunk_size;
    //check if chunk is live
    for (int i_x=0;i_x<chunk_full;i_x++)
    for (int i_z=0;i_z<chunk_full;i_z++)
    {
        if (live_chunk[i_x][i_z].m_x==chunk_x && live_chunk[i_x][i_z].m_z==chunk_z)
        {
            return live_chunk[i_x][i_z].m_heights[local_x][local_z];
        }
    }
    //check if height has been changed
    int max_chunks=ChunkIDs.size();
    for (int chunk=0;chunk<max_chunks;chunk++)
    {
        if (ChunkIDs[chunk].chunk_x==chunk_x && ChunkIDs[chunk].chunk_z==chunk_z) //have been shown
        {
            if (!ChunkIDs[chunk].chunk_height_change.empty()) //not empty -> pos in hc buffer
            {
                int max_hcs=ChunkIDs[chunk].chunk_height_change.size();
                for (int hc_i=0;hc_i<max_hcs;hc_i++)
                {
                    if (ChunkIDs[chunk].chunk_height_change[hc_i].hc_x==local_x &&
                        ChunkIDs[chunk].chunk_height_change[hc_i].hc_z==local_z) //height has been changed
                    {
                        return ChunkIDs[chunk].chunk_height_change[hc_i].hc_height;
                    }
                }
            }
            //position height have been stored
            return getHeightFromFile(int(x),int(z));
        }
        //Position height have not been altered/stored
        //cout<<chunk_x<<' '<<chunk_z<<endl;
        return getHeightFromModule(int(x),int(z));
    }
    cout<<"ERROR getting height\n";
    return 1.0; //error
}

bool world::drawSurface(bool draw[chunk_full][chunk_full],float player_pos[3])
{
    int chunk_x=player_pos[0]/chunk_size; int chunk_z=player_pos[2]/chunk_size; //world coord to chunk coord

    //Lighting
    float mat_ambi[]={0.2, 0.2, 0.2, 1.0};
    float mat_diff[]={0.8, 0.8, 0.8, 1.0};
    float mat_spec[]={0.1, 0.1, 0.1, 1.0};
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambi);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diff);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
    glEnable(GL_LIGHTING);
    //Texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_CULL_FACE);

    for (int shift_x=chunk_min;shift_x<chunk_max;shift_x++)
    for (int shift_z=chunk_min;shift_z<chunk_max;shift_z++)
    {
        if (!draw[shift_x+chunk_offset][shift_z+chunk_offset]) continue; //FC check
   /*     //Color
        float R=((float)shift_x+5)/10;
        float G=((float)shift_z+5)/10;
        float B=1-((float)shift_x+5)/10;
        glColor4f(R,G,B,1);*/

        //Drawing
        glPushMatrix();
        glTranslatef((chunk_x+shift_x)*chunk_size,0,(chunk_z+shift_z)*chunk_size);
        if (true)//shift_x>2 || shift_x<-2 || shift_z>2 || shift_z<-2) //Outer chunks
        {
            //TRIANGLE_STRIP snabbare än QUAD_STRIP
            int skip=1;
            Vec3f normal;
            for (int z=0;z<chunk_size;z+=skip)
            {
                bool toggle=true;
                glBegin(GL_TRIANGLE_STRIP);
                for (int x=0;x<chunk_size+1;x+=skip)
                {
              //      if (x+skip>chunk_size+1) skip=chunk_size+1-x;
                //    if (z+skip>chunk_size+1) skip=chunk_size+1-z;
                    if (toggle)
                    {
                        Vec3f p1(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z);
                        Vec3f p2(x+skip,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+skip][z],z);
                        Vec3f p3(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+skip],z+skip);
                        Vec3f v1=p2-p1;
                        Vec3f v2=p3-p1;
                        normal=v2.cross(v1);
                    }
                    int rel_height=live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]/2+20;

                    glTexCoord2f(float(rel_height)/100+m_textureOffset[x][z]+0.05,toggle); //Texture
                    glNormal3f(normal[0],normal[1],normal[2]); //Normal
                    glVertex3f(x+0.5,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+0.5);
                    glTexCoord2f(float(rel_height)/100+m_textureOffset[x][z],toggle);
                    glNormal3f(normal[0],normal[1],normal[2]); //Normal
                    glVertex3f(x+0.5,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+skip],z+skip+0.5);
                    toggle=!toggle;
                }
                glEnd();
            }
            glPopMatrix();
        }
        else //Inner chunks
        {
            //QUADS
            glBegin(GL_QUADS);
            for (int x=0;x<chunk_size;x++)
            {
                for (int z=0;z<chunk_size;z++)
                {
                    //Topside
                    if (live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]<player_pos[1])
                    {
                        glNormal3f(0,1,0);
                        glTexCoord2f(0.001,0);
                        glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z);
                        glNormal3f(0,1,0);
                        glTexCoord2f(0.049,0);
                        glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                        glNormal3f(0,1,0);
                        glTexCoord2f(0.049,1);
                        glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                        glNormal3f(0,1,0);
                        glTexCoord2f(0.001,1);
                        glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z);
                    }
                    //Right/Left Sides +/-x
                    if (live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]>
                        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z]) //there is a right side
                    {
                        if (player_pos[0]-1>x+live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_x*chunk_size) //Player to the right of the block
                        {
                            float diff=live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]-
                                       live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z];
                            glNormal3f(1,0,0);
                            glTexCoord2f(0.001,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z);
                            glNormal3f(1,0,0);
                            glTexCoord2f(0.049,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                            glNormal3f(1,0,0);
                            glTexCoord2f(0.049,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z],z+1);
                            glNormal3f(1,0,0);
                            glTexCoord2f(0.001,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z],z);
                        }
                    }
                    else //there is a left side
                    {
                        if (player_pos[0]-1<x+live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_x*chunk_size) //Player to the left of the block
                        {
                            float diff=live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z]-
                                       live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z];
                            glNormal3f(-1,0,0);
                            glTexCoord2f(0.001,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z);
                            glNormal3f(-1,0,0);
                            glTexCoord2f(0.049,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                            glNormal3f(-1,0,0);
                            glTexCoord2f(0.049,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z],z+1);
                            glNormal3f(-1,0,0);
                            glTexCoord2f(0.001,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x+1][z],z);
                        }
                    }
                    //Front/Back Sides +/-z
                    if (live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]>
                        live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1]) //there is a front side
                    {
                        if (player_pos[2]-1>z+live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_z*chunk_size) //Player in front of the block
                        {
                            float diff=live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z]-
                                       live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1];
                            glNormal3f(0,0,1);
                            glTexCoord2f(0.001,0);
                            glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                            glNormal3f(0,0,1);
                            glTexCoord2f(0.001,diff);
                            glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1],z+1);
                            glNormal3f(0,0,1);
                            glTexCoord2f(0.049,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1],z+1);
                            glNormal3f(0,0,1);
                            glTexCoord2f(0.049,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                        }
                    }
                    else //there is a back side
                    {
                        if (player_pos[2]-1<z+live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_z*chunk_size) //Player back of block
                        {
                            float diff=live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1]-
                                       live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z];
                            glNormal3f(0,0,-1);
                            glTexCoord2f(0.001,0);
                            glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                            glNormal3f(0,0,-1);
                            glTexCoord2f(0.001,diff);
                            glVertex3f(x,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1],z+1);
                            glNormal3f(0,0,-1);
                            glTexCoord2f(0.049,diff);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z+1],z+1);
                            glNormal3f(0,0,-1);
                            glTexCoord2f(0.049,0);
                            glVertex3f(x+1,live_chunk[shift_x+chunk_offset][shift_z+chunk_offset].m_heights[x][z],z+1);
                        }
                    }
                }
            }
            glEnd();
            glPopMatrix();
        }

    }
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    return true;
}

float world::getHeightFromFile(int x,int z)
{
    cout<<"ERROR getting height from file\n";
    return 1.0; //error
}

float world::getHeightFromModule(int x,int z)
{
    chunk temp_chunk;
    return temp_chunk.getHeightFromModule(x,z,m_pNoise);

    cout<<"ERROR getting height from Module\n";
    return 1.0; //error
}

void world::setHeight(float x,float z,float height)
{
    int chunk_x=x/chunk_size; int chunk_z=z/chunk_size; //world coord to chunk coord
    int local_x=x-(int(x/chunk_size))*chunk_size; int local_z=z-(int(z/chunk_size))*chunk_size;

    if (local_x==0 || local_z==0) setOffChunkHeight(x,z,height); //update block outside chunk (26)

    //Check if live
    for (int i_x=0;i_x<chunk_full;i_x++)
    for (int i_z=0;i_z<chunk_full;i_z++)
    {
        if (live_chunk[i_x][i_z].m_x==chunk_x && live_chunk[i_x][i_z].m_z==chunk_z) //is live
        {
            live_chunk[i_x][i_z].m_heights[local_x][local_z]=height; //change height
        }
    }

    //Check if height is already height change list
    int max_chunks=ChunkIDs.size();
    for (int chunk=0;chunk<max_chunks;chunk++)
    {
        if (ChunkIDs[chunk].chunk_x==chunk_x && ChunkIDs[chunk].chunk_z==chunk_z) //chunk found
        {
            stHeightChange temp_hc;
            temp_hc.hc_x=local_x; temp_hc.hc_z=local_z; temp_hc.hc_height=height;
            ChunkIDs[chunk].chunk_height_change.push_back(temp_hc); //add height change to vector
        }
    }
}

int world::getLiveChunkXval(int offset_x,int offset_z)
{
    return live_chunk[offset_x][offset_z].m_x;
}

int world::getLiveChunkZval(int offset_x,int offset_z)
{
    return live_chunk[offset_x][offset_z].m_z;
}

void world::setBuildblockType(int x,int z,int type)
{
    stBuildblock temp;
    temp.xpos=x; temp.zpos=z; temp.type=type;
    Buildblocks.push_back(temp);
}

int world::getBuildblockType(int x,int z)
{
    int max_buildblocks=Buildblocks.size();
    for (int i=0;i<max_buildblocks;i++)
    {
        if (Buildblocks[i].xpos==x && Buildblocks[i].zpos==z)
        {
            return Buildblocks[i].type;
        }
    }
    return 0;
}

void world::removeBuildblock(float x,float z)
{
    for (vector<stBuildblock>::iterator iter=Buildblocks.begin();iter!=Buildblocks.begin();iter++)
    {
        if (iter->xpos==x && iter->zpos==z)
        {
            Buildblocks.erase(iter);
            return;
        }
    }
}

void world::setObjectHeight(float x,float z,float height)
{
    //Already stored?
    for (vector<stObjectHeight>::iterator iter=ObjectHeights.begin();iter!=ObjectHeights.end();iter++)
    {
        if (iter->xpos==x && iter->zpos==z)
        {
            if (height==0) ObjectHeights.erase(iter);
            else iter->object_height=height;
            return;
        }
    }
    //Create a new
    stObjectHeight temp;
    temp.xpos=x; temp.zpos=z; temp.object_height=height;
    ObjectHeights.push_back(temp);
}

float world::getObjectHeight(float x,float z)
{
    int size=ObjectHeights.size();
    for (int i=0;i<size;i++)
    {
        if (ObjectHeights[i].xpos==x && ObjectHeights[i].zpos==z)
        {
            return ObjectHeights[i].object_height;
        }
    }
    return 0;
}

void world::setOffChunkHeight(float x,float z,float height)
{
    int chunk_x=x/chunk_size; int chunk_z=z/chunk_size; //world coord to chunk coord
    int local_x=x-(int(x/chunk_size))*chunk_size; int local_z=z-(int(z/chunk_size))*chunk_size;

    bool done=false;
    bool dubble=false;
    bool final=false;
    while (!done)
    {
        done=true;
        if (final)
        {
            chunk_x-=1;
            local_x=chunk_size;
        }
        else
        {
            if (local_x==0)
            {
                chunk_x-=1;
                local_x=chunk_size;
                if (local_z==0)
                {
                    dubble=true;
                    done=false;
                }
            }
            else if (local_z==0)
            {
                chunk_z-=1;
                local_z=chunk_size;
                if (dubble)
                {
                    chunk_x+=1;
                    local_x=x-(int(x/chunk_size))*chunk_size; int local_z=z-(int(z/chunk_size))*chunk_size;
                    done=false;
                    final=true;
                }
            }
        }

        //Check if live
        for (int i_x=0;i_x<chunk_full;i_x++)
        for (int i_z=0;i_z<chunk_full;i_z++)
        {
            if (live_chunk[i_x][i_z].m_x==chunk_x && live_chunk[i_x][i_z].m_z==chunk_z) //is live
            {
                live_chunk[i_x][i_z].m_heights[local_x][local_z]=height; //change height
            }
        }

        //Check if height is already height change list
        int max_chunks=ChunkIDs.size();
        for (int chunk=0;chunk<max_chunks;chunk++)
        {
            if (ChunkIDs[chunk].chunk_x==chunk_x && ChunkIDs[chunk].chunk_z==chunk_z) //chunk found
            {
                stHeightChange temp_hc;
                temp_hc.hc_x=local_x; temp_hc.hc_z=local_z; temp_hc.hc_height=height;
                ChunkIDs[chunk].chunk_height_change.push_back(temp_hc); //add height change to vector
            }
        }
    }
}

float world::getAverageHeight(float x,float z) //ej helt klar, ta bort vissa grejer?
{
    float height[4];
    bool heights_found=false;

    int chunk_x=x/chunk_size; int chunk_z=z/chunk_size; //world coord to chunk coord
    int local_x=x-(int(x/chunk_size))*chunk_size; int local_z=z-(int(z/chunk_size))*chunk_size;
    float part_x=x-int(x); float part_z=z-int(z);
    //check if chunk is live
    for (int i_x=0;i_x<chunk_full;i_x++)
    for (int i_z=0;i_z<chunk_full;i_z++)
    {
        if (heights_found) break;
        if (live_chunk[i_x][i_z].m_x==chunk_x && live_chunk[i_x][i_z].m_z==chunk_z)
        {
            height[0]=live_chunk[i_x][i_z].m_heights[local_x][local_z];
            height[1]=live_chunk[i_x][i_z].m_heights[local_x+1][local_z];
            height[2]=live_chunk[i_x][i_z].m_heights[local_x][local_z+1];
            height[3]=live_chunk[i_x][i_z].m_heights[local_x+1][local_z+1];
            heights_found=true;
            break;
        }
    }

    //calc slopes
    float slope_low=height[1]-height[0];
    float slope_top=height[3]-height[2];
    //float slope_left=height[2]-height[0];
    //float slope_right=height[3]-height[1];
    //calc heights on part values
    float slope_top_height=height[2]+slope_top*part_x;
    float slope_low_height=height[0]+slope_low*part_x;
    //float slope_left_height=height[0]+slope_left*part_z;
    //float slope_right_height=height[1]+slope_right*part_z;
    //calc new slopes
    float slope_vertical=slope_top_height-slope_low_height;
    //float slope_horizontal=slope_left_height-slope_right_height;
    //calc new heights
    float height_a=slope_low_height+slope_vertical*part_z;
    //float height_b=slope_left_height+slope_horizontal*part_x;
    //cout<<height_a<<' '<<height_b<<endl;

    return height_a;//(height_a+height_b)/2;
}
