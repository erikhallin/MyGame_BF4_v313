#include "heli.h"
#include <gl\gl.h>

heli::heli()
{
    m_active=false;
    m_inUse=false;
    m_user=false;
}

void heli::newHeli(float x, float y, float z)
{
    m_active=true;
    m_inUse=false;
    m_user=false;
    m_bladeRot=0;
    m_yrot=0;
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_HP=200;
    m_noiseID=-1;
    m_yvel=0;
    m_terrain_height=y;
}

int heli::update(float cycleTime) //Passive
{
    if (!m_inUse) m_noiseID=-1; //Turn of engine if empty
    if (m_HP<1) {m_active=false; m_inUse=false; return 1;} //Death
    if (m_pos[1]>m_terrain_height)
    {
        m_yvel-=cycleTime*0.00007;
        m_pos[1]+=m_yvel;
        if (m_pos[1]<m_terrain_height)
        {
            m_pos[1]=m_terrain_height;
            m_yvel=0;
        }
    }
    else m_pos[1]=m_terrain_height;
    return 0;
}

int heli::update(float x,float y,float z,float rot,float vel) //In use
{
    if (!m_inUse) m_noiseID=-1; //Turn of engine if empty
    if (m_HP<1) {m_active=false; m_inUse=false; return 1;} //Death
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    //m_yrot=rot;
    m_bladeRot+=vel*10;
    if (m_bladeRot>360) m_bladeRot-=360;
    return 0;
}

