#ifndef BUILDBLOCK_H
#define BUILDBLOCK_H

class buildblock
{
    public:
        buildblock();

        bool   m_active;
        float  m_pos[3];
        float  m_rot;
        bool   m_moving;
        bool   m_inHands;
        int    m_carrier;
        int    m_type;
        int    m_HP;
        int    m_textureID;

        void   newBuildblock(int,int);
        void   updatePos(float,float,float,float);
        int    update(void);
        void   drawBuildblock(void);
        bool   buildblockBulletHitTest(float,float,float);
        void   place(float,float,float);


};

#endif
