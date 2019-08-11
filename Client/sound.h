#ifndef SOUND_H
#define SOUND_H

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <AL/al.h>
#include <AL/alc.h>
#define numOfSound 37

class sound
{
      protected:
                
      public:
             int error;
             static const int numOfSounds=numOfSound;
             ALuint source[20]; 
             ALuint buffer[numOfSound];
             ALCdevice *device;
             ALCcontext *context; 
             sound();                           //Open OpenAL
             ~sound();                          //Close OpenAL
             bool loadWAVE(int, const char*);   //give filename and sound id number, returns if source/buffer was created 
             bool playWAVE(int, float data[]);  //data contains: ListenerPos    0:1:2,  ListenerVel  3:4:5,  
                                                //               ListenerOri at-6:7:8,          up-9:10:11,
                                                //               SourcePos 12:13:14,    SourceVel 15:16:17,
                                                //               Pitch           18,    Gain            19,  Loop         20
             void playSimpleSound(int id,float); 
             int  getAndLoadFreeSource(int);
             void updateSound(int,float data[]);
             void stopSound(int);
};


#endif

/*  EXAMPLES

Pre-comp
    Set number of sounds (numOfSound)

Initializing

    sound Sound;

Loading
    Sound.loadWAVE(0,"test1.wav");

Playing
    float data[]={0,0,0, 0,0,0, 0,0,-1,
                  0,1,0, 0,0,0, 0,0,0,
                  1,  1,  0};
    Sound.playWAVE(0,data);    
*/
