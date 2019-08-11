#ifndef MINE_H
#define MINE_H

class mine
{
    protected:

    public:
           bool  m_active;
           int   m_HP;
           float m_pos[3];
           int   m_startDelay;
           bool  m_trigged;
           int   m_textureID;


           mine();
           void newMine(int,float,int);
           bool updateMine(void);
           void drawMine(void);
           bool mineBulletHitTest(float,float,float);

};

#endif
