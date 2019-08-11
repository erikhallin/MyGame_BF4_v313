#include "radar.h"
#include "md2model.h"
#include <gl\gl.h>

//#include <iostream> //XXXXXXXXXXXXXXXXXXXXXXXXXXdebug

radar::radar()
{
    m_active=false;
    m_outpostPrice=2;
    int offset=0;
    for (float i=0;i<20;i++) //Vertex array for display
    {
        for (float j=0;j<20;j++)
        {
            m_vex_pixel[offset+0]=j;/*x*/m_vex_pixel[offset+1]=i;/*y*/m_vex_pixel[offset+2]=0;/*z*/
            m_vex_pixel[offset+3]=j+0.9;/*x*/m_vex_pixel[offset+4]=i;/*y*/m_vex_pixel[offset+5]=0;/*z*/
            m_vex_pixel[offset+6]=j+0.9;/*x*/m_vex_pixel[offset+7]=i+0.9;/*y*/m_vex_pixel[offset+8]=0;/*z*/
            m_vex_pixel[offset+9]=j;/*x*/m_vex_pixel[offset+10]=i+0.9;/*y*/m_vex_pixel[offset+11]=0;/*z*/
            offset+=12;
        }
    }

    //XXXXXXXXXXXXXXX

    m_extra[0]=0;
    m_extra[1]=0;
    m_extra[2]=0;

    //XXXXXXXXXXXXX
}

void radar::newRadar(float x,float y,float z,int owner)
{
    m_active=true;
    m_HP=500;
    m_zoomLevel=2;
    m_currOutpost=0;
    m_outpostCounter=0;
    m_output=false;
    m_outpostPrice=2;
    m_doorProg=0;
    m_add1=true;
    m_pos[0]=m_focus[0]=x;
    m_pos[1]=m_focus[1]=y;
    m_pos[2]=m_focus[2]=z;
    if (owner>9) //Teams
    {
        m_team=int(owner/10);
        m_owner=owner-m_team*10;
    }
    else //FFA
    {
        m_owner=owner;
        m_team=0;
    }
    for (int i=0;i<10;i++) m_buttonPress[i]=0; //Depress buttons
    for (int i=0;i<99;i++) m_outpostID[i]=-1; //Clear outpostID array
}

void radar::newOutpost(int outpostID)
{
    for (int i=0;i<99;i++)
    {
        if (m_outpostID[i]==-1)
        {
            m_outpostID[i]=outpostID;
            m_outpostCounter++;
            break;
        }
    }

}

void radar::removeOutpost(int outpostID)
{
    int slot=-1;
    for (int i=0;i<99;i++)
    {
        if (m_outpostID[i]==outpostID)
        {
            m_outpostID[i]=-1;
            m_outpostCounter--;
            break;
        }
    }
}

int radar::update(float cycleTime)
{
    //Buttons repress
    for (int i=0;i<10;i++)
    {
        if (m_buttonPress[i]<0) m_buttonPress[i]+=cycleTime*0.001;
        if (m_buttonPress[i]>0) m_buttonPress[i]=0;
    }
    //Door progress
    if (m_doorProg>0)
    {
        if (m_add1) m_doorProg+=cycleTime*0.003;
        else m_doorProg-=cycleTime*0.005;
        if (m_doorProg>1) m_add1=false;
        if (m_doorProg<0) {m_doorProg=0; m_add1=true;}
    }

    if (m_HP<1) //Death
    {
        m_active=false;
        return 1;
    }
    return 0;
}

