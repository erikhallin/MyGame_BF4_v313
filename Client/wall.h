#ifndef WALL_H
#define WALL_H

class wall
{
    public:
           
           bool   m_active;
           float  m_pos[3];
           int    m_HP;
           int    m_textureID;
           wall();
           void newWall(int,float,int);
           int  update(void);
           void drawWall(void);
           bool wallBulletHitTest(float,float,float);
      
};

#endif
