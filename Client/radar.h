#ifndef RADAR_H
#define RADAR_H

#include "md2model.h"

class radar
{
    public:
        bool      m_active;
        float     m_pos[3];
        float     m_focus[3];

        float     m_extra[3];

        int       m_HP;
        int       m_owner;
        int       m_team;
        int       m_zoomLevel;
        bool      m_output;
        int       m_outpostCounter;
        int       m_outpostID[99];
        int       m_currOutpost;
        int       m_outpostPrice;
        float     m_buttonPress[10];
        float     m_doorProg;
        bool      m_add1;
        float     m_vex_pixel[4800];
        int       m_textureID;
        MD2Model* m_modelID;


        radar();
        void newRadar(float,float,float,int);
        void newOutpost(int);
        void removeOutpost(int);
        int  update(float);
        void drawRadar(float pixelColor[],int);
        bool radarBulletHitTest(float,float,float);
        bool radarInteractionHitTest(float,float,float);
        int  buttonHitTest(float,float,float);

};

#endif // RADAR_H
