#include "turret.h"
#include <gl\gl.h>
#include <math.h>

turret::turret()
{
    m_active=false;
    m_owner=-1;
    m_team=0;
    m_pos[0]=0;m_pos[1]=0;m_pos[2]=0;
    m_yrot=90;
    m_xrot=90;
    m_HP=0;
    m_fireCountdown=290;
}

void turret::newTurret(int x, float y, int z, int owner, int type) //11=team1 12=team2
{
    m_active=true;
    m_type=type;
    m_add1=true;
    m_recoil=0;
    m_HP=210;
    m_rot_barrel=0;
    m_rot_barrel_speed=0;
    m_pos[0]=x;m_pos[1]=y;m_pos[2]=z;
    if (owner<10) m_owner=owner;
    else m_team=owner-10;
    m_yrot=180;
    m_xrot=90;
}

int turret::updateTurret(float cycleTime)
{
    if (m_HP<1) { m_team=0; m_owner=-1; return 1;}
    //m_pos[1]=height;
    switch (m_type)
    {
        case 1:{//Chain
                if (m_rot_barrel_speed>0) m_rot_barrel_speed-=cycleTime*0.002;
                if (m_rot_barrel_speed<0) m_rot_barrel_speed=0;
                m_rot_barrel+=m_rot_barrel_speed;
                if (m_rot_barrel>360) m_rot_barrel-=360;
               }break;
        case 2:{//Rocket
                if (m_fireCountdown>0) m_fireCountdown-=cycleTime;
               }break;
        case 3:{//Laser
                //Recoil update
                if (m_recoil>0)
                {
                    if (m_add1) m_recoil+=cycleTime*0.01;
                    else m_recoil-=cycleTime*0.0005;
                    if (m_recoil>1) m_add1=false;
                    if (m_recoil<0) {m_recoil=0; m_add1=true;}
                }
                //Fire
                if (m_fireCountdown>0) m_fireCountdown-=cycleTime;
               }break;
    }

    return 0;
}

