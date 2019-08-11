#include <gl\gl.h>
#include "buildblock.h"

buildblock::buildblock()
{
    m_active=false;
    m_moving=false;
    m_carrier=-1;
    m_inHands=false;
}

void buildblock::newBuildblock(int type, int carrier)
{
    m_active=true;
    m_HP=100;
    m_moving=true;
    m_inHands=true;
    m_type=type;
    m_carrier=carrier;
}

void buildblock::updatePos(float x,float y,float z,float rot)
{
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_rot=rot;
}

int buildblock::update(void)
{
    if (m_HP<1) //Death
    {
        m_active=false;
        return 1;
    }
/*
    if (!m_moving)
    {
        //m_pos[1]=height;
        m_pos[0]=(int)m_pos[0];
        m_pos[2]=(int)m_pos[2];
    }*/
    return 0;
}

void buildblock::drawBuildblock(void)
{
    float vex_block[]={0,2,0, 0,2,1, 1,2,1, 1,2,0, //Top
                       0,0,0, 0,0,1, 1,0,1, 1,0,0, //Bottom
                       0,2,1, 0,0,1, 1,0,1, 1,2,1, //Back
                       1,2,1, 1,0,1, 1,0,0, 1,2,0, //Right
                       1,2,0, 1,0,0, 0,0,0, 0,2,0, //Front
                       0,2,0, 0,0,0, 0,0,1, 0,2,1  //Left
                    };

    float offset=float(m_type-1)/10;
    float tex_block[]={
                       0.001+offset,0.374,  0.098+offset,0.374,  0.098+offset,0.473,  0.001+offset,0.473,
                       0.001+offset,0.374,  0.001+offset,0.473,  0.098+offset,0.473,  0.098+offset,0.374,
                       0.001+offset,0.628,  0.001+offset,0.476,  0.098+offset,0.476,  0.098+offset,0.628,
                       0.001+offset,0.628,  0.001+offset,0.476,  0.098+offset,0.476,  0.098+offset,0.628,
                       0.001+offset,0.628,  0.001+offset,0.476,  0.098+offset,0.476,  0.098+offset,0.628,
                       0.001+offset,0.628,  0.001+offset,0.476,  0.098+offset,0.476,  0.098+offset,0.628
                      };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glVertexPointer(3, GL_FLOAT, 0, vex_block);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_block);
    glTranslatef(m_pos[0],m_pos[1],m_pos[2]);
    if (m_moving) {glRotatef(m_rot,0,1,0); glRotatef(-20,0,0,1);glTranslatef(-1.1,-1.3,-0.5);}
    glDrawArrays(GL_QUADS, 0, 24);
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool buildblock::buildblockBulletHitTest(float xpos, float ypos, float zpos)
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

void buildblock::place(float xpos,float height,float zpos)
{
    m_moving=false;
    m_pos[0]=(int)xpos;
    m_pos[1]=height;
    m_pos[2]=(int)zpos;
}
