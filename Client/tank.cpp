#include "tank.h"
#include <gl\gl.h>

tank::tank()
{
    m_active=false;
    m_inUse=false;
    m_user=false;
}

void tank::newTank(float x, float y, float z)
{
    m_active=true;
    m_inUse=false;
    m_user=false;
    m_pos[0]=x;
    m_pos[1]=y;
    m_pos[2]=z;
    m_yrot=90;
    m_xrot=0;
    m_zrot=0;
    m_yrot_tower=90;
    m_xrot_tower=90;
    m_wheelSpinR=0;
    m_wheelSpinL=0;
    m_HP=1000;
    m_cannonProg=0;
    m_machinegunProg=0;
    m_add1=true;
    m_add2=true;
    m_noiseID=-1;
}

int tank::update(void) //Passive
{
    if (m_HP<1) {m_active=false; return 1;} //Death
    //m_pos[1]=height;
    return 0;
}

int tank::update(float data[7],float cycleTime) //In use
{
    if (!m_inUse) m_noiseID=-1; //Turn of engine if empty
    if (m_HP<1) {m_active=false; return 1;} //Death
    m_pos[0]=data[0];
    m_pos[1]=data[1];
    m_pos[2]=data[2];
    m_xrot_tower=data[3];
    m_yrot_tower=data[4];
    m_yrot=data[5];
    m_wheelState=data[6];
    //Wheel Update
    float sens=2;
    switch ((int)m_wheelState)
    {
        case 11: m_wheelSpinR+=sens;m_wheelSpinL+=sens; break;
        case 12: m_wheelSpinR-=sens;m_wheelSpinL-=sens; break;
        case 13: m_wheelSpinR+=sens;m_wheelSpinL-=sens; break;
        case 14: m_wheelSpinR-=sens;m_wheelSpinL+=sens; break;
    }
    //Recoil update
    if (m_cannonProg>0)
    {
        if (m_add1) m_cannonProg+=cycleTime*0.01;
        else m_cannonProg-=cycleTime*0.003;
        if (m_cannonProg>1) m_add1=false;
        if (m_cannonProg<0) {m_cannonProg=0; m_add1=true;}
    }
    if (m_machinegunProg>0)
    {
        if (m_add2) m_machinegunProg+=cycleTime*0.02;
        else m_machinegunProg-=cycleTime*0.01;
        if (m_machinegunProg>1) m_add2=false;
        if (m_machinegunProg<0) {m_machinegunProg=0; m_add2=true;}
    }
    return 0;
}

