#include "spawner.h"
#include <gl\gl.h>

spawner::spawner()
{
    m_active=false;
    m_team=0;
    m_open=true;
    m_HP=200;
    m_content=0;
    m_output=0;
    m_hitCount=0;
}

void spawner::newSpawner(int team,int ownerID,int xpos, int zpos, float ypos)
{
    m_team=team;
    m_active=true;
    m_moving=false;
    m_owner=ownerID;
    m_HP=200;
    m_pos[0]=xpos;
    m_pos[1]=ypos;
    m_pos[2]=zpos;
    m_price[0]=2; //Wass
    m_price[1]=3; //Tass
    m_price[2]=4; //Dass
}

void spawner::updateSpawner(float x,float y,float z,float yhed)
{
    //Spawner is carried
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_yhed=yhed;
}

bool spawner::updateSpawner(void)
{
    if (m_HP<1) {return false;}
    return true;
}

void spawner::drawSpawner(void)
{
    float closedBox[]={//Box
                       0,2,0, 0,2,1, 1,2,1, 1,2,0,
                       0,0,0, 0,0,1, 1,0,1, 1,0,0,
                       0,2,1, 0,0,1, 1,0,1, 1,2,1,
                       1,2,1, 1,0,1, 1,0,0, 1,2,0,
                       1,2,0, 1,0,0, 0,0,0, 0,2,0,
                       0,2,0, 0,0,0, 0,0,1, 0,2,1,
                       //Output
                       -0.2,1,0.1,  -0.2,1,0.9,  -0.2,1.8,0.9,  -0.2,1.8,0.1, //Plate
                       -0.2,1,0.9,  0.1,1,0.9,  0.1,1.8,0.9,  -0.2,1.8,0.9, //Right
                       -0.2,1,0.1,  -0.2,1.8,0.1,  0.1,1.8,0.1,  0.1,1,0.1, //Left
                       -0.2,1.8,0.9,  0.1,1.8,0.9,  0.1,1.8,0.1,  -0.2,1.8,0.1, //Top
                       -0.2,1,0.9,  -0.2,1,0.1,  0.1,1,0.1,  0.1,1,0.9 //Bottom
                       };
    float openBox[]={//Box
                     1,2,0, 1,2,1, 2,2.5,1, 2,2.5,0, //Lid
                     0,0,0, 0,0,1, 1,0,1, 1,0,0,     //Bottom
                     0,2,1, 0,0,1, 1,0,1, 1,2,1,     //Back
                     1,2,1, 1,0,1, 1,0,0, 1,2,0,     //Right
                     1,2,0, 1,0,0, 0,0,0, 0,2,0,     //Front
                     0,2,0, 0,0,0, 0,0,1, 0,2,1,     //Left
                     //Output
                    -0.2,1,0.1,  -0.2,1,0.9,  -0.2,1.8,0.9,  -0.2,1.8,0.1, //Plate
                    -0.2,1,0.9,  0.1,1,0.9,  0.1,1.8,0.9,  -0.2,1.8,0.9, //Right
                    -0.2,1,0.1,  -0.2,1.8,0.1,  0.1,1.8,0.1,  0.1,1,0.1, //Left
                    -0.2,1.8,0.9,  0.1,1.8,0.9,  0.1,1.8,0.1,  -0.2,1.8,0.1, //Top
                    -0.2,1,0.9,  -0.2,1,0.1,  0.1,1,0.1,  0.1,1,0.9 //Bottom
                     };
    float tex_box[]={//Box
                     0.879,0.414,  1,0.414,  1,0.523,  0.879,0.523,
                     0,0,  0.1,0,  0.1,0.1,  0,0.1,
                     0,0.410,  0,0.207,  0.113,0.207,  0.113,0.410,
                     0.113,0.410,  0.113,0.207,  0.230,0.207,  0.230,0.410,
                     0.230,0.410,  0.230,0.207,  0.348,0.207,  0.348,0.410,
                     0.348,0.410,  0.348,0.207,  0.465,0.207,  0.465,0.410,
                     //Output
                     0.469,0.146,  0.600,0.146,  0.600,0.277,  0.469,0.277,
                     0.602,0.146,  0.623,0.146,  0.623,0.277,  0.602,0.277,
                     0.602,0.277,  0.602,0.146,  0.623,0.146,  0.623,0.277,
                     0.602,0.146,  0.623,0.146,  0.623,0.277,  0.602,0.277,
                     0.602,0.277,  0.602,0.146,  0.623,0.146,  0.623,0.277
                    };
    float color0[]={0.3,0.3,0.7, 0.3,0.3,0.7, 0.5,0.5,0.9, 0.5,0.5,0.9,
                    0.3,0.3,0.7, 0.3,0.3,0.7, 0.5,0.5,0.9, 0.5,0.5,0.9,
                    0.3,0.3,0.7, 0.3,0.3,0.7, 0.5,0.5,0.9, 0.5,0.5,0.9,
                    0.3,0.3,0.7, 0.3,0.3,0.7, 0.5,0.5,0.9, 0.5,0.5,0.9,
                    0.3,0.3,0.7, 0.3,0.3,0.7, 0.3,0.3,0.7, 0.3,0.3,0.7,
                    0.5,0.5,0.9, 0.5,0.5,0.9, 0.5,0.5,0.9, 0.5,0.5,0.9,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color1[]={0.3,0.7,0.3, 0.3,0.7,0.3, 0.5,0.9,0.5, 0.5,0.9,0.5,
                    0.3,0.7,0.3, 0.3,0.7,0.3, 0.5,0.9,0.5, 0.5,0.9,0.5,
                    0.3,0.7,0.3, 0.3,0.7,0.3, 0.5,0.9,0.5, 0.5,0.9,0.5,
                    0.3,0.7,0.3, 0.3,0.7,0.3, 0.5,0.9,0.5, 0.5,0.9,0.5,
                    0.3,0.7,0.3, 0.3,0.7,0.3, 0.3,0.7,0.3, 0.3,0.7,0.3,
                    0.5,0.9,0.5, 0.5,0.9,0.5, 0.5,0.9,0.5, 0.5,0.9,0.5,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color2[]={0.7,0.3,0.3, 0.7,0.3,0.3, 0.9,0.5,0.5, 0.9,0.5,0.5,
                    0.7,0.3,0.3, 0.7,0.3,0.3, 0.9,0.5,0.5, 0.9,0.5,0.5,
                    0.7,0.3,0.3, 0.7,0.3,0.3, 0.9,0.5,0.5, 0.9,0.5,0.5,
                    0.7,0.3,0.3, 0.7,0.3,0.3, 0.9,0.5,0.5, 0.9,0.5,0.5,
                    0.7,0.3,0.3, 0.7,0.3,0.3, 0.7,0.3,0.3, 0.7,0.3,0.3,
                    0.9,0.5,0.5, 0.9,0.5,0.5, 0.9,0.5,0.5, 0.9,0.5,0.5,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color3[]={0.7,0.1,0.7, 0.8,0.0,0.6, 0.9,0.1,0.9, 0.8,0.4,0.8,
                    0.8,0.0,0.6, 0.5,0.2,0.5, 0.6,0.1,0.5, 0.9,0.1,0.9,
                    0.5,0.2,0.5, 0.6,0.1,0.4, 0.7,0.0,0.6, 0.6,0.1,0.5,
                    0.6,0.1,0.4, 0.7,0.1,0.7, 0.8,0.4,0.8, 0.7,0.0,0.6,
                    0.6,0.1,0.4, 0.5,0.2,0.5, 0.8,0.0,0.6, 0.7,0.1,0.7,
                    0.8,0.4,0.8, 0.9,0.1,0.9, 0.6,0.1,0.5, 0.7,0.0,0.6,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color4[]={0.4,0.3,0.3, 0.5,0.4,0.4, 0.4,0.4,0.5, 0.4,0.5,0.3,
                    0.5,0.4,0.4, 0.2,0.3,0.3, 0.1,0.2,0.3, 0.4,0.4,0.5,
                    0.2,0.3,0.3, 0.1,0.2,0.3, 0.3,0.3,0.3, 0.1,0.2,0.3,
                    0.1,0.2,0.3, 0.4,0.3,0.3, 0.4,0.5,0.3, 0.3,0.3,0.3,
                    0.1,0.2,0.3, 0.2,0.3,0.3 ,0.5,0.4,0.4, 0.4,0.3,0.3,
                    0.4,0.5,0.3, 0.4,0.4,0.5, 0.1,0.2,0.3, 0.3,0.3,0.3,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color5[]={0.8,0.4,0.3, 0.8,0.3,0.1, 0.9,0.5,0.1, 0.9,0.4,0.0,
                    0.8,0.3,0.1, 0.6,0.3,0.0, 0.7,0.3,0.0, 0.9,0.5,0.1,
                    0.6,0.3,0.0, 0.5,0.2,0.1, 0.5,0.3,0.0, 0.7,0.3,0.0,
                    0.5,0.2,0.1, 0.8,0.4,0.3, 0.9,0.4,0.0, 0.5,0.3,0.0,
                    0.5,0.2,0.1, 0.6,0.3,0.0, 0.8,0.3,0.1, 0.8,0.4,0.3,
                    0.9,0.4,0.0, 0.9,0.5,0.1, 0.7,0.3,0.0, 0.5,0.3,0.0,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color6[]={0.2,0.8,0.8, 0.3,0.7,0.6, 0.2,0.6,0.7, 0.3,0.6,0.8,
                    0.3,0.7,0.6, 0.0,0.5,0.5, 0.1,0.3,0.4, 0.2,0.6,0.7,
                    0.0,0.5,0.5, 0.1,0.4,0.5, 0.0,0.6,0.5, 0.1,0.3,0.4,
                    0.1,0.4,0.5, 0.2,0.8,0.8, 0.3,0.6,0.8, 0.0,0.6,0.5,
                    0.1,0.4,0.5, 0.0,0.5,0.5, 0.3,0.7,0.6, 0.2,0.8,0.8,
                    0.3,0.6,0.8, 0.2,0.6,0.7, 0.1,0.3,0.4, 0.0,0.6,0.5,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };
    float color7[]={0.9,0.8,0.8, 0.8,0.9,0.9, 0.8,0.8,0.8, 0.7,0.8,0.9,
                    0.8,0.9,0.9, 0.5,0.5,0.5, 0.5,0.3,0.4, 0.8,0.8,0.8,
                    0.5,0.5,0.5, 0.4,0.4,0.5, 0.6,0.6,0.5, 0.5,0.3,0.4,
                    0.4,0.4,0.5, 0.9,0.8,0.8, 0.7,0.8,0.9, 0.6,0.6,0.5,
                    0.4,0.4,0.5, 0.5,0.5,0.5, 0.8,0.9,0.9, 0.9,0.8,0.8,
                    0.7,0.8,0.9, 0.8,0.8,0.8, 0.5,0.3,0.4, 0.6,0.6,0.5,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1,
                    1,1,1,  1,1,1,  1,1,1,  1,1,1
                    };

    float vex_priceDisplay[]={
                        0,0,0,  0,0,1,  0,0.2,1,  0,0.2,0
                             };
    float col_priceDisplay[]={
                        0.1,0.1,0.1,  0.1,0.1,0.1,  0.1,0.1,0.1,  0.1,0.1,0.1
                             };
    float vex_priceDisplayBox[]={
                        0,0,0,  0,0,0.16,  0,0.16,0.16,  0,0.16,0
                             };
    float col_priceDisplayBoxON[]={
                        0.4,0.9,0.4,  0.4,0.9,0.4,  0.4,0.9,0.4,  0.4,0.9,0.4
                             };
    float col_priceDisplayBoxOFF[]={
                        0.2,0.2,0.2,  0.2,0.2,0.2,  0.2,0.2,0.2,  0.2,0.2,0.2
                             };
    float steps[5]={0.1,0.3,0.5,0.7,1};

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    if (m_open) glVertexPointer(3, GL_FLOAT, 0, openBox);
    else        glVertexPointer(3, GL_FLOAT, 0, closedBox);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
    if (m_team==0)
    {
        switch (m_owner)
        {
            case 0:glColorPointer(3, GL_FLOAT, 0, color0);break;
            case 1:glColorPointer(3, GL_FLOAT, 0, color1);break;
            case 2:glColorPointer(3, GL_FLOAT, 0, color2);break;
            case 3:glColorPointer(3, GL_FLOAT, 0, color3);break;
            case 4:glColorPointer(3, GL_FLOAT, 0, color4);break;
            case 5:glColorPointer(3, GL_FLOAT, 0, color5);break;
            case 6:glColorPointer(3, GL_FLOAT, 0, color6);break;
            case 7:glColorPointer(3, GL_FLOAT, 0, color7);break;
        }
    }
    else
    {
        if (m_team==1) glColorPointer(3, GL_FLOAT, 0, color0);
        if (m_team==2) glColorPointer(3, GL_FLOAT, 0, color1);
    }
    glTranslatef(m_pos[0]+0.02, m_pos[1], m_pos[2]+0.02);
    if (m_moving) {glRotatef(m_yhed,0,1,0); glRotatef(-20,0,0,1);glTranslatef(-1,-1.3,-0.5);}
    glScalef(0.97,0.97,0.97);
    if (m_output==0) glDrawArrays(GL_QUADS, 0, 24);
    else glDrawArrays(GL_QUADS, 0, 44);
    glPushMatrix();
        //Draw price-meters
        glEnableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY); //XXXXXXXX
        glDisable(GL_TEXTURE_2D); //XXXXXXXXXXXXXXXXXXXXXXXXXXXX
        glTranslatef(0.0,0.8,0.0);
        glScalef(0.8,0.8,0.8);
        for (int i=0;i<3;i++)
        {
            glRotatef(90,0,1,0);
            glTranslatef(-1.27,0,0.12);
            glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplay);
            glColorPointer(3, GL_FLOAT, 0, col_priceDisplay);
//                   glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
            glDrawArrays(GL_QUADS,0,4); //Draw background
            glPushMatrix();
                glTranslatef(-0.01,0.04,0.06);
                glScalef(0.8,0.8,0.8);
                for (int j=0;j<5;j++) //Draw squares
                {
                    float relPowCount=1;
                    if (m_price[i]!=0) relPowCount=(float)m_content/(float)m_price[i];
                    glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                    if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                    else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                    glDrawArrays(GL_QUADS,0,4);
                    glTranslatef(0,0,0.23);
                }
            glPopMatrix();
            glTranslatef(0.02,0.0,-0.12);
        }
        glDisableClientState(GL_COLOR_ARRAY); //XXXXXXXXXXXXXXXXXXX
    glPopMatrix();
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool spawner::spawnerCollision(float x,float y,float z)
{
    float xhitbox=1;
    float yhitbox=2;
    float zhitbox=1;
    if (x>m_pos[0]-xhitbox+0.5 &&  x<m_pos[0]+xhitbox+0.5 &&
        y>m_pos[1]-yhitbox+2.0 &&  y<m_pos[1]+yhitbox+2.0 &&
        z>m_pos[2]-zhitbox+0.5 &&  z<m_pos[2]+zhitbox+0.5) return true;
    return false;
}

bool spawner::spawnerCollision(float x,float y,float z, int playerID)
{
    if (playerID==m_carrier) return false;
    float xhitbox=1;
    float yhitbox=2;
    float zhitbox=1;
    if (x>m_pos[0]-xhitbox+0.5 &&  x<m_pos[0]+xhitbox+0.5 &&
        y>m_pos[1]-yhitbox+2.0 &&  y<m_pos[1]+yhitbox+2.0 &&
        z>m_pos[2]-zhitbox+0.5 &&  z<m_pos[2]+zhitbox+0.5) return true;
    return false;
}

bool spawner::spawnerBulletHitTest(float xpos, float ypos, float zpos)
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

void spawner::place(float xpos,float height,float zpos)
{
    m_moving=false;
    m_pos[0]=(int)xpos;
    m_pos[1]=height;
    m_pos[2]=(int)zpos;
}