void radar::drawRadar(float pixelColor[],int powCount)
{
    float vex_button[]={
        //Top
        -1,-1,1,  1,-1,1,  1,1,1,  -1,1,1,
        -1,-1,-1,  -1,-1,1,  -1,1,1,  -1,1,-1,
        -1,1,-1,  -1,1,1,  1,1,1,  1,1,-1,
        1,1,-1,  1,1,1,  1,-1,1,  1,-1,-1,
        1,-1,-1,  1,-1,1,  -1,-1,1,  -1,-1,-1
        };
    float col_button[]={
        0.9,0.5,0.5,  0.5,0.9,0.5,  0.5,0.5,0.9,  0.9,0.9,0.5,
        0.9,0.5,0.5,  0.5,0.9,0.5,  0.5,0.5,0.9,  0.9,0.9,0.5,
        0.9,0.5,0.5,  0.5,0.9,0.5,  0.5,0.5,0.9,  0.9,0.9,0.5,
        0.9,0.5,0.5,  0.5,0.9,0.5,  0.5,0.5,0.9,  0.9,0.9,0.5
        };
    float tex_dirUP[]={
        0.648,0.001, 0.709,0.001, 0.709,0.063, 0.648,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_dirDOWN[]={
        0.709,0.063, 0.648,0.063, 0.648,0.001, 0.709,0.001,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_dirRIGHT[]={
        0.709,0.001, 0.709,0.063, 0.648,0.063, 0.648,0.001,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_dirLEFT[]={
        0.648,0.063, 0.648,0.001, 0.709,0.001, 0.709,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_reset[]={
        0.774,0.001, 0.835,0.001, 0.835,0.063, 0.774,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_add[]={
        0.523,0.001, 0.582,0.001, 0.582,0.063, 0.523,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_substract[]={
        0.586,0.001, 0.647,0.001, 0.647,0.063, 0.586,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063,
        0.712,0.001, 0.773,0.001, 0.773,0.063, 0.712,0.063
        };
    float tex_output[]={
        0.762,0.885, 0.877,0.885, 0.877,0.999, 0.762,0.999,
        0.881,0.885, 0.996,0.885, 0.996,0.999, 0.881,0.999,
        0.881,0.885, 0.996,0.885, 0.996,0.999, 0.881,0.999,
        0.881,0.885, 0.996,0.885, 0.996,0.999, 0.881,0.999,
        0.881,0.885, 0.996,0.885, 0.996,0.999, 0.881,0.999
        };

    float vex_door[]={
        0,0,0,  0.7,0,0,  0.7,1.7,0,   0,1.7,0
        };
    float tex_door[]={
        0.844,0.480, 0.996,0.480, 0.996,0.854, 0.844,0.854
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

    float vex_display[]={0,0,0,  1,0,0,  1,1,0,  0,1,0};

    glPushMatrix();
        glTranslatef(m_pos[0]+2, m_pos[1], m_pos[2]+2);
        //Draw Building
        glPushMatrix();
            glRotatef(-90,1,0,0);
            glScalef(0.10,0.10,0.10);
            m_modelID->draw();
            m_modelID->advance(0.004f);
        glPopMatrix();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        //Draw Display
        glPushMatrix();
            glTranslatef(-0.7,1.9,1.84);
            glRotatef(-90,0,0,1);
            glScalef(0.07,0.07,0.07);
            glVertexPointer(3, GL_FLOAT, 0, m_vex_pixel);
            glColorPointer(3, GL_FLOAT, 0, pixelColor);
            glDrawArrays(GL_QUADS,0,1600);
        glPopMatrix();

        glDisableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        //Buttons
        glVertexPointer(3, GL_FLOAT, 0, vex_button);
        //Draw D-Pad
        glPushMatrix();
            glTranslatef(1.23,1.7,1.81);
            glScalef(0.12,0.12,0.12);
            glTranslatef(0,0,m_buttonPress[1]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_dirUP);
            glDrawArrays(GL_QUADS,0,20); //Up
            glTranslatef(2.2,-2.2,m_buttonPress[2]-m_buttonPress[1]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_dirRIGHT);
            glDrawArrays(GL_QUADS,0,20); //Right
            glTranslatef(-4.4,0,m_buttonPress[0]-m_buttonPress[2]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_dirLEFT);
            glDrawArrays(GL_QUADS,0,20); //Left
            glTranslatef(2.2,-2.2,m_buttonPress[3]-m_buttonPress[0]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_dirDOWN);
            glDrawArrays(GL_QUADS,0,20); //Down
            //Reset
            glTranslatef(0,-4,m_buttonPress[6]-m_buttonPress[3]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_reset);
            glDrawArrays(GL_QUADS,0,20); //Down
        glPopMatrix();

        //Draw Zoom button
        glPushMatrix();
            glTranslatef(-1.13,1.62,1.80);
            glScalef(0.15,0.15,0.15);
            glTranslatef(0,0,m_buttonPress[4]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_add);
            glDrawArrays(GL_QUADS,0,20);
            glTranslatef(0,-4,m_buttonPress[5]-m_buttonPress[4]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_substract);
            glDrawArrays(GL_QUADS,0,20);
        glPopMatrix();

        //Draw Outpost output
        if (m_output)
        {
            glPushMatrix();
                glScalef(0.3,0.3,0.3);
                glTranslatef(-5.82,4.55,-2.23);
                glRotatef(-90,0,1,0);
                glTexCoordPointer(2, GL_FLOAT, 0, tex_output);
                glDrawArrays(GL_QUADS,0,20);
            glPopMatrix();
        }

        glEnableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        //Draw price-meter
        glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplay);
        glColorPointer(3, GL_FLOAT, 0, col_priceDisplay);
        glPushMatrix();
            glTranslatef(-1.83,0.64,0.22);
            glDrawArrays(GL_QUADS,0,4); //Draw background
            glTranslatef(-0.01,0.04,0.06);
            glScalef(0.8,0.8,0.8);
            for (int j=0;j<5;j++) //Draw squares
            {
                float relPowCount=1;
                if (m_outpostPrice!=0) relPowCount=(float)powCount/(float)m_outpostPrice;
                glVertexPointer(3, GL_FLOAT, 0, vex_priceDisplayBox);
                if (relPowCount>=steps[j]) glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxON);
                else glColorPointer(3, GL_FLOAT, 0, col_priceDisplayBoxOFF);
                glDrawArrays(GL_QUADS,0,4);
                glTranslatef(0,0,0.23);
            }
        glPopMatrix();

        glDisableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        //Draw select outpost
        glPushMatrix();
            //Draw display
            glVertexPointer(3, GL_FLOAT, 0, vex_display);
            if (m_currOutpost>-1 && m_currOutpost<6) //First lane
            {
                float tex_number[]={0.701+float(m_currOutpost)*0.050781,0.115,
                                    0.652+float(m_currOutpost)*0.050781,0.115,
                                    0.652+float(m_currOutpost)*0.050781,0.164,
                                    0.701+float(m_currOutpost)*0.050781,0.164};
                glTexCoordPointer(2, GL_FLOAT, 0, tex_number);
            }
            else if (m_currOutpost>5 && m_currOutpost<12) //Second lane
            {
                float tex_number[]={0.701+float(m_currOutpost-6)*0.050781,0.064,
                                    0.652+float(m_currOutpost-6)*0.050781,0.064,
                                    0.652+float(m_currOutpost-6)*0.050781,0.113,
                                    0.701+float(m_currOutpost-6)*0.050781,0.113};
                glTexCoordPointer(2, GL_FLOAT, 0, tex_number);
            }
  /*          else if (m_currOutpost<0) //- ta bort?
            {
                float tex_number[]={0.586,0.001, 0.647,0.001, 0.647,0.063, 0.586,0.063};
                glTexCoordPointer(2, GL_FLOAT, 0, tex_number);
            }*/
            else if (m_currOutpost>11) //+
            {
                float tex_number[]={0.523,0.001, 0.582,0.001, 0.582,0.063, 0.523,0.063};
                glTexCoordPointer(2, GL_FLOAT, 0, tex_number);
            }
            glTranslatef(-0.99,1.40,-1.8);
            glScalef(0.35,0.35,0.35);
            glDrawArrays(GL_QUADS,0,4);
            //Get current texture

            //Draw +/- buttons
            glVertexPointer(3, GL_FLOAT, 0, vex_button);
            glColorPointer(3, GL_FLOAT, 0, col_button);
            glRotatef(180,0,1,0);
            glScalef(0.4,0.4,0.4);
            glTranslatef(2,1.3,m_buttonPress[8]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_add);
            glDrawArrays(GL_QUADS,0,20);
            glTranslatef(-6.5,0,m_buttonPress[9]-m_buttonPress[8]);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_substract);
            glDrawArrays(GL_QUADS,0,20);
        glPopMatrix();

        //Draw Doors
        glPushMatrix();
            glVertexPointer(3, GL_FLOAT, 0, vex_door);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_door);
            glTranslatef(0.05,0.21,-1.85);
            glRotatef(m_doorProg*100,0,1,0);
            glDrawArrays(GL_QUADS,0,4);
            glRotatef(-m_doorProg*100,0,1,0);
            glTranslatef(1.4,0,0);
            glRotatef(180-m_doorProg*100,0,1,0);
            glDrawArrays(GL_QUADS,0,4);
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

bool radar::radarBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=2;
    float yhitbox=2;
    float zhitbox=2;
    float xoffset=2;
    float yoffset=2;
    float zoffset=2;
    if (xpos>m_pos[0]+xoffset-xhitbox && xpos<m_pos[0]+xoffset+xhitbox &&
        ypos>m_pos[1]+yoffset-yhitbox && ypos<m_pos[1]+yoffset+yhitbox &&
        zpos>m_pos[2]+zoffset-zhitbox && zpos<m_pos[2]+zoffset+zhitbox)
    return true; else return false;
}

bool radar::radarInteractionHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=2.0;
    float yhitbox=1;
    float zhitbox=2.0;
    float xoffset=m_pos[0]+2;
    float yoffset=m_pos[1]+1;
    float zoffset=m_pos[2]+2;

    if (xpos>xoffset-xhitbox && xpos<xoffset+xhitbox &&
        ypos>yoffset-yhitbox && ypos<yoffset+yhitbox &&
        zpos>zoffset-zhitbox && zpos<zoffset+zhitbox)
    return true; else return false;
}

int radar::buttonHitTest(float x, float y, float z)
{
  //  std::cout << "Interaction at: " << x << " " << y << " " << z << "\n"; //XXXXXXXXXXXXXXXXXXXXXXXXXX
  //  std::cout << "m_extra at: " << m_extra[0] << " " << m_extra[1] << " " << m_extra[2] << "\n"; //XXXXX
    //D-Pad
    float xOffset=0.15;
    float yOffset=0.15;
    float zOffset=0.5;
    //Left
    if (x<2.86+xOffset && x>2.86-xOffset &&
        y<1.47+yOffset && y>1.47-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_focus[0]-=m_zoomLevel;
        m_buttonPress[0]=-0.5;
    }
    //Up
    if (x<3.17+xOffset && x>3.17-xOffset &&
        y<1.78+yOffset && y>1.78-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_focus[2]-=m_zoomLevel;
        m_buttonPress[1]=-0.5;
    }
    //Right
    if (x<3.52+xOffset && x>3.52-xOffset &&
        y<1.41+yOffset && y>1.41-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_focus[0]+=m_zoomLevel;
        m_buttonPress[2]=-0.5;
    }
    //Down
    if (x<3.17+xOffset && x>3.17-xOffset &&
        y<1.39+yOffset && y>1.39-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_focus[2]+=m_zoomLevel;
        m_buttonPress[3]=-0.5;
    }
    //Reset
    if (x<3.17+xOffset && x>3.17-xOffset &&
        y<0.7+yOffset && y>0.7-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_focus[0]=m_pos[0]; m_focus[1]=m_pos[1]; m_focus[2]=m_pos[2];
        m_zoomLevel=2;
        m_buttonPress[6]=-0.5;
    }

    xOffset=0.2;
    yOffset=0.2;
    //ZoomIn
    if (x<0.84+xOffset && x>0.84-xOffset &&
        y<1.62+yOffset && y>1.62-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        if (m_zoomLevel>1) m_zoomLevel-=1;
        m_buttonPress[4]=-0.5;
    }
    //ZoomOut
    if (x<0.84+xOffset && x>0.84-xOffset &&
        y<0.99+yOffset && y>0.99-yOffset &&
        z<4.0+zOffset && z>4.0-zOffset)
    {
        m_zoomLevel+=1;
        m_buttonPress[5]=-0.5;
    }

    //Get Outpost
    xOffset=0.3;
    yOffset=0.3;
    zOffset=0.3;
    if (x<0.15+xOffset && x>0.15-xOffset &&
        y<1.35+yOffset && y>1.35-yOffset &&
        z<1.30+zOffset && z>1.30-zOffset)
    {
        return 2;
    }

    //Buy outpost
    if (x<0.10+xOffset && x>0.10-xOffset &&
        y<1.38+yOffset && y>1.38-yOffset &&
        z<2.71+zOffset && z>2.71-zOffset)
    {
        return 1;
    }

    //Next outpost
    if (x<0.67+xOffset && x>0.67-xOffset &&
        y<1.58+yOffset && y>1.58-yOffset &&
        z<0.16+zOffset && z>0.16-zOffset)
    {
        m_buttonPress[8]=-0.5;
        for (int i=m_currOutpost+1;i<99;i++)
        {
            if (m_outpostID[i]!=-1)
            {
                m_currOutpost=i;
                break;
            }
        }
    }
    //Prev outpost
    if (x<1.62+xOffset && x>1.62-xOffset &&
        y<1.58+yOffset && y>1.58-yOffset &&
        z<0.10+zOffset && z>0.10-zOffset)
    {
        m_buttonPress[9]=-0.5;
        for (int i=m_currOutpost-1;i>-1;i--)
        {
            if (m_outpostID[i]!=-1)
            {
                m_currOutpost=i;
                break;
            }
        }
    }

    //Teleport
    xOffset=0.6;
    yOffset=0.9;
    if (x<2.82+xOffset && x>2.82-xOffset &&
        y<1.20+yOffset && y>1.20-yOffset &&
        z<0.10+zOffset && z>0.10-zOffset)
    {
        m_doorProg=0.01;
        return 3;
    }

    return 0;
}
