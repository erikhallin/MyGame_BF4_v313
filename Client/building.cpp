#include <gl\gl.h>
#include "building.h"

building::building()
{
    m_active=false;
    m_powCount=0;
    for (int i=0;i<10;i++) m_inventory[i]=false;
    m_price[0]=0;
    m_price[1]=1;
    m_price[2]=2;
    m_price[3]=3;
}

void building::newBuilding(int xpos,float ypos,int zpos,int type,int owner)
{
    m_active=true;
    for (int i=0;i<10;i++) m_inventory[i]=false;
    m_owner=owner;
    m_type=type;
    m_busy=false;
    m_buildProgress=1;
    switch (type)
    {
        case 1:{//Wass
                m_pos[0]=xpos-1;
                m_pos[2]=zpos;
                m_price[0]=0; //Rev
                m_price[1]=2; //Uzi
                m_price[2]=5; //Sniper
                m_price[3]=5; //RL
               }break;
        case 2:{//Tass
                m_pos[0]=xpos;
                m_pos[2]=zpos;
                m_price[0]=1; //Speed
                m_price[1]=10; //Tank
                m_price[2]=5; //Heli
                m_price[3]=0; //Reset
               }break;
        case 3:{//Dass
                m_pos[0]=xpos-1;
                m_pos[2]=zpos;
                m_price[0]=1; //Wall
                m_price[1]=1; //Mine
                m_price[2]=8; //Radar
                m_price[3]=2; //Turr.exp
               }break;
        case 6:{//Turr.exp
                m_pos[0]=xpos-1;
                m_pos[2]=zpos;
                m_price[0]=2; //Gattling
                m_price[1]=2; //Rocket
                m_price[2]=2; //Laser
                m_price[3]=3;
               }break;

    }
    m_pos[1]=ypos;
    m_HP=500;
}

int building::update(int powCount,float cycleTime)
{
    m_powCount=powCount;
    if (m_busy && m_buildProgress>0) m_buildProgress-=cycleTime*0.0005;
    if (!m_busy) m_buildProgress=1;
    if (m_HP<1) //Death
    {
        m_active=false;
        return 1;
    }
    return 0;
}

