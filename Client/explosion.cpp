#include <gl\gl.h>
#include <stdlib.h>
#include <math.h>
#include "explosion.h"

explosion::explosion()
{
    m_active=false;
    m_progress=0;
    m_color=8;
    m_size=0;
}

void explosion::newExplosion(float x,float y,float z, int explosionSize, int color)
{
    m_active=true;
    if (explosionSize>90) {m_size=2; m_numOfPolygon=rand()%30+20;}
    else {m_size=1; m_numOfPolygon=rand()%10+5;}
    m_xpos=x;m_ypos=y;m_zpos=z;m_progress=0.01;
    m_color=color;
    for (int i=0;i<m_numOfPolygon;i++)
    {
        Polygon[i].xpos=0;Polygon[i].ypos=0;Polygon[i].zpos=0;
        Polygon[i].xrot=0;Polygon[i].yrot=0;
        Polygon[i].xvel=(rand()%20-10)*0.0005*m_size;
        Polygon[i].yvel=(rand()%20+5)*0.001*m_size;
        Polygon[i].zvel=(rand()%20-10)*0.0005*m_size;
        Polygon[i].progressDelay=rand()%10*0.005;
        Polygon[i].XrotationSpeed=(rand()%20-10)*0.1;
        Polygon[i].YrotationSpeed=(rand()%20-10)*0.1;
    }
}

void explosion::update(float cycleTime)
{
    //update progress
    m_progress+=cycleTime*0.0005;
    if (m_progress>1) {m_progress=0; m_active=false; return;}

    for (int i=0;i<m_numOfPolygon;i++)
    {
        //Update Pos/Speed
        float sens=0.5;
        Polygon[i].yvel-=0.0002*cycleTime*sens;
        Polygon[i].xpos+=Polygon[i].xvel*cycleTime*sens;
        Polygon[i].ypos+=Polygon[i].yvel*cycleTime*sens;
        Polygon[i].zpos+=Polygon[i].zvel*cycleTime*sens;
        Polygon[i].yrot+=Polygon[i].YrotationSpeed*cycleTime*sens;
        Polygon[i].xrot+=Polygon[i].XrotationSpeed*cycleTime*sens;
    }
}

void explosion::drawExplosion(void)
{
    //draw parts
    const float piover180=0.0174532925;
    float smallTriangle[9]={-0.3,0,0.3, 0.6,0,0.3, -0.3,0,-0.6};
    float bigTriangle[9]={-1,0,1, 2,0,1, -1,0,-2};
    //Change color
    switch (m_color)
    {
        case 0: {float mat_spec[]={0.1,0.1,0.7,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 1: {float mat_spec[]={0.1,0.7,0.1,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 2: {float mat_spec[]={0.7,0.1,0.1,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 3: {float mat_spec[]={0.7,0.1,0.7,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 4: {float mat_spec[]={0.4,0.3,0.3,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 5: {float mat_spec[]={0.8,0.4,0.3,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 6: {float mat_spec[]={0.2,0.8,0.8,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 7: {float mat_spec[]={0.9,0.8,0.8,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 8: {float mat_spec[]={0.2,0.2,0.1,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 9: {float mat_spec[]={0.2,0.2,0.2,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec); //Turret
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 10: {float mat_spec[]={0.3,0.3,0.1,1};glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec); //Sand
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 11: {float mat_spec[]={0.1,0.1,0.1,1};glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec); //Stone
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        case 12: {float mat_spec[]={0.3,0.1,0.1,1};glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec); //Mountain
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
        default:{float mat_spec[]={0.2,0.2,0.1,1}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_spec);
                                                   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec); break;}
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    if (m_size==2) glVertexPointer(3, GL_FLOAT, 0, bigTriangle);
    else glVertexPointer(3, GL_FLOAT, 0, smallTriangle);
    for (int i=0;i<m_numOfPolygon;i++)
    {
        m_progress-=Polygon[i].progressDelay;
        if (m_progress>0.01)
        {
            //Calculate Normals
            float xnorm=(float)(sinf((Polygon[i].xrot)*piover180)*cosf((Polygon[i].yrot)*piover180));
            float ynorm=(float)cosf((Polygon[i].xrot)*piover180);
            float znorm=-(float)(sinf((Polygon[i].xrot)*piover180)*sinf((Polygon[i].yrot)*piover180));
            float triangleNormals[9]={xnorm,ynorm,znorm, xnorm,ynorm,znorm, xnorm,ynorm,znorm};
            glNormalPointer(GL_FLOAT, 0, triangleNormals);

            glPushMatrix();
            glTranslatef(m_xpos+Polygon[i].xpos,
                         m_ypos+Polygon[i].ypos,
                         m_zpos+Polygon[i].zpos);
            glRotatef(Polygon[i].yrot,0,1,0);
            glRotatef(Polygon[i].xrot,1,0,0);
            glScalef(0.2,0.2,0.2);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glPopMatrix();
        }
        m_progress+=Polygon[i].progressDelay;
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}
