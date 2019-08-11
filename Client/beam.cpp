#include "beam.h"
#include <gl\gl.h>

beam::beam()
{
    m_active=false;            
}

void beam::newBeam(float data[6])
{
    m_active=true;
    m_age=1000;
    m_start_pos[0]=data[0]; 
    m_start_pos[1]=data[1]; 
    m_start_pos[2]=data[2]; 
    m_end_pos[0]=data[3];
    m_end_pos[1]=data[4];
    m_end_pos[2]=data[5];    
}

void beam::update(float cycleTime)
{
    m_age-=cycleTime;
    if (m_age<0) m_active=false;     
}

void beam::drawBeam(void)
{
    float intens1=(m_age/1000)*(m_age/1000);
    float intens2=0;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glBegin(GL_QUADS);
    glColor4f(1,1,1,intens1);
    glVertex4f(m_start_pos[0],m_start_pos[1],m_start_pos[2],1);
    glColor4f(1,1,1,intens2);
    glVertex4f(m_start_pos[0],m_start_pos[1]-0.1,m_start_pos[2],1);
    glColor4f(1,1,1,intens2);
    glVertex4f(m_end_pos[0],m_end_pos[1]-0.1,m_end_pos[2],1);
    glColor4f(1,1,1,intens1);
    glVertex4f(m_end_pos[0],m_end_pos[1],m_end_pos[2],1);
 
    glColor4f(1,1,1,intens2);
    glVertex4f(m_end_pos[0],m_end_pos[1]+0.1,m_end_pos[2],1);
    glColor4f(1,1,1,intens1);
    glVertex4f(m_end_pos[0],m_end_pos[1],m_end_pos[2],1);   
    glColor4f(1,1,1,intens1);
    glVertex4f(m_start_pos[0],m_start_pos[1],m_start_pos[2],1);
    glColor4f(1,1,1,intens2);
    glVertex4f(m_start_pos[0],m_start_pos[1]+0.1,m_start_pos[2],1);

    glEnd();  
    
    glDisable(GL_BLEND);    
}
