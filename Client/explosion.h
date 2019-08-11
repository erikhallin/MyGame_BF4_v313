#ifndef EXPLOSION_H
#define EXPLOSION_H

class explosion
{
    public:
        bool  m_active;
        float m_xpos, m_ypos, m_zpos;
        float m_progress;
        int m_numOfPolygon;
        int m_size;
        int m_color;
        struct polygon
        {
            float xpos,ypos,zpos;
            float xvel,yvel,zvel;    
            float xrot,yrot;
            float XrotationSpeed,YrotationSpeed;
            float xnorm,ynorm,znorm;  //normal for light
            float progressDelay;
            int   rotationSpeed;
        }Polygon[50];
        explosion();
        void newExplosion(float,float,float,int,int);
        void update(float);
        void drawExplosion(void);
};

#endif