void tank::drawTank(void)
{
    float vex_chassi[]={
        //Bottom
        -1.5,-0.75,1,  1.5,-0.75,1,  1.5,0,1,  -1.5,0,1, //B
        1.5,-0.75,-1,  -1.5,-0.75,-1,  -1.5,0,-1,  1.5,0,-1, //F
        1.5,-0.75,1,  1.5,-0.75,-1,  1.5,0,-1,  1.5,0,1, //R
        -1.5,-0.75,-1,  -1.5,-0.75,1,  -1.5,0,1,  -1.5,0,-1, //L
        -1.5,-0.75,-1,  1.5,-0.75,-1,  1.5,-0.75,1,  -1.5,-0.75,1, //U

        //Top
        -2,0,1.5,  2,0,1.5,  1.5,0.5,1,  -1.5,0.5,1, //B
        2,0,-1.5,  -2,0,-1.5,  -1.5,0.5,-1,  1.5,0.5,-1, //F
        2,0,1.5,  2,0,-1.5,  1.5,0.5,-1,  1.5,0.5,1, //R
        -2,0,-1.5,  -2,0,1.5,  -1.5,0.5,1,  -1.5,0.5,-1, //L
        -1.5,0.5,1,  1.5,0.5,1,  1.5,0.5,-1,  -1.5,0.5,-1 //T
                       };
    float tex_chassi[]={
        //Bottom
        0.352,0.799,  0.662,0.799,  0.662,0.875,  0.352,0.875, //b/f
        0.352,0.799,  0.662,0.799,  0.662,0.875,  0.352,0.875,
        0.664,0.799,  0.867,0.799,  0.869,0.875,  0.664,0.875, //sides
        0.664,0.799,  0.867,0.799,  0.869,0.875,  0.664,0.875,
        0.352,0.590,  0.662,0.590,  0.662,0.797,  0.352,0.797, //U

        //Top
        0.352,0.514,  0.762,0.514,  0.713,0.588,  0.402,0.588, //b/f
        0.352,0.514,  0.762,0.514,  0.713,0.588,  0.402,0.588,
        0.001,0.494,  0.307,0.494,  0.258,0.570,  0.051,0.570, //sides
        0.001,0.494,  0.307,0.494,  0.258,0.570,  0.051,0.570,
        0.664,0.592,  0.975,0.592,  0.975,0.795,  0.664,0.795 //T
                       };
    float vex_wheel[]={
        -0.5,-0.25,0.25,  0.5,0.25,0.25,  0,0.5,0.25,  -0.5,0.25,0.25, //B1
        0.5,0.25,0.25,  -0.5,-0.25,0.25,  0,-0.5,0.25,  0.5,-0.25,0.25, //B2
        0.5,0.25,-0.25,  -0.5,-0.25,-0.25,  -0.5,0.25,-0.25,  0,0.5,-0.25, //F1
        -0.5,-0.25,-0.25,  0.5,0.25,-0.25,  0.5,-0.25,-0.25,  0,-0.5,-0.25, //F2

        //Track
        0,0.5,0.25,  0.5,0.25,0.25,  0.5,0.25,-0.25,  0,0.5,-0.25,
        0.5,0.25,0.25,  0.5,-0.25,0.25,  0.5,-0.25,-0.25,  0.5,0.25,-0.25,
        0.5,-0.25,0.25,  0,-0.5,0.25,  0,-0.5,-0.25,  0.5,-0.25,-0.25,
        0,-0.5,0.25,  -0.5,-0.25,0.25,  -0.5,-0.25,-0.25,  0,-0.5,-0.25,
        -0.5,-0.25,0.25,  -0.5,0.25,0.25,  -0.5,0.25,-0.25,  -0.5,-0.25,-0.25,
        -0.5,0.25,0.25,  0,0.5,0.25,  0,0.5,-0.25,  -0.5,0.25,-0.25,
                      };
    float tex_wheel[]={
        0.764,0.504,  0.879,0.561,  0.820,0.588,  0.764,0.561, //side
        0.764,0.504,  0.879,0.561,  0.820,0.588,  0.764,0.561,
        0.764,0.504,  0.879,0.561,  0.820,0.588,  0.764,0.561,
        0.764,0.504,  0.879,0.561,  0.820,0.588,  0.764,0.561,

        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590, //Track
        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590,
        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590,
        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590,
        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590,
        0.883,0.543,  0.930,0.543,  0.930,0.590,  0.883,0.590
                      };
    float vex_tower[]={
        -1,-0.7,1,  1,-0.7,1,  1,0.7,1,  -1,0.7,1, //B
        1,-0.7,-1,  -1,-0.7,-1,  -1,0.7,-1,  1,0.7,-1, //F
        1,-0.7,1,  1,-0.7,-1,  1,0.7,-1,  1,0.7,1, //R
        -1,-0.7,-1,  -1,-0.7,1,  -1,0.7,1,  -1,0.7,-1, //L
        -1,0.7,1,  1,0.7,1,  1,0.7,-1,  -1,0.7,-1 //T
                      };
    float tex_tower[]={
        0.001,0.355,  0.195,0.355,  0.195,0.490,  0.001,0.490, //sides
        0.001,0.355,  0.195,0.355,  0.195,0.490,  0.001,0.490,
        0.197,0.355,  0.393,0.355,  0.393,0.490,  0.197,0.490, //f/b
        0.197,0.355,  0.393,0.355,  0.393,0.490,  0.197,0.490,
        0.395,0.299,  0.588,0.299,  0.588,0.490,  0.395,0.490 //Top
                      };
    float vex_cannon[]={
    /*    -1,-0.1,0.1,  1,-0.1,0.1,  1,0.1,0.1,  -1,0.1,0.1, //B
        1,-0.1,-0.1,  -1,-0.1,-0.1,  -1,0.1,-0.1,  1,0.1,-0.1, //F
        1,-0.1,0.1,  1,-0.1,-0.1,  1,0.1,-0.1,  1,0.1,0.1, //R
        -1,-0.1,-0.1,  -1,-0.1,0.1,  -1,0.1,0.1,  -1,0.1,-0.1, //L
        -1,0.1,0.1,  1,0.1,0.1,  1,0.1,-0.1,  -1,0.1,-0.1, //T
        1,-0.1,0.1,  -1,-0.1,0.1,  -1,-0.1,-0.1,  1,-0.1,-0.1 //U
   */
        0,-0.1,0.1,  2,-0.1,0.1,  2,0.1,0.1,  0,0.1,0.1, //B
        2,-0.1,-0.1,  0,-0.1,-0.1,  0,0.1,-0.1,  2,0.1,-0.1, //F
        2,-0.1,0.1,  2,-0.1,-0.1,  2,0.1,-0.1,  2,0.1,0.1, //R
        0,-0.1,-0.1,  0,-0.1,0.1,  0,0.1,0.1,  0,0.1,-0.1, //L
        0,0.1,0.1,  2,0.1,0.1,  2,0.1,-0.1,  0,0.1,-0.1, //T
        2,-0.1,0.1,  0,-0.1,0.1,  0,-0.1,-0.1,  2,-0.1,-0.1 //U
                       };
    float tex_cannon[]={
        0.001,0.316,  0.195,0.316,  0.195,0.355,  0.001,0.355, //sides
        0.195,0.355,  0.001,0.355,  0.001,0.316,  0.195,0.316,   //x
        0.197,0.316,  0.236,0.316,  0.236,0.355,  0.197,0.355, //F
        0.238,0.316,  0.277,0.316,  0.277,0.355,  0.238,0.355, //B
        0.001,0.316,  0.195,0.316,  0.195,0.355,  0.001,0.355,
        0.195,0.355,  0.001,0.355,  0.001,0.316,  0.195,0.316,   //x
                       };

glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    if (m_inUse) glTranslatef(m_pos[0],m_pos[1]-1.2,m_pos[2]);
    else glTranslatef(m_pos[0],m_pos[1]+0.7,m_pos[2]);
    glRotatef(m_yrot,0,1,0);
    glRotatef(m_xrot,0,0,1);
    glRotatef(m_zrot,1,0,0);
    glScalef(0.7,0.7,0.7);
    glPushMatrix();
        //Draw Chassi
        glVertexPointer(3, GL_FLOAT, 0, vex_chassi);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_chassi);
        glDrawArrays(GL_QUADS, 0, 40);
        glPushMatrix();
            //Draw Wheels
            glVertexPointer(3, GL_FLOAT, 0, vex_wheel);
            glTexCoordPointer(2, GL_FLOAT, 0, tex_wheel);
            glTranslatef(1.2,-0.5,1.25);
            glRotatef(-m_wheelSpinR,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinR,0,0,1);
            glTranslatef(-1.2,0,0);
            glRotatef(-m_wheelSpinR,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinR,0,0,1);
            glTranslatef(-1.2,0,0);
            glRotatef(-m_wheelSpinR,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinR,0,0,1);
            glTranslatef(0,0,-2.5);
            glRotatef(-m_wheelSpinL,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinL,0,0,1);
            glTranslatef(1.2,0,0);
            glRotatef(-m_wheelSpinL,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinL,0,0,1);
            glTranslatef(1.2,0,0);
            glRotatef(-m_wheelSpinL,0,0,1);
            glDrawArrays(GL_QUADS, 0, 40);
            glRotatef(m_wheelSpinL,0,0,1);
        glPopMatrix();
        //Draw Tower
        glVertexPointer(3, GL_FLOAT, 0, vex_tower);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_tower);
        glScalef(0.7,0.7,0.7);
        glTranslatef(0,1.3,0);
        glRotatef(m_yrot_tower-m_yrot,0,1,0);
        glDrawArrays(GL_QUADS, 0, 20);
        //Draw Cannon1
        glVertexPointer(3, GL_FLOAT, 0, vex_cannon);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_cannon);
        glTranslatef(0,0.2,-0.7);
        glRotatef(180-m_xrot_tower*2-m_xrot,0,0,1);
        glTranslatef(-m_cannonProg*0.9,0,0);
        glScalef(1,2,2);
        glDrawArrays(GL_QUADS, 0, 24);
        glScalef(1,0.5,0.5);
        //Draw Cannon2
        glTranslatef(0-m_machinegunProg+m_cannonProg*0.9,0,1.8);
        glDrawArrays(GL_QUADS, 0, 24);
    glPopMatrix();

glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void tank::drawFPSTank(void)
{
    float vex_chassi[]={
        //Bottom
        -1.5,-0.75,1,  1.5,-0.75,1,  1.5,0,1,  -1.5,0,1, //B
        1.5,-0.75,-1,  -1.5,-0.75,-1,  -1.5,0,-1,  1.5,0,-1, //F
        1.5,-0.75,1,  1.5,-0.75,-1,  1.5,0,-1,  1.5,0,1, //R
        -1.5,-0.75,-1,  -1.5,-0.75,1,  -1.5,0,1,  -1.5,0,-1, //L
        -1.5,-0.75,-1,  1.5,-0.75,-1,  1.5,-0.75,1,  -1.5,-0.75,1, //U

        //Top
        -2,0,1.5,  2,0,1.5,  1.5,0.5,1,  -1.5,0.5,1, //B
        2,0,-1.5,  -2,0,-1.5,  -1.5,0.5,-1,  1.5,0.5,-1, //F
        2,0,1.5,  2,0,-1.5,  1.5,0.5,-1,  1.5,0.5,1, //R
        -2,0,-1.5,  -2,0,1.5,  -1.5,0.5,1,  -1.5,0.5,-1, //L
        -1.5,0.5,1,  1.5,0.5,1,  1.5,0.5,-1,  -1.5,0.5,-1 //T
                       };
    float tex_chassi[]={
        //Bottom
        0.352,0.799,  0.662,0.799,  0.662,0.875,  0.352,0.875, //b/f
        0.352,0.799,  0.662,0.799,  0.662,0.875,  0.352,0.875,
        0.664,0.799,  0.867,0.799,  0.869,0.875,  0.664,0.875, //sides
        0.664,0.799,  0.867,0.799,  0.869,0.875,  0.664,0.875,
        0.352,0.590,  0.662,0.590,  0.662,0.797,  0.352,0.797, //U

        //Top
        0.352,0.514,  0.762,0.514,  0.713,0.590,  0.402,0.590, //b/f
        0.352,0.514,  0.762,0.514,  0.713,0.590,  0.402,0.590,
        0.001,0.494,  0.307,0.494,  0.258,0.570,  0.051,0.570, //sides
        0.001,0.494,  0.307,0.494,  0.258,0.570,  0.051,0.570,
        0.664,0.592,  0.975,0.592,  0.975,0.797,  0.664,0.797 //T
                       };
    float vex_cannon[]={
    /*    -1,-0.1,0.1,  1,-0.1,0.1,  1,0.1,0.1,  -1,0.1,0.1, //B
        1,-0.1,-0.1,  -1,-0.1,-0.1,  -1,0.1,-0.1,  1,0.1,-0.1, //F
        1,-0.1,0.1,  1,-0.1,-0.1,  1,0.1,-0.1,  1,0.1,0.1, //R
        -1,-0.1,-0.1,  -1,-0.1,0.1,  -1,0.1,0.1,  -1,0.1,-0.1, //L
        -1,0.1,0.1,  1,0.1,0.1,  1,0.1,-0.1,  -1,0.1,-0.1, //T
        1,-0.1,0.1,  -1,-0.1,0.1,  -1,-0.1,-0.1,  1,-0.1,-0.1 //U
   */
        0,-0.1,0.1,  2,-0.1,0.1,  2,0.1,0.1,  0,0.1,0.1, //B
        2,-0.1,-0.1,  0,-0.1,-0.1,  0,0.1,-0.1,  2,0.1,-0.1, //F
        2,-0.1,0.1,  2,-0.1,-0.1,  2,0.1,-0.1,  2,0.1,0.1, //R
        0,-0.1,-0.1,  0,-0.1,0.1,  0,0.1,0.1,  0,0.1,-0.1, //L
        0,0.1,0.1,  2,0.1,0.1,  2,0.1,-0.1,  0,0.1,-0.1, //T
        2,-0.1,0.1,  0,-0.1,0.1,  0,-0.1,-0.1,  2,-0.1,-0.1 //U
                       };
    float tex_cannon[]={
        0.001,0.316,  0.195,0.316,  0.195,0.355,  0.001,0.355, //sides
        0.195,0.355,  0.001,0.355,  0.001,0.316,  0.195,0.316,   //x
        0.197,0.316,  0.236,0.316,  0.236,0.355,  0.197,0.355, //F
        0.238,0.316,  0.277,0.316,  0.277,0.355,  0.238,0.355, //B
        0.001,0.316,  0.195,0.316,  0.195,0.355,  0.001,0.355,
        0.195,0.355,  0.001,0.355,  0.001,0.316,  0.195,0.316,   //x
                       };
 /*   float vex_plate[]={
        1,-0.7,-1,  -1,-0.7,-1,  -1,0.7,-1,  1,0.7,-1
                      };
    float tex_plate[]={
        0.197,0.355,  0.393,0.355,  0.393,0.492,  0.197,0.492
                      };*/
glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTranslatef(m_pos[0],m_pos[1]-0.8,m_pos[2]);
    glRotatef(m_yrot,0,1,0);
    glRotatef(m_xrot,0,0,1);
    glRotatef(m_zrot,1,0,0);
    glScalef(0.7,0.7,0.7);
    glPushMatrix();
        //Draw Chassi
        glVertexPointer(3, GL_FLOAT, 0, vex_chassi);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_chassi);
        glDrawArrays(GL_QUADS, 0, 40);
        //Draw Tower
        glScalef(0.7,0.7,0.7);
        glTranslatef(0,1.3,0);
        glRotatef(m_yrot_tower-m_yrot,0,1,0);
        //Draw Plate
        //glVertexPointer(3, GL_FLOAT, 0, vex_plate);
        //glTexCoordPointer(2, GL_FLOAT, 0, tex_plate);
        //glDrawArrays(GL_QUADS, 0, 4);
        //Draw Cannon1
        glVertexPointer(3, GL_FLOAT, 0, vex_cannon);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_cannon);
        glTranslatef(0.5,0.2,-1.1);
        glRotatef(180-m_xrot_tower*2-m_xrot,0,0,1);
        glTranslatef(-m_cannonProg,0,0);
        glScalef(1,2,2);
        glDrawArrays(GL_QUADS, 0, 24);
        glScalef(1,0.5,0.5);
        //Draw Cannon2
        glTranslatef(0-m_machinegunProg+m_cannonProg,0,2.2);
        glDrawArrays(GL_QUADS, 0, 24);
    glPopMatrix();

glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

bool tank::tankBulletHitTest(float xpos, float ypos, float zpos)
{
    float xhitbox=1.5;
    float yhitbox=0.75;
    float zhitbox=1.5;
    if (xpos>m_pos[0]-xhitbox && xpos<m_pos[0]+xhitbox &&
        ypos>m_pos[1]-yhitbox+1 && ypos<m_pos[1]+yhitbox+1 &&
        zpos>m_pos[2]-zhitbox && zpos<m_pos[2]+zhitbox)
    return true;
    else
    return false;
}

void tank::place(float xpos,float height,float zpos)
{
    m_inUse=false;
    m_noiseID=-1;
    m_pos[0]=xpos;
    m_pos[1]=height;
    m_pos[2]=zpos;
}