void heli::drawHeli(void)
{
    float vex_heli[]={
                    //Stand
                    0.1,0,0.15,  0.2,0,0.15,  0.2,0.5,0.15,  0.1,0.5,0.15,
                    0.2,0,0.05,  0.1,0,0.05,  0.1,0.5,0.05,  0.2,0.5,0.05,
                    0.2,0,0.15,  0.2,0,0.05,  0.2,0.5,0.05,  0.2,0.5,0.15,
                    0.1,0,0.05,  0.1,0,0.15,  0.1,0.5,0.15,  0.1,0.5,0.05,

                    //Backpack
                    0,0.5,0.2,  0.3,0.5,0.2,  0.3,0.8,0.2,  0,0.8,0.2, //F
                    0.3,0.5,0.0,  0,0.5,0.0,  0,0.8,0.0,  0.3,0.8,0.0, //B
                    0.3,0.5,0.2,  0.3,0.5,0,  0.3,0.8,0,  0.3,0.8,0.2, //S
                    0.0,0.5,0,  0.0,0.5,0.2,  0.0,0.8,0.2,  0.0,0.8,0, //S
                    0,0.5,0,  0.3,0.5,0,  0.3,0.5,0.2,  0,0.5,0.2, //L
                    0.3,0.8,0,  0,0.8,0,  0,0.8,0.2,  0.3,0.8,0.2, //T

                    //Top
                    0.05,0.8,0.175,  0.25,0.8,0.175,  0.25,1,0.175,  0.05,1,0.175,//B
                    0.25,0.8,0.025,  0.05,0.8,0.025,  0.05,1,0.025,  0.25,1,0.025,//F
                    0.25,0.8,0.175,  0.25,0.8,0.025,  0.25,1,0.025,  0.25,1,0.175,//S
                    0.05,0.8,0.025,  0.05,0.8,0.175,  0.05,1,0.175,  0.05,1,0.025,//S
                    0.05,1,0.175,  0.25,1,0.175,  0.25,1,0.025,  0.05,1,0.025, //T

                    //Arm1
                    0.25,0.88,0.12,  0.4,0.88,0.12,  0.4,0.92,0.12,  0.25,0.92,0.12,
                    0.4,0.88,0.08,  0.25,0.88,0.08,  0.25,0.92,0.08,  0.4,0.92,0.08,
                    0.25,0.92,0.12,  0.4,0.92,0.12,  0.4,0.92,0.08,  0.25,0.92,0.08,
                    0.4,0.88,0.12,  0.25,0.88,0.12,  0.25,0.88,0.08,  0.4,0.88,0.08,

                    //Arm2
                    -0.1,0.88,0.12,  0.05,0.88,0.12,  0.05,0.92,0.12,  -0.1,0.92,0.12,
                    0.05,0.88,0.08,  -0.1,0.88,0.08,  -0.1,0.92,0.08,  0.05,0.92,0.08,
                    -0.1,0.92,0.12,  0.05,0.92,0.12,  0.05,0.92,0.08,  -0.1,0.92,0.08,
                    0.05,0.88,0.12,  -0.1,0.88,0.12,  -0.1,0.88,0.08,  0.05,0.88,0.08,

                    //Fan1
                    //Outsides
                    0.4,0.85,0.25,  0.7,0.85,0.25,  0.7,0.95,0.25,  0.4,0.95,0.25,
                    0.7,0.85,-0.05,  0.4,0.85,-0.05,  0.4,0.95,-0.05,  0.7,0.95,-0.05,
                    0.7,0.85,0.25,  0.7,0.85,0.-0.05,  0.7,0.95,-0.05,  0.7,0.95,0.25,
                    0.4,0.85,0.-0.05,  0.4,0.85,0.25,  0.4,0.95,0.25,  0.4,0.95,-0.05,
                    //Insides
                    0.45,0.85,0,  0.65,0.85,0,  0.65,0.95,0,  0.45,0.95,0,
                    0.65,0.85,0.2,  0.45,0.85,0.2,  0.45,0.95,0.2,  0.65,0.95,0.2,
                    0.45,0.85,0.2,  0.45,0.85,0,  0.45,0.95,0,  0.45,0.95,0.2,
                    0.65,0.85,0,  0.65,0.85,0.2,  0.65,0.95,0.2,  0.65,0.95,0,
                    //Tops
                    0.4,0.95,0.25,  0.7,0.95,0.25,  0.65,0.95,0.2,  0.45,0.95,0.2,
                    0.7,0.95,-0.05,  0.4,0.95,-0.05,  0.45,0.95,0,  0.65,0.95,0,
                    0.7,0.95,0.25,  0.7,0.95,-0.05,  0.65,0.95,0,  0.65,0.95,0.2,
                    0.4,0.95,-0.05,  0.4,0.95,0.25,  0.45,0.95,0.2,  0.45,0.95,0,
                    //Downs
                    0.7,0.85,0.25,  0.4,0.85,0.25,  0.45,0.85,0.2,  0.65,0.85,0.2,
                    0.4,0.85,-0.05,  0.7,0.85,-0.05,  0.65,0.85,0,  0.45,0.85,0,
                    0.7,0.85,-0.05,  0.7,0.85,0.25,  0.65,0.85,0.2,  0.65,0.85,0,
                    0.4,0.85,0.25,  0.4,0.85,-0.05,  0.45,0.85,0,  0.45,0.85,0.2,

                    //Fan2
                    //Outsides
                    -0.4,0.85,0.25,  -0.1,0.85,0.25,  -0.1,0.95,0.25,  -0.4,0.95,0.25,
                    -0.1,0.85,-0.05,  -0.4,0.85,-0.05,  -0.4,0.95,-0.05,  -0.1,0.95,-0.05,
                    -0.1,0.85,0.25,  -0.1,0.85,0.-0.05,  -0.1,0.95,-0.05,  -0.1,0.95,0.25,
                    -0.4,0.85,0.-0.05,  -0.4,0.85,0.25,  -0.4,0.95,0.25,  -0.4,0.95,-0.05,
                    //Insides
                    -0.35,0.85,0,  -0.15,0.85,0,  -0.15,0.95,0,  -0.35,0.95,0,
                    -0.15,0.85,0.2,  -0.35,0.85,0.2,  -0.35,0.95,0.2,  -0.15,0.95,0.2,
                    -0.35,0.85,0.2,  -0.35,0.85,0,  -0.35,0.95,0,  -0.35,0.95,0.2,
                    -0.15,0.85,0,  -0.15,0.85,0.2,  -0.15,0.95,0.2,  -0.15,0.95,0,
                    //Tops
                    -0.4,0.95,0.25,  -0.1,0.95,0.25,  -0.15,0.95,0.2,  -0.35,0.95,0.2,
                    -0.1,0.95,-0.05,  -0.4,0.95,-0.05,  -0.35,0.95,0,  -0.15,0.95,0,
                    -0.1,0.95,0.25,  -0.1,0.95,-0.05,  -0.15,0.95,0,  -0.15,0.95,0.2,
                    -0.4,0.95,-0.05,  -0.4,0.95,0.25,  -0.35,0.95,0.2,  -0.35,0.95,0,
                    //Downs
                    -0.1,0.85,0.25,  -0.4,0.85,0.25,  -0.35,0.85,0.2,  -0.15,0.85,0.2,
                    -0.4,0.85,-0.05,  -0.1,0.85,-0.05,  -0.15,0.85,0,  -0.35,0.85,0,
                    -0.1,0.85,-0.05,  -0.1,0.85,0.25,  -0.15,0.85,0.2,  -0.15,0.85,0,
                    -0.4,0.85,0.25,  -0.4,0.85,-0.05,  -0.35,0.85,0,  -0.35,0.85,0.2
                     };
    float tex_heli[]={
                    //Stand
                    0.001,0.807,  0.039,0.807,  0.039,0.999,  0.001,0.999,
                    0.001,0.807,  0.039,0.807,  0.039,0.999,  0.001,0.999,
                    0.001,0.807,  0.039,0.807,  0.039,0.999,  0.001,0.999,
                    0.001,0.807,  0.039,0.807,  0.039,0.999,  0.001,0.999,

                    //Backpack
                    0.043,0.906,  0.135,0.906,  0.135,0.999,  0.043,0.999, //F
                    0.232,0.906,  0.326,0.906,  0.326,0.999,  0.232,0.999, //B
                    0.139,0.906,  0.230,0.906,  0.230,0.999,  0.139,0.999, //S
                    0.139,0.906,  0.230,0.906,  0.230,0.999,  0.139,0.999, //S
                    0.434,0.906,  0.518,0.906,  0.518,0.999,  0.434,0.999, //L
                    0.328,0.906,  0.422,0.906,  0.422,0.999,  0.328,0.999, //T

                    //Top
                    0.520,0.922,  0.598,0.922,  0.598,0.999,  0.520,0.999,//B
                    0.680,0.922,  0.758,0.922,  0.758,0.999,  0.680,0.999,//F
                    0.600,0.922,  0.678,0.922,  0.678,0.999,  0.600,0.999,//S
                    0.600,0.922,  0.678,0.922,  0.678,0.999,  0.600,0.999,//S
                    0.760,0.922,  0.835,0.922,  0.835,0.999,  0.760,0.999,//T

                    //Arm1
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,

                    //Arm2
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,
                    0.840,0.967,  0.939,0.967,  0.939,0.999,  0.840,0.999,

                    //Fan1
                    //Outsides
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,

                    //Insides
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,

                    //Tops
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,

                    //Downs
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,

                    //Fan1
                    //Outsides
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,
                    0.041,0.848,  0.215,0.848,  0.215,0.904,  0.041,0.904,

                    //Insides
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,
                    0.217,0.848,  0.342,0.848,  0.342,0.904,  0.217,0.904,

                    //Tops
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,
                    0.840,0.922,  0.999,0.922,  0.967,0.963,  0.871,0.963,

                    //Downs
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920,
                    0.840,0.877,  0.999,0.877,  0.967,0.920,  0.871,0.920
                     };
    float vex_blade[]={
                    //Center
                    -0.1,-0.1,-0.1,  0.1,-0.1,-0.1,  0.1,0.1,-0.1,  -0.1,0.1,-0.1,
                    0.1,-0.1,0.1,  -0.1,-0.1,0.1,  -0.1,0.1,0.1,  0.1,0.1,0.1,
                    0.1,-0.1,0.1,  0.1,-0.1,-0.1,  0.1,0.1,-0.1,  0.1,0.1,0.1,
                    -0.1,-0.1,-0.1,  -0.1,-0.1,0.1,  -0.1,0.1,0.1,  -0.1,0.1,-0.1,
                    -0.1,0.1,0.1,  0.1,0.1,0.1,  0.1,0.1,-0.1,  -0.1,0.1,-0.1,
                    0.1,-0.1,0.1,  -0.1,-0.1,0.1,  -0.1,-0.1,-0.1,  0.1,-0.1,-0.1,

                    //Blades
                    0.1,0,0.08,  0.6,0,0.08,  0.6,0,-0.08,  0.1,0,-0.08,
                    0.08,0,-0.1,  0.08,0,-0.6,  -0.08,0,-0.6,  -0.08,0,-0.1,
                    -0.1,0,-0.08,  -0.6,0,-0.08,  -0.6,0,0.08,  -0.1,0,0.08,
                    -0.08,0,0.1,  -0.08,0,0.6,  0.08,0,0.6,  0.08,0,0.1
                      };
    float tex_blade[]={
                    //Center
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,
                    0.490,0.877,  0.518,0.877,  0.518,0.902,  0.490,0.902,

                    //Blade
                    0.344,0.877,  0.488,0.877,  0.488,0.904,  0.344,0.904,
                    0.344,0.877,  0.488,0.877,  0.488,0.904,  0.344,0.904,
                    0.344,0.877,  0.488,0.877,  0.488,0.904,  0.344,0.904,
                    0.344,0.877,  0.488,0.877,  0.488,0.904,  0.344,0.904
                      };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    //Draw Chassi
    glVertexPointer(3, GL_FLOAT, 0, vex_heli);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_heli);

    if (m_inUse)
    {
        glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
        glRotatef(m_yrot-90,0,1,0);
        glTranslatef(-0.25,-1.7,0.5);
    }
    else
    {
        glTranslatef(m_pos[0]+0.25, m_pos[1], m_pos[2]+0.25);
        glRotatef(m_yrot-90,0,1,0);
    }