int turret::updateTurret(float x, float y, float z, float cycleTime)
{
    if (m_HP<1) { m_team=0; m_owner=-1; return 1;}
    //xxxxxxxxx
 //   cycleTime=10;
    //xxxxxxxxxx
    //m_pos[1]=height;
    float rad2deg=57.2957795;
    float xdiff=x-m_pos[0];
    float ydiff=y-m_pos[1];
    float zdiff=-z+m_pos[2];
    float length=sqrt((xdiff)*(xdiff)+
                    (zdiff)*(zdiff));
    if (length==0) return 0;
    float xrel=xdiff/length;
    float yrel=ydiff/length;
 //   float zrel=zdiff/length;
    float degA=acos(xrel)*rad2deg;
    float degB=180-degA;
    float degF;
    if (z>m_pos[2]) degF=degA+180; else degF=degB;
    switch (m_type)
    {
        case 1:{//Gattling
                //Adjust yrot
                if (m_yrot>degF+40) m_yrot-=1;
                else if (m_yrot<degF-40) m_yrot+=1;
                if (m_yrot>degF) m_yrot-=0.4;
                else if (m_yrot<degF) m_yrot+=0.4;
                //Adjust xrot
                degF=90-asin(yrel)*rad2deg;
                if (m_xrot>degF+40) m_xrot-=1;
                else if (m_xrot<degF-40) m_xrot+=1;
                if (m_xrot>degF) m_xrot-=0.4;
                else if (m_xrot<degF) m_xrot+=0.4;
                //Rotate Barrels
                if (m_rot_barrel_speed<10) m_rot_barrel_speed+=0.002*cycleTime;
                if (m_rot_barrel_speed>10) m_rot_barrel_speed=10;
                m_rot_barrel+=m_rot_barrel_speed;
                //Fire
                if (m_fireCountdown>0) m_fireCountdown-=cycleTime;
                if (m_rot_barrel_speed==10 && m_fireCountdown<1) {m_fireCountdown=100; return 2;}
               }break;
        case 2:{//Rocket
                //Adjust yrot
                if (m_yrot>degF+40) m_yrot-=1;
                else if (m_yrot<degF-40) m_yrot+=1;
                if (m_yrot>degF) m_yrot-=0.4;
                else if (m_yrot<degF) m_yrot+=0.4;
                //Adjust xrot
                degF=90-asin(yrel)*rad2deg;
                if (m_xrot>degF+40) m_xrot-=1;
                else if (m_xrot<degF-40) m_xrot+=1;
                if (m_xrot>degF) m_xrot-=0.4;
                else if (m_xrot<degF) m_xrot+=0.4;
                //Fire
                if (m_fireCountdown>0) m_fireCountdown-=cycleTime;
                if (m_fireCountdown<1) {m_fireCountdown=2000; return 2;}
                else return 0;
               }break;
        case 3:{//Laser
                //Adjust yrot
     //           if (m_yrot>degF+40) m_yrot-=1;
    //            else if (m_yrot<degF-40) m_yrot+=1;
                if (m_yrot>degF+10) m_yrot-=0.3;
                else if (m_yrot<degF-10) m_yrot+=0.3;
                if (m_yrot>degF) m_yrot-=0.05;
                else if (m_yrot<degF) m_yrot+=0.05;
                //Adjust xrot
                degF=90-asin(yrel)*rad2deg;
     //           if (m_xrot>degF+40) m_xrot-=1;
      //          else if (m_xrot<degF-40) m_xrot+=1;
                if (m_xrot>degF+10) m_xrot-=0.3;
                else if (m_xrot<degF-10) m_xrot+=0.3;
                if (m_xrot>degF) m_xrot-=0.05;
                else if (m_xrot<degF) m_xrot+=0.05;
                //Recoil update
                if (m_recoil>0)
                {
                    if (m_add1) m_recoil+=cycleTime*0.01;
                    else m_recoil-=cycleTime*0.0005;
                    if (m_recoil>1) m_add1=false;
                    if (m_recoil<0) {m_recoil=0; m_add1=true;}
                }
                //Fire
                if (m_fireCountdown>0) m_fireCountdown-=cycleTime;
                if (m_fireCountdown<1) {m_fireCountdown=10000; m_recoil=0.01; return 2;}
                else return 0;
               }break;
    }
    return 0;
}

