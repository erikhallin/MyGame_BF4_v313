#ifndef SPEED_H
#define SPEED_H

class speed
{
    protected:

    public:
           bool  m_active;
           int   m_HP;
           float m_pos[3];
           float m_yrot;
           bool  m_inUse;
           int   m_user;
           int   m_noiseID;
           int   m_textureID;

           speed();
           void newSpeed(float,float,float);
           int  update(void);
           int  update(float,float,float,float);
           void drawSpeed(void);
           bool speedBulletHitTest(float,float,float);
           void place(float,float,float);

};

#endif
