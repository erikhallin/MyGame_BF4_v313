#include "powerup.h"
#include <gl\gl.h>

powerup::powerup()
{
    m_active=false;
    m_moving=false;
    m_carrier=-1;
}

void powerup::newPowerup(int x, float y, int z)
{
    m_active=true;
    m_HP=10;
    m_moving=false;
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
}

void powerup::updatePos(float x,float y,float z,float yhed)
{
    //Spawner is carried
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_yhed=yhed;
}

int powerup::update(void)
{
    if (m_HP<1)
    {
        m_active=false;
        return 1;
    }
    return 0;
}

void powerup::drawPowerup(void)
{
    float pow[]={0.5,1,0.5, 0,0,1, 1,0,1,
                 0.5,1,0.5, 1,0,1, 1,0,0,
                 0.5,1,0.5, 1,0,0, 0,0,0,
                 0.5,1,0.5, 0,0,0, 0,0,1
                };
    float tex_pow[]={0.539,0.410,  0.469,0.281,  0.609,0.281,
                     0.539,0.410,  0.469,0.281,  0.609,0.281,
                     0.539,0.410,  0.469,0.281,  0.609,0.281,
                     0.539,0.410,  0.469,0.281,  0.609,0.281,
                    };
 /*   float color[]={0.2,0.7,0.0, 0.0,0.5,0.0, 0.1,0.5,0.1,
                   0.2,0.7,0.0, 0.1,0.5,0.1, 0.2,0.5,0.3,
                   0.2,0.7,0.0, 0.2,0.5,0.3, 0.1,0.6,0.3,
                   0.2,0.7,0.0, 0.1,0.6,0.3, 0.0,0.5,0.0};*/
glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glVertexPointer(3, GL_FLOAT, 0, pow);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_pow);
    //glColorPointer(3, GL_FLOAT, 0, color);
    if (m_moving)
    {
        glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
        glRotatef(m_yhed,0,1,0);
        glRotatef(70,0,0,1);
        glTranslatef(-1.35,0.05,-0.4);
    }
    else
    {
        glTranslatef(m_pos[0]+0.1, m_pos[1], m_pos[2]+0.1);
    }
    glScalef(0.8,0.8,0.8);
    glDrawArrays(GL_TRIANGLES, 0, 12);
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_COLOR_ARRAY);
}

bool powerup::powerupBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=0.5;
    float yhitbox=1;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+1 && ypos<m_pos[1]+yhitbox+1 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}

void powerup::place(float xpos,float height,float zpos)
{
    m_moving=false;
    m_pos[0]=(int)xpos;
    m_pos[1]=height;
    m_pos[2]=(int)zpos;
}
