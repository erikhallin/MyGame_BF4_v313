#ifndef TURRET_H
#define TURRET_H

class turret
{
    protected:

    public:
           bool  m_active;
           int   m_owner;
           int   m_type;
           int   m_HP;
           int   m_team;
           float m_pos[3];
           float m_yrot;
           float m_xrot;
           float m_fireCountdown;
           float m_rot_barrel;
           float m_rot_barrel_speed;
           bool  m_add1;
           float m_recoil;
           int   m_textureID;


           turret();
           void newTurret(int,float,int,int,int);
           int  updateTurret(float);
           int  updateTurret(float,float,float,float);
           void drawTurret(void);
           bool turretBulletHitTest(float,float,float);
           bool turretCollision(float x,float y,float z);

};

#endif
