#include "chunk.h"
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <vector>

#include <iostream>
using namespace std;

chunk::chunk()
{
    //Needs chunk coord!
}

chunk::chunk(int xpos,int zpos)
{
    m_x=xpos; m_z=zpos; //Set position
}

chunk::chunk(int xpos,int zpos,PerlinNoise* pNoise)
{
    //persistence, frequency, amplitude, octaves, randomseed

    m_x=xpos; m_z=zpos; //Set position
    for (int x=0;x<chunk_size+1;x++)
    for (int z=0;z<chunk_size+1;z++)
    {
        m_heights[x][z]=pNoise->GetHeight(xpos*chunk_size+x,zpos*chunk_size+z);
    }
}

chunk::chunk(int xpos,int zpos,bool load_from_file)
{
    //om det inte går och ladda?.... skriv 1or temp
    m_x=xpos; m_z=zpos; //Set position
    //skapa filnamnssträng
    char cX_text[20];
    char cZ_text[20];
    itoa(xpos,cX_text,10);
    itoa(zpos,cZ_text,10);
    string sFilename("Res/Map/");
    sFilename.append(cX_text);
    sFilename.append("_");
    sFilename.append(cZ_text);
    sFilename.append(".txt");
    cout << "Loaded chunk: " << sFilename << endl;

    ifstream file;
    file.open(sFilename.c_str(),ios::in | ios::binary);
    if (file.fail()) //give default values
    {
        for (int x=0;x<chunk_size+1;x++)
        for (int z=0;z<chunk_size+1;z++)
        {
            m_heights[x][z]=(x+z)/10;//temp innan perlin
            if (x==0 || z==0) m_heights[x][z]=10;
        }
    }
    else
    {
        // Open the stream
        // Determine the file length
        file.seekg (0, ios::end);
        int length = file.tellg();
        file.seekg (0, ios::beg);
        // Create a vector to store the data
        vector<float> v(length);
        //cout << length << endl;
        // Load the data
        file.read((char*) &v[0], length);

        int i=0;
        for (int x=0;x<chunk_size+1;x++)
        for (int z=0;z<chunk_size+1;z++)
        {
            m_heights[x][z]=v[i++];
        }

        file.close();
    }
}

bool chunk::saveChunk(void)
{
    //skapa fil, namn x_z.txt
    //skapa filnamnssträng
    char cX_text[20];
    char cZ_text[20];
    itoa(m_x,cX_text,10);
    itoa(m_z,cZ_text,10);
    string sFilename("Res/Map/");
    sFilename.append(cX_text);
    sFilename.append("_");
    sFilename.append(cZ_text);
    sFilename.append(".txt");

    ofstream file;
    file.open(sFilename.c_str(),ios::out | ios::binary);
    vector<float> v(2704); //vector for file writing
    //cout << sizeof(m_heights) << endl;

    int i=0;
    for (int x=0;x<chunk_size+1;x++)
    for (int z=0;z<chunk_size+1;z++)
    {
        v[i++]=m_heights[x][z]; //heights to vector
    }
    file.write((char*) &v[0], 2704); //write data

    file.close();

    cout << "Saved chunk: " << sFilename << endl;

    return true;
}

void chunk::addRowToChunk(int current_row,PerlinNoise* pNoise)
{
    if (current_row>chunk_size) {cout<<"ERROR addRowToChunk "<<current_row<<endl;return;} //ERROR
    for (int x=0;x<chunk_size+1;x++)
    {
        m_heights[x][current_row]=pNoise->GetHeight(m_x*chunk_size+x,m_z*chunk_size+current_row);
    }
}

void chunk::finishLoading(int current_row,PerlinNoise* pNoise)
{
    if (current_row>chunk_size) {cout<<"ERROR finishLoading\n";return;} //ERROR
    for (int z=current_row;z<chunk_size+1;z++)
    for (int x=0;x<chunk_size+1;x++)
    {
        m_heights[x][z]=pNoise->GetHeight(m_x*chunk_size+x,m_z*chunk_size+z);
    }
    cout<<"Finished loading extra chunk\n";
}

float chunk::getHeightFromModule(int xpos,int zpos,PerlinNoise* pNoise)
{
    return pNoise->GetHeight(xpos,zpos);
}
