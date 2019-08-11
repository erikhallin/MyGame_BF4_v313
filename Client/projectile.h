#ifndef PROJECTILE_H
#define PROJECTILE_H

class proj
{
//      protected:
                
      public:
             proj();
             bool    m_active;
             int     m_type;
             float   m_age;
             int     m_fromPlayer;
             float   m_xpos,m_ypos,m_zpos;
             float   m_xhed,m_yhed,m_zhed;
             float   m_yrot,m_xrot;
             int     m_textureID;
             
             void    newProj(float data[]);
             bool    updateProj(float);
             void    drawProj(void);
             bool    projHitTest(float,float,float);
};

#endif
