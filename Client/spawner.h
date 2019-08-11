#ifndef SPAWNER_H
#define SPAWNER_H



class spawner
{
      protected:

      public:
             spawner();
             bool  m_active;
             bool  m_moving;
             int   m_carrier;
             float m_pos[3];
             float m_yhed;
             int   m_HP;
             int   m_owner;
             int   m_content;
             bool  m_open;
             int   m_team;
             int   m_textureID;
             int   m_output;
             int   m_price[10];
             int   m_hitCount;

             void  newSpawner(int,int,int,int,float);
             bool  updateSpawner(void);
             void  updateSpawner(float,float,float,float);
             void  drawSpawner(void);
             bool  spawnerCollision(float,float,float);
             bool  spawnerCollision(float,float,float,int);
             bool  spawnerBulletHitTest(float,float,float);
             void  place(float,float,float);

};


#endif
