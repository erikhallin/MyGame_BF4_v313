#include "speed.h"
#include <gl\gl.h>

speed::speed()
{
    m_active=false;
    m_inUse=false;
    m_user=false;
}

void speed::newSpeed(float x, float y, float z)
{
    m_active=true;
    m_inUse=false;
    m_user=false;
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_yrot=0;
    m_HP=200;
    m_noiseID=-1;
}

int speed::update(void) //Passive
{
    if (!m_inUse) m_noiseID=-1; //Turn of engine if empty
    if (m_HP<1) {m_active=false; return 1;} //Death
    //m_pos[1]=height;
    return 0;
}

int speed::update(float x,float y,float z,float rot) //In use
{
    if (!m_inUse) m_noiseID=-1; //Turn of engine if empty
    if (m_HP<1) {m_active=false; return 1;} //Death
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_yrot=rot;
    return 0;
}

void speed::drawSpeed(void)
{
    float vex_speed[]={
/*        //Floor
        0,0,1,  1,0,1,  1,0.2,1,  0,0.2,1, //F
        1,0,0,  0,0,0,  0,0.2,0,  1,0.2,0, //B
        1,0,1,  1,0,0,  1,0.2,0,  1,0.2,1, //R
        0,0,0,  0,0,1,  0,0.2,1,  0,0.2,0,//L
        0,0.2,1,  1,0.2,1,  1,0.2,0,  0,0.2,0, //T
        1,0,1,  0,0,1,  0,0,0,  1,0,0, //U

        //Stand
        0.55,0.2,0.85,  0.45,0.2,0.85,  0.45,1.2,0.85,  0.55,1.2,0.85, //F
        0.45,0.2,0.95,  0.55,0.2,0.95,  0.55,1.2,0.95,  0.45,1.2,0.95, //B
        0.55,0.2,0.95,  0.55,0.2,0.85,  0.55,1.2,0.85,  0.55,1.2,0.95, //R
        0.45,0.2,0.85,  0.45,0.2,0.95,  0.45,1.2,0.95,  0.45,1.2,0.85, //L

        //Handle
        1.1,1.2,0.8,  -0.1,1.2,0.8,  -0.1,1.35,0.8,  1.1,1.35,0.8, //F
        -0.1,1.2,1,  1.1,1.2,1,  1.1,1.35,1,  -0.1,1.35,1, //B
        1.1,1.2,1,  1.1,1.2,0.8,  1.1,1.35,0.8,  1.1,1.35,1, //R
        -0.1,1.2,0.8,  -0.1,1.2,1,  -0.1,1.35,1,  -0.1,1.35,0.8, //L
        -0.1,1.35,1,  1.1,1.35,1,  1.1,1.35,0.8,  -0.1,1.35,0.8, //T
        1.1,1.2,1,  -0.1,1.2,1,  -0.1,1.2,0.8,  1.1,1.2,0.8 //U   */

        //Floor
        -0.5,0,0.5,  0.5,0,0.5,  0.5,0.2,0.5,  -0.5,0.2,0.5, //F
        0.5,0,-0.5,  -0.5,0,-0.5,  -0.5,0.2,-0.5,  0.5,0.2,-0.5, //B
        0.5,0,0.5,  0.5,0,-0.5,  0.5,0.2,-0.5,  0.5,0.2,0.5, //R
        -0.5,0,-0.5,  -0.5,0,0.5,  -0.5,0.2,0.5,  -0.5,0.2,-0.5,//L
        -0.5,0.2,0.5,  0.5,0.2,0.5,  0.5,0.2,-0.5,  -0.5,0.2,-0.5, //T
        0.5,0,0.5,  -0.5,0,0.5,  -0.5,0,-0.5,  0.5,0,-0.5, //U

        //Stand
        0.05,0.2,0.35,  -0.05,0.2,0.35,  -0.05,1.2,0.35,  0.05,1.2,0.35, //F
        -0.05,0.2,0.45,  0.05,0.2,0.45,  0.05,1.2,0.45,  -0.05,1.2,0.45, //B
        0.05,0.2,0.45,  0.05,0.2,0.35,  0.05,1.2,0.35,  0.05,1.2,0.45, //R
        -0.05,0.2,0.35,  -0.05,0.2,0.45,  -0.05,1.2,0.45,  -0.05,1.2,0.35, //L

        //Handle
        0.6,1.2,0.3,  -0.6,1.2,0.3,  -0.6,1.35,0.3,  0.6,1.35,0.3, //F
        -0.6,1.2,0.5,  0.6,1.2,0.5,  0.6,1.35,0.5,  -0.6,1.35,0.5, //B
        0.6,1.2,0.5,  0.6,1.2,0.3,  0.6,1.35,0.3,  0.6,1.35,0.5, //R
        -0.6,1.2,0.3,  -0.6,1.2,0.5,  -0.6,1.35,0.5,  -0.6,1.35,0.3, //L
        -0.6,1.35,0.5,  0.6,1.35,0.5,  0.6,1.35,0.3,  -0.6,1.35,0.3, //T
        0.6,1.2,0.5,  -0.6,1.2,0.5,  -0.6,1.2,0.3,  0.6,1.2,0.3 //U
                      };
    float tex_speed[]={
        //Floor
        0.001,0.611,  0.154,0.611,  0.154,0.648,  0.001,0.648, //F
        0.001,0.611,  0.154,0.611,  0.154,0.648,  0.001,0.648, //B
        0.001,0.611,  0.154,0.611,  0.154,0.648,  0.001,0.648, //R
        0.001,0.611,  0.154,0.611,  0.154,0.648,  0.001,0.648, //L
        0.001,0.650,  0.154,0.650,  0.154,0.803,  0.001,0.803, //T
        0.156,0.650,  0.311,0.650,  0.311,0.805,  0.156,0.805, //U

        //Stand
        0.313,0.615,  0.350,0.615,  0.350,0.846,  0.313,0.846, //F
        0.313,0.615,  0.350,0.615,  0.350,0.846,  0.313,0.846, //B
        0.313,0.615,  0.350,0.615,  0.350,0.846,  0.313,0.846, //R
        0.313,0.615,  0.350,0.615,  0.350,0.846,  0.313,0.846, //L

        //Handle
        0.001,0.572,  0.311,0.572,  0.311,0.607,  0.001,0.607, //F
        0.001,0.572,  0.311,0.572,  0.311,0.607,  0.001,0.607, //B
        0.313,0.572,  0.350,0.572,  0.350,0.607,  0.313,0.607, //R
        0.313,0.572,  0.350,0.572,  0.350,0.607,  0.313,0.607, //L
        0.001,0.572,  0.311,0.572,  0.311,0.607,  0.001,0.607, //T
        0.001,0.572,  0.311,0.572,  0.311,0.607,  0.001,0.607 //U
                      };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    //Draw Chassi
    glVertexPointer(3, GL_FLOAT, 0, vex_speed);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_speed);

    if (m_inUse)
    {
        glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
        glRotatef(m_yrot+90,0,1,0);
        glTranslatef(0,-1.8,0);
    }
    else
    {
        glTranslatef(m_pos[0]+0.5, m_pos[1]+0.2, m_pos[2]+0.5);
        glRotatef(m_yrot+90,0,1,0);
    }
    glDrawArrays(GL_QUADS, 0, 64);

glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool speed::speedBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=0.5;
    float yhitbox=0.75;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+0.7 && ypos<m_pos[1]+yhitbox+0.7 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}

void speed::place(float xpos,float height,float zpos)
{
    m_inUse=false;
    m_noiseID=-1;
    m_pos[0]=xpos;
    m_pos[1]=height;
    m_pos[2]=zpos;
}