glPushMatrix();
    glScalef(2,2,2);
    glDrawArrays(GL_QUADS, 0, 220);
glPopMatrix();
    //Draw Rotors
    glScalef(0.3,0.3,0.3);
    glVertexPointer(3, GL_FLOAT, 0, vex_blade);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_blade);
glPushMatrix();
    glTranslatef(3.65,6,0.65);
    glRotatef(m_bladeRot,0,1,0);
    glDrawArrays(GL_QUADS, 0, 40);
glPopMatrix();
glPushMatrix();
    glTranslatef(-1.65,6,0.65);
    glRotatef(m_bladeRot,0,-1,0);
    glDrawArrays(GL_QUADS, 0, 40);
glPopMatrix();
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool heli::heliBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=0.5;
    float yhitbox=1.5;
    float zhitbox=0.5;
    if (xpos>m_pos[0]-xhitbox+0.5 && xpos<m_pos[0]+xhitbox+0.5 &&
        ypos>m_pos[1]-yhitbox+1 && ypos<m_pos[1]+yhitbox+1 &&
        zpos>m_pos[2]-zhitbox+0.5 && zpos<m_pos[2]+zhitbox+0.5)
    return true;
    else
    return false;
}

void heli::place(float xpos,float height,float zpos)
{
    m_inUse=false;
    m_noiseID=-1;
    m_pos[0]=xpos;
    m_terrain_height=height;
    m_pos[2]=zpos;
}