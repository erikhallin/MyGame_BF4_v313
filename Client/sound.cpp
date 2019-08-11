#include <stdio.h>
#include "sound.h"

sound::sound()
{
    device = alcOpenDevice(NULL);
    if(!device)
    {error=1; return;} //endWithError("no sound device");
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    if(!context) {error=2; return;}// endWithError("no sound context");

    alGenBuffers(numOfSounds, buffer);
    alGenSources(20, source);
    if(alGetError() != AL_NO_ERROR) {error=3; return;}// endWithError("Error GenSource");
    error=0;
    alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);
}

sound::~sound()
{
    alDeleteSources(numOfSounds, source);
    alDeleteBuffers(numOfSounds, buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool sound::loadWAVE(int id, const char* filename)
{
    id-=1;  //SoundID has +1 than bufferID
    FILE *fp = NULL;
    fp=fopen(filename,"rb");
    if (!fp) {fclose(fp);error=3; return false;}// endWithError("Failed to open file");

    char type[4];
    DWORD size,chunkSize;
    short formatType,channels;
    DWORD sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    DWORD dataSize;

    int offset[13];


    fread(type,sizeof(char),4,fp);
    offset[0]=ftell(fp);
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F') {fclose(fp);error=4; return false;}// endWithError ("No RIFF");

    fread(&size, sizeof(DWORD),1,fp);
    offset[1]=ftell(fp);
    fread(type, sizeof(char),4,fp);
    offset[2]=ftell(fp);
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E') {fclose(fp);error=5; return false;}// endWithError("not WAVE");
    fread(type,sizeof(char),4,fp);
    offset[3]=ftell(fp);
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' ') {fclose(fp);error=6; return false;}// endWithError("not fmt ");

    fread(&chunkSize,sizeof(DWORD),1,fp);
    offset[4]=ftell(fp);
    fread(&formatType,sizeof(short),1,fp);
    offset[5]=ftell(fp);
    fread(&channels,sizeof(short),1,fp);
    offset[6]=ftell(fp);
    fread(&sampleRate,sizeof(DWORD),1,fp);
    offset[7]=ftell(fp);
    fread(&avgBytesPerSec,sizeof(DWORD),1,fp);
    offset[8]=ftell(fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    offset[9]=ftell(fp);
    fread(&bitsPerSample,sizeof(short),1,fp);
    offset[10]=ftell(fp);

    fread(type,sizeof(char),4,fp);
    offset[11]=ftell(fp);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a') {fclose(fp);error=7; return false;}// endWithError("Missing DATA");
    fread(&dataSize,sizeof(DWORD),1,fp);
    offset[12]=ftell(fp);

    unsigned char* buf= new unsigned char[dataSize];
    fread(buf,sizeof(BYTE),dataSize,fp);

    //Sound Format
    ALenum format;
    if(bitsPerSample == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    if(!format) {fclose(fp);error=8; return false;}// endWithError("Wrong BitPerSample");

    //Sound Frequency
    ALuint frequency=sampleRate;

    alBufferData(buffer[id], format, buf, dataSize, frequency); //------------------------------------- va *buffer innan!!!!!!
    delete[] buf;
    if(alGetError() != AL_NO_ERROR) {fclose(fp);error=9; return false;} //endWithError("Error loading ALBuffer");

    fclose(fp);

    return true;
}

bool sound::playWAVE(int id, float data[21])
{
    id-=1; //SoundID has +1 than bufferID
    //Find free source
    int freeSource=-1;
    for (int i=0;i<10;i++)
    {
        int status;
        alGetSourcei(source[i], AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING) {freeSource=i; break;}
    }

    if (freeSource<0)
    {
        freeSource=0;
        alSourceStop(source[freeSource]);
    }

    float ListenerPos[]={data[0],data[1],data[2]};
    float ListenerVel[]={data[3],data[4],data[5]};
    float ListenerOri[]={data[6],data[7],data[8],data[9],data[10],data[11]};
    float SourcePos[]={data[12],data[13],data[14]};
    float SourceVel[]={data[15],data[16],data[17]};

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    alSourcei (source[freeSource], AL_BUFFER,   buffer[id]);
    alSourcef (source[freeSource], AL_PITCH,    data[18] );
    alSourcef (source[freeSource], AL_GAIN,     data[19] );
    alSourcefv(source[freeSource], AL_POSITION, SourcePos);
    alSourcefv(source[freeSource], AL_VELOCITY, SourceVel);
    alSourcei (source[freeSource], AL_LOOPING,  (ALboolean)data[20] );
    alSourcef (source[freeSource], AL_ROLLOFF_FACTOR,  1          );

    alSourcePlay(source[freeSource]);
    if(alGetError() != AL_NO_ERROR) {error=10; return false;} //endWithError("Error playing sound");
    return true;
}

void sound::playSimpleSound(int id, float gain)
{
    id-=1; //SoundID has +1 than bufferID
    //Find free source
    int freeSource=-1;
    for (int i=0;i<8;i++)
    {
        int status;
        alGetSourcei(source[i], AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING) {freeSource=i; break;}
    }

    if (freeSource<0)
    {
        freeSource=0;
        alSourceStop(source[freeSource]);
    }

    float ListenerPos[]={0,0,0};
    float ListenerVel[]={0,0,0};
    float ListenerOri[]={0,0,-1,0,1,0};
    float SourcePos[]={0,0,0};
    float SourceVel[]={0,0,0};

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    alSourcei (source[freeSource], AL_BUFFER,   buffer[id]);
    alSourcef (source[freeSource], AL_PITCH,    1.0f     );
    alSourcef (source[freeSource], AL_GAIN,     gain     );
    alSourcefv(source[freeSource], AL_POSITION, SourcePos);
    alSourcefv(source[freeSource], AL_VELOCITY, SourceVel);
    alSourcei (source[freeSource], AL_LOOPING,  AL_FALSE );

    alSourcePlay(source[freeSource]);
    if(alGetError() != AL_NO_ERROR) error=10;
}

int sound::getAndLoadFreeSource(int bufferID)
{
    int freeSource=-1;
    for (int i=10;i<20;i++) //10-19 long sounds
    {
        int status;
        alGetSourcei(source[i], AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING) {freeSource=i; break;}
    }
    if (freeSource!=-1) {alSourcei (source[freeSource], AL_BUFFER,   buffer[bufferID-1]);alSourcePlay(source[freeSource]);}
    return freeSource;
}

void sound::updateSound(int sourceID, float data[])
{
    float ListenerPos[]={data[0],data[1],data[2]};
    float ListenerVel[]={data[3],data[4],data[5]};
    float ListenerOri[]={data[6],data[7],data[8],data[9],data[10],data[11]};
    float SourcePos[]={data[12],data[13],data[14]};
    float SourceVel[]={data[15],data[16],data[17]};

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    alSourcef (source[sourceID], AL_PITCH,    data[18] );
    alSourcef (source[sourceID], AL_GAIN,     data[19] );
    alSourcefv(source[sourceID], AL_POSITION, SourcePos);
    alSourcefv(source[sourceID], AL_VELOCITY, SourceVel);
    alSourcei (source[sourceID], AL_LOOPING,  (ALboolean)data[20] );
    alSourcef (source[sourceID], AL_ROLLOFF_FACTOR,  1          );

    int status;
    alGetSourcei(source[sourceID], AL_SOURCE_STATE, &status);
    if(status != AL_PLAYING) {alSourcePlay(source[sourceID]);}
}

void sound::stopSound(int id)
{
    alSourceStop(source[id]);
}
