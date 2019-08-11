#include <gl\gl.h>
#include "wall.h"

wall::wall()
{
    m_active=false;           
}

void wall::newWall(int x,float y,int z)
{
    m_active=true;
    m_HP=1000;
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
}

int wall::update(void)
{
    if (m_HP<1) //Death
    {
        m_active=false;
        return 1; 
    }
    return 0;    
}

void wall::drawWall(void)
{
    float vex_wall[]={
                       0,4,0, 0,4,1, 1,4,1, 1,4,0, //Top
                       0,4,1, 0,0,1, 1,0,1, 1,4,1, //Back
                       1,4,1, 1,0,1, 1,0,0, 1,4,0, //Right
                       1,4,0, 1,0,0, 0,0,0, 0,4,0, //Front
                       0,4,0, 0,0,0, 0,0,1, 0,4,1  //Left
                    };
    float tex_wall[]={ 
                       0.301,0.217,  0.400,0.217,  0.400,0.312,  0.301,0.312, //Top
                       //Sides
                       0.303,0.629,  0.303,0.316,  0.398,0.316,  0.398,0.629,
                       0.303,0.629,  0.303,0.316,  0.398,0.316,  0.398,0.629,
                       0.303,0.629,  0.303,0.316,  0.398,0.316,  0.398,0.629,
                       0.303,0.629,  0.303,0.316,  0.398,0.316,  0.398,0.629,                         
                     };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glVertexPointer(3, GL_FLOAT, 0, vex_wall);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_wall);
    glTranslatef(m_pos[0],m_pos[1],m_pos[2]);
    glDrawArrays(GL_QUADS, 0, 20);    
glPopMatrix();     
    glDisableClientState(GL_VERTEX_ARRAY);         
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);       
}

bool wall::wallBulletHitTest(float xpos,float ypos,float zpos)
{
    float xhitbox=0.5;
    float yhitbox=2;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+2 && ypos<m_pos[1]+yhitbox+2 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;      
}
