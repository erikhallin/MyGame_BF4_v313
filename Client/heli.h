#ifndef HELI_H
#define HELI_H

class heli
{
    protected:

    public:
           bool  m_active;
           int   m_HP;
           float m_pos[3];
           float m_terrain_height;
           float m_yrot;
           float m_bladeRot;
           bool  m_inUse;
           int   m_user;
           int   m_noiseID;
           float m_yvel;
           int   m_textureID;

           heli();
           void newHeli(float,float,float);
           int  update(float);
           int  update(float,float,float,float,float);
           void drawHeli(void);
           bool heliBulletHitTest(float,float,float);
           void place(float,float,float);

};

#endif
