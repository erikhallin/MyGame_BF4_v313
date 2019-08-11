#include <gl\gl.h>
#include "outpost.h"

outpost::outpost()
{
    m_active=false;
}

void outpost::newOutpost(float x,float y,float z,int radarID)
{
    m_active=true;
    m_pos[0]=x; m_pos[1]=y; m_pos[2]=z;
    m_connected_radarID=radarID;
    m_HP=100;
    m_moving=false;
    m_yrot=0;
}

int outpost::update(void)
{
    if (m_HP<1)
    {
        m_active=false;
        return 1;
    }

    //m_pos[1]=height;

    return 0;
}

int outpost::update(float xpos,float ypos,float zpos,float yrot)
{
    if (m_HP<1)
    {
        m_active=false;
        return 1;
    }
    m_pos[0]=xpos;
    m_pos[1]=ypos;
    m_pos[2]=zpos;
    m_yrot=yrot;

    return 0;
}

void outpost::drawOutpost(void)
{
    float body[]={
        0,0, 1,  1,0, 1,  1, 1, 1, 0, 1, 1,//  1 2 3 4 //back
        1,0, 1,  1,0,0,  1, 1,0,  1, 1, 1,//  2 5 6 3 //R side
        1,0,0, 0,0,0, 0, 1,0,  1, 1,0,//  5 7 8 6 //front
        0,0,0, 0,0, 1, 0, 1, 1, 0, 1,0,//  7 1 4 8 //L side
        0,0,0,  1,0,0,  1,0, 1, 0,0, 1,//  7 5 2 1 //bottom
        0, 1, 1,  1, 1, 1,  1, 1,0, 0, 1,0// 4 3 6 8 //top
        };

    glEnableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
        glVertexPointer(3, GL_FLOAT, 0, body);
        glTranslatef(m_pos[0],m_pos[1],m_pos[2]);
        if (m_moving) glRotatef(m_yrot,0,1,0);
        glDrawArrays(GL_QUADS,0,24);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
}

bool outpost::outpostBulletHitTest(float xpos,float ypos,float zpos)
{
    float xhitbox=0.5;
    float yhitbox=0.5;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+0.5 && ypos<m_pos[1]+yhitbox+0.5 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}

void outpost::place(float xpos,float height,float zpos)
{
    m_moving=false;
    m_pos[0]=(int)xpos;
    m_pos[1]=height;
    m_pos[2]=(int)zpos;
}
