#ifndef NUMMER_H
#define NUMMER_H

#include <gl\gl.h>
#include <math.h>

//Definition borde finnas i .cpp    

//nummer
void ritaMinus(void)
   {
   glBegin(GL_LINES);
   glVertex3f(0.0,0.5,0);
   glVertex3f(1.0,0.5,0);   
   glEnd();
   }
void rita0(void)
   {
   glBegin(GL_LINE_LOOP);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glEnd();
   }
void rita1(void)
   {
   glBegin(GL_LINES);
   glVertex3f(0.5f, 1.0f, 0.0f);
   glVertex3f(0.5f, 0.0f, 0.0f);
   glEnd();
   }   
void rita2(void)
   {
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glEnd();
   }   
void rita3(void)
   {
   glBegin(GL_LINES);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.5f, 0.0f);
   glVertex3f(0.0f, 0.5f, 0.0f);
   glVertex3f(1.0f, 0.5f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 0.5f, 0.0f);
   glEnd();
   }   
void rita4(void)
   {
   glBegin(GL_LINES);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.5f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glEnd();
   }   
void rita5(void)
   {
   glBegin(GL_LINE_STRIP);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glEnd();
   }   
void rita6(void)
   {
   glBegin(GL_LINE_STRIP);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glVertex3f(0.5f, 0.5f, 0.0f);
   glEnd();
   }   
void rita7(void)
   {
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glEnd();
   }   
void rita8(void)
   {
   glBegin(GL_LINES);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(1.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glEnd();
   }   
void rita9(void)
   {
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(0.0f, 1.0f, 0.0f);
   glVertex3f(0.5f, 0.5f, 0.0f);
   glEnd();
   }   
      
void drawTal(float tal)
{
    if (tal<1000)
    {
        glPushMatrix();
        int i, tals[3];
        tals[0]= (int)tal/100;
        tals[1]= (int)(tal-tals[0]*100)/10;
        tals[2]= (int)tal-tals[0]*100-tals[1]*10;
        if (tals[0]<0 || tals[1]<0 || tals[2]<0) ritaMinus();
        glTranslatef(1.5f,0.0f,0.0f);
        for (i=0;i<3;i++) tals[i]=(int)fabs(tals[i]);
        for (i=0;i<3;i++) 
        {
           switch (tals[i])
           {
              case 0: {rita0(); break;} 
              case 1: {rita1(); break;} 
              case 2: {rita2(); break;}
              case 3: {rita3(); break;}
              case 4: {rita4(); break;}
              case 5: {rita5(); break;}
              case 6: {rita6(); break;}
              case 7: {rita7(); break;}
              case 8: {rita8(); break;}
              case 9: {rita9(); break;}
           }
           glTranslatef(1.5f,0.0f,0.0f);//nästa plats
        }
        glPopMatrix();
    }
    else 
    {
         ritaMinus();
         glTranslatef(0,-2.5,0);
    }
}


#endif
