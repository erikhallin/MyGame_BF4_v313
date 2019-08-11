#ifndef TANK_H
#define TANK_H

class tank
{
    protected:

    public:
           bool  m_active;
           int   m_HP;
           float m_pos[3];
           float m_yrot;
           float m_xrot;
           float m_zrot;
           float m_yrot_tower;
           float m_xrot_tower;
           float m_wheelState;
           float m_wheelSpinR;
           float m_wheelSpinL;
           bool  m_inUse;
           int   m_user;
           float m_cannonProg;
           float m_machinegunProg;
           bool  m_add1;
           bool  m_add2;
           int   m_noiseID;
           int   m_textureID;

           tank();
           void newTank(float,float,float);
           int  update(void);
           int  update(float data[],float);
           void drawTank(void);
           void drawFPSTank(void);
           bool tankBulletHitTest(float,float,float);
           void place(float,float,float);

};

#endif
