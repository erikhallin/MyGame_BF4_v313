#ifndef BUILDBOX_H
#define BUILDBOX_H

class buildbox
{
    public:
        buildbox();

        bool   m_active;
        float  m_pos[3];
        float  m_rot;
        bool   m_moving;
        bool   m_inHands;
        int    m_carrier;
        int    m_type;
        int    m_content;
        int    m_HP;
        int    m_textureID;

        void   newBuildbox(int,int);
        void   updatePos(float,float,float,float);
        int    update(void);
        void   drawBuildbox(void);
        bool   buildboxBulletHitTest(float,float,float);
        void   place(float,float,float);

};

#endif
