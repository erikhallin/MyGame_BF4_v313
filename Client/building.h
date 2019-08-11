#ifndef BUILDING_H
#define BUILDING_H

class building
{
    public:
           building();
           
           bool   m_active;
           int    m_HP;
           int    m_type;
           float  m_pos[3];
           int    m_owner;
           bool   m_inventory[10];
           int    m_powCount;
           int    m_price[10];
           bool   m_busy;
           float  m_buildProgress;
           int    m_textureID;
           
           void newBuilding(int,float,int,int,int);
           int  update(int,float);
           void drawBuilding(void);
           bool buildingBulletHitTest(float,float,float);
           bool buildingInteractionHitTest(float,float,float);
      
};

#endif
