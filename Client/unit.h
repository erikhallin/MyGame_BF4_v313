#ifndef UNIT_H
#define UNIT_H

class unit
{
      public:
             float m_xpos, m_ypos, m_zpos;
             float m_xvel, m_yvel, m_zvel;
             float m_ybodyhed, m_zhed, m_yheadhed;
             float m_xdir,m_ydir,m_zdir;
             float m_avgvel;
             float m_progress;
             float m_progress2;
             float m_ben;
             float m_benbend;
             float m_arm1;
             float m_arm2;
             int   m_HP;
             int   m_playerID;
             int   m_jumpState;
             bool  m_add1;
             bool  m_add2;
             bool  m_add3;
             bool  m_add4;
             bool  m_moving;
             bool  m_spawning;
             bool  m_carrySpawner;
             int   m_carryID;
             bool  m_carryPow;
            // int   m_carryPowID;
             float m_size;
             bool  m_spawnerMoving;
             bool  m_spawnerOnline;
             int   m_currWeapon;
             int   m_prevWeapon;
             int   m_nextWeapon;
             int   m_deathCounter;
             float m_stepTimer;
             bool  m_loadedShovel;
             float m_meleeAttackProg;
             float m_interactionProg;
             float m_rifleAttackProg;
             float m_rifleAttackProg2;
             int   m_team;
             float m_weaponSwapProg;
             int   m_inHands; //5=terrain
             int   m_inHandsID;
             bool  m_carries;
             int   m_carryType; //(1)Spawner,(2)BBox,(3)BBlock,(4)Pow,(5)Heli
             bool  m_carryBBox;
             bool  m_carryBBlock;
             bool  m_inventory[10];
             bool  m_zooming;
             int   m_inTransportType;
             int   m_transportID;
             float m_transportFloat;
             int   m_fireCount;
             int   m_hitCount;
             float m_healingDelay;
             //Texture
             GLuint m_playerSkin;

             unit();
             bool m_online;
             bool unitBulletHitTest(float,float,float);
             int  updateUnit(float);
             void drawUnit(void);
             void drawFPSview(bool,bool);
             void getData(float playerData[]);
             bool unitCollision(float,float,float);
             void drawRadar(int);
             void getTextureID(int);
};



#endif
