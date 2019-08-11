#include "mine.h"
#include <gl\gl.h>

mine::mine()
{
    m_active=false;
    m_pos[0]=0;m_pos[1]=0;m_pos[2]=0;
    m_HP=0;
    m_startDelay=0;
}

void mine::newMine(int x,float y,int z)
{
    m_active=true;
    m_trigged=false;
    m_pos[0]=x;m_pos[1]=y;m_pos[2]=z;
    m_HP=10;
    m_startDelay=300;
}

bool mine::updateMine(void)
{
    if (m_HP<1) {return false;}
    if (m_trigged) {if (--m_startDelay<-20) return false;}
    //m_pos[1]=height;
    if (m_startDelay>0) m_startDelay--;
    return true;
}

void mine::drawMine(void)
{
    float mine[]={
                  0.1,0.1,0.1,  0.1,0.1,0.9,  0.9,0.1,0.9,  0.9,0.1,0.1,
                  0.1,0.1,0.1,  0.1,0,0.1,  0.1,0,0.9,  0.1,0.1,0.9,
                  0.1,0.1,0.1,  0.9,0.1,0.1,  0.9,0,0.1,  0.1,0,0.1,
                  0.9,0.1,0.1,  0.9,0.1,0.9,  0.9,0,0.9,  0.9,0,0.1,
                  0.1,0.1,0.9,  0.1,0,0.9,  0.9,0,0.9,  0.9,0.1,0.9,
                 };
    float tex_mine[]={0.344,0.414,  0.465,0.414,  0.465,0.535,  0.344,0.535,
                      0.500,0.535,  0.465,0.535,  0.465,0.414,  0.500,0.414,
                      0.500,0.414,  0.500,0.535,  0.465,0.535,  0.465,0.414,
                      0.500,0.414,  0.500,0.535,  0.465,0.535,  0.465,0.414,
                      0.500,0.535,  0.465,0.535,  0.465,0.414,  0.500,0.414
                     };
 /*   float color_mine[]={
                        0.8,0.8,0.4,  0.8,0.8,0.4,  0.8,0.8,0.4,  0.8,0.8,0.4,
                        0.8,0.8,0.4,  0.6,0.6,0.3,  0.6,0.6,0.3,  0.8,0.8,0.4,
                        0.8,0.8,0.4,  0.8,0.8,0.4,  0.6,0.6,0.3,  0.6,0.6,0.3,
                        0.8,0.8,0.4,  0.8,0.8,0.4,  0.6,0.6,0.3,  0.6,0.6,0.3,
                        0.8,0.8,0.4,  0.6,0.6,0.3,  0.6,0.6,0.3,  0.8,0.8,0.4
                       }; */
glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    //glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mine);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_mine);
    //glColorPointer(3, GL_FLOAT, 0, color_mine);
    glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
    glDrawArrays(GL_QUADS, 0, 20);
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_COLOR_ARRAY);
}

bool mine::mineBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=0.5;
    float yhitbox=0.3;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+1 && ypos<m_pos[1]+yhitbox+1 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}