void turret::drawTurret(void)
{
    float stand[]={0.25,0,0.25,  0.25,0,0.75,  0.25,1.5,0.75,  0.25,1.5,0.25,
                   0.75,0,0.25,  0.75,1.5,0.25,  0.75,1.5,0.75,  0.75,0,0.75,
                   0.25,0,0.75,  0.75,0,0.75,  0.75,1.5,0.75,  0.25,1.5,0.75,
                   0.25,0,0.25,  0.25,1.5,0.25,  0.75,1.5,0.25,  0.75,0,0.25,
                  };
    float tex_stand[]={0.504,0.574,  0.504,0.523,  0.664,0.523,  0.664,0.574,
                       0.504,0.523,  0.664,0.523,  0.664,0.574,  0.504,0.574,
                       0.504,0.574,  0.504,0.523,  0.664,0.523,  0.664,0.574,
                       0.504,0.523,  0.664,0.523,  0.664,0.574,  0.504,0.574
                      };
    float color_stand[]={0.4,0.4,0.4,  0.4,0.4,0.4,  0.2,0.2,0.2,  0.2,0.2,0.2,
                         0.4,0.4,0.4,  0.2,0.2,0.2,  0.2,0.2,0.2,  0.4,0.4,0.4,
                         0.4,0.4,0.4,  0.4,0.4,0.4,  0.2,0.2,0.2,  0.2,0.2,0.2,
                         0.4,0.4,0.4,  0.2,0.2,0.2,  0.2,0.2,0.2,  0.4,0.4,0.4,
                        };
    float tube_closed[]={
                  -1,-1,-3,  -1,-1,3,  -1,1,3,  -1,1,-3,
                  1,-1,-3,  1,1,-3,  1,1,3,  1,-1,3,
                  1,1,-3,  -1,1,-3,  -1,1,3,  1,1,3,
                  1,-1,-3,  1,-1,3,  -1,-1,3,  -1,-1,-3,
                  -1,1,3,  -1,-1,3,  1,-1,3,  1,1,3,
                  //Lid
                  -1,1,-3,  1,1,-3,  1,-1,-3,  -1,-1,-3,
                  //Fins
                  0,1,1,  0,1,3,  0,3,3,  0,2,1,
                  1,0,1,  1,0,3,  3,0,3,  2,0,1,
                  -1,0,1,  -1,0,3,  -3,0,3, -2,0,1,
                  //Rocket
                  -0.7,0.7,-1,  0.7,0.7,-1,  0.7,-0.7,-1,  -0.7,-0.7,-1,
                  -0.7,0.7,-1,  -0.7,-0.7,-1,  -0.7,-0.7,-2,  -0.7,0.7,-2,
                  -0.7,0.7,-1,  -0.7,0.7,-2,  0.7,0.7,-2,  0.7,0.7,-1,
                  0.7,0.7,-1,  0.7,0.7,-2,  0.7,-0.7,-2,  0.7,-0.7,-1,
                  -0.7,-0.7,-1,  0.7,-0.7,-1,  0.7,-0.7,-2,  -0.7,-0.7,-2
                 };
    float tube_open[]={
                  -1,-1,-3,  -1,-1,3,  -1,1,3,  -1,1,-3,
                  1,-1,-3,  1,1,-3,  1,1,3,  1,-1,3,
                  1,1,-3,  -1,1,-3,  -1,1,3,  1,1,3,
                  1,-1,-3,  1,-1,3,  -1,-1,3,  -1,-1,-3,
                  -1,1,3,  -1,-1,3,  1,-1,3,  1,1,3,
                  //Lid
                  -1,1,-3,  1,1,-3,  1,1,-5,  -1,1,-5,
                  //Fins
                  0,1,1,  0,1,3,  0,3,3,  0,2,1,
                  1,0,1,  1,0,3,  3,0,3,  2,0,1,
                  -1,0,1,  -1,0,3,  -3,0,3, -2,0,1,
                  //Rocket
                  -0.7,0.7,-1,  0.7,0.7,-1,  0.7,-0.7,-1,  -0.7,-0.7,-1, //Out
                  -0.7,0.7,-1,  -0.7,-0.7,-1,  -0.7,-0.7,2,  -0.7,0.7,2,
                  -0.7,0.7,-1,  -0.7,0.7,2,  0.7,0.7,2,  0.7,0.7,-1,
                  0.7,0.7,-1,  0.7,0.7,2,  0.7,-0.7,2,  0.7,-0.7,-1,
                  -0.7,-0.7,-1,  0.7,-0.7,-1,  0.7,-0.7,2,  -0.7,-0.7,2
                 };
    float tex_tube[]={//Tube
                      0.504,0.414,  0.758,0.414,  0.758,0.523,  0.504,0.523,
                      0.504,0.523,  0.504,0.414,  0.758,0.414,  0.758,0.523,
                      0.504,0.523,  0.504,0.414,  0.758,0.414,  0.758,0.523,
                      0.504,0.414,  0.758,0.414,  0.758,0.523,  0.504,0.523,
                      0.761,0.531,  0.875,0.531,  0.875,0.645,  0.761,0.645,
                      //Lid
                      0.761,0.414,  0.875,0.414,  0.875,0.531,  0.761,0.531,
                      //Fins
                      0.668,0.527,  0.758,0.527,  0.758,0.645,  0.668,0.586,
                      0.668,0.527,  0.758,0.527,  0.758,0.645,  0.668,0.586,
                      0.668,0.527,  0.758,0.527,  0.758,0.645,  0.668,0.586,
                      //Rocket
                      0.117,0.414,  0.234,0.414,  0.234,0.531,  0.117,0.531,
                      0,0,  0.1,0,  0.1,0.1,  0,0.1,
                      0,0,  0.1,0,  0.1,0.1,  0,0.1,
                      0,0,  0.1,0,  0.1,0.1,  0,0.1,
                      0,0,  0.1,0,  0.1,0.1,  0,0.1
                     };
    float color_tube[]={0.2,0.2,0.2,  0.4,0.4,0.4,  0.4,0.4,0.4,  0.2,0.2,0.2,
                        0.2,0.2,0.2,  0.2,0.2,0.2,  0.4,0.4,0.4,  0.4,0.4,0.4,
                        0.2,0.2,0.2,  0.2,0.2,0.2,  0.4,0.4,0.4,  0.4,0.4,0.4,
                        0.2,0.2,0.2,  0.4,0.4,0.4,  0.4,0.4,0.4,  0.2,0.2,0.2,
                        0.4,0.4,0.4,  0.4,0.4,0.4,  0.4,0.4,0.4,  0.4,0.4,0.4,
                        //Lid
                        0.15,0.15,0.15,  0.15,0.15,0.15,  0.15,0.15,0.15,  0.15,0.15,0.15,
                        //Fins
                        0.4,0.4,0.4,  0.5,0.5,0.5,  0.5,0.5,0.5,  0.4,0.4,0.4,
                        0.4,0.4,0.4,  0.5,0.5,0.5,  0.5,0.5,0.5,  0.4,0.4,0.4,
                        0.4,0.4,0.4,  0.5,0.5,0.5,  0.5,0.5,0.5,  0.4,0.4,0.4,
                        //Rocket
                        0.8,0,0, 0.8,0,0.2, 0.8,0.2,0, 0.6,0.2,0,
                        0.8,0,0, 0.8,0,0.2, 0.8,0.2,0, 0.6,0.2,0,
                        0.8,0,0, 0.8,0,0.2, 0.8,0.2,0, 0.6,0.2,0,
                        0.8,0,0, 0.8,0,0.2, 0.8,0.2,0, 0.6,0.2,0,
                        0.8,0,0, 0.8,0,0.2, 0.8,0.2,0, 0.6,0.2,0
                       };
    float vex_gattling_barrel[]={
        -6,-0.1,0.1,  0,-0.1,0.1,  0,0.1,0.1,  -6,0.1,0.1, //B
        0,-0.1,-0.1,  -6,-0.1,-0.1,  -6,0.1,-0.1,  0,0.1,-0.1, //F
        -6,0.1,0.1,  0,0.1,0.1,  0,0.1,-0.1,  -6,0.1,-0.1, //T
        0,-0.1,0.1,  -6,-0.1,0.1,  -6,-0.1,-0.1,  0,-0.1,-0.1 //U
                                };
    float tex_gattling_barrel[]={
        0.001,0.184,  0.467,0.184,  0.467,0.203,  0.001,0.203, //side
        0.467,0.203,  0.001,0.203,  0.001,0.184,  0.467,0.184, //side
        0.001,0.184,  0.467,0.184,  0.467,0.203,  0.001,0.203, //side
        0.467,0.203,  0.001,0.203,  0.001,0.184,  0.467,0.184, //side
                                };
    float vex_gattling_holder[]={
        -2,-0.5,0.5,  2,-0.5,0.5,  2,0.5,0.5,  -2,0.5,0.5, //B
        2,-0.5,-0.5,  -2,-0.5,-0.5,  -2,0.5,-0.5,  2,0.5,-0.5, //F
        2,-0.5,0.5,  2,-0.5,-0.5,  2,0.5,-0.5,  2,0.5,0.5, //R
        -2,-0.5,-0.5,  -2,-0.5,0.5,  -2,0.5,0.5,  -2,0.5,-0.5, //L
        -2,0.5,0.5,  2,0.5,0.5,  2,0.5,-0.5,  -2,0.5,-0.5, //T
        2,-0.5,0.5,  -2,-0.5,0.5,  -2,-0.5,-0.5,  2,-0.5,-0.5 //U
                                };
    float tex_gattling_holder[]={
        0.001,0.107,  0.250,0.107,  0.250,0.182,  0.001,0.182, //sides
        0.250,0.182,  0.001,0.182,  0.001,0.107,  0.250,0.107, //sides
        0.252,0.107,  0.326,0.107,  0.326,0.182,  0.252,0.182, //r/l
        0.252,0.107,  0.326,0.107,  0.326,0.182,  0.252,0.182, //r/l
        0.001,0.107,  0.250,0.107,  0.250,0.182,  0.001,0.182, //sides
        0.001,0.107,  0.250,0.107,  0.250,0.182,  0.001,0.182 //sides
                                };
    float vex_gattling_ring[]={
        -0.1,-0.4,0.4,  0.1,-0.4,0.4,  0.1,0.4,0.4,  -0.1,0.4,0.4, //B
        0.1,-0.4,-0.4,  -0.1,-0.4,-0.4,  -0.1,0.4,-0.4,  0.1,0.4,-0.4, //F
        0.1,-0.4,0.4,  0.1,-0.4,-0.4,  0.1,0.4,-0.4,  0.1,0.4,0.4, //R
        -0.1,-0.4,-0.4,  -0.1,-0.4,0.4,  -0.1,0.4,0.4,  -0.1,0.4,-0.4, //L
        -0.1,0.4,0.4,  0.1,0.4,0.4,  0.1,0.4,-0.4,  -0.1,0.4,-0.4, //T
        0.1,-0.4,0.4,  -0.1,-0.4,0.4,  -0.1,-0.4,-0.4,  0.1,-0.4,-0.4 //U
                              };
    float tex_gattling_ring[]={
        0.328,0.109,  0.346,0.109,  0.346,0.180,  0.328,0.180, //sides
        0.346,0.180,  0.328,0.180,  0.328,0.109,  0.346,0.109, //sides
        0.348,0.109,  0.408,0.109,  0.408,0.180,  0.348,0.180, //r/l
        0.348,0.109,  0.408,0.109,  0.408,0.180,  0.348,0.180, //r/l
        0.328,0.109,  0.346,0.109,  0.346,0.180,  0.328,0.180, //sides
        0.346,0.180,  0.328,0.180,  0.328,0.109,  0.346,0.109  //sides
                              };
    float vex_gattling_middel[]={
        -1.5,-0.75,1.5,  1.5,-0.75,1.5,  1.5,0.75,1.5,  -1.5,0.75,1.5, //B
        1.5,-0.75,-1.5,  -1.5,-0.75,-1.5,  -1.5,0.75,-1.5,  1.5,0.75,-1.5, //F
        1.5,-0.75,1.5,  1.5,-0.75,-1.5,  1.5,0.75,-1.5,  1.5,0.75,1.5, //R
        -1.5,-0.75,-1.5,  -1.5,-0.75,1.5,  -1.5,0.75,1.5,  -1.5,0.75,-1.5, //L
        -1.5,0.75,1.5,  1.5,0.75,1.5,  1.5,0.75,-1.5,  -1.5,0.75,-1.5, //T
        1.5,-0.75,1.5,  -1.5,-0.75,1.5,  -1.5,-0.75,-1.5,  1.5,-0.75,-1.5 //U
                                };
    float tex_gattling_middel[]={
        0.854,0.193,  0.999,0.193,  0.999,0.264,  0.854,0.264, //f/b
        0.854,0.193,  0.999,0.193,  0.999,0.264,  0.854,0.264, //f/b
        0.707,0.193,  0.852,0.193,  0.852,0.264,  0.707,0.264, //sides
        0.707,0.193,  0.852,0.193,  0.852,0.264,  0.707,0.264, //sides
        0.707,0.266,  0.852,0.266,  0.852,0.410,  0.707,0.410, //T
        0.854,0.266,  0.999,0.266,  0.999,0.410,  0.854,0.410 //L
                                };
    float vex_laser_barrel[]={
        -11,-2.5,-1,  -6,-2.5,-1,  -6,-0.5,0,  -11,-0.5,0, //b
        -6,-2.5,1,  -11,-2.5,1,  -11,-0.5,0,  -6,-0.5,0, //f
        -11,-2.5,1,  -6,-2.5,1,  -6,-2.5,-1,  -11,-2.5,-1 //U
                       };
    float tex_laser_barrel[]={
        0.713,0.092,  0.436,0.092,  0.436,0.143,  0.713,0.143,
        0.436,0.143,  0.713,0.143,  0.713,0.092,  0.436,0.092,
        0.713,0.092,  0.436,0.092,  0.436,0.143,  0.713,0.143
                       };
    float vex_laser_middle[]={
        -6,-3,-1.5,  -4,-3,-1.5,  -4,0,-1.5,  -6,0,-1.5, //f
        -4,-3,1.5,  -6,-3,1.5,  -6,0,1.5,  -4,0,1.5, //b
        -6,0,-1.5,  -4,0,-1.5,  -4,0,1.5,  -6,0,1.5, //t
        -4,-3,-1.5,  -6,-3,-1.5,  -6,-3,1.5,  -4,-3,1.5, //u
        -6,-3,1.5,  -6,-3,-1.5,  -6,0,-1.5,  -6,0,1.5 //l
                       };
    float tex_laser_middle[]={
        0.434,0.001,  0.328,0.001,  0.328,0.090,  0.434,0.090, //Sides
        0.328,0.090,  0.434,0.090,  0.434,0.001,  0.328,0.001, //Sides
        0.434,0.001,  0.328,0.001,  0.328,0.090,  0.434,0.090, //Sides
        0.328,0.090,  0.434,0.090,  0.434,0.001,  0.328,0.001, //Sides
        0.436,0.001,  0.523,0.001,  0.523,0.090,  0.436,0.090 //Front
                       };
    float vex_laser_back[]={
        -4,-5,-3,  4,-5,-3,  4,3,-3,  -4,3,-3, //f
        4,-5,3,  -4,-5,3,  -4,3,3,  4,3,3, //b
        -4,3,-3,  4,3,-3,  4,3,3,  -4,3,3, //t
        4,-5,-3,  -4,-5,-3,  -4,-5,3,  4,-5,3, //u
        4,-5,-3,  4,-5,3,  4,3,3,  4,3,-3, //r
        -4,-5,3,  -4,-5,-3,  -4,3,-3,  -4,3,3 //l
                       };
    float tex_laser_back[]={
        0.999,0.098,  0.811,0.098,  0.811,0.184,  0.999,0.184, //Sides
        0.811,0.098,  0.999,0.098,  0.999,0.184,  0.811,0.184, //Sides
        0.999,0.001,  0.811,0.001,  0.811,0.096,  0.999,0.096, //Top
        0.713,0.001,  0.525,0.001,  0.525,0.090,  0.713,0.090, //Bottom
        0.809,0.001,  0.715,0.001,  0.715,0.096,  0.809,0.096, //Right
        0.809,0.098,  0.715,0.098,  0.715,0.184,  0.809,0.184 //Left
                       };

    float vex_laser_plate[]={
        0,0,0,  5,0,0,  5,1,0, 0,1,0
                            };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    //glEnableClientState(GL_COLOR_ARRAY);
    switch (m_type)
    {

        case 1:{//Gattling
                glVertexPointer(3, GL_FLOAT, 0, stand);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_stand);
                glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 16);
                glTranslatef(0.5,1.5,0.5);
                glRotatef(-m_yrot,0,1,0);
                glRotatef(m_xrot-90,0,0,1);
                glScalef(0.25,0.25,0.25);
                //Middel box
                glVertexPointer(3, GL_FLOAT, 0, vex_gattling_middel);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_middel);
                glPushMatrix();
                    glTranslatef(0,0,0);
                    glDrawArrays(GL_QUADS, 0, 24);
                glPopMatrix();
                //Left Gun
                glPushMatrix();
                    glTranslatef(0,0,-2);
                    //Holder
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_holder);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_holder);
                    glPushMatrix();
                        glDrawArrays(GL_QUADS, 0, 24);
                    glPopMatrix();
                    glRotatef(m_rot_barrel,1,0,0);
                    //Barrels
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_barrel);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_barrel);
                    glPushMatrix();
                        glTranslatef(0,0.3,0);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,-0.6,0);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,0.3,-0.3);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,0,0.6);
                        glDrawArrays(GL_QUADS, 0, 16);
                    glPopMatrix();
                    //Rings
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_ring);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_ring);
                    glPushMatrix();
                        glScalef(1.1,1.1,1.1);
                        glTranslatef(-3,0,0);
                        glDrawArrays(GL_QUADS, 0, 24);
                        glTranslatef(-2,0,0);
                        glDrawArrays(GL_QUADS, 0, 24);
                    glPopMatrix();
                glPopMatrix();
                //Right Gun
                glPushMatrix();
                    glTranslatef(0,0,2);
                    //Holder
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_holder);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_holder);
                    glPushMatrix();
                        glDrawArrays(GL_QUADS, 0, 24);
                    glPopMatrix();
                    glRotatef(-m_rot_barrel,1,0,0);
                    //Barrels
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_barrel);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_barrel);
                    glPushMatrix();
                        glTranslatef(0,0.3,0);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,-0.6,0);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,0.3,-0.3);
                        glDrawArrays(GL_QUADS, 0, 16);
                        glTranslatef(0,0,0.6);
                        glDrawArrays(GL_QUADS, 0, 16);
                    glPopMatrix();
                    //Rings
                    glVertexPointer(3, GL_FLOAT, 0, vex_gattling_ring);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_gattling_ring);
                    glPushMatrix();
                        glScalef(1.1,1.1,1.1);
                        glTranslatef(-3,0,0);
                        glDrawArrays(GL_QUADS, 0, 24);
                        glTranslatef(-2,0,0);
                        glDrawArrays(GL_QUADS, 0, 24);
                    glPopMatrix();
                glPopMatrix();
               }break;
        case 2:{//Rocket
                glVertexPointer(3, GL_FLOAT, 0, stand);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_stand);
                glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 16);
                glTranslatef(0.5,1.5,0.5);
                if (m_fireCountdown<200 || m_fireCountdown>1700) glVertexPointer(3, GL_FLOAT, 0, tube_open);
                else glVertexPointer(3, GL_FLOAT, 0, tube_closed);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_tube);
                glRotatef(90-m_yrot,0,1,0);
                glRotatef(90-m_xrot,1,0,0);
                glScalef(0.4,0.4,0.3);
                glDrawArrays(GL_QUADS, 0, 56);
               }break;
        case 3:{//Laser
                glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
                //Stand
                glVertexPointer(3, GL_FLOAT, 0, stand);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_stand);
                glDrawArrays(GL_QUADS, 0, 16);
                //Back
                glTranslatef(0.5,1.7,0.5);
                glRotatef(-m_yrot,0,1,0);
                glRotatef(m_xrot-90,0,0,1);
                glScalef(0.18,0.1,0.1);
                glPushMatrix();
                    glScalef(1,1,1.2);
                    glVertexPointer(3, GL_FLOAT, 0, vex_laser_back);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_laser_back);
                    glDrawArrays(GL_QUADS, 0, 24);
                    //Recharge bar
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    glEnableClientState(GL_COLOR_ARRAY);
                    glDisable(GL_TEXTURE_2D);
                    //translate
                    glTranslatef(-2.5,0.7,3.05);
                    //rotate

                    //load plate
                    glVertexPointer(3, GL_FLOAT, 0, vex_laser_plate);
                    //calc color
                    float color_background[]={0.2,0.2,0.2, 0.15,0.15,0.15, 0.3,0.3,0.3, 0.35,0.35,0.35};
                    float temp_color=(10000-(float)m_fireCountdown)/10000; if (temp_color>0.8) temp_color=0.8;
                    float color_load[]={temp_color*0.9,(1-temp_color)/2,0.6-temp_color/2, temp_color,(1-temp_color)/2,0.6-temp_color/2,
                                        temp_color+0.1,(1-temp_color)/2,0.7-temp_color/2, temp_color+0.2,(1-temp_color)/2,0.6-temp_color/2};

                    //draw background
                    glColorPointer(3, GL_FLOAT, 0, color_background);
                    glDrawArrays(GL_QUADS, 0, 4);
                    glPushMatrix();
                        //draw bar plate
                        glTranslatef(0.1,0.12,0.05);
                        glScalef(0.95,0.8,0.9);
                        glColorPointer(3, GL_FLOAT, 0, color_load);
                        glDrawArrays(GL_QUADS, 0, 4);
                    glPopMatrix();
                    //Other side
                    glTranslatef(0,0,-6.1);
                    //draw background
                    glColorPointer(3, GL_FLOAT, 0, color_background);
                    glDrawArrays(GL_QUADS, 0, 4);
                    glPushMatrix();
                        //draw bar plate
                        glTranslatef(0.1,0.12,-0.05);
                        glScalef(0.95,0.8,0.9);
                        glColorPointer(3, GL_FLOAT, 0, color_load);
                        glDrawArrays(GL_QUADS, 0, 4);
                    glPopMatrix();
                    //Front
                    glTranslatef(-1.55,0,4.25);
                    glRotatef(90,0,1,0);
                    //draw background
                    glColorPointer(3, GL_FLOAT, 0, color_background);
                    glScalef(0.5,1,1);
                    glDrawArrays(GL_QUADS, 0, 4);
                    glPushMatrix();
                        //draw bar plate
                        glTranslatef(0.1,0.12,-0.05);
                        glScalef(0.95,0.8,0.9);
                        glColorPointer(3, GL_FLOAT, 0, color_load);
                        glDrawArrays(GL_QUADS, 0, 4);
                    glPopMatrix();
                    //enable/disable
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glDisableClientState(GL_COLOR_ARRAY);
                    glEnable(GL_TEXTURE_2D);
                glPopMatrix();
                //Mid
                glTranslatef(m_recoil,0,0);
                glVertexPointer(3, GL_FLOAT, 0, vex_laser_middle);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_laser_middle);
                glDrawArrays(GL_QUADS, 0, 20);
                //Barrel
                glTranslatef(m_recoil*3,0,0);
                glVertexPointer(3, GL_FLOAT, 0, vex_laser_barrel);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_laser_barrel);
                glDrawArrays(GL_QUADS, 0, 12);
               }break;
    }

glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_COLOR_ARRAY);
}

bool turret::turretBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=0.5;
    float yhitbox=1;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+1 && ypos<m_pos[1]+yhitbox+1 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}

bool turret::turretCollision(float x,float y,float z)
{
    float xhitbox=1;
    float yhitbox=2;
    float zhitbox=1;
    if (x>m_pos[0]-xhitbox+0.5 &&  x<m_pos[0]+xhitbox+0.5 &&
        y>m_pos[1]-yhitbox+2.0 &&  y<m_pos[1]+yhitbox+2.0 &&
        z>m_pos[2]-zhitbox+0.5 &&  z<m_pos[2]+zhitbox+0.5) return true;
    return false;
}
