#ifndef POWERUP_H
#define POWERUP_H



class powerup
{
      protected:

      public:
             powerup();
             bool  m_active;
             bool  m_moving;
             float m_pos[3];
             float m_yhed;
             int   m_HP;
             int   m_carrier;
             int   m_content;
             int   m_textureID;

             void  newPowerup(int,float,int);
             int   update(void);
             void  updatePos(float,float,float,float);
             void  drawPowerup(void);
             bool  powerupBulletHitTest(float,float,float);
             void  place(float,float,float);
};


#endif
