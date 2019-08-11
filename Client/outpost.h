#ifndef OUTPOST_H
#define OUTPOST_H


class outpost
{
    public:

        bool  m_active;
        float m_pos[3];
        float m_yrot;
        int   m_HP;
        int   m_connected_radarID;
        bool  m_moving;
        int   m_carrier;

        outpost();
        void  newOutpost(float,float,float,int);
        int   update(void);
        int   update(float,float,float,float);
        void  drawOutpost(void);
        bool  outpostBulletHitTest(float,float,float);
        void  place(float,float,float);

    protected:
    private:
};

#endif // OUTPOST_H
