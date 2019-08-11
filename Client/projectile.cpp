#include <gl\gl.h>
#include <math.h>
#include "projectile.h"

proj::proj()
{
    m_active=false;
    m_age=0;
}

void proj::newProj(float data[])
{
    m_active=true;
    m_age=0;
    m_xpos=data[0];
    m_ypos=data[1];
    m_zpos=data[2];
    m_yrot=data[3];
    m_xrot=data[4];
    m_fromPlayer=(int)data[5];
    m_type=(int)data[6];

    float piover180=0.0174532925;
    m_xhed=(float)(sin((m_xrot)*piover180)*cos((m_yrot)*piover180));
    m_yhed=(float)cos((m_xrot)*piover180);
    m_zhed=-(float)sin((m_xrot)*piover180)*sin((m_yrot)*piover180);
}

bool proj::updateProj(float cycleTime)
{
    //Lifetime
    m_age+=cycleTime;
    if (m_age>10000) return false;
    //get new pos
    float sens=0.03;
    switch (m_type)
    {
        case 1:{//Rocket
                sens=0.03;
               }break;
        case 2:{//Tank Cannon
                sens=0.03;
               }break;
    }
    m_xpos+=m_xhed*cycleTime*sens;
    m_ypos+=m_yhed*cycleTime*sens;
    m_zpos+=m_zhed*cycleTime*sens;
    /*//check if inside world
    if (m_xpos<1 || m_xpos>999 || m_zpos<1 || m_zpos>999)
    return false;
    else*/
    return true;
}

void proj::drawProj(void)
{
    if (m_type==2) return; //Tank Cannon Shells cant be seen

    float rocket[]={
                    //Tube
                    -0.05,0.05,0.5,  -0.05,0.05,-0.5,  -0.05,-0.05,-0.5,  -0.05,-0.05,0.5,
                    0.05,0.05,0.5,  0.05,-0.05,0.5,  0.05,-0.05,-0.5,  0.05,0.05,-0.5,
                    0.05,0.05,0.5,  0.05,0.05,-0.5,  -0.05,0.05,-0.5,  -0.05,0.05,0.5,
                    0.05,-0.05,0.5,  -0.05,-0.05,0.5,  -0.05,-0.05,-0.5,  0.05,-0.05,-0.5,
                    //Hat
                    -0.05,-0.05,-0.5,  -0.05,0.05,-0.5,  0,-0.001,-0.6,  0,0.001,-0.6,
                    0.05,0.05,-0.5,  -0.05,0.05,-0.5,  -0.001,0,-0.6,  0.001,0,-0.6,
                    0.05,-0.05,-0.5,  0.05,0.05,-0.5,  0,0.001,-0.6,  0,-0.001,-0.6,
                    -0.05,-0.05,-0.5,  0.05,-0.05,-0.5,  0.001,0,-0.6,  -0.001,0,-0.6,
                    //Fins
                    0.05,0,0.4,  0.25,0,0.4,  0.15,0,0.3,  0.05,0,0.3,
                    0,0.05,0.4,  0,0.25,0.4,  0,0.15,0.3,  0,0.05,0.3,
                    -0.05,0,0.4,  -0.25,0,0.4,  -0.15,0,0.3,  -0.05,0,0.3,
                    0,-0.05,0.4,  0,-0.25,0.4,  0,-0.15,0.3,  0,-0.05,0.3
                   };
    float tex_rocket[]={
                        //Tube
                        0.703,0.207,  0.703,0.410,  0.645,0.410,  0.645,0.207,
                        0.645,0.207,  0.703,0.207,  0.703,0.410,  0.645,0.410,
                        0.703,0.207,  0.703,0.410,  0.645,0.410,  0.645,0.207,
                        0.645,0.207,  0.703,0.207,  0.703,0.410,  0.645,0.410,
                        //Hat
                        0.469,0.344,  0.535,0.410,  0.440,0.410,  0.469,0.409,
                        0.469,0.344,  0.535,0.410,  0.440,0.410,  0.469,0.409,
                        0.469,0.344,  0.535,0.410,  0.440,0.410,  0.469,0.409,
                        0.469,0.344,  0.535,0.410,  0.440,0.410,  0.469,0.409,
                        //Fins
                        0.641,0.410,  0.543,0.410,  0.594,0.359,  0.641,0.359,
                        0.641,0.410,  0.543,0.410,  0.594,0.359,  0.641,0.359,
                        0.641,0.410,  0.543,0.410,  0.594,0.359,  0.641,0.359,
                        0.641,0.410,  0.543,0.410,  0.594,0.359,  0.641,0.359
                       };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    //glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, rocket);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_rocket);
    //glColorPointer(3, GL_FLOAT, 0, colorR);
    glTranslatef(m_xpos,m_ypos,m_zpos);
    glRotatef(m_yrot-90,0,1,0);
    glRotatef(90-m_xrot,1,0,0);
    //glScalef(0.2,0.2,0.5);
    glDrawArrays(GL_QUADS, 0, 48);

glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool proj::projHitTest(float xpos,float ypos,float zpos)
{
    float xhitbox=0.5;
    float yhitbox=0.5;
    float zhitbox=0.5;
    if (xpos>m_xpos-xhitbox && xpos<m_xpos+xhitbox &&
        ypos>m_ypos-yhitbox && ypos<m_ypos+yhitbox &&
        zpos>m_zpos-zhitbox && zpos<m_zpos+zhitbox)
    return true;
    else
    return false;
}