void building::drawBuilding(void)
{
    float vex_base[]={
                        //Main Block
                        1,0,0,  1,2,0,  2,2,0,  2,0,0, //Right
                        1,0,4,  2,0,4,  2,2,4,  1,2,4, //Left
                        1,1,4,  1,2,4,  1,2,0,  1,1,0, //Front
                        2,0,4,  2,0,0,  2,2,0,  2,2,4, //Back
                        1,2,4,  2,2,4,  2,2,0,  1,2,0, //Top
                        //Table
                        1,0,0,  0,0,0,  0,1,0,  1,1,0, //Right
                        0,0,4,  1,0,4,  1,1,4,  0,1,4, //Left
                        0,0,0,  0,0,4,  0,1,4,  0,1,0, //Front
                        0,1,4,  1,1,4,  1,1,0,  0,1,0  //Top
                     };
    float tex_base[]={
                        //Main Block
                        0.486,0.807,  0.486,0.999,  0.391,0.999,  0.391,0.807, //Right
                        0.391,0.807,  0.486,0.807,  0.486,0.999,  0.391,0.999, //Left
                        0.389,0.709,  0.389,0.805,  0.001,0.805,  0.001,0.709, //Front
                        0.001,0.807,  0.389,0.807,  0.389,0.999,  0.001,0.999, //Back
                        0.389,0.611,  0.389,0.707,  0.001,0.707,  0.001,0.611, //Top
                        //Table
                        0.488,0.904,  0.584,0.904,  0.584,0.999,  0.488,0.999, //Right
                        0.488,0.904,  0.584,0.904,  0.584,0.999,  0.488,0.999, //Left
                        0.001,0.514,  0.389,0.514,  0.389,0.609,  0.001,0.609, //Front
                        0.389,0.416,  0.389,0.511,  0.001,0.511,  0.001,0.416  //Top
                     };
    float vex_box[]={
                        //Lid
                        0,0,0,  0,0.2,0,  1,0.2,0,  1,0,0, //Front
                        0,0,1,  0,0.2,1,  1,0.2,1,  1,0,1,   //Back
                        1,0,1,  1,0,0,  1,0.2,0,  1,0.2,1, //Right
                        0,0,1,  0,0.2,1,  0,0.2,0,  0,0,0, //Left
                        0,0.2,1,  1,0.2,1,  1,0.2,0,  0,0.2,0, //Top
                        //Pillars
                        0.2,-0.5,0.2,  0.2,-0.5,0.3,  0.2,0.15,0.3,  0.2,0.15,0.2,
                        0.2,-0.5,0.7,  0.2,-0.5,0.8,  0.2,0.15,0.8,  0.2,0.15,0.7,
                        0.8,-0.5,0.2,  0.8,-0.5,0.3,  0.8,0.15,0.3,  0.8,0.15,0.2,
                        0.8,-0.5,0.7,  0.8,-0.5,0.8,  0.8,0.15,0.8,  0.8,0.15,0.7
                    };
    float tex_box[]={
                        //Lid
                        0.561,0.807,  0.561,0.828,  0.488,0.828,  0.488,0.807, //Front
                        0.488,0.807,  0.488,0.828,  0.561,0.828,  0.561,0.807, //Back
                        0.488,0.807,  0.561,0.807,  0.561,0.828,  0.488,0.828, //Right
                        0.561,0.807,  0.561,0.828,  0.488,0.828,  0.488,0.807, //Left
                        0.561,0.830,  0.561,0.902,  0.488,0.902,  0.488,0.830, //Top
                        //Pillars
                        0.563,0.807,  0.584,0.807,  0.584,0.902,  0.563,0.902,
                        0.563,0.807,  0.584,0.807,  0.584,0.902,  0.563,0.902,
                        0.563,0.807,  0.584,0.807,  0.584,0.902,  0.563,0.902,
                        0.563,0.807,  0.584,0.807,  0.584,0.902,  0.563,0.902
                    };
    float vex_panel[]={
                        0,0,0,  0,0,1,  0,1,1, 0,1,0
                      };
    float tex_panel[]={
                        0.001,0.320,  0.094,0.320,  0.094,0.414,  0.001,0.414
                      };
    float vex_garage[]={
                        2,0,0,  2,2,0,  4,2,0,  4,0,0, //Right
                        2,0,4,  4,0,4,  4,2,4,  2,2,4, //Left
                        4,0,4,  4,2,4,  4,2,0,  4,0,0, //Front
                        2,2,4,  4,2,4,  4,2,0,  2,2,0 //Top
                       };
    float tex_garage[]={
                        0.975,0.611,  0.975,0.805,  0.781,0.805,  0.781,0.611, //Right
                        0.781,0.611,  0.975,0.611,  0.975,0.805,  0.781,0.805, //Left
                        0.391,0.611,  0.391,0.805,  0.779,0.805,  0.779,0.611, //Front
                        0.975,0.807,  0.975,0.999,  0.586,0.999,  0.586,0.807  //Top
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
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    switch (m_type)
    {
        case 1:{//Wass
                //Base
                glVertexPointer(3, GL_FLOAT, 0, vex_base);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_base);
                glTranslatef(m_pos[0]+2,m_pos[1],m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 36);
                //Panels & Boxes
                glPushMatrix();
                    glVertexPointer(3, GL_FLOAT, 0, vex_panel);
                    //glTexCoordPointer(2, GL_FLOAT, 0, tex_panel);
                    glTranslatef(0.98,1.19,-0.7);
                    glScalef(0.7,0.7,0.7);
                    for (int i=0;i<4;i++)
                    {
                        glTranslatef(0,0,1.35);
                        float panelTex[]={0.001+((float)i*0.096),0.320,  0.093+((float)i*0.096),0.320,
                                          0.093+((float)i*0.096),0.414,  0.001+((float)i*0.096),0.414};
                        glTexCoordPointer(2, GL_FLOAT, 0, panelTex);
                        glDrawArrays(GL_QUADS,0,4);
                    }
                glPopMatrix();
                glPushMatrix();
                    glVertexPointer(3, GL_FLOAT, 0, vex_box);
                    glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
                    glTranslatef(0.2,1,-0.7);
                    glScalef(0.4,0.6,0.6);
                    for (int i=0;i<4;i++)
                    {
                        glTranslatef(0,0,1.6);
                        if (m_inventory[i])
                        {
                            glPushMatrix();
                            glTranslatef(0,0.5,0);
                            glDrawArrays(GL_QUADS,0,36);
                            glPopMatrix();
                        }
                        else glDrawArrays(GL_QUADS,0,36);
                    }
                glPopMatrix();
                glPushMatrix();
                    //Draw price-meters
                    glEnableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY); //XXXXXXXX
                    glDisable(GL_TEXTURE_2D); //XXXXXXXXXXXXXXXXXXXXXXXXXXXX
                    glTranslatef(-0.01,0.7,0.1);
                    glScalef(0.8,0.8,0.8);
                    for (int i=0;i<4;i++)
                    {
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
                                if (m_price[i]!=0) relPowCount=(float)m_powCount/(float)m_price[i];
                                glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                                if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                                else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                                glDrawArrays(GL_QUADS,0,4);
                                glTranslatef(0,0,0.23);
                            }
                        glPopMatrix();
                        glTranslatef(0,0,1.25);
                    }
                    glDisableClientState(GL_COLOR_ARRAY); //XXXXXXXXXXXXXXXXXXX
                glPopMatrix();
               }break;
        case 2:{//Tass
                //Base
                glVertexPointer(3, GL_FLOAT, 0, vex_base);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_base);
                glTranslatef(m_pos[0],m_pos[1],m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 36);
                glVertexPointer(3, GL_FLOAT, 0, vex_garage);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_garage);
                if (m_busy)
                {
                    glPushMatrix();
                    glScalef(1,m_buildProgress+0.1,1);
                    glDrawArrays(GL_QUADS,0,16);
                    glPopMatrix();
                }
                else glDrawArrays(GL_QUADS,0,16);
                //Panels & Boxes
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_panel);
                //glTexCoordPointer(2, GL_FLOAT, 0, tex_panel);
                glTranslatef(0.98,1.2,-0.7);
                glScalef(0.7,0.7,0.7);
                for (int i=4;i<8;i++)
                {
                    glTranslatef(0,0,1.35);
                    float panelTex[]={0.001+((float)i*0.096),0.320,  0.093+((float)i*0.096),0.320,
                                      0.093+((float)i*0.096),0.414,  0.001+((float)i*0.096),0.414};
                    glTexCoordPointer(2, GL_FLOAT, 0, panelTex);
                    glDrawArrays(GL_QUADS,0,4);
                }
                glPopMatrix();
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_box);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
                glTranslatef(0.2,1,-0.7);
                glScalef(0.4,0.6,0.6);
                for (int i=0;i<4;i++)
                {
                    glTranslatef(0,0,1.6);
                    if (m_inventory[i])
                    {
                        glPushMatrix();
                        glTranslatef(0,0.5,0);
                        glDrawArrays(GL_QUADS,0,36);
                        glPopMatrix();
                    }
                    else glDrawArrays(GL_QUADS,0,36);
                }
                glPopMatrix();
                glPushMatrix();
                    //Draw price-meters
                    glEnableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY); //XXXXXXXX
                    glDisable(GL_TEXTURE_2D); //XXXXXXXXXXXXXXXXXXXXXXXXXXXX
                    glTranslatef(-0.01,0.7,0.1);
                    glScalef(0.8,0.8,0.8);
                    for (int i=0;i<3;i++)
                    {
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
                                if (m_price[i]!=0) relPowCount=(float)m_powCount/(float)m_price[i];
                                glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                                if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                                else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                                glDrawArrays(GL_QUADS,0,4);
                                glTranslatef(0,0,0.23);
                            }
                        glPopMatrix();
                        glTranslatef(0,0,1.25);
                    }
                    glDisableClientState(GL_COLOR_ARRAY); //XXXXXXXXXXXXXXXXXXX
                glPopMatrix();
               }break;
        case 3:{//Dass
                //Base
                glVertexPointer(3, GL_FLOAT, 0, vex_base);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_base);
                glTranslatef(m_pos[0]+2,m_pos[1],m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 36);
                //Panels & Boxes
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_panel);
                //glTexCoordPointer(2, GL_FLOAT, 0, tex_panel+offset);
                glTranslatef(0.98,1.2,-0.7);
                glScalef(0.7,0.7,0.7);
                for (int i=0;i<4;i++)
                {
                    glTranslatef(0,0,1.35);
                    float panelTex[]={0.001+((float)i*0.096),0.225,  0.093+((float)i*0.096),0.225,
                                      0.093+((float)i*0.096),0.318,  0.001+((float)i*0.096),0.318};
                    glTexCoordPointer(2, GL_FLOAT, 0, panelTex);
                    glDrawArrays(GL_QUADS,0,4);
                }
                glPopMatrix();
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_box);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
                glTranslatef(0.2,1,-0.7);
                glScalef(0.4,0.6,0.6);
                for (int i=0;i<4;i++)
                {
                    glTranslatef(0,0,1.6);
                    if (m_inventory[i])
                    {
                        glPushMatrix();
                        glTranslatef(0,0.5,0);
                        glDrawArrays(GL_QUADS,0,36);
                        glPopMatrix();
                    }
                    else glDrawArrays(GL_QUADS,0,36);
                }
                glPopMatrix();
                glPushMatrix();
                    //Draw price-meters
                    glEnableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY); //XXXXXXXX
                    glDisable(GL_TEXTURE_2D); //XXXXXXXXXXXXXXXXXXXXXXXXXXXX
                    glTranslatef(-0.01,0.7,0.1);
                    glScalef(0.8,0.8,0.8);
                    for (int i=0;i<4;i++)
                    {
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
                                if (m_price[i]!=0) relPowCount=(float)m_powCount/(float)m_price[i];
                                glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                                if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                                else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                                glDrawArrays(GL_QUADS,0,4);
                                glTranslatef(0,0,0.23);
                            }
                        glPopMatrix();
                        glTranslatef(0,0,1.25);
                    }
                    glDisableClientState(GL_COLOR_ARRAY); //XXXXXXXXXXXXXXXXXXX
                glPopMatrix();
               }break;
        case 6:{//Turr.exp
                //Base
                glVertexPointer(3, GL_FLOAT, 0, vex_base);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_base);
                glTranslatef(m_pos[0]+2,m_pos[1],m_pos[2]);
                glDrawArrays(GL_QUADS, 0, 36);
                //Panels & Boxes
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_panel);
                //glTexCoordPointer(2, GL_FLOAT, 0, tex_panel+offset);
                glTranslatef(0.98,1.2,-0.7);
                glScalef(0.7,0.7,0.7);
                for (int i=4;i<8;i++)
                {
                    glTranslatef(0,0,1.35);
                    float panelTex[]={0.001+((float)i*0.096),0.225,  0.093+((float)i*0.096),0.225,
                                      0.093+((float)i*0.096),0.318,  0.001+((float)i*0.096),0.318};
                    glTexCoordPointer(2, GL_FLOAT, 0, panelTex);
                    glDrawArrays(GL_QUADS,0,4);
                }
                glPopMatrix();
                glPushMatrix();
                glVertexPointer(3, GL_FLOAT, 0, vex_box);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_box);
                glTranslatef(0.2,1,-0.7);
                glScalef(0.4,0.6,0.6);
                for (int i=0;i<4;i++)
                {
                    glTranslatef(0,0,1.6);
                    if (m_inventory[i])
                    {
                        glPushMatrix();
                        glTranslatef(0,0.5,0);
                        glDrawArrays(GL_QUADS,0,36);
                        glPopMatrix();
                    }
                    else glDrawArrays(GL_QUADS,0,36);
                }
                glPopMatrix();
                glPushMatrix();
                    //Draw price-meters
                    glEnableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY); //XXXXXXXX
                    glDisable(GL_TEXTURE_2D); //XXXXXXXXXXXXXXXXXXXXXXXXXXXX
                    glTranslatef(-0.01,0.7,0.1);
                    glScalef(0.8,0.8,0.8);
                    for (int i=0;i<4;i++)
                    {
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
                                if (m_price[i]!=0) relPowCount=(float)m_powCount/(float)m_price[i];
                                glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                                if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                                else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                                glDrawArrays(GL_QUADS,0,4);
                                glTranslatef(0,0,0.23);
                            }
                        glPopMatrix();
                        glTranslatef(0,0,1.25);
                    }
                    glDisableClientState(GL_COLOR_ARRAY); //XXXXXXXXXXXXXXXXXXX
                glPopMatrix();
               }break;
    }
glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool building::buildingBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox;
    float yhitbox;
    float zhitbox;
    float xoffset;
    float yoffset;
    float zoffset;
    switch (m_type)
    {
        case 2:{//Tass
                 if (m_busy)
                 {
                     xoffset=m_pos[0]+1;
                     yoffset=m_pos[1]+1;
                     zoffset=m_pos[2]+2;
                     xhitbox=1;
                     yhitbox=1;
                     zhitbox=2;
                 }
                 else
                 {
                     xoffset=m_pos[0]+2;
                     yoffset=m_pos[1]+1;
                     zoffset=m_pos[2]+2;
                     xhitbox=2;
                     yhitbox=1;
                     zhitbox=2;
                 }
               }break;
        default:{
                 xoffset=m_pos[0]+3;
                 yoffset=m_pos[1]+1;
                 zoffset=m_pos[2]+2;
                 xhitbox=1;
                 yhitbox=1;
                 zhitbox=2;
                }break;
    }
    if (xpos>xoffset-xhitbox && xpos<xoffset+xhitbox &&
        ypos>yoffset-yhitbox && ypos<yoffset+yhitbox &&
        zpos>zoffset-zhitbox && zpos<zoffset+zhitbox)
    return true; else return false;
}

bool building::buildingInteractionHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox;
    float yhitbox;
    float zhitbox;
    float xoffset;
    float yoffset;
    float zoffset;
    switch (m_type)
    {
        case 2:{//Tass
                 xoffset=m_pos[0]+1.5;
                 yoffset=m_pos[1]+1;
                 zoffset=m_pos[2]+2;
                 xhitbox=1;
                 yhitbox=1;
                 zhitbox=2;
               }break;
        default:{
                 xoffset=m_pos[0]+3;
                 yoffset=m_pos[1]+1;
                 zoffset=m_pos[2]+2;
                 xhitbox=1;
                 yhitbox=1;
                 zhitbox=2;
                }break;
    }
    if (xpos>xoffset-xhitbox && xpos<xoffset+xhitbox &&
        ypos>yoffset-yhitbox && ypos<yoffset+yhitbox &&
        zpos>zoffset-zhitbox && zpos<zoffset+zhitbox)
    return true; else return false;
}
