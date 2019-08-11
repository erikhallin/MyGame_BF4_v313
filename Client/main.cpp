#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include <ctime>
#include <WinSock2.h>
#include <vector>

#include "nummer.h"
#include "text.h"
#include "unit.h"
#include "projectile.h"
#include "explosion.h"
#include "sound.h"
#include "spawner.h"
#include "powerup.h"
#include "turret.h"
#include "mine.h"
#include "imageloader.h"
#include "resource.h"
#include "buildbox.h"
#include "buildblock.h"
#include "building.h"
#include "wall.h"
#include "heli.h"
#include "speed.h"
#include "tank.h"
#include "beam.h"
#include "md2model.h"
#include "logfile.h"
#include "radar.h"
#include "outpost.h"
#include "world.h"

using namespace std;

//=========================================================================================================================

//Windows values
HDC			hDC=NULL;		    // Private GDI Device Context
HGLRC		hRC=NULL;		    // Permanent Rendering Context
HWND		hWnd=NULL;		    // Holds Our Window Handle
HINSTANCE	hInstance;	  	    // Holds The Instance Of The Application
bool    	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool        activeSwitch=true;
bool	    fullscreen=false;	// Fullscreen Flag Set To Fullscreen Mode By Default
int         windowWidth=640;
int         windowHeight=480;

//Misc
int   g_buildversion=312;
const float piover180=0.0174532925;
bool  g_debugMode=false;
float g_debug0=0;
float g_debug1=0;
float g_debug2=0;
float g_debug3=0;
enum  g_gameStates {gMeny, gLobby, gInGame, gPreLobby};
int   g_gameState=gMeny;
bool  g_firstTimeInit=true;
bool  g_showPlayerData=false;
bool  g_testServer=false;
MD2Model* _model;
logfile Logfile;
int   g_rememberID=0;
struct XZ{int x; int z;};

//Terrain
int          g_seed=1;
int          g_terrainWidth=1001;
int          g_terrainLength=1001;
float        g_lightPos[4] = { 0,0,0,1 };
int          g_terPoly=0;
int          g_numOfStars=200;
struct       star {int x; int y; int z; float intensity;};
star         Star[200];
world        World;

//Controls
int   cursorX=300;
int   cursorY=240;
bool  keys[256];
bool  mouse[2];
float g_scroll=0;
float g_pushDelay=0;
float g_fireDelay=0;
int   currNumber=12;
int   currLobbyNumber=6;
int   inputDelay=0;
int   delay_M1=0;
int   delay_M2=0;
int   toggle=0;
float g_transportFloat=0;
bool  onoff=true;
float g_heightZoom=400;
float g_maxVel=0.02;
bool  g_collided=false;
enum  g_views {vFPS=1,vTop=2,v3rd=3};
int   g_view=1;
int   g_specUnit=0;
bool  g_shovelLoaded=false;
bool  g_M2down=false;
bool  g_M2toggle=false;
int   g_muzzleFlash=0;
bool  g_wantToLeave=false;
bool  g_spectating=false;

//Multiplayer
SOCKET SServer;
bool   startGame=false;
bool   serverFound=true;
bool   haveIP=false;
enum   gameModes{gmFFA=1,gmTEAM};
int    g_gameMode=gmFFA;
int    g_numOfTeams=0;
unit   Unit[8];
string sIPnumber;
char   cIPnumber[20]="192.168.1.60";
int    g_numOfPlayers=1;
int    g_maxPlayers=8;
bool   g_timeToStart=false;
int    g_lobbyStatus=0;
int    g_allPlayerLobbyData[9][20];
bool   g_lobbyTeamSwapButton=false;
bool   g_lobbyLeaveButton=false;
float  g_tempPos[3];
int    g_tempID=0;
float  g_extraFloatA=0;
float  g_extraFloatB=0;

//Projectile
bool g_projCreated=false;
int  g_numOfProj=0;
int  g_nextProjID;
int  g_maxProj=100;
proj Proj[100];

//Explosion
int       g_numOfExplosion=0;
int       g_maxExplosion=30;
int       g_nextExplosionID;
explosion Explosion[30];

//Turrets
int    g_maxTurrets=100;
turret Turret[100];

//Mines
int    g_maxMines=100;
mine   Mine[100];

//Beam
int    g_maxBeams=100;
beam   Beam[100];

//Buildings
int        g_maxBuildboxes=99;
buildbox   Buildbox[99];
int        g_maxBuildblocks=99;
buildblock Buildblock[99];
enum       costs{cWass=5,cTass=5,cDass=5};
int        g_maxBuildings=99;
building   Building[99];
wall       Wall[999];
int        g_maxWalls=999;
int        g_maxRadars=99;
radar      Radar[99];
int        g_maxOutposts=999;
outpost    Outpost[999];

//Transport
heli   Heli[100];
int    g_maxHelis=100;
speed  Speed[100];
int    g_maxSpeeds=100;
tank   Tank[100];
int    g_maxTanks=100;

//Timing
float       iTimeSomeCyclesAgo;
float       iFPSdelay=0;
float       avgFPS=0;
float       avgCycleTime;
float       CurrTime=0;
float       LastTime=0;
float       CycleTime;
float       FPS;

//Text
float text0VertexArray[500];
int   text0v;
float text1VertexArray[500];
int   text1v;
float text2VertexArray[500];
int   text2v;
float text3VertexArray[500];
int   text3v;
float text4VertexArray[500];
int   text4v;
float text5VertexArray[500];
int   text5v;
float text6VertexArray[500];
int   text6v;
float text7VertexArray[500];
int   text7v;
float text8VertexArray[500];
int   text8v;
float text9VertexArray[500];
int   text9v;
float text10VertexArray[500];
int   text10v;
float text11VertexArray[500]; //Your name
int   text11v;
float text12VertexArray[500];
int   text12v;
float text13VertexArray[500];
int   text13v;
float text14VertexArray[500];
int   text14v;
float text15VertexArray[500];
int   text15v;
float text20VertexArray[500]; //Player name
int   text20v;
float text21VertexArray[500]; //Player name
int   text21v;
float text22VertexArray[500]; //Player name
int   text22v;
float text23VertexArray[500]; //Player name
int   text23v;
float text24VertexArray[500]; //Player name
int   text24v;
float text25VertexArray[500]; //Player name
int   text25v;
float text26VertexArray[500]; //Player name
int   text26v;
float text27VertexArray[500]; //Player name
int   text27v;
float text28VertexArray[500]; //Server name
int   text28v;

//Sound
sound Sound;
bool  g_soundOK=false;
int   g_soundError=0;
int   g_makeSound=0;
int   g_soundArr[20];
//               0            1          2           3          4
enum  Sounds {Serase=1,  Sexplosion,   ShitUnit,    Sjump,      Sland,
//               5            6          7           8          9
               Snohost,     Sselect,  Sselected,    Sshot,  ShitSpawn,
//              10           11         12          13         14
       ShitSpawnShield, SrocketLaunch, Srevolver,    Suzi,    Ssniper,
//              15           16         17          18         19
           SshovelMiss,      Sexit,    Sdeath,  Sspawning,       Sdig,
//              20           21         22          23         24
                Sundig,      Sstep,      Sbeep,     Swin,       Slost,
//              25                      26                     27
                SpowerupDelivery, SpowerupPickup,      SpowerupSpawn,
//              28           29         30          30         31
                Smine,SplaceObject,Stank_gun,Stank_cannon,Sturret_laser,
//              32                      33          34         35
                Sturret_gattling, Stank_motor, Sheli_motor,Sspeed_motor};
//-------------------- INCREMENT numOfSound in sound.h -------------------------

//Player
float g_xpos=510;   float g_xhed=1;   float g_xupp=0;  float g_xrig=0;   float g_xvel=0;   float g_xacc=0;  float g_nxvel=0;
float g_ypos=100;   float g_yhed=0;   float g_yupp=1;  float g_yrig=0;   float g_yvel=0;   float g_yacc=0;  float g_nzvel=0;
float g_zpos=510;   float g_zhed=0;   float g_zupp=0;  float g_zrig=0;   float g_zvel=0;   float g_zacc=0;
                                                                         float g_avgvel=0;
float g_headingY=0;
float g_headingXZ=90;
float g_groundLevel=10;
float g_onGroundLevel=true;
bool  g_flyMode=false;
bool  g_moving=false;
int   g_jumpState=0;
float g_jumpForce=0;
int   g_iHitPlayer=0;
int   g_playerID=0;
bool  g_online=true;
int   g_action=0;
int   g_animation=0;
int   g_deathCounter=0;
bool  g_zooming=false;
float g_sniperZoom=1;
bool  g_iSeeUnit=false;
bool  g_iSeeSpawner=false;
float g_dDist=0;
float g_beepDelay=0;
char  g_playerName[]={"PLAYER              "};
char  g_playerNames[9][20];
int   g_lobbyTeam=1;

//Gameplay
spawner  Spawner[8];
int      g_spawnXpos[8];
int      g_spawnZpos[8];
int      g_numOfPowerups=0;
int      g_maxNumOfPowerups=100;
powerup  Powerup[999];
float    g_checkSum=0;
bool     g_gameOver=false;
bool     g_victory=false;
int      g_flashTimer=0;
float    g_xFlashPos=0;
float    g_zFlashPos=0;
float    g_yFlashPos=0;
int      g_fireWeapon=0;
int      g_simulate=0;
enum     simulations{simOFF,simTP2outpostIN,simTP2outpostOUT,simTP2radarIN,simTP2radarOUT};
float    g_simulationTimer=0;
int      g_random;
int      g_randomLast=0;

//HUD
bool    g_showHUD=false;
int     g_HUDtimer=0;

//Frustum Culling
float g_viewAngleY=30;
float g_viewAngleXZ=25;
float g_maxViewDist=400;
float g_minViewDist=-6;
float g_planeBottom[3];
float g_planeTop[3];
float g_planeNear[3];
float g_planeFar[3];
float g_planeLeft[3];
float g_planeRight[3];
float g_pointFar[3];
float g_pointNear[3];
int   g_viewDist=100;

//Texture
int    g_numOfTextures=3;
GLuint g_textureID[10];

//Functions
//void   genHeights(void);
bool   fixConnection(void);
void   sendPlayerData(void);
void   getOtherPlayerData(void);
void   inputIP(void);
void   drawIP(void);
void   loadText(void);
void   drawSurface(float);
int    bulletHitTest(int);
void   drawHUD(void);
void   collisionDetection(void);
void   explosionAt(float,float,float,int,int,int);
void   loadSound(void);
bool   checkFrustumBox(float,float,float);
void   gameInit(void);
int    testCollision(float,float,float);
void   randomEvent(int,int);
void   fireWeapon(int);
void   exitGame(void);
void   doDamage(int,int,int);
void   extraSound(void);
void   drawText(int);
void   zoomingOff(void);
bool   checkRadarBox(float,float,float);
void   spawnSeeker(void);
void   gameOverTest(void);
void   flash(float,float,float);
void   showHUD(void);
void   informationSwap(void);
float  findCamPos(void);
void   setCamPos(float);
void   drawObjects(void);
void   inputMeny(void);
void   drawMeny(void);
void   infoSwapLobby(void);
void   inputLobby(void);
void   drawLobby(void);
void   drawSky(void);
void   genStars(void);
void   sendPlayerLobbyData(void);
void   getOtherPlayerLobbyData(void);
void   drawScope(void);
void   unpauseCheck(void);
void   genSpawnerPos(void);
void   getServerLobbyData(void);
GLuint loadTexture(image* Image);
void   loadTextures(void);
void   flattenGround(int);
//void   genTerrainTexture(void);
void   firstTimeInit(void);
void   interaction(int,int);
bool   pickUpObject(void);
void   putDownObject(void);
int    objectBehindTest(int,int);
int    completeBuildingCheck(int,int,int,int&);
void   displayPlayerData(void);
void   lowerGround(int x,int z,float amount);
void   genSomeStartObjects(void);
void   impact(int,int);
void   heightChange(int,int);
void   emptyHands(int);
void   makeSound(int,int,float,int);
void   drawAntennas(void);
int    interactionHitTest(int,int);
void   transportKeyCheck(void);
void   walkingKeyCheck(void);
void   fixTankAngle(void);
void   tankCollision(void);
void   calcCheckSum(void);
bool   resetTass(int);
void   adjustUnitPos(void);
float  getMobileObjectHeight(float,float);
void   calcRadarDisplay(float px[],int);
void   fireRecoil(int);
void   loadModels(void);
void   runSimulation(void);
bool   checkOutpostView(int,int,int);
void   drawTerrain(void);
void   showPlayerName(void);

//===========================================================================================================

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.01f,500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(void)										// All Setup For OpenGL Goes Here
{
    if (g_firstTimeInit) firstTimeInit();
    loadTextures();
    loadModels();

//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset(0.9, 1.1);
/*
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glHint(GL_FOG_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
*/
    //Fog
    GLfloat fogColor[4]= {0.01f, 0.01f, 0.01f, 1.0f};      // Fog Color
    glFogi(GL_FOG_MODE, GL_LINEAR);                     // Fog Mode GL_EXP, GL_EXP2, and GL_LINEAR
    glFogfv(GL_FOG_COLOR, fogColor);                    // Set Fog Color
    glFogf(GL_FOG_DENSITY, 0);                          // How Dense Will The Fog Be
    glFogf(GL_FOG_START, 10.0f);                        // Fog Start Depth
    glFogf(GL_FOG_END, 200.0f);                         // Fog End Depth
    glEnable(GL_FOG);                                   // Enables GL_FOG
    //Light
    //float mat_spec[]={0.2, 0.2, 0.1, 1.0};
    //glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
    //Light1
   // float ambi[]={1,1,1,1};
    float diff[]={1,1,1,1};
    float spec[]={1,1,1,1};
   // glLightfv(GL_LIGHT1, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec);

    float test[]={0.01,0,0};
    //glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, test);
    glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, test);
    glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, test);

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

	return TRUE;
}

void keyCheck(void)
{
    //ADJUST
    if (g_debugMode)
    {
        float sens=1;
        if (keys[110]) sens=10;
        if (keys[96]) {Radar[g_tempID].m_extra[0]=0;Radar[g_tempID].m_extra[1]=0;Radar[g_tempID].m_extra[2]=0;} //NUMPAD 0
        if (keys[97]) Radar[g_tempID].m_extra[0]-=0.001*sens; //NUMPAD 1
        if (keys[98]) Radar[g_tempID].m_extra[1]-=0.001*sens; //NUMPAD 2
        if (keys[99]) Radar[g_tempID].m_extra[2]-=0.001*sens; //NUMPAD 3
        if (keys[100]) Radar[g_tempID].m_extra[0]+=0.001*sens; //NUMPAD 4
        if (keys[101]) Radar[g_tempID].m_extra[1]+=0.001*sens; //NUMPAD 5
        if (keys[102]) Radar[g_tempID].m_extra[2]+=0.001*sens; //NUMPAD 6
    }

    //Key Delay
    if (g_pushDelay>=0) g_pushDelay-=CycleTime*0.17;
    if (g_fireDelay>=0) g_fireDelay-=CycleTime*0.17;
    //Mouse Toggle
    g_M2toggle=false;
    if (mouse[1])
    {
        if (!g_M2down) {g_M2toggle=true; g_M2down=true;}
    }
    else g_M2down=false;
    //Debugmode
   /* if (keys[81] && g_pushDelay<0) //q
    {
        g_flyMode=!g_flyMode;
        g_pushDelay=100;
        g_onGroundLevel=true;
        g_debugMode=!g_debugMode;
        Unit[g_playerID].m_HP-=10;
    }*/
    if (keys[9]) g_showPlayerData=true; else g_showPlayerData=false; //tab
    //Pause
    if (keys[80] && g_pushDelay<0) {activeSwitch=false; ShowCursor(true); g_pushDelay=100;} //p
    //VIEWING
    if (keys[86]) {g_view=vFPS; glEnable(GL_FOG); /*zoomingOff();*/} //v
    if (keys[67]) {g_view=vTop; glDisable(GL_FOG); zoomingOff();} //c
    if (keys[66]) {g_view=v3rd; glEnable(GL_FOG); /*zoomingOff();*/} //b
    if (keys[88]) g_heightZoom+=0.7; //x
    if (keys[90]) g_heightZoom-=0.7; //z
    if (Unit[g_playerID].m_spawning) {if (g_view<10) g_view+=10;}
    else {if (g_view>10) g_view-=10;}
    //view HUD
    if (keys[17] && g_onGroundLevel && !g_moving && g_view==vFPS && !g_zooming) {g_showHUD=true;} //ctrl
    else g_showHUD=false;
    showHUD();
    //Movement
    if (Unit[g_playerID].m_spawnerOnline && !Unit[g_playerID].m_spawning)
    {
        if (Unit[g_playerID].m_inTransportType!=0) transportKeyCheck();
        else walkingKeyCheck();
    }

    //Spawner is Offline
    if (!Unit[g_playerID].m_spawnerOnline)
    {
        if (g_pushDelay<0 && keys[32]) {g_spectating=!g_spectating; g_pushDelay=50;}
        while (cursorX>292)
        {
            g_headingY-=0.1;
            cursorX--;
        }
        while (cursorX<292)
        {
            g_headingY+=0.1;
            cursorX++;
        }
        while (cursorY>210)
        {
            if (g_headingXZ<170)
            {
                g_headingXZ+=0.1;
            }
            cursorY--;
        }
        while (cursorY<210)
        {
            if (g_headingXZ>10 )
            {
                g_headingXZ-=0.1;
            }
            cursorY++;
        }
        if (g_spectating)
        {
            g_view=v3rd;
            if (mouse[0] && g_pushDelay<0) //Spec next Unit
            {
                g_specUnit++;
                if (g_specUnit>=g_numOfPlayers) g_specUnit=0;
                g_pushDelay=50;
            }
            if (mouse[1] && g_pushDelay<0) //Spec prev Unit
            {
                g_specUnit--;
                if (g_specUnit<0) g_specUnit=g_numOfPlayers-1;
                g_pushDelay=50;
            }
            g_xpos=Unit[g_specUnit].m_xpos;
            g_ypos=Unit[g_specUnit].m_ypos;
            g_zpos=Unit[g_specUnit].m_zpos;
        }
        else
        {
            g_view=vFPS;
            g_xvel=0;
            g_yvel=0;
            g_zvel=0;
            float sens=0.2;
            if (keys[87]) //w
            {
                g_xvel+=g_xhed*sens;
                g_yvel+=g_yhed*sens;
                g_zvel+=g_zhed*sens;
            }
            if (keys[83]) //s
            {
                g_xvel+=-g_xhed*sens;
                g_yvel+=-g_yhed*sens;
                g_zvel+=-g_zhed*sens;
            }
            if (keys[65]) //a
            {
                g_xvel+=-g_xrig*sens;
                g_yvel+=-g_yrig*sens;
                g_zvel+=-g_zrig*sens;
            }
            if (keys[68]) //d
            {
                g_xvel+=g_xrig*sens;
                g_yvel+=g_yrig*sens;
                g_zvel+=g_zrig*sens;
            }
            if (keys[82]) g_ypos+=0.1; //r
            if (keys[70]) g_ypos-=0.1; //f
            g_xpos+=g_xvel;
            g_ypos+=g_yvel;
            g_zpos+=g_zvel;
        }
    }
    //Vektorer för glLookAt
    g_xhed=sinf((g_headingXZ)*piover180)*cosf((g_headingY)*piover180);
    g_yhed=cosf(g_headingXZ*piover180);
    g_zhed=-sinf((g_headingXZ)*piover180)*sinf((g_headingY)*piover180);
    g_xupp=-sinf((g_headingXZ+90)*piover180)*cosf(g_headingY*piover180);
    g_yupp=-cosf((g_headingXZ+90)*piover180);
    g_zupp=sinf((g_headingXZ+90)*piover180)*sinf(g_headingY*piover180);
    g_xrig=sinf((g_headingXZ)*piover180)*cosf((g_headingY-90)*piover180);
    g_yrig=0;//cosf((g_headingXZ)*piover180);
    g_zrig=-sinf((g_headingXZ)*piover180)*sinf((g_headingY-90)*piover180);

    //Vectors for Frustum Culling
    if (g_view==vFPS)
    {
        if (Unit[g_playerID].m_zhed>140) //Looking Down
        {
            g_viewAngleY=40;
            g_viewAngleXZ=20;
        }
        else
        {
            if (g_zooming)
            {
                g_viewAngleY=10;
                g_viewAngleXZ=10;
            }
            else
            {
                g_viewAngleY=35;
                g_viewAngleXZ=30;//30
            }
        }
    }
    else {g_viewAngleY=35;g_viewAngleXZ=40;}

    g_planeLeft[0]=sinf((g_headingXZ)*piover180)*cosf((g_headingY-90+g_viewAngleY)*piover180);
    g_planeLeft[1]=cosf(g_headingXZ*piover180);
    g_planeLeft[2]=-sinf((g_headingXZ)*piover180)*sinf((g_headingY-90+g_viewAngleY)*piover180);
    g_planeRight[0]=sinf((g_headingXZ)*piover180)*cosf((g_headingY+90-g_viewAngleY)*piover180);
    g_planeRight[1]=cosf(g_headingXZ*piover180);
    g_planeRight[2]=-sinf((g_headingXZ)*piover180)*sinf((g_headingY+90-g_viewAngleY)*piover180);
    g_planeTop[0]=sinf((g_headingXZ-g_viewAngleXZ+90)*piover180)*cosf(g_headingY*piover180);
    g_planeTop[1]=cosf((g_headingXZ-g_viewAngleXZ+90)*piover180);
    g_planeTop[2]=-sinf((g_headingXZ-g_viewAngleXZ+90)*piover180)*sinf(g_headingY*piover180);
    g_planeBottom[0]=sinf((g_headingXZ+g_viewAngleXZ-90)*piover180)*cosf(g_headingY*piover180);
    g_planeBottom[1]=cosf((g_headingXZ+g_viewAngleXZ-90)*piover180);
    g_planeBottom[2]=-sinf((g_headingXZ+g_viewAngleXZ-90)*piover180)*sinf(g_headingY*piover180);
    g_planeNear[0]=g_xhed;
    g_planeNear[1]=g_yhed;
    g_planeNear[2]=g_zhed;
    g_planeFar[0]=-g_xhed;
    g_planeFar[1]=-g_yhed;
    g_planeFar[2]=-g_zhed;
    //Point in near and far plane
    if (g_zooming)
    {
        g_pointFar[0]=g_xpos+g_xhed*(g_maxViewDist+100);
        g_pointFar[1]=g_ypos+g_yhed*(g_maxViewDist+100);
        g_pointFar[2]=g_zpos+g_zhed*(g_maxViewDist+100);
    }
    else
    {
        g_pointFar[0]=g_xpos+g_xhed*g_maxViewDist;
        g_pointFar[1]=g_ypos+g_yhed*g_maxViewDist;
        g_pointFar[2]=g_zpos+g_zhed*g_maxViewDist;
    }
    if (g_view==vFPS)
    {
        g_pointNear[0]=g_xpos+g_xhed*g_minViewDist;
        g_pointNear[1]=g_ypos+g_yhed*g_minViewDist;
        g_pointNear[2]=g_zpos+g_zhed*g_minViewDist;
    }
    else
    {
        g_pointNear[0]=g_xpos+g_xhed*(g_minViewDist-5);
        g_pointNear[1]=g_ypos+g_yhed*(g_minViewDist-5);
        g_pointNear[2]=g_zpos+g_zhed*(g_minViewDist-5);
    }
}

int GameCycle(void)
{
    switch (g_gameState)
    {
        case gMeny:
            {
                glClear(GL_COLOR_BUFFER_BIT);
                glLoadIdentity();

                inputMeny();
                drawMeny();
            }
            break;

        case gPreLobby:
            {
                glClear(GL_COLOR_BUFFER_BIT);
                glLoadIdentity();

                if (haveIP)
                {
                    if (!fixConnection())
                    {
                        Sound.playSimpleSound(Snohost,1);
                        serverFound=false;
                        gameInit();
                        g_gameState=gInGame;
                        Logfile.log("No Host Found");
                    }
                    else
                    {
                        g_lobbyStatus=1;
                        sendPlayerLobbyData(); //Sends Status,Team,fakeID,Name
                        getServerLobbyData(); //Get ServerStatus,gamemode,seed,realID,ServerName
                        g_gameState=gLobby;
                        Logfile.log("Host Found");
                    }
                }
                else
                {
                    inputIP();
                    drawIP();
                    if (haveIP) drawText(1);
                }
            }
            break;

        case gLobby:
            {
                glClear(GL_COLOR_BUFFER_BIT);
                glLoadIdentity();

                if (g_timeToStart) {gameInit(); g_gameState=gInGame;}
                else {inputLobby(); infoSwapLobby();}
                drawLobby();
            }
            break;

        case gInGame:
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);
                glLoadIdentity();

                if (activeSwitch)
                {
                    if (g_simulate==simOFF) keyCheck();
                    else runSimulation();
                }
                else unpauseCheck();
                if (activeSwitch) {if (fullscreen) SetCursorPos(292,210); else SetCursorPos(300,240);}

                informationSwap();

                collisionDetection();

                float camPos=0;
                if (g_view==v3rd) camPos=findCamPos();
               glPushMatrix();
                glDisable(GL_DEPTH_TEST);
                if (g_view!=vTop) drawSky();
                glEnable(GL_DEPTH_TEST);
               glPopMatrix();
               glPushMatrix();
                setCamPos(camPos);

                //drawSurface(camPos);
                drawTerrain();

                drawObjects();
                drawAntennas();

                glEnable(GL_CULL_FACE);
                drawHUD();
               glPopMatrix();
                if (g_zooming) drawScope();
                extraSound();
                if (!g_gameOver && serverFound) gameOverTest();
            }
            break;
    }
    return TRUE;
}

//==========================================================================================================================
//==========================================================================================================================

//Global Functions

void showPlayerName(void)
{
    if (interactionHitTest(g_playerID,50)==2) //Unit hit
    {
        glPushMatrix();
        glTranslatef(4,5,0);
        drawText(g_tempID+20);
        glPopMatrix();
    }
}

void drawTerrain(void)
{
    float player_pos[8][2];
    for (int player=0;player<8;player++)
    {
        if (Unit[player].m_online) //update only if player is online
        {
            if (player==g_playerID)
            {
                if (World.updateMyChunks(g_xpos,g_zpos)) //update chunks near you
                {
                    //Sound.playSimpleSound(Sexplosion,1); //TEST
                }
            }
            else
            {
                World.updateOtherChunks(Unit[player].m_xpos,Unit[player].m_zpos); //update chunks near others
            }
        }
        player_pos[player][0]=Unit[player].m_xpos;
        player_pos[player][1]=Unit[player].m_zpos;
    }
    World.checkAbandonedChunks(player_pos);

    //alt. FC Test live chunks
    bool corner[chunk_full][chunk_full];
    bool draw[chunk_full][chunk_full];
    for (int x=0;x<chunk_full;x++)
    for (int z=0;z<chunk_full;z++)
    {
        draw[x][z]=false; //default value
        int xpos=World.getLiveChunkXval(x,z)*chunk_size;
        int zpos=World.getLiveChunkZval(x,z)*chunk_size;
        int ypos=World.getHeight(xpos,zpos);
        if (checkFrustumBox(xpos,ypos,zpos)) corner[x][z]=true; //inside FC
        else corner[x][z]=false;
    }

    for (int x=0;x<chunk_full;x++)
    for (int z=0;z<chunk_full;z++)
    {
        if (corner[x][z]) //if true draw surrounding chunks to
        {
            draw[x][z]=true;
            if (x>1) draw[x-1][z]=true;
            if (x>1 && z>1) draw[x-1][z-1]=true;
            if (z>1) draw[x][z-1]=true;
        }
    }
    draw[chunk_offset][chunk_offset]=true; //always draw center chunk
    //Erase corners to make circle
    int co_min,co_max;
    for (int x=0;x<chunk_full;x++)
    for (int z=0;z<chunk_full;z++)
    {
        switch (z)
        {
            case 0: co_min=5; co_max=9; break;
            case 1: co_min=3; co_max=11; break;
            case 2: co_min=2; co_max=12; break;
            case 3: co_min=1; co_max=13; break;
            case 4: co_min=1; co_max=13; break;

            case 10:co_min=1; co_max=13; break;
            case 11:co_min=1; co_max=13; break;
            case 12:co_min=2; co_max=12; break;
            case 13:co_min=3; co_max=11; break;
            case 14:co_min=5; co_max=9; break;
            default:co_min=0; co_max=14; break;
        }
        if (x<co_min || x>co_max) draw[x][z]=false;
    }

    float pos[3]={g_xpos,g_ypos,g_zpos};
    World.drawSurface(draw,pos);
}

bool checkOutpostView(int x,int z,int radarID)
{
    for (int i=0;i<99;i++) //Go through the radars outpostID storage
    {
        if (Radar[radarID].m_outpostID[i]!=-1) //Active ID number
        {
            int outpostID=Radar[radarID].m_outpostID[i];
            if (Outpost[outpostID].m_active) //See if active (should always be true)
            {
                //Test coord
                int viewDist=50;
                int outpost_xpos=Outpost[outpostID].m_pos[0]; int outpost_zpos=Outpost[outpostID].m_pos[2];
                if (x<outpost_xpos+viewDist && x>outpost_xpos-viewDist &&
                    z<outpost_zpos+viewDist && z>outpost_zpos-viewDist)
                return true;
            }
        }
    }
    return false;
}

void runSimulation(void)
{
    switch (g_simulate)
    {
        case simTP2outpostIN:
            {
                if (g_simulationTimer<1) g_simulationTimer+=CycleTime/200;
                g_zpos+=g_simulationTimer/10;
                g_ypos-=g_simulationTimer/50;
                if (g_simulationTimer>1)
                {
                    g_xpos=Outpost[g_rememberID].m_pos[0]+0.5;
                    g_ypos=Outpost[g_rememberID].m_pos[1]+1;
                    g_zpos=Outpost[g_rememberID].m_pos[2]+0.5;
                    //Set view vectors
                    if (g_view==vFPS) //Look up
                    {
                        g_headingXZ=10;
                        g_xhed=0; g_yhed=1; g_zhed=0;
                    }
                    else if (g_view==v3rd) //Look down
                    {
                        g_headingXZ=170;
                        g_xhed=0; g_yhed=-1; g_zhed=0;
                    }
                    g_simulationTimer=0;
                    g_simulate=simTP2outpostOUT;
                }
            }break;
        case simTP2outpostOUT:
            {
                if (g_simulationTimer<1) g_simulationTimer+=CycleTime/200;
                g_ypos+=0.1;
                if (g_simulationTimer>1)
                {
                    g_simulationTimer=0;
                    g_simulate=simOFF;
                    g_yvel=0.05;
                    g_zvel=0.02;
                }
            }break;
        case simTP2radarIN:
            {
                if (g_simulationTimer<1) g_simulationTimer+=CycleTime/200;
                g_ypos-=0.1;
                if (g_simulationTimer>1)
                {
                    g_xpos=Radar[g_rememberID].m_pos[0]+2.8;
                    g_ypos=Radar[g_rememberID].m_pos[1]+1;
                    g_zpos=Radar[g_rememberID].m_pos[2]+0.5;
                    g_simulationTimer=0;
                    g_simulate=simTP2radarOUT;
                    Radar[g_rememberID].m_doorProg=0.01;
                }
            }break;
        case simTP2radarOUT:
            {
                if (g_simulationTimer<1) g_simulationTimer+=CycleTime/200;
                g_zpos-=0.04;
                if (g_simulationTimer>1)
                {
                    g_simulationTimer=0;
                    g_simulate=simOFF;
                    g_zvel=-0.02;
                }
            }break;
        default: g_simulationTimer=0;
                 g_simulate=simOFF;
    }
}

void loadModels(void)
{
	//Load the models
	MD2Model* modelID;
	modelID=MD2Model::load("Res/Models/radar.md2");
	if (modelID != NULL) modelID->setAnimation("run");
	for (int i=0;i<g_maxRadars;i++)
	{
	    Radar[i].m_modelID=modelID;
	}
}

void fireRecoil(int unit)
{
    int currWeapon=Unit[unit].m_currWeapon; //Get current Weapon
    if (Unit[unit].m_inTransportType) //In transport?
    {
        if (Unit[unit].m_inTransportType==3) currWeapon=13; //Tank
        else return;
    }
    float power=0; //Recoil power
    switch (currWeapon)
    {
        case 0:  power=0;break; //Melee
        case 1:  power=0.03;break; //Rev
        case 2:  power=0.03;break; //Uzi
        case 3:  power=0.1;break; //Sniper
        case 4:  power=0.1;break; //RL

        case 13: power=0.03;break; //Tank
    }
    if (power==0) return; //No recoil
    g_xvel-=g_xhed*power;
    g_yvel-=g_yhed*power;
    g_zvel-=g_zhed*power;
}

void calcRadarDisplay(float pixelColor[],int radarID) //Size 4800 (20x20 pixels * 4 corners * 3 RGB)
{
    int radar_xpos=Radar[radarID].m_pos[0]; int radar_zpos=Radar[radarID].m_pos[2];
    int radar_xfoc=Radar[radarID].m_focus[0]; int radar_zfoc=Radar[radarID].m_focus[2];
    int zoomLevel=Radar[radarID].m_zoomLevel;
    int viewDist=100;
    bool haveColor;
    int pixelOffset=0;
    float time=clock();
    float speed=Radar[radarID].m_zoomLevel*100;
    float start_value=(time-(int(time/speed)*speed))/speed;
    float rand_color=start_value;
    for (int x_pixel=0;x_pixel<20;x_pixel++) //Pixel Coord
    {
        rand_color=start_value+(float(x_pixel)/20);
        for (int z_pixel=0;z_pixel<20;z_pixel++) //Pixel Coord
        {
            if (pixelOffset>=4800) break; //Offset is off limit
            int map_xpos=radar_xfoc+zoomLevel*x_pixel-10*zoomLevel;
            int map_zpos=radar_zfoc+zoomLevel*z_pixel-10*zoomLevel;
            haveColor=false;

            rand_color+=0.21;
            while (rand_color>1) rand_color-=1;

            // (0) Fog of War
            if (map_xpos>radar_xpos+viewDist || map_xpos<radar_xpos-viewDist ||
                map_zpos>radar_zpos+viewDist || map_zpos<radar_zpos-viewDist)
            {
                bool outpostView=false;
                //if (map_xpos>0 && map_xpos<g_terrainLength && map_zpos>0 && map_zpos<g_terrainWidth) //inside map, check outposts
                if (checkOutpostView(map_xpos,map_zpos,radarID)) outpostView=true; //Seen by outpost
                if (!outpostView) //Not in view of outpost
                {
                    //Black/White noise
                    float temp_col=rand_color*0.8;
                    pixelColor[pixelOffset+0]=temp_col+0.1;/*R*/pixelColor[pixelOffset+1]=temp_col+0.1;/*G*/pixelColor[pixelOffset+2]=temp_col+0.1;/*B*/
                    pixelColor[pixelOffset+3]=temp_col;/*R*/pixelColor[pixelOffset+4]=temp_col;/*G*/pixelColor[pixelOffset+5]=temp_col+0.2;/*B*/
                    pixelColor[pixelOffset+6]=temp_col;/*R*/pixelColor[pixelOffset+7]=temp_col+0.2;/*G*/pixelColor[pixelOffset+8]=temp_col;/*B*/
                    pixelColor[pixelOffset+9]=temp_col+0.2;/*R*/pixelColor[pixelOffset+10]=temp_col;/*G*/pixelColor[pixelOffset+11]=temp_col;/*B*/
                    pixelOffset+=12;
                    haveColor=true;
                }
            }
            if (haveColor) continue; //Do not test for Units, buildings or terrain
            // (1) Test Units
            for (int unit=0;unit<g_maxPlayers;unit++)
            {
                if (Unit[unit].m_online && Unit[unit].m_spawnerOnline)
                {
                    if (Unit[unit].m_xpos<map_xpos+zoomLevel && Unit[unit].m_xpos>map_xpos &&
                        Unit[unit].m_zpos<map_zpos+zoomLevel && Unit[unit].m_zpos>map_zpos) //Unit detected
                    {
                        //Friend or foe
                        if (g_gameMode==2) //Teams
                        {
                            if (Unit[unit].m_team==Radar[radarID].m_team) //Friend
                            {
                                //Blue flash
                                pixelColor[pixelOffset+0]=rand_color;/*R*/pixelColor[pixelOffset+1]=rand_color;/*G*/pixelColor[pixelOffset+2]=1;/*B*/
                                pixelColor[pixelOffset+3]=rand_color;/*R*/pixelColor[pixelOffset+4]=rand_color;/*G*/pixelColor[pixelOffset+5]=1;/*B*/
                                pixelColor[pixelOffset+6]=rand_color;/*R*/pixelColor[pixelOffset+7]=rand_color;/*G*/pixelColor[pixelOffset+8]=1;/*B*/
                                pixelColor[pixelOffset+9]=rand_color;/*R*/pixelColor[pixelOffset+10]=rand_color;/*G*/pixelColor[pixelOffset+11]=1;/*B*/
                                pixelOffset+=12;
                                haveColor=true;
                            }
                            else //Foe
                            {
                                //Red flash
                                pixelColor[pixelOffset+0]=1;/*R*/pixelColor[pixelOffset+1]=rand_color;/*G*/pixelColor[pixelOffset+2]=rand_color;/*B*/
                                pixelColor[pixelOffset+3]=1;/*R*/pixelColor[pixelOffset+4]=rand_color;/*G*/pixelColor[pixelOffset+5]=rand_color;/*B*/
                                pixelColor[pixelOffset+6]=1;/*R*/pixelColor[pixelOffset+7]=rand_color;/*G*/pixelColor[pixelOffset+8]=rand_color;/*B*/
                                pixelColor[pixelOffset+9]=1;/*R*/pixelColor[pixelOffset+10]=rand_color;/*G*/pixelColor[pixelOffset+11]=rand_color;/*B*/
                                pixelOffset+=12;
                                haveColor=true;
                            }
                        }
                        else
                        {
                            if (unit==g_playerID) //You
                            {
                                //Blue flash
                                pixelColor[pixelOffset+0]=rand_color;/*R*/pixelColor[pixelOffset+1]=rand_color;/*G*/pixelColor[pixelOffset+2]=1;/*B*/
                                pixelColor[pixelOffset+3]=rand_color;/*R*/pixelColor[pixelOffset+4]=rand_color;/*G*/pixelColor[pixelOffset+5]=1;/*B*/
                                pixelColor[pixelOffset+6]=rand_color;/*R*/pixelColor[pixelOffset+7]=rand_color;/*G*/pixelColor[pixelOffset+8]=1;/*B*/
                                pixelColor[pixelOffset+9]=rand_color;/*R*/pixelColor[pixelOffset+10]=rand_color;/*G*/pixelColor[pixelOffset+11]=1;/*B*/
                                pixelOffset+=12;
                                haveColor=true;
                            }
                            else //Someone else
                            {
                                //Red flash
                                pixelColor[pixelOffset+0]=1;/*R*/pixelColor[pixelOffset+1]=rand_color;/*G*/pixelColor[pixelOffset+2]=rand_color;/*B*/
                                pixelColor[pixelOffset+3]=1;/*R*/pixelColor[pixelOffset+4]=rand_color;/*G*/pixelColor[pixelOffset+5]=rand_color;/*B*/
                                pixelColor[pixelOffset+6]=1;/*R*/pixelColor[pixelOffset+7]=rand_color;/*G*/pixelColor[pixelOffset+8]=rand_color;/*B*/
                                pixelColor[pixelOffset+9]=1;/*R*/pixelColor[pixelOffset+10]=rand_color;/*G*/pixelColor[pixelOffset+11]=rand_color;/*B*/
                                pixelOffset+=12;
                                haveColor=true;
                            }
                        }
                    }
                }
            }
            if (haveColor) continue; //Do not test for buildings or terrain
            // (2) Test Buildings (ObjectHeight) OBS bara för en ruta... för segt annars XXXXXXXXXXXXXXXX
         /*   for (int x=map_xpos;x<map_xpos+zoomLevel;x++)
            {
                if (haveColor) break;
                for (int z=map_zpos;z<map_zpos+zoomLevel;z++)
                {*/
                    if (World.getObjectHeight(map_xpos,map_zpos)!=0) //x/z
                    {
                        //Grey
                        pixelColor[pixelOffset+0]=0.3;/*R*/pixelColor[pixelOffset+1]=0.3;/*G*/pixelColor[pixelOffset+2]=0.3;/*B*/
                        pixelColor[pixelOffset+3]=0.4;/*R*/pixelColor[pixelOffset+4]=0.4;/*G*/pixelColor[pixelOffset+5]=0.4;/*B*/
                        pixelColor[pixelOffset+6]=0.4;/*R*/pixelColor[pixelOffset+7]=0.4;/*G*/pixelColor[pixelOffset+8]=0.4;/*B*/
                        pixelColor[pixelOffset+9]=0.5;/*R*/pixelColor[pixelOffset+10]=0.5;/*G*/pixelColor[pixelOffset+11]=0.5;/*B*/
                        pixelOffset+=12;
                        haveColor=true;
            //            break;
                    }
          //      }
         //  }

            if (haveColor) continue; //Do not test for terrain
            // (3) Terrain/Height? om det så kolla så man inte utanför array
            float height=World.getHeight(map_xpos,map_zpos)+50;
            float height_col=height/30;
            if (height_col<0) height_col=0; else if (height_col>3) height_col=3;
            if (height_col<1)
            {
                //Black to Blue
                pixelColor[pixelOffset+0]=0;/*R*/pixelColor[pixelOffset+1]=0;/*G*/pixelColor[pixelOffset+2]=height_col;/*B*/
                pixelColor[pixelOffset+3]=0;/*R*/pixelColor[pixelOffset+4]=0;/*G*/pixelColor[pixelOffset+5]=height_col;/*B*/
                pixelColor[pixelOffset+6]=0;/*R*/pixelColor[pixelOffset+7]=0;/*G*/pixelColor[pixelOffset+8]=height_col;/*B*/
                pixelColor[pixelOffset+9]=0;/*R*/pixelColor[pixelOffset+10]=0;/*G*/pixelColor[pixelOffset+11]=height_col;/*B*/
            }
            else if (height_col<2)
            {
                height_col-=1;
                //Blue to Yellow
                pixelColor[pixelOffset+0]=height_col;/*R*/pixelColor[pixelOffset+1]=height_col;/*G*/pixelColor[pixelOffset+2]=1-height_col;/*B*/
                pixelColor[pixelOffset+3]=height_col;/*R*/pixelColor[pixelOffset+4]=height_col;/*G*/pixelColor[pixelOffset+5]=1-height_col;/*B*/
                pixelColor[pixelOffset+6]=height_col;/*R*/pixelColor[pixelOffset+7]=height_col;/*G*/pixelColor[pixelOffset+8]=1-height_col;/*B*/
                pixelColor[pixelOffset+9]=height_col;/*R*/pixelColor[pixelOffset+10]=height_col;/*G*/pixelColor[pixelOffset+11]=1-height_col;/*B*/

            }
            else if (height_col<3)
            {
                height_col-=2;
                //Yellow to Hot Pink
                pixelColor[pixelOffset+0]=height_col;/*R*/pixelColor[pixelOffset+1]=1-height_col;/*G*/pixelColor[pixelOffset+2]=height_col;/*B*/
                pixelColor[pixelOffset+3]=height_col;/*R*/pixelColor[pixelOffset+4]=1-height_col;/*G*/pixelColor[pixelOffset+5]=height_col;/*B*/
                pixelColor[pixelOffset+6]=height_col;/*R*/pixelColor[pixelOffset+7]=1-height_col;/*G*/pixelColor[pixelOffset+8]=height_col;/*B*/
                pixelColor[pixelOffset+9]=height_col;/*R*/pixelColor[pixelOffset+10]=1-height_col;/*G*/pixelColor[pixelOffset+11]=height_col;/*B*/
            }
            else //over 3
            {
                //Hot Pink
                pixelColor[pixelOffset+0]=1.0;/*R*/pixelColor[pixelOffset+1]=0.0;/*G*/pixelColor[pixelOffset+2]=1.0;/*B*/
                pixelColor[pixelOffset+3]=1.0;/*R*/pixelColor[pixelOffset+4]=0.1;/*G*/pixelColor[pixelOffset+5]=1.0;/*B*/
                pixelColor[pixelOffset+6]=1.0;/*R*/pixelColor[pixelOffset+7]=0.2;/*G*/pixelColor[pixelOffset+8]=1.0;/*B*/
                pixelColor[pixelOffset+9]=1.0;/*R*/pixelColor[pixelOffset+10]=0.0;/*G*/pixelColor[pixelOffset+11]=1.0;/*B*/
            }
            pixelOffset+=12;
            haveColor=true;
        }
    }
    return;
}

float getMobileObjectHeight(float xpos,float zpos)
{
    for (float step=0;step<10;step+=0.1)
    {
        if (testCollision(xpos+0.2,World.getHeight(xpos+0.2,zpos)+step,zpos)==0 &&
            testCollision(xpos,World.getHeight(xpos,zpos+0.2)+step,zpos+0.2)==0 &&
            testCollision(xpos-0.2,World.getHeight(xpos-0.2,zpos)+step,zpos)==0 &&
            testCollision(xpos,World.getHeight(xpos,zpos-0.2)+step,zpos-0.2)==0)
        return step;
    }
    return 10;
}

void adjustUnitPos(void)
{
    //for lopa ett växande i
    for (float step=0.1;step<10;step+=0.1)
    {
        //Test new pos in +X direction
        if (testCollision(g_xpos+step,g_ypos-2,g_zpos)==0) //Free pos
        {
            g_xpos+=step;
            return;
        }
        //Test new pos in -X direction
        if (testCollision(g_xpos-step,g_ypos-2,g_zpos)==0) //Free pos
        {
            g_xpos-=step;
            return;
        }
        //Test new pos in +Z direction
        if (testCollision(g_xpos,g_ypos-2,g_zpos+step)==0) //Free pos
        {
            g_zpos+=step;
            return;
        }
        //Test new pos in -Z direction
        if (testCollision(g_xpos,g_ypos-2,g_zpos-step)==0) //Free pos
        {
            g_zpos-=step;
            return;
        }
        //Test new pos in +Y direction
        if (testCollision(g_xpos,g_ypos+step*2-2,g_zpos)==0) //Free pos
        {
            g_ypos+=step*2;
            return;
        }
    }
}

bool resetTass(int ID)
{
    for (float step=0.2;step<2;step+=0.1)
    {
        if (testCollision(Building[ID].m_pos[0]+2,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+0)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+2,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+1)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+2,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+2)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+2,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+3)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+3,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+0)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+3,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+1)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+3,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+2)!=0) return false;
        if (testCollision(Building[ID].m_pos[0]+3,Building[ID].m_pos[1]+step,Building[ID].m_pos[2]+3)!=0) return false;
    }
    return true;
}

void calcCheckSum(void)
{
    g_checkSum=g_xpos+g_ypos+g_zpos+
               g_headingY+g_headingXZ+
               g_transportFloat+g_moving+
               g_playerID+g_online+g_avgvel+
               g_jumpState+g_projCreated+g_makeSound+
               g_fireWeapon+g_action+g_animation+
               g_extraFloatA+g_extraFloatB;
    //g_random not included due to server updates this value
}

void fixTankAngle(void)
{
    int ID=Unit[g_playerID].m_transportID;
    float txhed=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat)*piover180);
    float tyhed=cosf(g_headingXZ*piover180);
    float tzhed=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat)*piover180);
    float length=sqrt(txhed*txhed+tzhed*tzhed); if (length==0) return;
    float rel_txhed=txhed/length;
    float rel_tzhed=tzhed/length;
    float txrig=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat-90)*piover180);
    float tzrig=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat-90)*piover180);
    length=sqrt(txrig*txrig+tzrig*tzrig); if (length==0) return;
    float rel_txrig=txrig/length;
    float rel_tzrig=tzrig/length;
    //Make local height array [0][0] is left-back
    float height[3][3];
    height[0][0]=World.getHeight(int(g_xpos-rel_txhed-rel_txrig),int(g_zpos-rel_tzhed-rel_tzrig))
                 +World.getObjectHeight(int(g_xpos-rel_txhed-rel_txrig),int(g_zpos-rel_tzhed-rel_tzrig));
    height[1][0]=World.getHeight(int(g_xpos-rel_txhed),          int(g_zpos-rel_tzhed))
                 +World.getObjectHeight(int(g_xpos-rel_txhed),int(g_zpos-rel_tzhed));
    height[2][0]=World.getHeight(int(g_xpos-rel_txhed+rel_txrig),int(g_zpos-rel_tzhed+rel_tzrig))
                 +World.getObjectHeight(int(g_xpos-rel_txhed+rel_txrig),int(g_zpos-rel_tzhed+rel_tzrig));
    height[0][1]=World.getHeight(int(g_xpos-rel_txrig),          int(g_zpos-rel_tzrig))
                 +World.getObjectHeight(int(g_xpos-rel_txrig),int(g_zpos-rel_tzrig));
    height[1][1]=World.getHeight(int(g_xpos),                    int(g_zpos))
                 +World.getObjectHeight(int(g_xpos),int(g_zpos));
    height[2][1]=World.getHeight(int(g_xpos+rel_txrig),          int(g_zpos+rel_tzrig))
                 +World.getObjectHeight(int(g_xpos+rel_txrig),int(g_zpos+rel_tzrig));
    height[0][2]=World.getHeight(int(g_xpos+rel_txhed-rel_txrig),int(g_zpos+rel_tzhed-rel_tzrig))
                 +World.getObjectHeight(int(g_xpos+rel_txhed-rel_txrig),int(g_zpos+rel_tzhed-rel_tzrig));
    height[1][2]=World.getHeight(int(g_xpos+rel_txhed),          int(g_zpos+rel_tzhed))
                 +World.getObjectHeight(int(g_xpos+rel_txhed),int(g_zpos+rel_tzhed));
    height[2][2]=World.getHeight(int(g_xpos+rel_txhed+rel_txrig),int(g_zpos+rel_tzhed+rel_tzrig))
                 +World.getObjectHeight(int(g_xpos+rel_txhed+rel_txrig),int(g_zpos+rel_tzhed+rel_tzrig));

    int A=0;
    int B=0;
    //Find highest front point
    float frontHeight=height[0][2];
    if (height[1][2]>frontHeight) {frontHeight=height[1][2];A=1;}
    if (height[2][2]>frontHeight) {frontHeight=height[2][2];A=2;}
    //Find highest back point
    float backHeight=height[0][0];
    if (height[1][0]>backHeight) {backHeight=height[1][0];B=1;}
    if (height[2][0]>backHeight) {backHeight=height[2][0];B=2;}

    if (frontHeight>backHeight)
    {
        if (frontHeight>Tank[ID].m_pos[1]+tan(Tank[ID].m_xrot*piover180)-2)
        g_extraFloatA=0.5;
        else g_extraFloatA=-0.1;
    }
    else if (frontHeight<backHeight)
    {
        if (backHeight>Tank[ID].m_pos[1]+tan(-Tank[ID].m_xrot*piover180)-2)
        g_extraFloatA=-0.5;
        else g_extraFloatA=0.1;
    }

    //Find highest Right point
    float rightHeight=height[2][0];
    if (height[1][2]>rightHeight) {rightHeight=height[2][1];A=1;}
    if (height[2][2]>rightHeight) {rightHeight=height[2][2];A=2;}
    //Find highest Left point
    float leftHeight=height[0][0];
    if (height[1][0]>leftHeight) {leftHeight=height[0][1];B=1;}
    if (height[2][0]>leftHeight) {leftHeight=height[0][2];B=2;}

    if (rightHeight>leftHeight)
    {
        if (rightHeight>Tank[ID].m_pos[1]+tan(Tank[ID].m_zrot*piover180)-2)
        g_extraFloatB=0.1;
        else g_extraFloatB=-0.05;
    }
    else if (rightHeight<leftHeight)
    {
        if (leftHeight>Tank[ID].m_pos[1]+tan(-Tank[ID].m_zrot*piover180)-2)
        g_extraFloatB=-0.1;
        else g_extraFloatB=0.05;
    }
}

void tankCollision(void)
{
    float xhed=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat)*piover180);
    float yhed=cosf(g_headingXZ*piover180);
    float zhed=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat)*piover180);
    float xrig=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat-90)*piover180);
    float zrig=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat-90)*piover180);
    //Update Pos
    float txpos,typos,tzpos; //Temp pos and vel
    float txvel=2*xhed;
    float tyvel=2*g_yvel;
    float tzvel=2*zhed;
    bool colFront=false;
    bool colBack=false;
    bool colLeft=false;
    bool colRight=false;
    int objectIDcol;
    float vec_length;

    //Check Front Collision
    if (!keys[83])
    {
        vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
        if (vec_length!=0)
        {
            txvel=xhed/vec_length;
            tzvel=zhed/vec_length;
        }
        txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
        txpos+=txvel*2;typos+=tyvel;tzpos+=tzvel*2;
        objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
        if (objectIDcol!=0) colFront=true;                //No Collision
    }
    //Check Back Collision
    if (!keys[87])
    {
        vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
        if (vec_length!=0)
        {
            txvel=xhed/vec_length;
            tzvel=zhed/vec_length;
        }
        txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
        txpos-=txvel*2;typos-=tyvel;tzpos-=tzvel*2;
        objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
        if (objectIDcol!=0) colBack=true;                 //No Collision
    }
    //Check Right Collision
    if (!keys[65])
    {
        vec_length=sqrt((g_xrig)*(g_xrig)+(g_zrig)*(g_zrig)); //Normalize X,Z Velocity
        if (vec_length!=0)
        {
            txvel=g_xrig/vec_length;
            tzvel=g_zrig/vec_length;
        }
        txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
        txpos+=txvel*2;typos+=tyvel;tzpos+=tzvel*2;
        objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
        if (objectIDcol!=0) colRight=true;                //No Collision
    }
    //Check Left Collision
    if (!keys[68])
    {
        vec_length=sqrt((g_xrig)*(g_xrig)+(g_zrig)*(g_zrig)); //Normalize X,Z Velocity
        if (vec_length!=0)
        {
            txvel=g_xrig/vec_length;
            tzvel=g_zrig/vec_length;
        }
        txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
        txpos-=txvel*2;typos-=tyvel;tzpos-=tzvel*2;
        objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
        if (objectIDcol!=0) colLeft=true;                 //No Collision
    }

    if (!colFront && !colBack && !colRight && !colLeft) //New Pos OK
    {
        g_xpos=g_xpos+g_xvel;
        g_ypos=g_ypos+g_yvel;
        g_zpos=g_zpos+g_zvel;
        //g_onObject=0;
    }
    else
    {
        bool Xerr=false;
        bool Yerr=false;
        bool Zerr=false;
        if (colFront)
        {
            vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=xhed/vec_length;
                tzvel=zhed/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel*2,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
            if (testCollision(txpos,typos,tzpos+tzvel*2)!=0) {g_zvel=0; Zerr=true;}
        }
        if (colBack)
        {
            vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=-xhed/vec_length;
                tzvel=-zhed/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel*2,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
            if (testCollision(txpos,typos,tzpos+tzvel*2)!=0) {g_zvel=0; Zerr=true;}
        }
        if (colRight)
        {
            vec_length=sqrt((xrig)*(xrig)+(zrig)*(zrig)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=xrig/vec_length;
                tzvel=zrig/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel*2,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
            if (testCollision(txpos,typos,tzpos+tzvel*2)!=0) {g_zvel=0; Zerr=true;}
        }
        if (colLeft)
        {
            vec_length=sqrt((xrig)*(xrig)+(zrig)*(zrig)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=-xrig/vec_length;
                tzvel=-zrig/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-0.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel*2,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
            if (testCollision(txpos,typos,tzpos+tzvel*2)!=0) {g_zvel=0; Zerr=true;}
        }
        g_xpos=g_xpos+g_xvel;
        g_ypos=g_ypos+g_yvel;
        g_zpos=g_zpos+g_zvel;
    }

    if (g_xpos<5) g_xpos=5;
    if (g_zpos<5) g_zpos=5;
}

void transportKeyCheck(void)
{
    //EXIT
    if (keys[69] && g_pushDelay<0) //e
    {
        //Only exit Transport on free ground
        if (World.getObjectHeight((int)Unit[g_playerID].m_xpos,(int)Unit[g_playerID].m_zpos)==0)
        {
            switch (Unit[g_playerID].m_inTransportType)
            {
                case 1: g_action=-23; break; //Heli
                case 2: g_action=-24; break; //Speed
                case 3: g_action=-25; break; //Tank
            }
            g_pushDelay=50;
        }
    }
    //Steering
    float xhed,yhed,zhed,xrig,zrig;
    float t_xhed=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat)*piover180);
    float t_yhed=cosf(g_headingXZ*piover180);
    float t_zhed=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat)*piover180);
    float t_xrig=sinf((g_headingXZ)*piover180)*cosf((g_transportFloat-90)*piover180);
    float t_zrig=-sinf((g_headingXZ)*piover180)*sinf((g_transportFloat-90)*piover180);

    if (Unit[g_playerID].m_inTransportType==3)
    {
        xhed=t_xhed;
        yhed=t_yhed;
        zhed=t_zhed;
        xrig=t_xrig;
        zrig=t_zrig;
    }
    else
    {
        xhed=g_xhed;
        yhed=0;
        zhed=g_zhed;
        xrig=g_xrig;
        zrig=g_zrig;
    }
    switch (Unit[g_playerID].m_inTransportType)
    {
        case 1:{//Heli
                //scale heading
                float elongfactor=(float)CycleTime/sqrt(xhed*xhed+zhed*zhed);
                float elong_xhed=elongfactor*xhed;//*g_uvel;
                float elong_zhed=elongfactor*zhed;//*g_uvel;

                if (!g_onGroundLevel)
                {
                    float sensitivity=0.0002;
                    if (keys[87]) {                      // w
                        g_xacc+=elong_xhed*sensitivity;
                        g_zacc+=elong_zhed*sensitivity;
                    }
                    if (keys[83]) {                      // s
                        g_xacc+=elong_xhed*-sensitivity;
                        g_zacc+=elong_zhed*-sensitivity;
                    }
                    if (keys[65]) {                      // a
                        g_xacc+=elong_zhed*sensitivity;
                        g_zacc+=elong_xhed*-sensitivity;
                    }
                    if (keys[68]) {                      // d
                        g_xacc+=elong_zhed*-sensitivity;
                        g_zacc+=elong_xhed*sensitivity;
                    }
                }

                if ((keys[87] || keys[83]) && (keys[65] || keys[68])) //lower speed if heading 2 directions
                {
                    g_xacc*=0.7;
                    g_zacc*=0.7;
                }

                //Elevate
                if (keys[32]) //space
                {
                    g_yvel+=0.0001*CycleTime;
                    g_avgvel+=0.0004*CycleTime;
                    if (g_avgvel>1) g_avgvel=1;
                }
                else
                {
                    g_avgvel-=0.0004*CycleTime;
                    if (g_avgvel<0) g_avgvel=0;
                }

                //add acc to vel
                g_xvel+=g_xacc;
                g_yvel+=g_yacc;
                g_zvel+=g_zacc;
                g_xacc=g_yacc=g_zacc=0;

                //Mouselook
                if (!g_onGroundLevel)
                {
                    float yrot=0;
                    while (cursorX>292)
                    {
                        yrot-=0.001;
                        //g_headingY-=0.1;
                        cursorX--;
                    }
                    while (cursorX<292)
                    {
                        yrot+=0.001;
                        //g_headingY+=0.1;
                        cursorX++;
                    }
                    while (cursorY>210)
                    {
                        if (g_headingXZ<170)
                        {
                            g_headingXZ+=0.1;
                        }
                        cursorY--;
                    }
                    while (cursorY<210)
                    {
                        if (g_headingXZ>10 )
                        {
                            g_headingXZ-=0.1;
                        }
                        cursorY++;
                    }
                    g_extraFloatA=g_transportFloat+yrot;
                    g_transportFloat+=yrot;
                    g_transportFloat*=0.98;
                }
                else g_transportFloat=0;
               }break;
        case 2:{//Speed
                //scale heading
                float elongfactor=(float)CycleTime/sqrt(xhed*xhed+zhed*zhed);
                float elong_xhed=elongfactor*xhed;//*g_uvel;
                float elong_zhed=elongfactor*zhed;//*g_uvel;

                float sensitivity=0.00015;
                if (keys[87]) {                      // w
                    g_xacc+=elong_xhed*sensitivity;
                    g_zacc+=elong_zhed*sensitivity;
                }
                if (keys[83]) {                      // s
                    g_xacc+=elong_xhed*-sensitivity;
                    g_zacc+=elong_zhed*-sensitivity;
                }

                float yrot=0;
                if (keys[65]) yrot+=0.05; //a
                if (keys[68]) yrot-=0.05; //d

                //add acc to vel
                g_xvel+=g_xacc;
                g_yvel+=g_yacc;
                g_zvel+=g_zacc;
                g_xacc=g_yacc=g_zacc=0;

                //Mouselook
                while (cursorX>292)
                {
                    yrot-=0.001;
                    //g_headingY-=0.02;
                    cursorX--;
                }
                while (cursorX<292)
                {
                    yrot+=0.001;
                    //g_headingY+=0.02;
                    cursorX++;
                }
                while (cursorY>210)
                {
                    if (g_headingXZ<170)
                    {
                        g_headingXZ+=0.05;
                    }
                    cursorY--;
                }
                while (cursorY<210)
                {
                    if (g_headingXZ>10 )
                    {
                        g_headingXZ-=0.05;
                    }
                    cursorY++;
                }
                g_extraFloatA=g_transportFloat+yrot;
                g_transportFloat+=yrot;
                g_transportFloat*=0.98;
               }break;
        case 3:{//Tank
                //scale heading
                float elongfactor;
                float elong_xhed;
                float elong_zhed;
                elongfactor=(float)CycleTime/sqrt(xhed*xhed+zhed*zhed);
                elong_xhed=elongfactor*xhed;
                elong_zhed=elongfactor*zhed;
                fixTankAngle();
                g_jumpState=10;

                float sensitivity=0.0002;
                if (keys[87]) {                      // w
                    g_xacc+=elong_xhed*sensitivity;
                    g_zacc+=elong_zhed*sensitivity;
                    g_jumpState=11; //Both wheels forward
                }
                if (keys[83]) {                      // s
                    g_xacc+=elong_xhed*-sensitivity;
                    g_zacc+=elong_zhed*-sensitivity;
                    g_jumpState=12; //Both wheels backwards
                }

                if (keys[65]) //a
                {
                    g_transportFloat+=0.5;
                    g_xacc=g_zacc=0;
                    g_jumpState=13; //R+ L-
                }

                if (keys[68]) //d
                {
                    g_transportFloat-=0.5;
                    g_xacc=g_zacc=0;
                    g_jumpState=14; //R- L+
                }

                //add acc to vel
                g_xvel+=g_xacc;
                g_yvel+=g_yacc;
                g_zvel+=g_zacc;
                g_xacc=g_yacc=g_zacc=0;

                while (cursorX>292)
                {
                    g_headingY-=0.1;
                    cursorX--;
                }
                while (cursorX<292)
                {
                    g_headingY+=0.1;
                    cursorX++;
                }
                while (cursorY>210)
                {
                    if (g_headingXZ<110)
                    {
                        g_headingXZ+=0.05;
                    }
                    cursorY--;
                }
                while (cursorY<210)
                {
                    if (g_headingXZ>50 )
                    {
                        g_headingXZ-=0.05;
                    }
                    cursorY++;
                }
                //Fire
                if (mouse[0] && g_fireDelay<0) g_fireWeapon=1;
                else if (mouse[1] && g_fireDelay<0) g_fireWeapon=2;
               }break;
    }

    //Collision
    if (Unit[g_playerID].m_inTransportType==3) tankCollision();
    else
    {
        //Update Pos
        float txpos,typos,tzpos; //Temp pos and vel
        float txvel=2*xhed;
        float tyvel=2*g_yvel;
        float tzvel=2*zhed;
        bool colFront=false;
        bool colBack=false;
        bool colLeft=false;
        bool colRight=false;
        int objectIDcol;
        float vec_length;

        //Check Front Collision
        if (!keys[83])
        {
            vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=xhed/vec_length;
                tzvel=zhed/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) colFront=true;                //No Collision
        }
        //Check Back Collision
        if (!keys[87])
        {
            vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=xhed/vec_length;
                tzvel=zhed/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos-=txvel;typos-=tyvel;tzpos-=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) colBack=true;                 //No Collision
        }
        //Check Right Collision
        if (!keys[65])
        {
            vec_length=sqrt((g_xrig)*(g_xrig)+(g_zrig)*(g_zrig)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=g_xrig/vec_length;
                tzvel=g_zrig/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) colRight=true;                //No Collision
        }
        //Check Left Collision
        if (!keys[68])
        {
            vec_length=sqrt((g_xrig)*(g_xrig)+(g_zrig)*(g_zrig)); //Normalize X,Z Velocity
            if (vec_length!=0)
            {
                txvel=g_xrig/vec_length;
                tzvel=g_zrig/vec_length;
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos-=txvel;typos-=tyvel;tzpos-=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) colLeft=true;                 //No Collision
        }

        if (!colFront && !colBack && !colRight && !colLeft) //New Pos OK
        {
            g_xpos=g_xpos+g_xvel;
            g_ypos=g_ypos+g_yvel;
            g_zpos=g_zpos+g_zvel;
            //g_onObject=0;
        }
        else
        {
            bool Xerr=false;
            bool Yerr=false;
            bool Zerr=false;
            if (colFront)
            {
                vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
                if (vec_length!=0)
                {
                    txvel=xhed/vec_length;
                    tzvel=zhed/vec_length;
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true;}
            }
            if (colBack)
            {
                vec_length=sqrt((xhed)*(xhed)+(zhed)*(zhed)); //Normalize X,Z Velocity
                if (vec_length!=0)
                {
                    txvel=-xhed/vec_length;
                    tzvel=-zhed/vec_length;
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true;}
            }
            if (colRight)
            {
                vec_length=sqrt((xrig)*(xrig)+(zrig)*(zrig)); //Normalize X,Z Velocity
                if (vec_length!=0)
                {
                    txvel=xrig/vec_length;
                    tzvel=zrig/vec_length;
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true;}
            }
            if (colLeft)
            {
                vec_length=sqrt((xrig)*(xrig)+(zrig)*(zrig)); //Normalize X,Z Velocity
                if (vec_length!=0)
                {
                    txvel=-xrig/vec_length;
                    tzvel=-zrig/vec_length;
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true;}
            }
            g_xpos=g_xpos+g_xvel;
            g_ypos=g_ypos+g_yvel;
            g_zpos=g_zpos+g_zvel;
        }
    if (g_xpos<5) g_xpos=5;
    if (g_zpos<5) g_zpos=5;
    }

    //Friction
    switch (Unit[g_playerID].m_inTransportType)
    {
        case 1:{//Heli
                //Gravity
                if (!g_flyMode)
                {
                    float mobileObjectHeight=getMobileObjectHeight(g_xpos,g_zpos);
                    g_groundLevel=World.getAverageHeight(g_xpos,g_zpos)+2+ //AAAAAAAAAAAAAAAAAAAAA
                                  World.getObjectHeight((int)g_xpos,(int)g_zpos)+
                                  mobileObjectHeight;
                    if (g_ypos<g_groundLevel)
                    {
                        g_ypos=g_groundLevel;
                        g_onGroundLevel=true;
                        g_xvel=g_yvel=g_zvel=0;
                    }
                    else if (g_ypos>g_groundLevel)
                    {
                        g_onGroundLevel=false;
                        //More gravity higher up
                        float heightLimit=(g_ypos+900)/1000;
                        g_yvel-=0.0001*heightLimit*CycleTime;
                    }
                }
                else g_ypos=g_groundLevel;

                if (!g_onGroundLevel) //Luftmotstånd
                {
                    //sänk hast, luftmotstånd, procentuellt
                    if (g_xvel>0.001 || g_xvel<-0.001) g_xvel*=0.99; else g_xvel=0;   //CycleTime för att bromsa relativt
                    //if (ypos>0) yvel-=0.01; else yvel=0;
                    if (g_zvel>0.001 || g_zvel<-0.001) g_zvel*=0.99; else g_zvel=0;
                }

               }break;
        case 2:{//Speed
                g_avgvel=fabs(g_xvel)+fabs(g_zvel);
           //     if (g_avgvel>0.140) g_avgvel=0.140;
                //Gravity
                if (!g_flyMode)
                {
                    float mobileObjectHeight=getMobileObjectHeight(g_xpos,g_zpos);
                    g_groundLevel=World.getAverageHeight(g_xpos,g_zpos)+2+
                                  World.getObjectHeight((int)g_xpos,(int)g_zpos)+
                                  mobileObjectHeight;
                    if (g_ypos<g_groundLevel)
                    {
                        g_onGroundLevel=true;
                        g_yvel=0;
                        if (g_ypos<g_groundLevel-0.1) //Uphill slowdown
                        {
                            g_xvel*=0.90;
                            g_zvel*=0.90;
                        }
                        g_ypos=g_groundLevel;
                    }
                    else if (g_ypos>g_groundLevel)
                    {
                        g_onGroundLevel=false;
                        //More gravity higher up XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                        float heightLimit=(g_ypos+900)/1000;
                        g_yvel-=0.0001*heightLimit*CycleTime;
                    }
                }
                else g_ypos=g_groundLevel;

                if (!g_onGroundLevel) //Luftmotstånd
                {
                    //sänk hast, luftmotstånd, procentuellt
                    if (g_xvel>0.001 || g_xvel<-0.001) g_xvel*=0.99; else g_xvel=0;   //CycleTime för att bromsa relativt
                    //if (ypos>0) yvel-=0.01; else yvel=0;
                    if (g_zvel>0.001 || g_zvel<-0.001) g_zvel*=0.99; else g_zvel=0;
                }
               }break;
        case 3:{//Tank
                g_avgvel=fabs(g_xvel)+fabs(g_zvel);
                if (g_avgvel>0.140) g_avgvel=0.140;
                //Gravity
                if (!g_flyMode)
                {
                    float mobileObjectHeight=getMobileObjectHeight(g_xpos,g_zpos);
                    g_groundLevel=World.getAverageHeight(g_xpos,g_zpos)+2+
                                  World.getObjectHeight((int)g_xpos,(int)g_zpos)+
                                  mobileObjectHeight;
                    if (g_ypos<g_groundLevel)
                    {
                        g_onGroundLevel=true;
                        g_yvel=0;
                        if (g_ypos<g_groundLevel-0.1) //Uphill slowdown
                        {
                            g_xvel*=0.90;
                            g_zvel*=0.90;
                        }
                        g_ypos=g_groundLevel;
                    }
                    else if (g_ypos>g_groundLevel)
                    {
                        g_onGroundLevel=false;
                        g_yvel-=0.0001*CycleTime;
                    }
                }
                else g_ypos=g_groundLevel;
                /*
                float force=0.0002;
                if (g_xvel>0.001) g_xvel-=force;
                else if (g_xvel<-0.001) g_xvel+=force;
                else g_xvel=0;
                if (g_zvel>0.001) g_zvel-=force;
                else if (g_zvel<-0.001) g_zvel+=force;
                else g_zvel=0;
                */
                //sänk hast, luftmotstånd, procentuellt
                if (g_xvel>0.0001 || g_xvel<-0.0001) g_xvel*=0.95; else g_xvel=0;   //CycleTime för att bromsa relativt
                //if (ypos>0) yvel-=0.01; else yvel=0;
                if (g_zvel>0.0001 || g_zvel<-0.0001) g_zvel*=0.95; else g_zvel=0;
               }break;
    }

}

void walkingKeyCheck(void)
{
    if (Unit[g_playerID].m_carries) g_view=v3rd; //Force 3rd if carries object

    //Pick up/down Object
    if (keys[69] && g_pushDelay<0) //e
    {
        if (Unit[g_playerID].m_carries)
        {
            putDownObject(); //Put down
            g_pushDelay=100;
        }
        else
        {
            g_fireWeapon=3;
            //if (pickUpObject()) g_pushDelay=100; //Pick up
        }
    }

    if (keys[87] || keys[83] || keys[65] || keys[68]) g_moving=true;
    else g_moving=false;

    if (keys[40] && g_headingXZ<170) g_headingXZ+=1;        //nerpil
    if (keys[38] && g_headingXZ>10)  g_headingXZ-=1;        //upppil
    if (keys[37]) g_headingY+=2;	                        //vänsterpil
    if (keys[39]) g_headingY-=2;                            //högerrpil

    //Fire weapon
    if (mouse[0] && g_fireDelay<0)
    {
        if (!Unit[g_playerID].m_carries && Unit[g_playerID].m_inHands==0 && Unit[g_playerID].m_weaponSwapProg==0)
        g_fireWeapon=1;//fireWeapon(0);
    }
    else if (mouse[1] && g_fireDelay<0)
    {
        if (!Unit[g_playerID].m_carries && Unit[g_playerID].m_weaponSwapProg==0) g_fireWeapon=2;//fireWeapon(1);
        else if (Unit[g_playerID].m_carries && Unit[g_playerID].m_currWeapon==0) g_fireWeapon=2;//fireWeapon(1);
    }
    if (Unit[g_playerID].m_inHands==0) //Only swap Weapon if empty Hands
    {
        //Change Weapon (keys)
        if (keys[49])
        {zoomingOff(); g_animation=10;g_shovelLoaded=false;}                                         //1 Shovel
        if (keys[50])
        {zoomingOff(); if (Unit[g_playerID].m_inventory[1]) {g_animation=11;g_shovelLoaded=false;}} //2 Revolver
        if (keys[51])
        {zoomingOff(); if (Unit[g_playerID].m_inventory[2]) {g_animation=12;g_shovelLoaded=false;}} //3 Uzi
        if (keys[52])
        {zoomingOff(); if (Unit[g_playerID].m_inventory[3]) {g_animation=13;g_shovelLoaded=false;}} //4 Sniper
        if (keys[53])
        {zoomingOff(); if (Unit[g_playerID].m_inventory[4]) {g_animation=14;g_shovelLoaded=false;}} //5 Rocket Launcher
        if (keys[54])
        {zoomingOff(); if (Unit[g_playerID].m_inventory[5]) {g_animation=15;g_shovelLoaded=false;}} //6 Radar

        //Change Weapon (wheel) not while zooming
        if (g_scroll>200 && !g_zooming) //Prev weapon
        {
            if (g_debugMode) Spawner[g_playerID].m_content+=1; //Add Powerup locally
            for (int i=1;i<10;i++)
            {
                if (Unit[g_playerID].m_currWeapon-i<0) break;
                if (Unit[g_playerID].m_inventory[Unit[g_playerID].m_currWeapon-i])
                {
                    g_animation=10+Unit[g_playerID].m_currWeapon-i;
                    g_shovelLoaded=false;
                    zoomingOff();
                    break;
                }
            }
        }
        if (g_scroll>100 && g_scroll<200 && !g_zooming) //Next weapon
        {
            if (g_debugMode) Spawner[g_playerID].m_content-=1; //Add Powerup locally
            for (int i=1;i<10;i++)
            {
                if (Unit[g_playerID].m_currWeapon+i>9) break;
                if (Unit[g_playerID].m_inventory[Unit[g_playerID].m_currWeapon+i])
                {
                    g_animation=10+Unit[g_playerID].m_currWeapon+i;
                    g_shovelLoaded=false;
                    zoomingOff();
                    break;
                }
            }
        }
    }
    g_scroll=0;

    if (keys[82]) g_groundLevel+=0.1;                       //r
    if (keys[70]) g_groundLevel-=0.1;                       //f

    //MOVEMENT
    //scale heading
    float elongfactor;
    float elong_xhed;
    float elong_zhed;
    if (g_xhed==0 || g_zhed==0) elongfactor=0;
    else elongfactor=(float)CycleTime/sqrt(g_xhed*g_xhed+g_zhed*g_zhed);
    elong_xhed=elongfactor*g_xhed;
    elong_zhed=elongfactor*g_zhed;

    //if (g_onGroundLevel)
    {
        float sensitivity=0.0015;
        if (keys[87]) {                      // w
            g_xacc+=elong_xhed*sensitivity;
            g_zacc+=elong_zhed*sensitivity;
        }
        if (keys[83]) {                      // s
            g_xacc+=elong_xhed*-sensitivity;
            g_zacc+=elong_zhed*-sensitivity;
        }
        if (keys[65]) {                      // a
            g_xacc+=elong_zhed*sensitivity;
            g_zacc+=elong_xhed*-sensitivity;
        }
        if (keys[68]) {                      // d
            g_xacc+=elong_zhed*-sensitivity;
            g_zacc+=elong_xhed*sensitivity;
        }
    }

    if ((keys[87] || keys[83]) && (keys[65] || keys[68])) //lower speed if heading 2 directions
    {
        g_xacc*=0.7;
        g_zacc*=0.7;
    }
    if (!g_onGroundLevel) //lower acc if in air
    {
        g_xacc*=0.1;
        g_zacc*=0.1;
    }
    if (Unit[g_playerID].m_carrySpawner) //lower acc if spawner is carried
    {
        g_xacc*=0.3;
        g_zacc*=0.3;
    }
    //if (g_view==vTop) {g_xacc=0; g_zacc=0;} //No movement during radarscan
    //Jump
    if (keys[32]) //space
    {
        if (g_onGroundLevel && !g_flyMode && !Unit[g_playerID].m_carries)
        {
            g_jumpState=1;
            if (g_jumpForce<1) g_jumpForce+=0.025;
        }
    }

    if (!keys[32] && g_jumpState==1) //Jump release, get airborn
    {
        g_makeSound=Sjump;
        g_jumpState=2;
        g_yacc=0.10+0.06*g_jumpForce;
        g_jumpForce=0;
        g_onGroundLevel=false;
    }
    //Slow down if in jumpState
    if (g_jumpState==1)
    {
        g_xacc*=0.05;
        g_zacc*=0.05;
    }
    //Slow down if zooming
    if (g_zooming)
    {
        g_xacc*=0.1;
        g_zacc*=0.1;
    }

    //add acc to vel
    g_xvel+=g_xacc;
    g_yvel+=g_yacc;
    g_zvel+=g_zacc;
    g_xacc=g_yacc=g_zacc=0;

    //Mouselook
    while (cursorX>292)
    {
        if (g_zooming) g_headingY-=0.05;
        else g_headingY-=0.1;
        cursorX--;
    }
    while (cursorX<292)
    {
        if (g_zooming) g_headingY+=0.05;
        else g_headingY+=0.1;
        cursorX++;
    }
    while (cursorY>210)
    {
        if (g_headingXZ<170)
        {
            if (g_zooming) g_headingXZ+=0.05;
            else g_headingXZ+=0.1;
        }
        cursorY--;
    }
    while (cursorY<210)
    {
        if (g_headingXZ>10 )
        {
            if (g_zooming) g_headingXZ-=0.05;
            else g_headingXZ-=0.1;
        }
        cursorY++;
    }
   // else {g_headingY+=0.3; g_headingXZ=90;}

    //Update Pos
    float txpos,typos,tzpos; //Temp pos and vel
    float txvel=2*g_xhed;
    float tyvel=2*g_yvel;
    float tzvel=2*g_zhed;
    bool colFront=false;
    bool colFrontLeft=false;
    bool colFrontRight=false;
    bool colBack=false;
    bool colLeft=false;
    bool colRight=false;
    int objectIDcol;
    //Normalize X,Z vectors
    float vec_lengthFB=sqrt((g_xhed)*(g_xhed)+(g_zhed)*(g_zhed));
    float vec_lengthLR=sqrt((g_xrig)*(g_xrig)+(g_zrig)*(g_zrig));

    //Check Front Collision
    if (!keys[83])
    {
        for (float step=1;step>0.1;step-=0.3) //Check 3 pos along vector
        {
            if (vec_lengthFB!=0)
            {
                g_debug1=txvel=g_xhed/vec_lengthFB*step;
                g_debug2=tzvel=g_zhed/vec_lengthFB*step;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvel=0; tzvel=step;}
                    else          {txvel=0; tzvel=-step;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvel=step; tzvel=0;}
                    else          {txvel=-step; tzvel=0;}
                }
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
            if (testCollision(txpos,typos,tzpos)!=0)
            {
                g_xvel*=0.95; g_yvel*=0.95; g_zvel*=0.95;
                txpos=g_xpos;typos=g_ypos-1.0;tzpos=g_zpos;
                txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
                if (testCollision(txpos,typos,tzpos)!=0) {colFront=true; break;} //Collision
            }
        }

        float txvelFront,tzvelFront,txvelRight,tzvelRight,txvelLeft,tzvelLeft;
        //Check Front Right Col
        if (vec_lengthFB!=0 && vec_lengthLR!=0)
        {
            txvelFront=g_xhed/vec_lengthFB;
            tzvelFront=g_zhed/vec_lengthFB;
            txvelRight=g_xrig/vec_lengthLR;
            tzvelRight=g_zrig/vec_lengthLR;
        }
        else
        {
            if (g_xhed==0)
            {
                if (g_zhed>0) {txvelFront=0; tzvelFront=1; txvelRight=-1; tzvelRight=0;}
                else          {txvelFront=0; tzvelFront=-1; txvelRight=1; tzvelRight=0;}
            }
            if (g_zhed==0)
            {
                if (g_xhed>0) {txvelFront=1; tzvelFront=0; txvelRight=0; tzvelRight=1;}
                else          {txvelFront=-1; tzvelFront=0; txvelRight=0; tzvelRight=-1;}
            }
        }
        txvel=(txvelFront+txvelRight)/2;
        tzvel=(tzvelFront+tzvelRight)/2;
        txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
        txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
        objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
        if (objectIDcol!=0) {colFrontRight=true;} //Collision

        //Check Front Left Col
        if (vec_lengthFB!=0 && vec_lengthLR!=0)
        {
            txvelFront=g_xhed/vec_lengthFB;
            tzvelFront=g_zhed/vec_lengthFB;
            txvelLeft=-g_xrig/vec_lengthLR;
            tzvelLeft=-g_zrig/vec_lengthLR;
        }
        else
        {
            if (g_xhed==0)
            {
                if (g_zhed>0) {txvelFront=0; tzvelFront=1; txvelLeft=1; tzvelLeft=0;}
                else          {txvelFront=0; tzvelFront=-1; txvelLeft=-1; tzvelLeft=0;}
            }
            if (g_zhed==0)
            {
                if (g_xhed>0) {txvelFront=1; tzvelFront=0; txvelLeft=0; tzvelLeft=-1;}
                else          {txvelFront=-1; tzvelFront=0; txvelLeft=0; tzvelLeft=1;}
            }
        }
        txvel=(txvelFront+txvelLeft)/2;
        tzvel=(tzvelFront+tzvelLeft)/2;
        txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
        txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
        objectIDcol=testCollision(txpos,typos,tzpos);
        if (objectIDcol!=0) {colFrontLeft=true;} //Collision
    }
    //Check Back Collision
    if (!keys[87])
    {
        for (float step=1;step>0.1;step-=0.3) //Check 3 pos along vector
        {
            if (vec_lengthFB!=0)
            {
                txvel=g_xhed/vec_lengthFB*step;
                tzvel=g_zhed/vec_lengthFB*step;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvel=0; tzvel=step;}
                    else          {txvel=0; tzvel=-step;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvel=step; tzvel=0;}
                    else          {txvel=-step; tzvel=0;}
                }
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos-=txvel;typos-=tyvel;tzpos-=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) {colBack=true; break;} //Collision
        }
    }
    //Check Right Collision
    if (!keys[65])
    {
        for (float step=1;step>0.1;step-=0.3) //Check 3 pos along vector
        {
            if (vec_lengthLR!=0)
            {
                txvel=g_xrig/vec_lengthLR*step;
                tzvel=g_zrig/vec_lengthLR*step;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvel=0; tzvel=step;}
                    else          {txvel=0; tzvel=-step;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvel=step; tzvel=0;}
                    else          {txvel=-step; tzvel=0;}
                }
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos+=txvel;typos+=tyvel;tzpos+=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) {colRight=true; break;} //Collision
        }
    }
    //Check Left Collision
    if (!keys[68])
    {
        for (float step=1;step>0.1;step-=0.3) //Check 3 pos along vector
        {
            if (vec_lengthLR!=0)
            {
                txvel=g_xrig/vec_lengthLR*step;
                tzvel=g_zrig/vec_lengthLR*step;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvel=0; tzvel=step;}
                    else          {txvel=0; tzvel=-step;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvel=step; tzvel=0;}
                    else          {txvel=-step; tzvel=0;}
                }
            }
            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            txpos-=txvel;typos-=tyvel;tzpos-=tzvel;
            objectIDcol=testCollision(txpos,typos,tzpos);  //Returns ID+/-1 of collided Spawner+ or Player-
            if (objectIDcol!=0) {colLeft=true; break;} //Collision
        }
    }

    if (!colFront && !colBack && !colRight && !colLeft) //New Pos OK
    {
        g_xpos=g_xpos+g_xvel;
        g_ypos=g_ypos+g_yvel;
        g_zpos=g_zpos+g_zvel;
        g_debug0=2;
    }
    else //New pos Not OK, check if side direction is OK
    {
        g_debug0=1;
        bool Xerr=false; bool Yerr=false; bool Zerr=false;
        float xadjust=0; float yadjust=0; float zadjust=0;

        if (colFrontRight && (!Xerr || !Yerr || !Zerr))
        {
            float txvelFront,tzvelFront,txvelRight,tzvelRight;
            //Check Front Right Col
            if (vec_lengthFB!=0 && vec_lengthLR!=0)
            {
                txvelFront=g_xhed/vec_lengthFB;
                tzvelFront=g_zhed/vec_lengthFB;
                txvelRight=g_xrig/vec_lengthLR;
                tzvelRight=g_zrig/vec_lengthLR;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvelFront=0; tzvelFront=1; txvelRight=-1; tzvelRight=0;}
                    else          {txvelFront=0; tzvelFront=-1; txvelRight=1; tzvelRight=0;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvelFront=1; tzvelFront=0; txvelRight=0; tzvelRight=1;}
                    else          {txvelFront=-1; tzvelFront=0; txvelRight=0; tzvelRight=-1;}
                }
            }
            txvel=(txvelFront+txvelRight)/2;
            tzvel=(tzvelFront+tzvelRight)/2;

            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/4;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
            if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/4;}
        }

        if (colFrontLeft && (!Xerr || !Yerr || !Zerr))
        {
            float txvelFront,tzvelFront,txvelLeft,tzvelLeft;
            //Check Front Left Col
            if (vec_lengthFB!=0 && vec_lengthLR!=0)
            {
                txvelFront=g_xhed/vec_lengthFB;
                tzvelFront=g_zhed/vec_lengthFB;
                txvelLeft=-g_xrig/vec_lengthLR;
                tzvelLeft=-g_zrig/vec_lengthLR;
            }
            else
            {
                if (g_xhed==0)
                {
                    if (g_zhed>0) {txvelFront=0; tzvelFront=1; txvelLeft=1; tzvelLeft=0;}
                    else          {txvelFront=0; tzvelFront=-1; txvelLeft=-1; tzvelLeft=0;}
                }
                if (g_zhed==0)
                {
                    if (g_xhed>0) {txvelFront=1; tzvelFront=0; txvelLeft=0; tzvelLeft=-1;}
                    else          {txvelFront=-1; tzvelFront=0; txvelLeft=0; tzvelLeft=1;}
                }
            }
            txvel=(txvelFront+txvelLeft)/2;
            tzvel=(tzvelFront+tzvelLeft)/2;

            txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
            if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/4;}
            if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
            if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/4;}
        }

        if (colFront && (!Xerr || !Yerr || !Zerr))
        {
            for (float step=1;step>0.1;step-=0.3)
            {
                if (vec_lengthFB!=0)
                {
                    txvel=g_xhed/vec_lengthFB*step;
                    tzvel=g_zhed/vec_lengthFB*step;
                }
                else
                {
                    if (g_xhed==0)
                    {
                        if (g_zhed>0) {txvel=0; tzvel=step;}
                        else          {txvel=0; tzvel=-step;}
                    }
                    if (g_zhed==0)
                    {
                        if (g_xhed>0) {txvel=step; tzvel=0;}
                        else          {txvel=-step; tzvel=0;}
                    }
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/4;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/4;}
                if (Xerr && Yerr && Zerr) break;
            }
        }
        if (colBack && (!Xerr || !Yerr || !Zerr))
        {
            for (float step=1;step>0.1;step-=0.3)
            {
                if (vec_lengthFB!=0)
                {
                    txvel=-g_xhed/vec_lengthFB*step;
                    tzvel=-g_zhed/vec_lengthFB*step;
                }
                else
                {
                    if (g_xhed==0)
                    {
                        if (g_zhed>0) {txvel=0; tzvel=step;}
                        else          {txvel=0; tzvel=-step;}
                    }
                    if (g_zhed==0)
                    {
                        if (g_xhed>0) {txvel=step; tzvel=0;}
                        else          {txvel=-step; tzvel=0;}
                    }
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/2;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/2;}
                if (Xerr && Yerr && Zerr) break;
            }
        }
        if (colRight && (!Xerr || !Yerr || !Zerr) && false) //Behövs ej?
        {
            for (float step=1;step>0.1;step-=0.3)
            {
                if (vec_lengthLR!=0)
                {
                    txvel=g_xrig/vec_lengthLR*step;
                    tzvel=g_zrig/vec_lengthLR*step;
                }
                else
                {
                    if (g_xhed==0)
                    {
                        if (g_zhed>0) {txvel=0; tzvel=step;}
                        else          {txvel=0; tzvel=-step;}
                    }
                    if (g_zhed==0)
                    {
                        if (g_xhed>0) {txvel=step; tzvel=0;}
                        else          {txvel=-step; tzvel=0;}
                    }
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/2;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/2;}
                if (Xerr && Yerr && Zerr) break;
            }
        }
        if (colLeft && (!Xerr || !Yerr || !Zerr) && false) //Behövs ej?
        {
            for (float step=1;step>0.1;step-=0.3)
            {
                if (vec_lengthLR!=0)
                {
                    txvel=-g_xrig/vec_lengthLR*step;
                    tzvel=-g_zrig/vec_lengthLR*step;
                }
                else
                {
                    if (g_xhed==0)
                    {
                        if (g_zhed>0) {txvel=0; tzvel=step;}
                        else          {txvel=0; tzvel=-step;}
                    }
                    if (g_zhed==0)
                    {
                        if (g_xhed>0) {txvel=step; tzvel=0;}
                        else          {txvel=-step; tzvel=0;}
                    }
                }
                txpos=g_xpos;typos=g_ypos-1.5;tzpos=g_zpos;
                if (testCollision(txpos+txvel,typos,tzpos)!=0) {g_xvel=0; Xerr=true; xadjust-=txvel/2;}
                if (testCollision(txpos,typos+tyvel,tzpos)!=0) {g_yvel=0; Yerr=true; yadjust+=0.01;}
                if (testCollision(txpos,typos,tzpos+tzvel)!=0) {g_zvel=0; Zerr=true; zadjust-=tzvel/2;}
                if (Xerr && Yerr && Zerr) break;
            }
        }
        //Update pos values
        g_xpos=g_xpos+g_xvel+xadjust*0.02;
        g_ypos=g_ypos+g_yvel+yadjust*0.5;
        g_zpos=g_zpos+g_zvel+zadjust*0.02;
        //Climbing
     //   if (keys[87] && testCollision(g_xpos+g_xhed,g_ypos+g_yhed+1,g_zpos+g_zhed)==0) g_ypos+=0.02*CycleTime;
    }
    if (testCollision(g_xpos,g_ypos-1.5,g_zpos)!=0) adjustUnitPos(); //If inside object, move Unit

    if (g_xpos<5) g_xpos=5; //XXXXXXXXXXXXXXXXXXXXX
    if (g_zpos<5) g_zpos=5; //XXXXXXXXXXXXXXXXXXXXX

    g_avgvel=fabs(g_xvel)+fabs(g_zvel);
    if (g_avgvel>0.140) g_avgvel=0.140;

    //Gravity
    if (!g_flyMode)
    {
        float mobileObjectHeight=getMobileObjectHeight(g_xpos,g_zpos);
        g_groundLevel=World.getAverageHeight(g_xpos,g_zpos)+2+ //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                      World.getObjectHeight((int)g_xpos,(int)g_zpos)+
                      mobileObjectHeight;
        if (g_ypos<g_groundLevel)
        {
            g_ypos=g_groundLevel;
            g_onGroundLevel=true;
            g_yvel=0;
            if (g_jumpState==2) {g_jumpState=0;g_makeSound=Sland;}
        }
        else if (g_ypos>g_groundLevel)
        {
            g_onGroundLevel=false;
            g_yvel-=0.0005*CycleTime;
        }
 /*       if (g_ypos<g_height[(int)(g_xpos+g_xvel*50)][(int)(g_zpos+g_zvel*50)])
        {
            g_xvel*=0.1;
            g_zvel*=0.1;
        }*/
    }
    else g_ypos=g_groundLevel;

    //markfriktion
    if (g_jumpState==1) //för hopp
    {
        //sänk hast, luftmotstånd, procentuellt
        if (g_xvel>0.001 || g_xvel<-0.001) g_xvel*=0.97; else g_xvel=0;   //CycleTime för att bromsa relativt
        //if (ypos>0) yvel-=0.01; else yvel=0;
        if (g_zvel>0.001 || g_zvel<-0.001) g_zvel*=0.97; else g_zvel=0;
    }
    else if (g_onGroundLevel)
    {
        //sänk hast, luftmotstånd, procentuellt
        if (g_xvel>0.001 || g_xvel<-0.001) g_xvel*=0.9; else g_xvel=0;   //CycleTime för att bromsa relativt
        //if (ypos>0) yvel-=0.01; else yvel=0;
        if (g_zvel>0.001 || g_zvel<-0.001) g_zvel*=0.9; else g_zvel=0;
    }
    else if (!g_onGroundLevel) //Luftmotstånd
    {
        //sänk hast, luftmotstånd, procentuellt
        if (g_xvel>0.001 || g_xvel<-0.001) g_xvel*=0.99; else g_xvel=0;   //CycleTime för att bromsa relativt
        //if (ypos>0) yvel-=0.01; else yvel=0;
        if (g_zvel>0.001 || g_zvel<-0.001) g_zvel*=0.99; else g_zvel=0;
    }

/*       //sänk hast, luftmotstånd, linjärt
       if       (g_xvel>0.0001)  g_xvel-=elong_xhed*0.001;
       else if (g_xvel<-0.0001)  g_xvel+=elong_xhed*0.001;
       else g_xvel=0;

       if       (g_zvel>0.0001)  g_zvel-=elong_zhed*0.001;
       else if (g_zvel<-0.0001)  g_zvel+=elong_zhed*0.001;
       else g_zvel=0;
    }
/*    else //luftfriktion & fall
    {
//       if (g_xvel>0.0001) g_xvel-=0.0001; else if (g_xvel<-0.0001) g_xvel+=0.0001; else g_xvel=0;
//       if (g_zvel>0.0001) g_zvel-=0.0001; else if (g_zvel<-0.0001) g_zvel+=0.0001; else g_zvel=0;
       g_yvel-=0.001;
    }*/
}

void drawAntennas(void)
{
    for (int spawn=0;spawn<g_maxPlayers;spawn++) //Players Online and active
    {
        if (!Spawner[spawn].m_active || !Unit[spawn].m_spawnerOnline) continue; //No line if Spawner is Offline
        int numOfBuildings=0; //Counter
        int buildingID[g_maxBuildings]; //ID storage
        float pos[g_maxBuildings][3]; //x,y,z cord. for every building
        for (int building=0;building<g_maxBuildings;building++) //Check all buildings
        {
            if (Building[building].m_active)
            {
                if (Building[building].m_owner==spawn) //Current player is owner
                {
                    buildingID[numOfBuildings]=building; //Store ID
                    if (Building[building].m_type==2) pos[numOfBuildings][0]=Building[building].m_pos[0]+1; //Antenna pos depends on building type
                    else pos[numOfBuildings][0]=Building[building].m_pos[0]+3;
                    pos[numOfBuildings][1]=Building[building].m_pos[1];
                    pos[numOfBuildings][2]=Building[building].m_pos[2];
                    numOfBuildings++; //Increment counter
                }
            }
        }
        if (numOfBuildings<1) continue; //Player had no buildings
        //Spawner pos
        pos[numOfBuildings][0]=Spawner[spawn].m_pos[0];
        pos[numOfBuildings][1]=Spawner[spawn].m_pos[1];
        pos[numOfBuildings][2]=Spawner[spawn].m_pos[2];
        numOfBuildings++;

        //Find Highest building
        float highest=pos[0][1]; //Highest building height, temp spawner
        int highestID=0; //Highest buildingID
        for (int i=0;i<numOfBuildings;i++) //Get highest building
        {
            if (pos[i][1]>highest)
            {
                highest=pos[i][1];
                highestID=i;
            }
        }
        highest+=4; //Minimum height of antenna = 3
        for (int i=0;i<numOfBuildings-1;i++) //Calculate Height
        {
            //Get vectors
            float xdiff=pos[numOfBuildings-1][0]-pos[i][0]; //Difference in x-axis
            float zdiff=pos[numOfBuildings-1][2]-pos[i][2]; //Difference in z-axis
            int xpart=abs((int)xdiff);
            int zpart=abs((int)zdiff);
            int part=xpart; if (zpart>xpart) part=zpart; //Highest length decides how many steps in terrain col. loop
            float xdir=xdiff/((float)part*2.0);
            float zdir=zdiff/((float)part*2.0);
            //Test if above terrain
            float x=pos[i][0];
            float z=pos[i][2];
            for (int j=0;j<part*2;j++) //Loop through all steps in path to spawner
            {
                x+=xdir;
                z+=zdir;
                while (highest<World.getHeight((int)x,(int)z)+2) //If below terrain increase height
                {
                    highest+=1;
                }
            }
        }
        //Draw Antennas
        switch (spawn) //Set color
        {
            case 0: glColor4f(0.5,0.5,0.9,1);break;
            case 1: glColor4f(0.3,0.7,0.3,1);break;
            case 2: glColor4f(0.3,0.3,0.7,1);break;
            case 3: glColor4f(0.7,0.1,0.7,1);break;
            case 4: glColor4f(0.4,0.3,0.3,1);break;
            case 5: glColor4f(0.8,0.4,0.3,1);break;
            case 6: glColor4f(0.2,0.8,0.8,1);break;
            case 7: glColor4f(0.9,0.8,0.8,1);break;
        }
        glBegin(GL_LINES);
        for (int i=0;i<numOfBuildings-1;i++)
        {
            glVertex4f(pos[i][0],pos[i][1]+2,pos[i][2],1);
            glVertex4f(pos[i][0],highest,pos[i][2],1);
            glVertex4f(pos[i][0],highest,pos[i][2],1); //Draw Antenna Connections
            glVertex4f(pos[numOfBuildings-1][0]+0.1,highest,pos[numOfBuildings-1][2]+0.1,1);
        }
        //Spawner
        int i=numOfBuildings-1;
        if (!Spawner[spawn].m_moving) glVertex4f(pos[i][0]+0.1,pos[i][1]+1.9,pos[i][2]+0.1,1);
        else glVertex4f(pos[i][0],pos[i][1]+0.5,pos[i][2],1);
        glVertex4f(pos[i][0]+0.1,highest,pos[i][2]+0.1,1);
        glEnd();
        //Draw TopBox
    }
}

void emptyHands(int unit)
{
     Logfile.log(g_playerNames[unit],"Tried to empty hands");
     Logfile.log(" Heading value:",Unit[unit].m_xdir+Unit[unit].m_ydir+Unit[unit].m_zdir);
     //Put down what in hands
     int range=20;
     if (interactionHitTest(unit,range)==0) //0=hit air
     {
         Logfile.log(" no collision detected, no placement");
         return;
     }
     int x=g_tempPos[0];
     int z=g_tempPos[2];
     if (World.getObjectHeight(x,z)!=0) //Position must be free of buildings
     {
         Logfile.log(" position contains building, no placement");
     }
     if (getMobileObjectHeight(g_tempPos[0],g_tempPos[2])) //and free of objects
     {
         Logfile.log(" position contains mobile object, no placement");
     }

     //What was put down
     int object=Unit[unit].m_inHands;
     int ID=Unit[unit].m_inHandsID;
     switch (object)
     {
         case 1:{//BuildBox
                 Buildbox[ID].m_moving=false;
                 Buildbox[ID].m_inHands=false;
                 Buildbox[ID].m_pos[0]=x;
                 Buildbox[ID].m_pos[1]=World.getHeight(x,z);
                 Buildbox[ID].m_pos[2]=z;
                 Logfile.log(g_playerNames[unit],"Placed BUILDBOX id:",ID);
                }break;
         case 2:{//BuildBlock
                 Buildblock[ID].m_moving=false;
                 Buildblock[ID].m_inHands=false;
                 Buildblock[ID].m_pos[0]=x;
                 Buildblock[ID].m_pos[1]=World.getHeight(x,z);
                 Buildblock[ID].m_pos[2]=z;
                 World.setBuildblockType(x,z,Buildblock[ID].m_type);
                 Logfile.log(g_playerNames[unit],"Placed BUILDBLOCK id:",ID);
                }break;
         case 4:{//Wall
                 int emptyID=-1; //Find empty Wall
                 for (int ID=0;ID<g_maxWalls;ID++)
                 {
                     if (!Wall[ID].m_active) {emptyID=ID; break;}
                 }
                 if (emptyID!=-1 && emptyID<g_maxWalls) //Give to Unit
                 {
                     Wall[emptyID].newWall(x,World.getHeight(x,z),z);
                     World.setObjectHeight(x,z,4);
                     Logfile.log(g_playerNames[unit],"Placed WALL id:",emptyID);
                 }
                }break;
         case 5:{//Terrain
                 heightChange(-1,unit);
                 Unit[unit].m_loadedShovel=false;
                 makeSound(Sundig,unit,1,2);
                 Logfile.log(g_playerNames[unit],"Placed TERRAIN");
                }break;
         case 6:{//Mine
                 int emptyID=-1; //Find empty Mine
                 for (int ID=0;ID<g_maxMines;ID++)
                 {
                     if (!Mine[ID].m_active) {emptyID=ID; break;}
                 }
                 if (emptyID!=-1 && emptyID<g_maxMines)
                 {
                     Mine[emptyID].newMine(x,World.getHeight(x,z),z);
                     World.setObjectHeight(x,z,0.1);
                     Logfile.log(g_playerNames[unit],"Placed MINE id:",emptyID);
                 }
                }break;
         case 7:{//Turret
                 int emptyID=-1; //Find empty Turret
                 for (int ID=0;ID<g_maxTurrets;ID++)
                 {
                     if (!Turret[ID].m_active) {emptyID=ID; break;}
                 }
                 if (emptyID!=-1)
                 {
                     int type=Unit[unit].m_inHandsID;
                     int owner=unit;
                     if (g_gameMode==2) owner=Unit[unit].m_team+10;
                     Turret[emptyID].newTurret(x,World.getHeight(x,z),z,owner,type);
                     World.setObjectHeight(x,z,2.5);
                     Logfile.log(g_playerNames[unit],"Placed TURRET id:",emptyID);
                 }
                }break;
         case 8:{//Outpost
                 int emptyID=-1; //Find empty Turret
                 for (int ID=0;ID<g_maxOutposts;ID++)
                 {
                     if (!Outpost[ID].m_active) {emptyID=ID; break;}
                 }
                 if (emptyID!=-1)
                 {
                     int radarID=Unit[unit].m_inHandsID;
                     Outpost[emptyID].newOutpost(x,World.getHeight(x,z),z,radarID);
                     Radar[radarID].newOutpost(emptyID);
                     Logfile.log(g_playerNames[unit],"Placed OUTPOST id:",emptyID);
                 }
                }break;
     }
     Unit[unit].m_inHands=0;
     makeSound(SplaceObject,unit,1,2);
}

void heightChange(int dir,int unit)
{
    int x=(int)Unit[unit].m_xpos; int z=(int)Unit[unit].m_zpos;
    if (Unit[unit].m_zhed<140)
    {
        float vec_length=sqrt((Unit[unit].m_xdir)*(Unit[unit].m_xdir)+(Unit[unit].m_zdir)*(Unit[unit].m_zdir));
        if (vec_length!=0)
        {
            float xdir=Unit[unit].m_xdir/vec_length;
            float zdir=Unit[unit].m_zdir/vec_length;
            if (xdir>0.5) x++;
            else if (xdir<-0.5) x--;
            if (zdir>0.5) z++;
            else if (zdir<-0.5) z--;
        }
    }
    lowerGround(x,z,dir);
    lowerGround(x-1,z-1,dir*0.8);
    lowerGround(x-1,z,dir*0.8);
    lowerGround(x-1,z+1,dir*0.8);
    lowerGround(x,z-1,dir*0.8);
    lowerGround(x,z+1,dir*0.8);
    lowerGround(x+1,z-1,dir*0.8);
    lowerGround(x+1,z,dir*0.8);
    lowerGround(x+1,z+1,dir*0.8);
    lowerGround(x-2,z-1,dir*0.5);
    lowerGround(x-2,z,dir*0.5);
    lowerGround(x-2,z+1,dir*0.5);
    lowerGround(x+2,z-1,dir*0.5);
    lowerGround(x+2,z,dir*0.5);
    lowerGround(x+2,z+1,dir*0.5);
    lowerGround(x-1,z+2,dir*0.5);
    lowerGround(x,z+2,dir*0.5);
    lowerGround(x+1,z+2,dir*0.5);
    lowerGround(x-1,z-2,dir*0.5);
    lowerGround(x,z-2,dir*0.5);
    lowerGround(x+1,z-2,dir*0.5);
}

void impact(int type,int unit)
{
    enum objectTypes{oTerrain=1,oUnit,oSpawner,oTurret,oMine,oWall,oBuilding,oHeli,oSpeed,oTank,
                     oProj,oBBox,oBBlock,oPowerup,oRadar,oOutpost};

    int currWeapon;
    if (unit<100) //Unit shooting
    {
        currWeapon=Unit[unit].m_currWeapon;
        if (Unit[unit].m_inTransportType==3) currWeapon=10;
    }
    else //Turret shooting
    {
        currWeapon=100;
    }
    switch (type)
    {
        case oTerrain:{//Terrain
                if (currWeapon==0)
                {
                    if (Unit[unit].m_inHands==0) //Pick up Terrain
                    {
                        heightChange(1,unit);
                        Unit[unit].m_inHands=5;
                        Unit[unit].m_loadedShovel=true;
                        Unit[unit].m_ybodyhed=Unit[unit].m_yheadhed;
                        makeSound(Sdig,unit,1,type);
                    }
                }
                else
                {
                    explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,20);
                }
               }break;
        case oUnit:{//Unit
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(int(Unit[g_tempID].m_xpos+0.5),int(Unit[g_tempID].m_zpos+0.5),Unit[g_tempID].m_ypos-0.5,2,10,g_tempID);
               }break;
        case oSpawner:{//Spawner
                if (Spawner[g_tempID].m_open)
                {
                    makeSound(ShitSpawn,g_tempID,1,type);
                    doDamage(unit,g_tempID,type);
                    explosionAt(int(Spawner[g_tempID].m_pos[0]+0.5),int(Spawner[g_tempID].m_pos[2]+0.5),Spawner[g_tempID].m_pos[1]+1,2,10,g_tempID);
                }
                else makeSound(ShitSpawnShield,g_tempID,0.5,type);
               }break;
        case oTurret:{//Turret
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oMine:{//Mine
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oWall:{//Wall
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oBuilding:{//Building
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oHeli:{//Heli
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oSpeed:{//Speed
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oTank:{//Tank
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oProj:{//Proj
                makeSound(ShitUnit,g_tempID,1,type);
                Proj[g_tempID].m_active=false;
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oBBox:{//BuildBox
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oBBlock:{//BuildBlock
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oPowerup:{//Powerup
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oRadar:{//Radar
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
        case oOutpost:{//Outpost
                makeSound(ShitUnit,g_tempID,1,type);
                doDamage(unit,g_tempID,type);
                explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],2,10,9);
               }break;
    }
}

void makeSound(int sound,int ID,float volume,int source)
{
    switch (source)
    {
        case  0:{//Error

                }break;
        case  1:{//Terrain

                }break;
        case  2:{//Unit
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Unit[ID].m_xpos,Unit[ID].m_ypos,Unit[ID].m_zpos,
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  3:{//Spawner
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Spawner[ID].m_pos[0],Spawner[ID].m_pos[1],Spawner[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  4:{//Turret
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Turret[ID].m_pos[0],Turret[ID].m_pos[1],Turret[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  5:{//Mine
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Mine[ID].m_pos[0],Mine[ID].m_pos[1],Mine[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  6:{//Wall
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Wall[ID].m_pos[0],Wall[ID].m_pos[1],Wall[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  7:{//Building
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Building[ID].m_pos[0],Building[ID].m_pos[1],Building[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  8:{//Heli
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Heli[ID].m_pos[0],Heli[ID].m_pos[1],Heli[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case  9:{//Speed
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Speed[ID].m_pos[0],Speed[ID].m_pos[1],Speed[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 10:{//Tank
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Tank[ID].m_pos[0],Tank[ID].m_pos[1],Tank[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 11:{//Proj
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Proj[ID].m_xpos,Proj[ID].m_ypos,Proj[ID].m_zpos,
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 12:{//BBox
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Buildbox[ID].m_pos[0],Buildbox[ID].m_pos[1],Buildbox[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 13:{//BBlock
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Buildblock[ID].m_pos[0],Buildblock[ID].m_pos[1],Buildblock[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 14:{//Powerup
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Powerup[ID].m_pos[0],Powerup[ID].m_pos[1],Powerup[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;
        case 15:{//Radar
                 float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                               g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                               Radar[ID].m_pos[0],Radar[ID].m_pos[1],Radar[ID].m_pos[2],
                               0,0,0,   1,volume,0};
                 Sound.playWAVE(sound,data);
                }break;

    }
/*
    if (unit<100) //Unit
    {
        float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                      g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                      Unit[ID].m_xpos,Unit[ID].m_ypos,Unit[ID].m_zpos,
                      0,0,0,   1,volume,0};
        Sound.playWAVE(sound,data);
    }
    else //Turret
    {
        float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                      g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                      Turret[unit-100].m_pos[0],Turret[unit-100].m_pos[1],Turret[unit-100].m_pos[2],
                      0,0,0,   1,volume,0};
        Sound.playWAVE(sound,data);
    }
*/
}

void genSomeStartObjects(void)
{
    //Powerups
    for (int i=0;i<100;i++)
    {
        int nextPowerupID=-1;
        for (int powID=0;powID<g_maxNumOfPowerups;powID++)
        {
            if (!Powerup[powID].m_active)
            {
                nextPowerupID=powID;
                break;
            }
        }
        if (nextPowerupID!=-1)
        {
            int x=rand()%(g_terrainWidth-100)+50;
            int z=rand()%(g_terrainLength-100)+50;
            if (World.getObjectHeight(x,z)!=0) continue;
            g_numOfPowerups++;
            Powerup[nextPowerupID].newPowerup(x,World.getHeight(int(x),int(z)),z);
        }
    }
}

void lowerGround(int x,int z,float amount)
{
    if (World.getObjectHeight(x,z)!=0) return; //Height cant be altered under buildings
    if (getMobileObjectHeight(float(x)+0.5,float(z)+0.5)!=0) {cout<<"MOB OBJ\n";return;} //Height cant be altered under objects

    float height=World.getHeight(x,z);
    //if (g_terrTex[x][z]!=0) amount*=0.5; //Solid Texture
    //if (height<0) amount*=0.3; //Low ground
    //if (height<-20) amount*=0.1; //Much low Ground
    World.setHeight(x,z,height-amount);
}

void displayPlayerData(void)
{
glPushMatrix();
    //Background
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_textureID[3]);
    glTranslatef(-2.6,1.9,-5);
    glColor4f(1.0,1.0,1.0,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,1);
    glVertex4f(0.0,0.0,0.0,1);
    glTexCoord2f(0,0);
    glVertex4f(0.0,-3.9,0.0,1);
    glTexCoord2f(1,0);
    glVertex4f(5.2,-3.9,0.0,1);
    glTexCoord2f(1,1);
    glVertex4f(5.2,0.0,0.0,1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //Show Buildversion
    glPushMatrix();
    glTranslatef(-18,-26.6,-60);
    drawTal(g_buildversion);
    glPopMatrix();
    //Show Names
    glPushMatrix();
        glColor4f(1,1,1,1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glScalef(0.05,0.05,0.05);
        glTranslatef(24,-12,0);
        int i=0;
        glPushMatrix();
            glRotatef(45,0,0,1);
            glVertexPointer(3, GL_FLOAT, 0, text20VertexArray);
            glDrawArrays(GL_LINES, 0, text20v/3);
        glPopMatrix();
        i++;
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text21VertexArray);
                glDrawArrays(GL_LINES, 0, text21v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text22VertexArray);
                glDrawArrays(GL_LINES, 0, text22v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text23VertexArray);
                glDrawArrays(GL_LINES, 0, text23v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text24VertexArray);
                glDrawArrays(GL_LINES, 0, text24v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text25VertexArray);
                glDrawArrays(GL_LINES, 0, text25v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text26VertexArray);
                glDrawArrays(GL_LINES, 0, text26v/3);
            glPopMatrix();
            i++;
        }
        glTranslatef(10,0,0);
        if (g_allPlayerLobbyData[i][0]!=0)
        {
            glPushMatrix();
                glRotatef(45,0,0,1);
                glVertexPointer(3, GL_FLOAT, 0, text27VertexArray);
                glDrawArrays(GL_LINES, 0, text27v/3);
            glPopMatrix();
            i++;
        }
        glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    //Show Data
    glPushMatrix();
        glScalef(0.1,0.1,0.1);
        glTranslatef(9,-9.2,0);
        //ID
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(ID);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //Online
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Unit[ID].m_online);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //SpawnerOnline
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Unit[ID].m_spawnerOnline);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //Team
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Unit[ID].m_team);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //Spawner
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Spawner[ID].m_active);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //FireCount
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Unit[ID].m_fireCount);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //Unit HitCount
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Unit[ID].m_hitCount);
            glTranslatef(5,0,0);
        }
        glTranslatef(-40,-2.5,0);
        //Spawner HitCount
        for (int ID=0;ID<g_maxPlayers;ID++)
        {
            drawTal(Spawner[ID].m_hitCount);
            glTranslatef(5,0,0);
        }
    glPopMatrix();

glPopMatrix();
}

void setBuildingObjectHeight(int xpos,int zpos,int type)
{
    switch (type)
    {
        case 1:{//Wass
                for (int x=xpos+1;x<xpos+3;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,2);
                }
               }break;
        case 2:{//Tass
                for (int x=xpos;x<xpos+4;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,2);
                }
               }break;
        case 3:{//Dass
                for (int x=xpos+1;x<xpos+3;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,2);
                }
               }break;
        case 6:{//TurrExp
                for (int x=xpos+1;x<xpos+3;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,2);
                }
               }break;
        case 7:{//Radar.Exp
                for (int x=xpos;x<xpos+4;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,3);
                }
               }break;
    }
}

bool completeBuildingCheck(int xstart,int zstart,int type,int pos[2])
{
    //if (xstart<20 || xstart>g_terrainWidth-20 || zstart<20 || zstart>g_terrainLength-20) return false; //Close to Borders
    for (int x=xstart-5;x<xstart+1;x++)
    for (int z=zstart-5;z<zstart+1;z++)
    {
        if (World.getBuildblockType(x,z)    ==type && World.getBuildblockType(x+1,z)  ==type &&
            World.getBuildblockType(x+2,z)  ==type && World.getBuildblockType(x+3,z)  ==type &&
            World.getBuildblockType(x,z+1)  ==type && World.getBuildblockType(x+1,z+1)==type &&
            World.getBuildblockType(x+2,z+1)==type && World.getBuildblockType(x+3,z+1)==type &&
            World.getBuildblockType(x,z+2)  ==type && World.getBuildblockType(x+1,z+2)==type &&
            World.getBuildblockType(x+2,z+2)==type && World.getBuildblockType(x+3,z+2)==type &&
            World.getBuildblockType(x,z+3)  ==type && World.getBuildblockType(x+1,z+3)==type &&
            World.getBuildblockType(x+2,z+3)==type && World.getBuildblockType(x+3,z+3)==type)
        {
            //Check height
            float height=World.getHeight(x,z);
            float tolerance=1;
            for (int xh=x;xh<x+4;xh++)
            for (int zh=z;zh<z+4;zh++)
            {
                if (height>World.getHeight(xh,zh)+tolerance || height<World.getHeight(xh,zh)-tolerance)
                {
                    return 0;
                }
            }
            //Flatten
            for (int xh=x-1;xh<x+5;xh++)
            for (int zh=z-1;zh<z+5;zh++)
            {
                World.setHeight(xh,zh,height);
            }
            pos[0]=x;
            pos[1]=z;
            return true;
        }
    }
    return false;
}

int objectBehindTest(int info,int unitID)
{
    float x,y,z;
    for (int i=1;i<10;i++)
    {
        x=Unit[unitID].m_xpos-Unit[unitID].m_xdir*(float)i/4;
        y=Unit[unitID].m_ypos-1.2;
        z=Unit[unitID].m_zpos-Unit[unitID].m_zdir*(float)i/4;

        //Check Spawners
        for (int spawner=0;spawner<g_maxPlayers;spawner++)
        {
            if (Unit[spawner].m_spawnerOnline)
            {
                if (Spawner[spawner].spawnerCollision(x,y,z))
                {
                    g_tempID=spawner;
                    return 1;
                }
            }
        }
        //Check BBox
        for (int BBox=0;BBox<g_maxBuildboxes;BBox++)
        {
            if (Buildbox[BBox].m_active)
            {
                if (Buildbox[BBox].buildboxBulletHitTest(x,y,z))
                {
                    g_tempID=BBox;
                    return 2;
                }
            }
        }
        //Check BBlock
        for (int BBlock=0;BBlock<g_maxBuildblocks;BBlock++)
        {
            if (Buildblock[BBlock].m_active)
            {
                if (Buildblock[BBlock].buildblockBulletHitTest(x,y,z))
                {
                    g_tempID=BBlock;
                    return 3;
                }
            }
        }
        //Check Powerup
        for (int pow=0;pow<g_maxNumOfPowerups;pow++)
        {
            if (Powerup[pow].m_active)
            {
                if (Powerup[pow].powerupBulletHitTest(x,y,z))
                {
                    g_tempID=pow;
                    return 4;
                }
            }
        }
        //Check Outpost
        for (int outpost=0;outpost<g_maxOutposts;outpost++)
        {
            if (Outpost[outpost].m_active)
            {
                if (Outpost[outpost].outpostBulletHitTest(x,y,z))
                {
                    g_tempID=outpost;
                    return 5;
                }
            }
        }
    }
    return -1;
}

bool pickUpObject(void)
{
    int objectID=objectBehindTest(0,g_playerID);
    if (objectID!=-1)
    {
        //(1)Spawner,(2)BBox,(3)BBlock,(4)Powerup,(5)Outpost
        g_action=1000+objectID;
        return true;
    }
    return false;
}

void putDownObject(void)
{
    //Can only place on free ground
    if (World.getObjectHeight((int)Unit[g_playerID].m_xpos,(int)Unit[g_playerID].m_zpos)!=0) return;
    //Spawner
    if (Unit[g_playerID].m_carrySpawner)
    {
        g_action=-10;
        g_ypos+=3;
        return;
    }
    //BBox
    if (Unit[g_playerID].m_carryBBox)
    {
        g_action=-20;
        return;
    }
    //BBlock
    if (Unit[g_playerID].m_carryBBlock)
    {
        g_action=-21;
        return;
    }
    //Powerup
    if (Unit[g_playerID].m_carryPow)
    {
        g_action=-22;
        return;
    }
    //-23(Heli)-24(Speed)-25(tank)
    //Outpost
    if (Unit[g_playerID].m_carryType=5)
    {
        g_action=-26;
        return;
    }
}

void interaction(int range,int unit)
{
    if (Unit[unit].m_inHands!=0) //Put down what in hands
    {
        emptyHands(unit);
        return;
    }

    range=10; //Shorten range to interact than to emptyHands
    int objectType=interactionHitTest(unit,range);
    if (objectType==0) return; //0=hit air

    if (objectType==3 && Unit[unit].m_carries) //Deliver something to Spawner
    {
        int spawnID=g_tempID;

        if (Unit[unit].m_carryPow) //Deliver Powerup
        {
            Logfile.log(g_playerNames[unit],"delivered POWERUP to SPAWNER id:",spawnID);
            Unit[unit].m_carries=false;
            Unit[unit].m_carryPow=false;
            int powID=Unit[unit].m_carryID;;
            Powerup[powID].m_active=false;
            Spawner[spawnID].m_content+=1;
            g_numOfPowerups--;
            if (unit==g_playerID) {g_view=vFPS;}
            if (g_gameMode==2)
            {
                for (int j=0;j<g_maxPlayers;j++)
                {
                    if (Unit[j].m_online)
                    {
                        if (Unit[spawnID].m_team==Unit[j].m_team)
                        {
                            Spawner[j].m_content+=1;
                        }
                    }
                }
            }
            makeSound(SpowerupDelivery,unit,1,2);
            return;
        }
        else if (Unit[unit].m_carryBBox) //Deliver empty BBox
        {
            Logfile.log(g_playerNames[unit],"delivered empty BUILDBOX to SPAWNER id:",spawnID);
            Unit[unit].m_carries=false;
            Unit[unit].m_carryBBox=false;
            int BBoxID=Unit[unit].m_carryID;
            Buildbox[BBoxID].m_active=false;
            Spawner[spawnID].m_content+=1;
            if (unit==g_playerID) {g_view=vFPS;}
            if (g_gameMode==2)
            {
                for (int j=0;j<g_maxPlayers;j++)
                {
                    if (Unit[j].m_online)
                    {
                        if (Unit[spawnID].m_team==Unit[j].m_team)
                        {
                            Spawner[j].m_content+=1;
                        }
                    }
                }
            }
            makeSound(SpowerupDelivery,unit,1,2);
            return;
        }
    }

    if (Unit[unit].m_inHands==0) //Interaction
    {
        if (objectType==12) //Buildbox (pick up buildblock maybe?)
        {
            int boxID=g_tempID;

            if (Buildbox[boxID].m_content>0)
            {
                Logfile.log(g_playerNames[unit],"interacted with BUILDBOX id:",boxID);
                int type=Buildbox[boxID].m_type;
                if (type==1 || type==2 || type==3 || type==6 || type==7) //Building parts
                {
                    //Find empty Buildblock
                    int emptyID=-1;
                    for (int ID=0;ID<g_maxBuildblocks;ID++)
                    {
                        if (!Buildblock[ID].m_active) {emptyID=ID; break;}
                    }
                    if (emptyID!=-1) //Give to Unit
                    {
                        Buildblock[emptyID].newBuildblock(type,unit);
                        Buildbox[boxID].m_content-=1;
                        Unit[unit].m_inHands=2;
                        Unit[unit].m_inHandsID=emptyID;
                        Logfile.log(g_playerNames[unit],"extracted BUILDBLOCK from BUILDBOX type:",type);
                    }
                }
                else switch (type) //Non-buildingparts
                {
                    case 4:{//Wall
                            Buildbox[boxID].m_content-=1;
                            Unit[unit].m_inHands=4;
                            Logfile.log(g_playerNames[unit],"extracted WALL from BUILDBOX id:",boxID);
                           }break;
                    case 5:{//Mine
                            Buildbox[boxID].m_content-=1;
                            Unit[unit].m_inHands=6;
                            Logfile.log(g_playerNames[unit],"extracted MINE from BUILDBOX id:",boxID);
                           }break;
                }
                makeSound(SplaceObject,unit,1,2);
            }
            return;
        }

        if (objectType==3) //Spawner panels
        {
            int spawnID=g_tempID;

            Logfile.log(g_playerNames[unit],"interacted with SPAWNER id:",spawnID);
            //Which Panel, Check Side
            int xu=(int)Unit[unit].m_xpos;
            int zu=(int)Unit[unit].m_zpos;
            int xs=(int)Spawner[spawnID].m_pos[0];
            int zs=(int)Spawner[spawnID].m_pos[2];
            enum sides {sWest=1,sNorth=2,sEast=3,sSouth=4};
            int side=0;
            if (xu>xs && zu==zs) side=sEast;
            if (xu<xs && zu==zs) side=sWest;
            if (xu==xs && zu>zs) side=sSouth;
            if (xu==xs && zu<zs) side=sNorth;
            switch (side)
            {
                case 0:     {//Bad Pos
                            }break;

                case sWest: {//Output
                             if (Spawner[spawnID].m_output!=0) //Something to get?
                             {
                                 int slot=-1;
                                 for (int ID=0;ID<g_maxBuildboxes;ID++) //Find empty Slot
                                 {
                                     if (!Buildbox[ID].m_active) {slot=ID; break;}
                                 }
                                 if (slot==-1) break;
                                 Buildbox[slot].newBuildbox(Spawner[spawnID].m_output,unit); //Make new Builbox
                                 Unit[unit].m_inHands=1;                //Put it in Hands of Unit
                                 Unit[unit].m_inHandsID=slot;           //Give ID
                                 Spawner[spawnID].m_output=0;        //Empty Output
                                 makeSound(SpowerupDelivery,unit,1,2);
                                 Logfile.log(g_playerNames[unit],"took outout from SPAWNER id:",spawnID);
                             }
                            }break;

                case sNorth:{//Defence Ass.
                             if (Spawner[spawnID].m_content>=Spawner[spawnID].m_price[2]) //Check Price
                             {
                                 if (Spawner[spawnID].m_output==0) //Output is Empty
                                 {
                                     Spawner[spawnID].m_output=3; //Output Loaded
                                     Spawner[spawnID].m_content-=Spawner[spawnID].m_price[2]; //Pay
                                     makeSound(SpowerupDelivery,unit,1,2);
                                     Logfile.log(g_playerNames[unit],"ordered DEF.ASS. from SPAWNER:",spawnID);
                                 }
                             }
                            }break;

                case sEast: {//Transport Ass.
                             if (Spawner[spawnID].m_content>=Spawner[spawnID].m_price[1]) //Check Price
                             {
                                 if (Spawner[spawnID].m_output==0) //Output is Empty
                                 {
                                     Spawner[spawnID].m_output=2; //Output Loaded
                                     Spawner[spawnID].m_content-=Spawner[spawnID].m_price[1]; //Pay
                                     makeSound(SpowerupDelivery,unit,1,2);
                                     Logfile.log(g_playerNames[unit],"ordered TRANS.ASS. from SPAWNER:",spawnID);
                                 }
                             }
                            }break;

                case sSouth:{//Weapon Ass.
                             if (Spawner[spawnID].m_content>=Spawner[spawnID].m_price[0]) //Check Price
                             {
                                 if (Spawner[spawnID].m_output==0) //Output is Empty
                                 {
                                     Spawner[spawnID].m_output=1; //Output Loaded
                                     Spawner[spawnID].m_content-=Spawner[spawnID].m_price[0]; //Pay
                                     makeSound(SpowerupDelivery,unit,1,2);
                                     Logfile.log(g_playerNames[unit],"ordered WEAP.ASS. from SPAWNER:",spawnID);
                                 }
                             }
                            }break;
            }
            return;
        }

        if (objectType==13) //BuildBlock to finish building
        {
            int bblockID=g_tempID;

            Logfile.log(g_playerNames[unit],"INTERACTED with BUILDINGBLOCK id", bblockID);
            int pos[2]={0,0};
            if (completeBuildingCheck((int)Buildblock[bblockID].m_pos[0],
                                      (int)Buildblock[bblockID].m_pos[2],
                                      (int)Buildblock[bblockID].m_type,pos)) //Check if Building is Ready
            {
                Logfile.log(g_playerNames[unit],"have accepted BuildingTEST");
                int xr=pos[0];
                int zr=pos[1];
                for (int ID=0;ID<g_maxBuildblocks;ID++)
                {
                    if (Buildblock[ID].m_active)
                    {
                        if (xr==Buildblock[ID].m_pos[0] && zr==Buildblock[ID].m_pos[2])
                        {
                            int type=Buildblock[ID].m_type;
                            int xcorner=(int)Buildblock[ID].m_pos[0];
                            int zcorner=(int)Buildblock[ID].m_pos[2];
                            //Find All BlockID and remove them
                            for (int x=xcorner;x<xcorner+4;x++)
                            for (int z=zcorner;z<zcorner+4;z++)
                            for (int ID=0;ID<g_maxBuildblocks;ID++)
                            {
                                if (Buildblock[ID].m_active)
                                {
                                    if (x==(int)Buildblock[ID].m_pos[0] && z==(int)Buildblock[ID].m_pos[2]) //Found Correct Block
                                    {
                                        Buildblock[ID].m_active=false; //Remove Blocks
                                        World.setObjectHeight(x,z,0);//Remove ObjectHeight
                                    }
                                }
                            }
                            //Create Building or Radar
                            if (type==7) //Radar
                            {
                                int slot=-1;
                                for (int ID=0;ID<g_maxRadars;ID++)
                                {
                                    if (!Radar[ID].m_active) {slot=ID; break;}
                                }
                                if (slot!=-1)
                                {
                                    if (g_gameMode==2) Radar[slot].newRadar(xcorner,World.getHeight(xcorner,zcorner),zcorner,unit+Unit[unit].m_team*10);
                                    else Radar[slot].newRadar(xcorner,World.getHeight(xcorner,zcorner),zcorner,unit);
                                    setBuildingObjectHeight(xcorner,zcorner,type); //Set Object height
                                    makeSound(SplaceObject,unit,1,2);
                                    Logfile.log(g_playerNames[unit],"Finished RADAR id:",slot);
                                }
                                return;
                            }
                            else //Building
                            {
                                int slot=-1;
                                for (int ID=0;ID<g_maxBuildings;ID++)
                                {
                                    if (!Building[ID].m_active) {slot=ID; break;}
                                }
                                if (slot!=-1)
                                {
                                    Building[slot].newBuilding(xcorner,World.getHeight(xcorner,zcorner),zcorner,type,unit);
                                    setBuildingObjectHeight(xcorner,zcorner,type); //Set Object height
                                    makeSound(SplaceObject,unit,1,2);
                                    Logfile.log(g_playerNames[unit],"Finished BUILDING id:",slot);
                                }
                                return;
                            }
                        }
                    }
                }
            }
            return;
        }

        if (objectType==7) //Building interaction
        {
            int buildID=g_tempID;

            float x,y,z;
            for (float step=0.3;step<20;step+=0.3)
            {
                x=Unit[unit].m_xpos+Unit[unit].m_xdir*step;
                y=Unit[unit].m_ypos+Unit[unit].m_ydir*step;
                z=Unit[unit].m_zpos+Unit[unit].m_zdir*step;

                if (Building[buildID].buildingInteractionHitTest(x,y,z)) //Check that panel was in range
                {
                    if (Unit[unit].m_xdir<0) return; //Wrong side
                    //Which panel?
                    int panel=(int)Unit[unit].m_zpos-(int)Building[buildID].m_pos[2];
                    if (panel>=0 && panel<=3)
                    {
                        if (!Building[buildID].m_inventory[panel]) //Buy
                        {
                            if (Spawner[Building[buildID].m_owner].m_content>=Building[buildID].m_price[panel])
                            {
                                Building[buildID].m_inventory[panel]=true; //Unlock
                                Spawner[Building[buildID].m_owner].m_content-=Building[buildID].m_price[panel]; //Pay
                                Logfile.log(g_playerNames[unit],"Bought BUILDBOX type:",panel);
                            }
                        }
                        else //Get
                        {
                            int type=Building[buildID].m_type;
                            switch (type)
                            {
                                case 1:{//wass
                                        switch (panel)
                                        {
                                            case 0:{//Revolver
                                                    Unit[unit].m_inventory[1]=true;
                                                    Logfile.log(g_playerNames[unit],"Unlocked REVOLVER at BuildingID:",buildID);
                                                   }break;
                                            case 1:{//Uzi
                                                    Unit[unit].m_inventory[2]=true;
                                                    Logfile.log(g_playerNames[unit],"Unlocked UZI at BuildingID:",buildID);
                                                   }break;
                                            case 2:{//Sniper
                                                    Unit[unit].m_inventory[3]=true;
                                                    Logfile.log(g_playerNames[unit],"Unlocked SNIPER at BuildingID:",buildID);
                                                   }break;
                                            case 3:{//RocketLauncher
                                                    Unit[unit].m_inventory[4]=true;
                                                    Logfile.log(g_playerNames[unit],"Unlocked ROCKETLAUNCHER at BuildingID:",buildID);
                                                   }break;
                                         }
                                        }break;
                                case 2:{//tass
                                        switch (panel)
                                        {
                                            case 0:{//Speed
                                                    if (Building[buildID].m_busy) break;
                                                    int slot=-1;
                                                    for (int i=0;i<g_maxSpeeds;i++)
                                                    {
                                                        if (!Speed[i].m_active) {slot=i; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Speed[slot].newSpeed(Building[buildID].m_pos[0]+2.5,
                                                                         Building[buildID].m_pos[1]+0.1,
                                                                         Building[buildID].m_pos[2]+1.5);
                                                    Building[buildID].m_busy=true;
                                                    Building[buildID].m_inventory[0]=false;
                                                    Building[buildID].m_inventory[3]=true;
                                                    int x=(int)Building[buildID].m_pos[0];
                                                    int z=(int)Building[buildID].m_pos[2];
                                                    World.setObjectHeight(x+2,z,0.1);
                                                    World.setObjectHeight(x+2,z+1,0.1);
                                                    World.setObjectHeight(x+2,z+2,0.1);
                                                    World.setObjectHeight(x+2,z+3,0.1);
                                                    World.setObjectHeight(x+3,z,0.1);
                                                    World.setObjectHeight(x+3,z+1,0.1);
                                                    World.setObjectHeight(x+3,z+2,0.1);
                                                    World.setObjectHeight(x+3,z+3,0.1);
                                                    Logfile.log(g_playerNames[unit],"Bought SPEED id:",slot);
                                                   }break;
                                            case 1:{//Tank
                                                    if (Building[buildID].m_busy) break;
                                                    int slot=-1;
                                                    for (int i=0;i<g_maxTanks;i++)
                                                    {
                                                        if (!Tank[i].m_active) {slot=i; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Tank[slot].newTank(Building[buildID].m_pos[0]+3,
                                                                       Building[buildID].m_pos[1]+0.1,
                                                                       Building[buildID].m_pos[2]+2);
                                                    Building[buildID].m_busy=true;
                                                    Building[buildID].m_inventory[1]=false;
                                                    Building[buildID].m_inventory[3]=true;
                                                    int x=(int)Building[buildID].m_pos[0];
                                                    int z=(int)Building[buildID].m_pos[2];
                                                    World.setObjectHeight(x+2,z,0.1);
                                                    World.setObjectHeight(x+2,z+1,0.1);
                                                    World.setObjectHeight(x+2,z+2,0.1);
                                                    World.setObjectHeight(x+2,z+3,0.1);
                                                    World.setObjectHeight(x+3,z,0.1);
                                                    World.setObjectHeight(x+3,z+1,0.1);
                                                    World.setObjectHeight(x+3,z+2,0.1);
                                                    World.setObjectHeight(x+3,z+3,0.1);
                                                    Logfile.log(g_playerNames[unit],"Bought TANK id:",slot);
                                                   }break;
                                            case 2:{//Heli
                                                    if (Building[buildID].m_busy) break;
                                                    int slot=-1;
                                                    for (int i=0;i<g_maxHelis;i++)
                                                    {
                                                        if (!Heli[i].m_active) {slot=i; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Heli[slot].newHeli(Building[buildID].m_pos[0]+2.5,
                                                                       Building[buildID].m_pos[1]+0.1,
                                                                       Building[buildID].m_pos[2]+1.5);
                                                    Building[buildID].m_busy=true;
                                                    Building[buildID].m_inventory[2]=false;
                                                    Building[buildID].m_inventory[3]=true;
                                                    int x=(int)Building[buildID].m_pos[0];
                                                    int z=(int)Building[buildID].m_pos[2];
                                                    World.setObjectHeight(x+2,z,0.1);
                                                    World.setObjectHeight(x+2,z+1,0.1);
                                                    World.setObjectHeight(x+2,z+2,0.1);
                                                    World.setObjectHeight(x+2,z+3,0.1);
                                                    World.setObjectHeight(x+3,z,0.1);
                                                    World.setObjectHeight(x+3,z+1,0.1);
                                                    World.setObjectHeight(x+3,z+2,0.1);
                                                    World.setObjectHeight(x+3,z+3,0.1);
                                                    Logfile.log(g_playerNames[unit],"Bought HELI id:",slot);
                                                   }break;
                                            case 3:{//Reset
                                                    if (!resetTass(buildID)) break;
                                                    Building[buildID].m_busy=false;
                                                    Building[buildID].m_inventory[3]=false;
                                                    int x=(int)Building[buildID].m_pos[0];
                                                    int z=(int)Building[buildID].m_pos[2];
                                                    World.setObjectHeight(x+2,z,2);
                                                    World.setObjectHeight(x+2,z+1,2);
                                                    World.setObjectHeight(x+2,z+2,2);
                                                    World.setObjectHeight(x+2,z+3,2);
                                                    World.setObjectHeight(x+3,z,2);
                                                    World.setObjectHeight(x+3,z+1,2);
                                                    World.setObjectHeight(x+3,z+2,2);
                                                    World.setObjectHeight(x+3,z+3,2);
                                                   }break;
                                         }
                                       }break;
                                case 3:{//dass
                                        switch (panel)
                                        {
                                            case 0:{//Get Wallbox
                                                    int slot=-1;
                                                    for (int ID=0;ID<g_maxBuildboxes;ID++) //Find empty Slot
                                                    {
                                                        if (!Buildbox[ID].m_active) {slot=ID; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Buildbox[slot].newBuildbox(4,unit); //Make new Builbox with Walls
                                                    Unit[unit].m_inHands=1;                //Put it in Hands of Unit
                                                    Unit[unit].m_inHandsID=slot;           //Give ID
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought BUILDBOX:WALLBOX id:",slot);
                                                   }break;
                                            case 1:{//Mine
                                                    int slot=-1;
                                                    for (int ID=0;ID<g_maxBuildboxes;ID++) //Find empty Slot
                                                    {
                                                        if (!Buildbox[ID].m_active) {slot=ID; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Buildbox[slot].newBuildbox(5,unit); //Make new Builbox with Walls
                                                    Unit[unit].m_inHands=1;                //Put it in Hands of Unit
                                                    Unit[unit].m_inHandsID=slot;           //Give ID
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought BUILDBOX:MINEBOX id:",slot);
                                                   }break;
                                            case 2:{//Radar
                                                    Unit[unit].m_inventory[5]=true;
                                                    Logfile.log(g_playerNames[unit],"Bought RADAR");
                                                   }break;
                                            case 3:{//Turr.exp
                                                    int slot=-1;
                                                    for (int ID=0;ID<g_maxBuildboxes;ID++) //Find empty Slot
                                                    {
                                                        if (!Buildbox[ID].m_active) {slot=ID; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Buildbox[slot].newBuildbox(6,unit); //Make new Builbox with turrexpBlocks
                                                    Unit[unit].m_inHands=1;                //Put it in Hands of Unit
                                                    Unit[unit].m_inHandsID=slot;           //Give ID
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought BUILDBOX:TURRET.EXP id:",slot);
                                                   }break;
                                        }
                                       }break;
                                case 6:{//TurrExp
                                        switch (panel)
                                        {
                                            case 0:{//Chain
                                                    Unit[unit].m_inHands=7;
                                                    Unit[unit].m_inHandsID=1;
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought TURRET:GATTLING");
                                                   }break;
                                            case 1:{//Rocket
                                                    Unit[unit].m_inHands=7;
                                                    Unit[unit].m_inHandsID=2;
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought TURRET:ROCKET");
                                                   }break;
                                            case 2:{//Laser
                                                    Unit[unit].m_inHands=7;
                                                    Unit[unit].m_inHandsID=3;
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought TURRET:LASER");
                                                   }break;
                                            case 3:{//TowerExp
                                                    int slot=-1;
                                                    for (int ID=0;ID<g_maxBuildboxes;ID++) //Find empty Slot
                                                    {
                                                        if (!Buildbox[ID].m_active) {slot=ID; break;}
                                                    }
                                                    if (slot==-1) break;
                                                    Buildbox[slot].newBuildbox(7,unit); //Make new Builbox with turrexpBlocks
                                                    Unit[unit].m_inHands=1;                //Put it in Hands of Unit
                                                    Unit[unit].m_inHandsID=slot;           //Give ID
                                                    Building[buildID].m_inventory[panel]=false; //Lock
                                                    Logfile.log(g_playerNames[unit],"Bought BUILDBOX:RADAR.EXP id:",slot);
                                                   }break;

                                        }
                                       }break;
                            }
                        }
                        makeSound(SplaceObject,unit,1,2);
                    }
                    return;
                }
            }
        }

        //Transports:
        if (objectType==8) //Enter Heli
        {
            int heliID=g_tempID;

            if (Heli[heliID].m_inUse) return; //Not usable by 2 Units at the same time
            Unit[unit].m_inTransportType=1;
            Unit[unit].m_transportID=heliID;
            Unit[unit].m_currWeapon=0;
            Heli[heliID].m_user=unit;
            Heli[heliID].m_inUse=true;
            Heli[heliID].m_noiseID=Sound.getAndLoadFreeSource(Sheli_motor);
            if (unit==g_playerID)
            {
                g_view=v3rd;
                g_transportFloat=0;
                g_headingY=Heli[heliID].m_yrot;
                g_xvel=g_yvel=g_zvel=0;
            }
            Logfile.log(g_playerNames[unit],"mounted HELI id:",heliID);
            return;
        }

        if (objectType==9) //Enter Speed
        {
            int speedID=g_tempID;

            if (Speed[speedID].m_inUse) return; //Not usable by 2 Units at the same time
            Unit[unit].m_inTransportType=2;
            Unit[unit].m_transportID=speedID;
            Unit[unit].m_currWeapon=0;
            Unit[unit].m_yheadhed=Speed[speedID].m_yrot;
            Speed[speedID].m_user=unit;
            Speed[speedID].m_inUse=true;
            Speed[speedID].m_noiseID=Sound.getAndLoadFreeSource(Sspeed_motor);
            if (unit==g_playerID)
            {
                g_view=v3rd;
                g_transportFloat=0;
                g_headingY=Speed[speedID].m_yrot;
                g_xvel=g_yvel=g_zvel=0;
                g_xpos=Speed[speedID].m_pos[0];
                g_ypos=Speed[speedID].m_pos[1];
                g_zpos=Speed[speedID].m_pos[2];
            }
            Logfile.log(g_playerNames[unit],"mounted SPEED id:",speedID);
            return;
        }

        if (objectType==10) //Enter Tank
        {
            if (Unit[unit].m_carries) return; //No objects in tanks
            int tankID=g_tempID;

            if (Tank[tankID].m_inUse) return; //Not usable by 2 Units at the same time
            Unit[unit].m_inTransportType=3;
            Unit[unit].m_transportID=tankID;
            Unit[unit].m_currWeapon=0;
            Unit[unit].m_yheadhed=Tank[tankID].m_yrot_tower;
            Tank[tankID].m_user=unit;
            Tank[tankID].m_inUse=true;
            Tank[tankID].m_noiseID=Sound.getAndLoadFreeSource(Stank_motor);
            if (unit==g_playerID)
            {
                //g_view=v3rd;
                g_xpos=Tank[tankID].m_pos[0];
                g_ypos=Tank[tankID].m_pos[1];
                g_zpos=Tank[tankID].m_pos[2];
                g_headingY=Tank[tankID].m_yrot_tower;
                g_headingXZ=Tank[tankID].m_xrot_tower;
                g_xvel=g_yvel=g_zvel=0;
            }
            Logfile.log(g_playerNames[unit],"mounted TANK id:",tankID);
            return;
        }

        if (objectType==15) //Radar
        {
            int radarID=g_tempID;
            float xrel=g_tempPos[0]+Unit[unit].m_xdir*0.3-Radar[radarID].m_pos[0];
            float yrel=g_tempPos[1]+Unit[unit].m_ydir*0.3-Radar[radarID].m_pos[1];
            float zrel=g_tempPos[2]+Unit[unit].m_zdir*0.3-Radar[radarID].m_pos[2];

            int result=Radar[radarID].buttonHitTest(xrel,yrel,zrel);
            switch (result) //Get outpost
            {
                case 1:{//buy outpost
                        if (!Radar[radarID].m_output) //Nothing in output
                        {
                            if (Spawner[unit].m_content>=Radar[radarID].m_outpostPrice)
                            {
                                Radar[radarID].m_output=true;
                                Spawner[unit].m_content-=Radar[radarID].m_outpostPrice;
                            }
                        }
                       }break;
                case 2:{//Get outpost
                        if (Radar[radarID].m_output)
                        {
                            Unit[unit].m_inHands=8;
                            Unit[unit].m_inHandsID=radarID; //Remember RadarID
                            Radar[radarID].m_output=false;
                        }
                       }break;
                case 3:{//Teleport to current Outpost
                        int outpostID=Radar[radarID].m_outpostID[Radar[radarID].m_currOutpost];
                        if (outpostID!=-1) //current outpost active
                        {
                            if (g_playerID==unit)
                            {
                                g_simulate=simTP2outpostIN;
                                g_rememberID=outpostID;
                            }
                        }
                       }break;
            }
        }
        if (objectType==16) //Outpost (teleportation)
        {
            int outpostID=g_tempID;
            //Check if Unit ontop
            if ((int)Unit[unit].m_xpos==(int)Outpost[outpostID].m_pos[0] &&
                (int)Unit[unit].m_zpos==(int)Outpost[outpostID].m_pos[2])
            {
                if (unit==g_playerID) //Movement on local computer
                {
                    g_rememberID=Outpost[outpostID].m_connected_radarID;
                    g_simulate=simTP2radarIN;
                }
            }
        }
    }
}

void firstTimeInit(void)
{
    Logfile.log("CLOCKS_PER_SEC for current CPU:",(float)CLOCKS_PER_SEC);
    Logfile.log("Game started");

    loadText();
    loadSound();

    //Clear names array
    for (int i=0;i<8;i++)
    for (int j=0;j<20;j++)
    {
        g_playerNames[i][j]=' ';
    }

    g_firstTimeInit=false;
}
/*
void genTerrainTexture(void)
{
    //Generate Terrain Shift
    for (int x=0;x<g_terrainWidth;x++)
    for (int z=0;z<g_terrainLength;z++)
    {
        g_terrTexShift[x][z]=float(rand()%100)/2000;
    }
    //Set Walls to Mountain Texture
    for (int x=0;x<g_terrainWidth;x++)
    {
        for (int z=0;z<39;z++)
        {
            g_terrTex[x][z]=0.4;
        }
        for (int z=g_terrainLength-39;z<g_terrainLength;z++)
        {
            g_terrTex[x][z]=0.4;
        }
    }
    for (int z=0;z<g_terrainLength;z++)
    {
        for (int x=0;x<39;x++)
        {
            g_terrTex[x][z]=0.4;
        }
        for (int x=g_terrainWidth-39;x<g_terrainWidth;x++)
        {
            g_terrTex[x][z]=0.4;
        }
    }
    //Make Fuzzy Borders
    float swapChance;
    int texID[10]={0,0,0,0,0,0,0,0,0,0};
    int mostCommonTex,secondMostCommonTex;
    float mostCommonTexID,secondMostCommonTexID;
    for (int r=0;r<1;r++) //Repeat
    {
        for (int x=30;x<g_terrainWidth-30;x++)
        for (int z=30;z<g_terrainLength-30;z++)
        {
            //Most Common Texture?
            for (int i=-1;i<2;i++)
            for (int j=-1;j<2;j++)
            {
                switch (int(g_terrTex[x+i][z+j]*10))
                {
                    case 0: texID[0]++; break;
                    case 1: texID[1]++; break;
                    case 2: texID[2]++; break;
                    case 3: texID[3]++; break;
                    case 4: texID[4]++; break;
                    case 5: texID[5]++; break;
                    case 6: texID[6]++; break;
                    case 7: texID[7]++; break;
                    case 8: texID[8]++; break;
                    case 9: texID[9]++; break;
                }
            }
            mostCommonTex=0;
            mostCommonTexID=0;
            secondMostCommonTex=0;
            secondMostCommonTexID=0;
            for (int i=0;i<10;i++) //Most Common
            {
                if (texID[i]>mostCommonTex) {mostCommonTex=texID[i]; mostCommonTexID=(float)i/10;}
            }
            for (int i=0;i<10;i++) //Second Most Common
            {
                if (i==int(mostCommonTexID*10)) continue;
                if (texID[i]>secondMostCommonTex) {secondMostCommonTex=texID[i]; secondMostCommonTexID=(float)i/10;}
            }
            if (g_terrTex[x][z]==mostCommonTexID) //Small Chance of Getting Second Most Common Texture
            {
                swapChance=secondMostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=secondMostCommonTexID;
            }
            else //High Chance of Getting Most Common Texture
            {
                swapChance=mostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=mostCommonTexID;
            }
            //Reset Values
            for (int i=0;i<10;i++) texID[i]=0;
        }
        //Change Direction
        for (int x=g_terrainWidth-30;x<30;x--)
        for (int z=g_terrainLength-30;z<30;z--)
        {
            //Most Common Texture?
            for (int i=-1;i<2;i++)
            for (int j=-1;j<2;j++)
            {
                switch (int(g_terrTex[x+i][z+j]*10))
                {
                    case 0: texID[0]++; break;
                    case 1: texID[1]++; break;
                    case 2: texID[2]++; break;
                    case 3: texID[3]++; break;
                    case 4: texID[4]++; break;
                    case 5: texID[5]++; break;
                    case 6: texID[6]++; break;
                    case 7: texID[7]++; break;
                    case 8: texID[8]++; break;
                    case 9: texID[9]++; break;
                }
            }
            mostCommonTex=0;
            mostCommonTexID=0;
            secondMostCommonTex=0;
            secondMostCommonTexID=0;
            for (int i=0;i<10;i++) //Most Common
            {
                if (texID[i]>mostCommonTex) {mostCommonTex=texID[i]; mostCommonTexID=(float)i/10;}
            }
            for (int i=0;i<10;i++) //Second Most Common
            {
                if (i==int(mostCommonTexID*10)) continue;
                if (texID[i]>secondMostCommonTex) {secondMostCommonTex=texID[i]; secondMostCommonTexID=(float)i/10;}
            }
            if (g_terrTex[x][z]==mostCommonTexID) //Small Chance of Getting Second Most Common Texture
            {
                swapChance=secondMostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=secondMostCommonTexID;
            }
            else //High Chance of Getting Most Common Texture
            {
                swapChance=mostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=mostCommonTexID;
            }
            //Reset Values
            for (int i=0;i<10;i++) texID[i]=0;
        }
        //Change Direction
        for (int z=30;z<g_terrainLength-30;z++)
        for (int x=30;x<g_terrainWidth-30;x++)
        {
            //Most Common Texture?
            for (int i=-1;i<2;i++)
            for (int j=-1;j<2;j++)
            {
                switch (int(g_terrTex[x+i][z+j]*10))
                {
                    case 0: texID[0]++; break;
                    case 1: texID[1]++; break;
                    case 2: texID[2]++; break;
                    case 3: texID[3]++; break;
                    case 4: texID[4]++; break;
                    case 5: texID[5]++; break;
                    case 6: texID[6]++; break;
                    case 7: texID[7]++; break;
                    case 8: texID[8]++; break;
                    case 9: texID[9]++; break;
                }
            }
            mostCommonTex=0;
            mostCommonTexID=0;
            secondMostCommonTex=0;
            secondMostCommonTexID=0;
            for (int i=0;i<10;i++) //Most Common
            {
                if (texID[i]>mostCommonTex) {mostCommonTex=texID[i]; mostCommonTexID=(float)i/10;}
            }
            for (int i=0;i<10;i++) //Second Most Common
            {
                if (i==int(mostCommonTexID*10)) continue;
                if (texID[i]>secondMostCommonTex) {secondMostCommonTex=texID[i]; secondMostCommonTexID=(float)i/10;}
            }
            if (g_terrTex[x][z]==mostCommonTexID) //Small Chance of Getting Second Most Common Texture
            {
                swapChance=secondMostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=secondMostCommonTexID;
            }
            else //High Chance of Getting Most Common Texture
            {
                swapChance=mostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=mostCommonTexID;
            }
            //Reset Values
            for (int i=0;i<10;i++) texID[i]=0;
        }
        //Change Direction
        for (int z=g_terrainLength-30;z<30;z--)
        for (int x=g_terrainWidth-30;x<30;x--)
        {
            //Most Common Texture?
            for (int i=-1;i<2;i++)
            for (int j=-1;j<2;j++)
            {
                switch (int(g_terrTex[x+i][z+j]*10))
                {
                    case 0: texID[0]++; break;
                    case 1: texID[1]++; break;
                    case 2: texID[2]++; break;
                    case 3: texID[3]++; break;
                    case 4: texID[4]++; break;
                    case 5: texID[5]++; break;
                    case 6: texID[6]++; break;
                    case 7: texID[7]++; break;
                    case 8: texID[8]++; break;
                    case 9: texID[9]++; break;
                }
            }
            mostCommonTex=0;
            mostCommonTexID=0;
            secondMostCommonTex=0;
            secondMostCommonTexID=0;
            for (int i=0;i<10;i++) //Most Common
            {
                if (texID[i]>mostCommonTex) {mostCommonTex=texID[i]; mostCommonTexID=(float)i/10;}
            }
            for (int i=0;i<10;i++) //Second Most Common
            {
                if (i==int(mostCommonTexID*10)) continue;
                if (texID[i]>secondMostCommonTex) {secondMostCommonTex=texID[i]; secondMostCommonTexID=(float)i/10;}
            }
            if (g_terrTex[x][z]==mostCommonTexID) //Small Chance of Getting Second Most Common Texture
            {
                swapChance=secondMostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=secondMostCommonTexID;
            }
            else //High Chance of Getting Most Common Texture
            {
                swapChance=mostCommonTex;
                if (rand()%10<swapChance) g_terrTex[x][z]=mostCommonTexID;
            }
            //Reset Values
            for (int i=0;i<10;i++) texID[i]=0;
        }
    }
}
*/
void flattenGround(int i)
{
    float avgHeight=(World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos-1)+
                     World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos-1)+
                     World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos-1)+
                     World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos)+
                     World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos)+
                     World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos)+
                     World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos+1)+
                     World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos+1)+
                     World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos+1))/9;

    if (World.getObjectHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos-1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos-1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos-1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos-1,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos-1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos-1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos-1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos-1,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos-1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos-1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos-1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos-1,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos+1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos+1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos+1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos-1,(int)Unit[i].m_zpos+1,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos+1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos+1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos+1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos,(int)Unit[i].m_zpos+1,height+0.05);
    }
    if (World.getObjectHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos+1)==0)
    {
        float height=World.getHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos+1);
        if (avgHeight<height) World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos+1,height-0.1);
        else World.setHeight((int)Unit[i].m_xpos+1,(int)Unit[i].m_zpos+1,height+0.05);
    }
}

void loadTextures(void)
{
	//Load Texture Files
    image* Image;
    int i=0;
    Image = loadBMP("Res/Texture/playerskin.bmp"); //0
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/terrain.bmp"); //1
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/build.bmp"); //2
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/playerdata.bmp"); //3
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/building.bmp"); //4
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/transport.bmp"); //5
	g_textureID[i] = loadTexture(Image); i++;
	Image = loadBMP("Res/Texture/radar.bmp"); //6
	g_textureID[i] = loadTexture(Image); i++;
	delete Image;

    //Give TextureID to Classes
    World.m_textureID=g_textureID[1];
    for (int i=0;i<g_maxPlayers;i++)
    {
        Unit[i].m_playerSkin=g_textureID[0];
        Spawner[i].m_textureID=g_textureID[0];
    }
    for (int i=0;i<g_maxNumOfPowerups;i++)
    {
        Powerup[i].m_textureID=g_textureID[0];
    }
    for (int i=0;i<g_maxMines;i++)
    {
        Mine[i].m_textureID=g_textureID[0];
    }
    for (int i=0;i<g_maxTurrets;i++)
    {
        Turret[i].m_textureID=g_textureID[0];
    }
    for (int i=0;i<g_maxProj;i++)
    {
        Proj[i].m_textureID=g_textureID[0];
    }
    for (int i=0;i<g_maxBuildboxes;i++)
    {
        Buildbox[i].m_textureID=g_textureID[2];
    }
    for (int i=0;i<g_maxBuildblocks;i++)
    {
        Buildblock[i].m_textureID=g_textureID[2];
    }
    for (int i=0;i<g_maxBuildings;i++)
    {
        Building[i].m_textureID=g_textureID[4];
    }
    for (int i=0;i<g_maxWalls;i++)
    {
        Wall[i].m_textureID=g_textureID[2];
    }
    for (int i=0;i<g_maxHelis;i++)
    {
        Heli[i].m_textureID=g_textureID[5];
    }
    for (int i=0;i<g_maxSpeeds;i++)
    {
        Speed[i].m_textureID=g_textureID[5];
    }
    for (int i=0;i<g_maxTanks;i++)
    {
        Tank[i].m_textureID=g_textureID[5];
    }
    for (int i=0;i<g_maxRadars;i++)
    {
        Radar[i].m_textureID=g_textureID[6];
    }
}

GLuint loadTexture(image* Image) //Makes the image into a texture, and returns the id of the texture
{
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 Image->width, Image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 Image->pixels);               //The actual pixel data
	//Set Texture Settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //GL_NEAREST = pixligt, GL_LINEAR = suddigt

	return textureId; //Returns the id of the texture
}

void getServerLobbyData(void)
{
    int serverLobbyData[20];
    recv (SServer, (char*)&serverLobbyData, sizeof(serverLobbyData), 0);
    for (int j=0;j<20;j++)
    {
        g_allPlayerLobbyData[8][j]=serverLobbyData[j]; //Store server data
    }
    g_gameMode=serverLobbyData[1];
    g_seed=serverLobbyData[2];
    g_playerID=serverLobbyData[3];
    //Check if Test Server
    if (serverLobbyData[5]=='T' && serverLobbyData[6]=='E' && serverLobbyData[7]=='S' && serverLobbyData[8]=='T')
    {
        g_testServer=true; Sound.playSimpleSound(Sexplosion,1);
    }
    if (serverLobbyData[5]=='R' && serverLobbyData[6]=='U' && serverLobbyData[7]=='S' && serverLobbyData[8]=='H')
    {
        for (int i=0;i<g_maxPlayers;i++) Spawner[i].m_content=25;
        Sound.playSimpleSound(SshovelMiss,1);
    }
}

void genSpawnerPos(void)
{
    float start_pos[g_maxPlayers][2];
    start_pos[0][0]=50000; start_pos[0][1]=50000; //start pos att 50 000,50 000
    for (int i=1;i<g_maxPlayers;i++) //new pos for all players
    {
        bool pos_ok=true;
        for (int bonus=0;bonus<30000;bonus+=100) //find new pos
        {
            int angle=rand()%360; //random angle
            int distance=rand()%1500+600+bonus; //random distance
            int add_x=distance*sinf(angle*piover180);
            int add_z=distance*cosf(angle*piover180);
            //give new pos
            start_pos[i][0]=start_pos[i-1][0]+add_x;
            start_pos[i][1]=start_pos[i-1][1]+add_z;
            for (int prev_player=0;prev_player<i;prev_player++) //test if new pos is OK
            {
                int range=250; //on both sides
                if (start_pos[i][0]>start_pos[prev_player][0]-range && start_pos[i][0]<start_pos[prev_player][0]+range &&
                    start_pos[i][1]>start_pos[prev_player][1]-range && start_pos[i][1]<start_pos[prev_player][1]+range) //too close
                {
                    pos_ok=false;
                    break;
                }
            }
            if (pos_ok) break; //if new pos is OK, break
        }
        if (!pos_ok) {i=0; continue;} //redo everything (happens if bonus>30 000)
    }

    switch (g_gameMode) //send start pos to global value
    {
        case gmFFA:
            {
                for (int i=0;i<g_maxPlayers;i++)
                {
                    g_spawnXpos[i]=start_pos[i][0];
                    g_spawnZpos[i]=start_pos[i][1];
                }
            }
            break;
        case gmTEAM:
            {
                int start_index_A;
                int start_index_B;
                while (true)
                {
                    start_index_A=rand()%g_maxPlayers;
                    start_index_B=rand()%g_maxPlayers;
                    if (start_index_A!=start_index_B) break;
                }
                g_spawnXpos[0]=start_pos[start_index_A][0];
                g_spawnZpos[0]=start_pos[start_index_A][1];
                g_spawnXpos[1]=start_pos[start_index_B][0];
                g_spawnZpos[1]=start_pos[start_index_B][1];
            }
            break;
    }
}

void unpauseCheck(void)
{
    if (mouse[0]) //passar inte om fullscreen
    {
        if (cursorX>90 && cursorX<550 && cursorY>210 && cursorY<270) {activeSwitch=true; ShowCursor(false);}
    }
    if (keys[80] && active && g_pushDelay<0) {activeSwitch=true; ShowCursor(false); g_pushDelay=100;}
    if (g_pushDelay>=0) g_pushDelay-=CycleTime*0.17;
}

void drawScope(void)
{
    glPushMatrix();
    glTranslatef(-5,-5,-35);
    if (World.getHeight(int(g_xpos+g_xhed*7),int(g_zpos+g_zhed*7))>g_ypos) //Scope covered
    {
        glColor4f(0.15,0.15,0.15,1);
        glBegin(GL_QUADS);
        glVertex4f(0,0,0,1);
        glVertex4f(10,0,0,1);
        glColor4f(0.2,0.2,0.2,1);
        glVertex4f(10,10,0,1);
        glVertex4f(0,10,0,1);
        glEnd();
    }
    glColor4f(0.1,0.1,0.1,1);
    glBegin(GL_QUADS);
    glVertex4f(-10,-10,0,1);
    glVertex4f(2,-10,0,1);
    glVertex4f(2,20,0,1);
    glVertex4f(-10,20,0,1);
    glVertex4f(8,-10,0,1);
    glVertex4f(20,-10,0,1);
    glVertex4f(20,20,0,1);
    glVertex4f(8,20,0,1);
    glVertex4f(2,-10,0,1);
    glVertex4f(8,-10,0,1);
    glVertex4f(8,3,0,1);
    glVertex4f(2,3,0,1);
    glVertex4f(2,7,0,1);
    glVertex4f(8,7,0,1);
    glVertex4f(8,20,0,1);
    glVertex4f(2,20,0,1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex4f(2,2,0,1);
    glVertex4f(4,2,0,1);
    glVertex4f(2,4,0,1);
    glVertex4f(2,6,0,1);
    glVertex4f(4,8,0,1);
    glVertex4f(2,8,0,1);
    glVertex4f(6,8,0,1);
    glVertex4f(8,6,0,1);
    glVertex4f(8,8,0,1);
    glVertex4f(8,4,0,1);
    glVertex4f(6,2,0,1);
    glVertex4f(8,2,0,1);
    glEnd();
    glPopMatrix();
    //cross
    glTranslatef(0,0,-5);
    glColor4f(0.8,0.8,0.8,1);
    glBegin(GL_LINES);
    glVertex3f(-0.03,0,0);
    glVertex3f(0.03,0,0);
    glVertex3f(0,0.03,0);
    glVertex3f(0,-0.03,0);
    glEnd();
}

void genStars(void)
{
    for (int i=0;i<g_numOfStars;i++)
    {
        Star[i].x=rand()%20-10;
        Star[i].y=rand()%10+1;
        Star[i].z=rand()%20-10;
        Star[i].intensity=float(rand()%80+20)/100;
    }
}

void drawSky(void)
{
    //Draw Stars
    gluLookAt(0,0,0,
              g_xhed,g_yhed,g_zhed,
              g_xupp,g_yupp,g_zupp);
    glBegin(GL_POINTS);
    for (int i=0;i<g_numOfStars;i++)
    {
        glColor4f(Star[i].intensity,Star[i].intensity,Star[i].intensity,1);
        glVertex3f(Star[i].x,Star[i].y,Star[i].z);
    }
    glEnd();
}

void infoSwapLobby(void)
{
    if (g_fireDelay>=0) {g_fireDelay-=CycleTime*0.17; return;}
    else
    {
        g_fireDelay=10;
        sendPlayerLobbyData(); //skicka värden för om leavat/readyat

        if (g_lobbyStatus==0 && g_wantToLeave) //Leaving lobby
        {
            g_gameState=gPreLobby;
            haveIP=false;
            g_wantToLeave=false;
            //Clear names array
            for (int i=0;i<8;i++)
            for (int j=0;j<20;j++)
            {
                g_playerNames[i][j]=' ';
            }
        }
        else getOtherPlayerLobbyData();
    }
}

void inputLobby(void)
{
    //Swap Team
    if (g_gameMode==2)
    {
        if (keys[84] && g_pushDelay<0) //t
        {
            g_lobbyTeamSwapButton=true;
            if (g_lobbyTeam==1) {g_lobbyTeam=2; g_pushDelay=30;}
            else if (g_lobbyTeam==2) {g_lobbyTeam=1; g_pushDelay=30;}
        }
    }
    //Ready
    if (keys[82] && g_pushDelay<0) //r
    {
        if (g_lobbyStatus==1) {g_lobbyStatus=2; g_pushDelay=30;}
        else if (g_lobbyStatus==2) {g_lobbyStatus=1; g_pushDelay=30;}
    }
    //Leave
    if (keys[76]) {g_lobbyStatus=0; g_wantToLeave=true; g_lobbyLeaveButton=true; g_pushDelay=30;} //l
    if (g_pushDelay>=0) g_pushDelay-=CycleTime*0.17;
    else {g_lobbyTeamSwapButton=g_lobbyLeaveButton=false;}
}

void drawLobby(void)
{
   glPushMatrix();
    //rita ramar

    //Draw Buttons
    glTranslatef(0.45,-0.75,-2);
    if (g_lobbyStatus==2) glColor4f(0.6,0.6,0.2,1); else glColor4f(0.3,0.2,0.2,1);
    glBegin(GL_QUADS);
    glVertex4f(0.0,0.0,0.0,1);
    glVertex4f(0.5,0.0,0.0,1);
    glVertex4f(0.5,0.2,0.0,1);
    glVertex4f(0.0,0.2,0.0,1);
    glEnd();
    glTranslatef(-0.8,0,0);
    if (g_gameMode==2)
    {
        if (g_lobbyTeamSwapButton) glColor4f(0.6,0.6,0.2,1); else glColor4f(0.3,0.2,0.2,1);
        glBegin(GL_QUADS);
        glVertex4f(0.0,0.0,0.0,1);
        glVertex4f(0.7,0.0,0.0,1);
        glVertex4f(0.7,0.2,0.0,1);
        glVertex4f(0.0,0.2,0.0,1);
        glEnd();
    }
    glTranslatef(-0.6,0,0);
    if (g_lobbyLeaveButton) glColor4f(0.6,0.6,0.2,1); else glColor4f(0.3,0.2,0.2,1);
    glBegin(GL_QUADS);
    glVertex4f(0.0,0.0,0.0,1);
    glVertex4f(0.5,0.0,0.0,1);
    glVertex4f(0.5,0.2,0.0,1);
    glVertex4f(0.0,0.2,0.0,1);
    glEnd();
   glPopMatrix();
    //Draw Text on buttons
   glPushMatrix();
    glColor4f(1,1,1,1);
    glScalef(0.7,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, text12VertexArray);
    glTranslatef(10.2,-10.15,-30);
    glDrawArrays(GL_LINES, 0, text12v/3);
    glTranslatef(-16.8,0,0);
    if (g_gameMode==2)
    {
        glVertexPointer(3, GL_FLOAT, 0, text14VertexArray);
        glDrawArrays(GL_LINES, 0, text14v/3);
    }
    glVertexPointer(3, GL_FLOAT, 0, text13VertexArray);
    glTranslatef(-13.3,0,0);
    glDrawArrays(GL_LINES, 0, text13v/3);
    glDisableClientState(GL_VERTEX_ARRAY);
   glPopMatrix();

   glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(-10,6.5,-20);
    glEnableClientState(GL_VERTEX_ARRAY);
    //Draw text: LOBBY
    glVertexPointer(3, GL_FLOAT, 0, text9VertexArray);
    glDrawArrays(GL_LINES, 0, text9v/3);
    //Draw Server name
    glTranslatef(8,0,0);
    glVertexPointer(3, GL_FLOAT, 0, text28VertexArray);
    glDrawArrays(GL_LINES, 0, text28v/3);
    //Draw PlayerNames
    glTranslatef(-12,0,-7);
    int playerID=0;
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text20VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text20v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text21VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text21v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text22VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text22v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text23VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text23v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text24VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text24v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text25VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text25v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text26VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text26v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    if (g_allPlayerLobbyData[playerID][0]!=0)
    {
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(15,0,0);
        glTranslatef(0,-1.1,0);
        glVertexPointer(3, GL_FLOAT, 0, text27VertexArray);
        if (g_allPlayerLobbyData[playerID][1]==2) glColor4f(0.1,0.8,0.1,1); else glColor4f(0.1,0.1,0.8,1);
        if (g_allPlayerLobbyData[playerID][0]==2) glColor4f(0.8,0.8,0.3,1);
        glDrawArrays(GL_LINES, 0, text27v/3);
        if (g_allPlayerLobbyData[playerID][1]==2) glTranslatef(-15,0,0);
        playerID++;
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    //drawTal(g_seed);

   glPopMatrix();
}

void inputMeny(void)
{
    if (keys[13]) g_gameState=gPreLobby; //Enter

    if (inputDelay>130)
    {
        if (currLobbyNumber<10)
        {
            if (keys[65]) {g_playerName[currLobbyNumber]='A'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[66]) {g_playerName[currLobbyNumber]='B'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[67]) {g_playerName[currLobbyNumber]='C'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[68]) {g_playerName[currLobbyNumber]='D'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[69]) {g_playerName[currLobbyNumber]='E'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[70]) {g_playerName[currLobbyNumber]='F'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[71]) {g_playerName[currLobbyNumber]='G'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[72]) {g_playerName[currLobbyNumber]='H'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[73]) {g_playerName[currLobbyNumber]='I'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[74]) {g_playerName[currLobbyNumber]='J'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[75]) {g_playerName[currLobbyNumber]='K'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[76]) {g_playerName[currLobbyNumber]='L'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[77]) {g_playerName[currLobbyNumber]='M'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[78]) {g_playerName[currLobbyNumber]='N'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[79]) {g_playerName[currLobbyNumber]='O'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[80]) {g_playerName[currLobbyNumber]='P'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[81]) {g_playerName[currLobbyNumber]='Q'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[82]) {g_playerName[currLobbyNumber]='R'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[83]) {g_playerName[currLobbyNumber]='S'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[84]) {g_playerName[currLobbyNumber]='T'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[85]) {g_playerName[currLobbyNumber]='U'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[86]) {g_playerName[currLobbyNumber]='V'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[87]) {g_playerName[currLobbyNumber]='W'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[88]) {g_playerName[currLobbyNumber]='X'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[89]) {g_playerName[currLobbyNumber]='Y'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[90]) {g_playerName[currLobbyNumber]='Z'; currLobbyNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
        }
        if (keys[8] && currLobbyNumber>0)
        {currLobbyNumber--; g_playerName[currLobbyNumber]=' '; inputDelay=0;Sound.playSimpleSound(Serase,1);}
        if (keys[13]) //enter
        {
            g_playerName[currLobbyNumber]='\0';
            g_gameState=gPreLobby;
            inputDelay=0;
            Sound.playSimpleSound(Sselected,0.4);
            Logfile.log("Name:",g_playerName);
        }
    }
    else inputDelay+=CycleTime;
}

void drawMeny(void)
{
    //Load name
    float* VertexArray;
    char text11[20];
    for (int i=0;i<20;i++)
    {
        text11[i]=g_playerName[i];
    }
    VertexArray=getTextVertexArray(text11, 20);
    for (int i=0;i<VertexArray[0];i++) text11VertexArray[i]=VertexArray[i+1];
    text11v=(int)VertexArray[0]-1;

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(-8,3,-20);
    glEnableClientState(GL_VERTEX_ARRAY);
    //Enter name
    glVertexPointer(3, GL_FLOAT, 0, text10VertexArray);
    glDrawArrays(GL_LINES, 0, text10v/3);
    //Draw name
    glTranslatef(1,-3,0);
    glVertexPointer(3, GL_FLOAT, 0, text11VertexArray);
    glColor4f(0.7,0.7,0.3,1);
    glDrawArrays(GL_LINES, 0, text11v/3);
    //Press enter
    glTranslatef(-1,-3,0);
    glVertexPointer(3, GL_FLOAT, 0, text8VertexArray);
    glColor4f(1,1,1,1);
    glDrawArrays(GL_LINES, 0, text8v/3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void informationSwap(void)
{
    if (serverFound) sendPlayerData();
    else //Make new random value
    {
        g_randomLast=g_random;
        g_random=rand()%10000+10000;
    }
    getOtherPlayerData();
    g_projCreated=false;
    g_makeSound=0;
    g_fireWeapon=0;
    g_action=0;
    g_checkSum=0;
    g_animation=0;
    g_extraFloatA=0;
    g_extraFloatB=0;
}

float findCamPos(void)
{
    float campos=0;
    for (float i=8;i>0;i-=0.1)
    if (World.getHeight(int(g_xpos-g_xhed*i),int(g_zpos-g_zhed*i))<g_ypos-g_yhed*i) {campos=i;break;}
    return campos;
}

void setCamPos(float campos)
{
    if (Unit[g_playerID].m_spawning)
    {
        if (Unit[g_playerID].m_spawnerMoving)
        {
            int carrier=Spawner[g_playerID].m_carrier;
            gluLookAt(Unit[carrier].m_xpos,Unit[carrier].m_ypos+20,Unit[carrier].m_zpos,
                      Unit[carrier].m_xpos,Unit[carrier].m_ypos,Unit[carrier].m_zpos,
                      0,0,-1);
        }
        else
        {
            float spawnprogress=(Unit[g_playerID].m_size+0.5)*4;
            gluLookAt(g_xpos-g_xhed*spawnprogress,g_ypos-g_yhed*spawnprogress+3-spawnprogress,g_zpos-g_zhed*spawnprogress,
                                  g_xpos+g_xhed,g_ypos+g_yhed,g_zpos+g_zhed,
                                  g_xupp,g_yupp,g_zupp);
        }
    }
    else
    {
        switch (g_view)
        {
            case vFPS:
                {
                if (Unit[g_playerID].m_carrySpawner)
                gluLookAt(g_xpos+g_xhed,g_ypos+g_yhed-0.7,g_zpos+g_zhed,
                          g_xpos+g_xhed*2,g_ypos+g_yhed*2-0.7,g_zpos+g_zhed*2,
                          g_xupp,g_yupp,g_zupp);
                else
                gluLookAt(g_xpos,g_ypos-g_jumpForce,g_zpos,
                          g_xpos+g_xhed,g_ypos+g_yhed-g_jumpForce,g_zpos+g_zhed,
                          g_xupp,g_yupp,g_zupp);
                }
                break;
            case vTop:
                gluLookAt(g_xpos,g_ypos+g_heightZoom,g_zpos,
                          g_xpos,g_ypos,g_zpos,
                          0,0,-1); break;
            case v3rd:
                gluLookAt(g_xpos-g_xhed*campos,g_ypos-g_yhed*campos,g_zpos-g_zhed*campos,
                          g_xpos+g_xhed,g_ypos+g_yhed,g_zpos+g_zhed,
                          g_xupp,g_yupp,g_zupp); break;
            default:
                gluLookAt(g_xpos-g_xhed*10,g_ypos-g_yhed*10+1,g_zpos-g_zhed*10,
                          g_xpos+g_xhed,g_ypos+g_yhed-1,g_zpos+g_zhed,
                          g_xupp,g_yupp,g_zupp); break;
        }
    }
    //Lighting
    if (g_zooming)
    {
        g_lightPos[0]=g_xpos+g_xhed*1;
        g_lightPos[1]=g_ypos+g_yhed*1+50;
        g_lightPos[2]=g_zpos+g_zhed*1;
    }
    else if (g_view==v3rd)
    {
        g_lightPos[0]=g_xpos-g_xhed*campos;
        g_lightPos[1]=g_ypos-g_yhed*campos+5;
        g_lightPos[2]=g_zpos-g_zhed*campos;
    }
    else
    {
        g_lightPos[0]=g_xpos;
        g_lightPos[1]=g_ypos+4;
        g_lightPos[2]=g_zpos;
    }
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);
    if (g_muzzleFlash>0)
    {
        g_lightPos[0]=g_xpos+g_xhed;
        g_lightPos[1]=g_ypos+g_yhed;
        g_lightPos[2]=g_zpos+g_zhed;
        g_muzzleFlash--;
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, g_lightPos);
    }
    else glDisable(GL_LIGHT1);
    if (g_flashTimer>0)
    {
        g_lightPos[0]=g_xFlashPos;
        g_lightPos[1]=g_yFlashPos;
        g_lightPos[2]=g_zFlashPos;
        g_flashTimer--;
        glEnable(GL_LIGHT2);
        glLightfv(GL_LIGHT2, GL_POSITION, g_lightPos);
    }
    else glDisable(GL_LIGHT2);
}

void drawObjects(void)
{
    glColor4f(1,1,1,1);
    //Draw Units
    for (int i=0;i<g_maxPlayers;i++)
    {
        if (Unit[i].m_online)
        {
            if (Unit[i].m_deathCounter>0)
            {
                Unit[i].m_deathCounter-=CycleTime;
                if (Unit[i].m_deathCounter<=0) //Ready to spawn
                {
                    if (i==g_playerID)
                    {
                        g_headingY=90;
                        g_headingXZ=90;
                        g_jumpState=0;
                        g_xpos=Unit[i].m_xpos=int(Spawner[i].m_pos[0])+0.5;
                        g_ypos=Unit[i].m_ypos=Spawner[i].m_pos[1]+2;
                        g_zpos=Unit[i].m_zpos=int(Spawner[i].m_pos[2])+0.5;
                        Sound.playSimpleSound(Sspawning,0.4);
                    }
                    else
                    {
                        Unit[i].m_xpos=int(Spawner[i].m_pos[0])+0.5;
                        Unit[i].m_ypos=Spawner[i].m_pos[1]+2;
                        Unit[i].m_zpos=int(Spawner[i].m_pos[2])+0.5;
                    }
                }
                continue;
            }
            //Check if close to its spawner -> healing
            float healingRadius=5;
            if (Unit[i].m_xpos<Spawner[i].m_pos[0]+healingRadius && Unit[i].m_xpos>Spawner[i].m_pos[0]-healingRadius &&
                Unit[i].m_ypos<Spawner[i].m_pos[1]+healingRadius && Unit[i].m_ypos>Spawner[i].m_pos[1]-healingRadius &&
                Unit[i].m_zpos<Spawner[i].m_pos[2]+healingRadius && Unit[i].m_zpos>Spawner[i].m_pos[2]-healingRadius)
            {
                if (Unit[i].m_healingDelay<=0) //Check timer
                {
                    if (Unit[i].m_HP>0 && Unit[i].m_HP<100)
                    {
                        Unit[i].m_HP+=1; //Heal if not dead or full HP
                        Unit[i].m_healingDelay=50;
                    }
                }
            }
            int unitStatus=Unit[i].updateUnit(CycleTime);
            if (unitStatus==0) //Death
            {
                Logfile.log(g_playerNames[i],"<broke down>");
                Logfile.log("Random number control:",rand()%1000);
                explosionAt(Unit[i].m_xpos,Unit[i].m_zpos,Unit[i].m_ypos,0,80,i); //Show Explosion animation
                Unit[i].m_spawning=true;
                Spawner[i].m_open=true;
                for (int r=1;r<10;r++) Unit[i].m_inventory[r]=false; //Remove Inventory
                Unit[i].m_zooming=false; //UnZoom
                if (Unit[i].m_carries)
                {
                    if (Unit[i].m_carrySpawner)
                    {
                        Unit[Unit[i].m_carryID].m_spawnerMoving=false;
                        Spawner[Unit[i].m_carryID].m_moving=false;
                        //Spawner[Unit[i].m_carryID].m_carrier=-1;
                        Unit[i].m_carries=false;
                        Unit[i].m_carrySpawner=false;
                        //Unit[i].m_carryID=-1;
                    }
                    if (Unit[i].m_carryPow)
                    {
                        Powerup[Unit[i].m_carryID].m_moving=false;
                        //Powerup[Unit[i].m_carryID].m_carrier=-1;
                        Unit[i].m_carries=false;
                        Unit[i].m_carryPow=false;
                        //Unit[i].m_carryID=-1;
                    }
                    if (Unit[i].m_carryBBox)
                    {
                        Buildbox[Unit[i].m_carryID].m_moving=false;
                        //Buildbox[Unit[i].m_carryID].m_carrier=-1;
                        Unit[i].m_carries=false;
                        Unit[i].m_carryBBox=false;
                        //Unit[i].m_carryID=-1;
                    }
                    if (Unit[i].m_carryBBlock)
                    {
                        Buildblock[Unit[i].m_carryID].m_moving=false;
                        //Buildblock[Unit[i].m_carryID].m_carrier=-1;
                        Unit[i].m_carries=false;
                        Unit[i].m_carryBBlock=false;
                       // Unit[i].m_carryID=-1;
                    }
                }
                if (Unit[i].m_inTransportType!=0) //Exit Transport
                {
                    switch (Unit[i].m_inTransportType)
                    {
                        case 1:{//Heli
                                 Unit[i].m_moving=false;
                                 Unit[i].m_inTransportType=0;
                                 int ID=Unit[i].m_transportID;
                                 Sound.stopSound(Heli[ID].m_noiseID);
                                 Heli[ID].m_noiseID=-1;
                                 Heli[ID].m_inUse=false;
                                 Heli[ID].m_pos[0]=(int)Unit[i].m_xpos;
                                 Heli[ID].m_pos[2]=(int)Unit[i].m_zpos;
                               }break;
                        case 2:{//Speed
                                 Unit[i].m_moving=false;
                                 Unit[i].m_inTransportType=0;
                                 int ID=Unit[i].m_transportID;
                                 Sound.stopSound(Speed[ID].m_noiseID);
                                 Speed[ID].m_noiseID=-1;
                                 Speed[ID].m_inUse=false;
                                 Speed[ID].m_pos[0]=(int)Unit[i].m_xpos;
                                 Speed[ID].m_pos[2]=(int)Unit[i].m_zpos;
                               }break;
                        case 3:{//Tank
                                 Unit[i].m_moving=false;
                                 Unit[i].m_inTransportType=0;
                                 int ID=Unit[i].m_transportID;
                                 Sound.stopSound(Tank[ID].m_noiseID);
                                 Tank[ID].m_noiseID=-1;
                                 Tank[ID].m_inUse=false;
                                 Tank[ID].m_pos[0]=(int)Unit[i].m_xpos;
                                 Tank[ID].m_pos[2]=(int)Unit[i].m_zpos;
                               }break;
                    }
                }
                Unit[i].m_deathCounter=2000;
                if (i==g_playerID)
                {
                    g_animation=10; //Change Weapon to Melee
                    g_view=vFPS;
                    zoomingOff();
                    Sound.playSimpleSound(Sdeath,1);
                    g_xvel=g_yvel=g_zvel=0;
                    g_jumpState=0; g_jumpForce=0;
                }
            }
            else if (unitStatus==2) //Spawning Done
            {
                Unit[i].m_spawning=false;
                Unit[i].m_HP=100;
                Spawner[i].m_open=false;
                if (i==g_playerID)
                {
                    g_ypos+=2.1;
                    g_yvel=0.05;
                    g_zvel=0.05;
                }
            }
            if (unitStatus==3) //Spawning Aborted
            {
                if (i==g_playerID)
                {
                    g_headingY=90;
                    g_headingXZ=90;
                    g_jumpState=0;
                    g_xpos=Unit[i].m_xpos=int(Spawner[i].m_pos[0])+0.5;
                    g_ypos=Unit[i].m_ypos=Spawner[i].m_pos[1];
                    g_zpos=Unit[i].m_zpos=int(Spawner[i].m_pos[2])+0.5;
                }
                else
                {
                    Unit[i].m_xpos=int(Spawner[i].m_pos[0])+0.5;
                    Unit[i].m_ypos=Spawner[i].m_pos[1]+2;
                    Unit[i].m_zpos=int(Spawner[i].m_pos[2])+0.5;
                }
            }
            if (g_view!=vFPS || i!=g_playerID)
            {
                if (Unit[i].m_spawnerOnline)
                {
                    if (checkRadarBox(Unit[i].m_xpos,Unit[i].m_ypos,Unit[i].m_zpos))
                    {
                        Unit[i].drawUnit();
                        g_iSeeUnit=true;
                    }
                }
            }
            else
            {
                Unit[i].drawFPSview(g_iSeeUnit,g_iSeeSpawner);
                g_iSeeUnit=false;
                g_iSeeSpawner=false;
            }
        }
    }
    //XXXXXXXXXXXXXXXXXXX
    if (g_debugMode)
    {
        glBegin(GL_LINES);
        glColor4f(1,0,0,1);
        glVertex4f(g_xpos,1+g_ypos,g_zpos,1);
        glVertex4f(g_xpos+g_xhed,1+g_ypos+g_yhed,g_zpos+g_zhed,1);
        glColor4f(0,1,0,1);
        glVertex4f(g_xpos,1+g_ypos,g_zpos,1);
        glVertex4f(g_xpos+g_xupp,1+g_ypos+g_yupp,g_zpos+g_zupp,1);
        glColor4f(0,0,1,1);
        glVertex4f(g_xpos,1+g_ypos,g_zpos,1);
        glVertex4f(g_xpos+g_xrig,1+g_ypos+g_yrig,g_zpos+g_zrig,1);
        glEnd();
        glColor4f(1,1,1,1);
    }
    //XXXXXXXXXXXXXXXXXX

    //Draw Projectiles
    glDisable(GL_CULL_FACE);
    for (int i=0;i<g_maxProj;i++)
    {
        if (Proj[i].m_active)
        {
            if (!Proj[i].updateProj(CycleTime))
            {
                explosionAt(Proj[i].m_xpos,Proj[i].m_zpos,Proj[i].m_ypos,1,100,20);
                Proj[i].m_active=false;
            }
            Proj[i].drawProj();
        }
    }
    //Draw Explosions
    glEnable(GL_LIGHTING);
    for (int i=0;i<g_maxExplosion;i++)
    {
        if (Explosion[i].m_active)
        {
            Explosion[i].update(CycleTime);
            if (checkRadarBox(Explosion[i].m_xpos,Explosion[i].m_ypos,Explosion[i].m_zpos))
            {
                Explosion[i].drawExplosion();
            }
        }
    }
    glDisable(GL_LIGHTING);
    //Draw Spawners
    //glDisable(GL_CULL_FACE);
    for (int i=0;i<g_maxPlayers;i++)
    {
        if (Spawner[i].m_active)
        {
            if (Spawner[i].m_moving)
            {
                int carrier=Spawner[i].m_carrier;
                Spawner[i].updateSpawner(Unit[carrier].m_xpos,Unit[carrier].m_ypos,Unit[carrier].m_zpos,Unit[carrier].m_ybodyhed);
            }
            else
            if (!Spawner[i].updateSpawner())
            {
                Logfile.log(g_playerNames[i],"'s SPAWNER <broke down>");
                explosionAt(Spawner[i].m_pos[0]+0.5,Spawner[i].m_pos[2]+0.5,Spawner[i].m_pos[1]+1,1,100,i);
                Unit[i].m_spawnerOnline=false;
                Spawner[i].m_active=false;
                if (i==g_playerID) {g_view=v3rd; glEnable(GL_FOG); zoomingOff();} //Change View
            }
            if (checkRadarBox(Spawner[i].m_pos[0],Spawner[i].m_pos[1]+1,Spawner[i].m_pos[2]))
            {
                Spawner[i].drawSpawner();
                g_iSeeSpawner=true;
            }
        }
    }
    glEnable(GL_CULL_FACE);
    //Draw Powerups
    for (int i=0;i<g_maxNumOfPowerups;i++)
    {
        if (Powerup[i].m_active)
        {
            if (Powerup[i].m_moving)
            {
                int carrier=Powerup[i].m_carrier;
                Powerup[i].updatePos(Unit[carrier].m_xpos,Unit[carrier].m_ypos,Unit[carrier].m_zpos,Unit[carrier].m_ybodyhed);
            }
            int status=Powerup[i].update();
            if (status==1) //death
            {
                explosionAt(Powerup[i].m_pos[0]+0.5,Powerup[i].m_pos[2]+0.5,Powerup[i].m_pos[1],1,200,9);
                g_numOfPowerups--;
            }
            if (checkRadarBox(Powerup[i].m_pos[0],Powerup[i].m_pos[1],Powerup[i].m_pos[2]))
            {
                Powerup[i].drawPowerup();
            }
        }
    }
    glDisable(GL_CULL_FACE);
    //Draw Turrets
    for (int i=0;i<g_maxTurrets;i++)
    {
        if (Turret[i].m_active)
        {
            //Find nearest (enemy)
            float turX=Turret[i].m_pos[0];
            float turZ=Turret[i].m_pos[2];
            float tempX,tempZ;
            float dist=1;
            switch (Turret[i].m_type)
            {
                case 1: dist=50; break;
                case 2: dist=75; break;
                case 3: dist=100; break;
            }
            float tempdist;
            int targetID=-1;
            for (int j=0;j<g_maxPlayers;j++) //find target
            {
                if (Unit[j].m_online && Unit[j].m_HP>0 && Unit[j].m_spawnerOnline)
                {
                    if (g_gameMode==1) //FFA
                    {
                        if (j==Turret[i].m_owner) continue;
                    }
                    if (g_gameMode==2) //Teams
                    {
                        if (Turret[i].m_team==Unit[j].m_team) continue;
                    }
                    tempX=Unit[j].m_xpos;
                    tempZ=Unit[j].m_zpos;
                    tempdist=sqrt((turX-tempX)*(turX-tempX)+(turZ-tempZ)*(turZ-tempZ));
                    if (tempdist<dist) {dist=tempdist;targetID=j;}
                }
            }
            int status;
            if (targetID!=-1)
            {
                status=Turret[i].updateTurret(Unit[targetID].m_xpos,Unit[targetID].m_ypos-2,Unit[targetID].m_zpos,(float)CycleTime);
                if (status==2) //Fire
                {
                    switch (Turret[i].m_type)
                    {
                        case 1:{//Gattling
                                int iHitObject=bulletHitTest(100+i); //Hit Test
                                if (iHitObject!=0) impact(iHitObject,100+i); //Something was hit
                                flash(Turret[i].m_pos[0],Turret[i].m_pos[1]+2,Turret[i].m_pos[2]);
                               }break;
                        case 2:{//Rocket
                                int slot=-1;
                                for (int p=0;p<g_maxProj;p++)
                                {
                                    if (!Proj[p].m_active)
                                    {
                                        slot=p; break;
                                    }
                                }
                                if (slot!=-1)
                                {
                                    float data[7]={Turret[i].m_pos[0]+0.5,Turret[i].m_pos[1]+1.6,Turret[i].m_pos[2]+0.5,
                                                   180-Turret[i].m_yrot,Turret[i].m_xrot,100+i,1};
                                    Proj[slot].newProj(data);
                                    makeSound(SrocketLaunch,i,1,4);
                                }
                               }break;
                        case 3:{//Laser
                                int slot=-1;
                                for (int b=0;b<g_maxBeams;b++)
                                {
                                    if (!Beam[b].m_active)
                                    {
                                        slot=b; break;
                                    }
                                }
                                if (slot!=-1)
                                {
                                    int iHitObject=bulletHitTest(100+i); //Hit Test
                                    if (iHitObject!=0) impact(iHitObject,100+i); //Something was hit
                                    float data[6]={Turret[i].m_pos[0]+0.5,Turret[i].m_pos[1]+1.5,Turret[i].m_pos[2]+0.5,
                                                   g_tempPos[0],g_tempPos[1],g_tempPos[2]};
                                    Beam[slot].newBeam(data);
                                    flash(Turret[i].m_pos[0],Turret[i].m_pos[1]+2,Turret[i].m_pos[2]);
                                }
                               }break;
                    }
                }
                else if (status==1) //Death
                {
                    explosionAt(Turret[i].m_pos[0]+0.5,Turret[i].m_pos[2]+0.5,Turret[i].m_pos[1]+1,1,100,9);
                    Turret[i].m_active=false;
                    World.setObjectHeight((int)Turret[i].m_pos[0],(int)Turret[i].m_pos[2],0);
                    Logfile.log("TURRET <broke down> id:",i);
                }
            }
            else //noone in sight
            {
                status=Turret[i].updateTurret((float)CycleTime);
                if (status==1) //Death
                {
                    explosionAt(Turret[i].m_pos[0]+0.5,Turret[i].m_pos[2]+0.5,Turret[i].m_pos[1]+1,1,100,9);
                    World.setObjectHeight((int)Turret[i].m_pos[0],(int)Turret[i].m_pos[2],0);
                    Turret[i].m_active=false;
                    Logfile.log("TURRET <broke down> id:",i);
                }
            }
            if (checkRadarBox(Turret[i].m_pos[0],Turret[i].m_pos[1]+1,Turret[i].m_pos[2]))
            {
                Turret[i].drawTurret();
            }
        }
    }
    //Draw Mines
    for (int i=0;i<g_maxMines;i++)
    {
        if (Mine[i].m_active)
        {
            if (!Mine[i].updateMine()) //Out of HP
            {
                explosionAt(Mine[i].m_pos[0]+0.5,Mine[i].m_pos[2]+0.5,Mine[i].m_pos[1]-1,1,200,20);
                Mine[i].m_active=false;
                World.setObjectHeight((int)Mine[i].m_pos[0],(int)Mine[i].m_pos[2],0);
                Logfile.log("MINE <broke down> id:",i);
            }
            if (checkRadarBox(Mine[i].m_pos[0],Mine[i].m_pos[1],Mine[i].m_pos[2]))
            {
                Mine[i].drawMine();
            }
        }
    }
    //Draw Buildboxes
    for (int i=0;i<g_maxBuildboxes;i++)
    {
        if (Buildbox[i].m_active)
        {
            if (Buildbox[i].m_inHands) continue; //Not really active
            //Update
            if (Buildbox[i].m_moving)
            {
                float rot=Unit[Buildbox[i].m_carrier].m_ybodyhed;
                float x=Unit[Buildbox[i].m_carrier].m_xpos;
                float y=Unit[Buildbox[i].m_carrier].m_ypos;
                float z=Unit[Buildbox[i].m_carrier].m_zpos;
                Buildbox[i].updatePos(x,y,z,rot);
            }
            int status=Buildbox[i].update();
            if (status==1) //Death
            {
                explosionAt(Buildbox[i].m_pos[0]+0.5,Buildbox[i].m_pos[2]+0.5,Buildbox[i].m_pos[1]+1,2,100,9);
            }
            //Draw
            if (checkRadarBox(Buildbox[i].m_pos[0],Buildbox[i].m_pos[1],Buildbox[i].m_pos[2])) //Check FC
            {
                Buildbox[i].drawBuildbox();
            }
        }
    }
    //Draw Buildblocks
    for (int i=0;i<g_maxBuildblocks;i++)
    {
        if (Buildblock[i].m_active)
        {
            if (Buildblock[i].m_inHands) continue; //Not really active if inHands
            //Update
            if (Buildblock[i].m_moving)
            {
                float rot=Unit[Buildblock[i].m_carrier].m_ybodyhed;
                float x=Unit[Buildblock[i].m_carrier].m_xpos;
                float y=Unit[Buildblock[i].m_carrier].m_ypos;
                float z=Unit[Buildblock[i].m_carrier].m_zpos;
                Buildblock[i].updatePos(x,y,z,rot);
            }
            int status=Buildblock[i].update();
            if (status==1) //Death
            {
                explosionAt(Buildblock[i].m_pos[0]+0.5,Buildblock[i].m_pos[2]+0.5,Buildblock[i].m_pos[1]+1,1,100,9);
            }
            //Draw
            if (checkRadarBox(Buildblock[i].m_pos[0],Buildblock[i].m_pos[1],Buildblock[i].m_pos[2])) //Check FC
            {
                Buildblock[i].drawBuildblock();
            }
        }
    }
    //Draw Buildings
    for (int i=0;i<g_maxBuildings;i++)
    {
        if (Building[i].m_active)
        {
            int status=Building[i].update(Spawner[Building[i].m_owner].m_content,CycleTime);
            if (status==1) //Death
            {
                int type=Building[i].m_type;
                switch (type)
                {
                    case 1:{//Wass
                            int xpos=(int)Building[i].m_pos[0]+1;
                            int zpos=(int)Building[i].m_pos[2];
                            for (int x=xpos+1;x<xpos+3;x++)
                            for (int z=zpos;z<zpos+4;z++)
                            {
                                World.setObjectHeight(x,z,0);
                            }
                           }break;
                    case 2:{//Tass
                            int xpos=(int)Building[i].m_pos[0];
                            int zpos=(int)Building[i].m_pos[2];
                            for (int x=xpos;x<xpos+4;x++)
                            for (int z=zpos;z<zpos+4;z++)
                            {
                                World.setObjectHeight(x,z,0);
                            }
                           }break;
                    case 3:{//Dass
                            int xpos=(int)Building[i].m_pos[0]+1;
                            int zpos=(int)Building[i].m_pos[2];
                            for (int x=xpos+1;x<xpos+3;x++)
                            for (int z=zpos;z<zpos+4;z++)
                            {
                                World.setObjectHeight(x,z,0);
                            }
                           }break;
                    case 6:{//Turr.Exp
                            int xpos=(int)Building[i].m_pos[0]+1;
                            int zpos=(int)Building[i].m_pos[2];
                            for (int x=xpos+1;x<xpos+3;x++)
                            for (int z=zpos;z<zpos+4;z++)
                            {
                                World.setObjectHeight(x,z,0);
                            }
                           }break;
                }
                Logfile.log("BUILDING <broke down> id:",i);
                explosionAt(Building[i].m_pos[0]+1,Building[i].m_pos[2]+1,Building[i].m_pos[1]+1,1,100,9);
                explosionAt(Building[i].m_pos[0]+1,Building[i].m_pos[2]+3,Building[i].m_pos[1]+1,1,100,9);
                explosionAt(Building[i].m_pos[0]+3,Building[i].m_pos[2]+1,Building[i].m_pos[1]+1,1,100,9);
                explosionAt(Building[i].m_pos[0]+3,Building[i].m_pos[2]+3,Building[i].m_pos[1]+1,1,100,9);
            }
            if (checkRadarBox(Building[i].m_pos[0],Building[i].m_pos[1],Building[i].m_pos[2]) ||
                checkRadarBox(Building[i].m_pos[0]+4,Building[i].m_pos[1],Building[i].m_pos[2]) ||
                checkRadarBox(Building[i].m_pos[0],Building[i].m_pos[1],Building[i].m_pos[2]+4) ||
                checkRadarBox(Building[i].m_pos[0]+4,Building[i].m_pos[1],Building[i].m_pos[2]+4)) //Check FC
            {
                Building[i].drawBuilding();
            }
        }
    }
    //Draw Walls
    for (int i=0;i<g_maxWalls;i++)
    {
        if (Wall[i].m_active)
        {
            int status=Wall[i].update();
            if (status==1) //Death
            {
                World.setObjectHeight((int)Wall[i].m_pos[0],(int)Wall[i].m_pos[2],0);
                Logfile.log("WALL <broke down> id:",i);
                explosionAt(Wall[i].m_pos[0]+0.5,Wall[i].m_pos[2]+0.5,Wall[i].m_pos[1]+1,2,100,9);
            }
            if (checkRadarBox(Wall[i].m_pos[0],Wall[i].m_pos[1],Wall[i].m_pos[2]))
            {
                Wall[i].drawWall();
            }
        }
    }
    //Draw Helis
    for (int i=0;i<g_maxHelis;i++)
    {
        if (Heli[i].m_active)
        {
            if (Heli[i].m_inUse)
            {
                int unit=Heli[i].m_user;
                int status=Heli[i].update(Unit[unit].m_xpos,Unit[unit].m_ypos,Unit[unit].m_zpos,
                                          Unit[unit].m_ybodyhed,Unit[unit].m_avgvel);
                if (status==1) //Death
                {
                    Unit[unit].m_carries=false;
                    Unit[unit].m_carryType=0;
                    Unit[unit].m_inTransportType=0;
                    Sound.stopSound(Heli[i].m_noiseID);
                    Logfile.log("HELI <broke down> id:",i);
                    explosionAt(Heli[i].m_pos[0]+0.5,Heli[i].m_pos[2]+0.5,Heli[i].m_pos[1]+1,1,100,9);
                }

                if (Heli[i].m_noiseID!=-1) //Play Motor sound
                {
                    float vel=Unit[unit].m_avgvel*5;
                    if (vel<1) vel=1;
                    float soundData[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                                       g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                                       Heli[i].m_pos[0],Heli[i].m_pos[1],Heli[i].m_pos[2],
                                       0,0,0,   vel,1,0};
                    Sound.updateSound(Heli[i].m_noiseID,soundData);
                }
            }
            else
            {
                int status=Heli[i].update(CycleTime);
                if (status==1) //Death
                {
                    if (Heli[i].m_noiseID!=-1) Sound.stopSound(Heli[i].m_noiseID); //Stop Sound (should be off, to be safe)
                    Logfile.log("HELI <broke down> id:",i);
                    explosionAt(Heli[i].m_pos[0]+0.5,Heli[i].m_pos[2]+0.5,Heli[i].m_pos[1]+1,1,100,9);
                }
            }
            if (checkRadarBox(Heli[i].m_pos[0],Heli[i].m_pos[1],Heli[i].m_pos[2]))
            {
                Heli[i].drawHeli();
            }
        }
    }
    //Draw Speeds
    for (int i=0;i<g_maxSpeeds;i++)
    {
        if (Speed[i].m_active)
        {
            if (Speed[i].m_inUse)
            {
                int unit=Speed[i].m_user;
                int status=Speed[i].update(Unit[unit].m_xpos,Unit[unit].m_ypos,Unit[unit].m_zpos,Unit[unit].m_ybodyhed);
                if (status==1) //Death
                {
                    Unit[unit].m_carries=false;
                    Unit[unit].m_carryType=0;
                    Unit[unit].m_inTransportType=0;
                    Sound.stopSound(Speed[i].m_noiseID);
                    Logfile.log("SPEED <broke down> id:",i);
                    explosionAt(Speed[i].m_pos[0]+0.5,Speed[i].m_pos[2]+0.5,Speed[i].m_pos[1]+1,1,100,9);
                }

                if (Speed[i].m_noiseID!=-1) //Play Motor sound
                {
                    float vel=Unit[unit].m_avgvel/0.100;
                    if (vel<0.2) vel=0.2;
                    float soundData[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                                       g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                                       Speed[i].m_pos[0],Speed[i].m_pos[1],Speed[i].m_pos[2],
                                       0,0,0,   vel,1,0};
                    Sound.updateSound(Speed[i].m_noiseID,soundData);
                }
            }
            else
            {
                int status=Speed[i].update();
                if (status==1) //Death
                {
                    if (Speed[i].m_noiseID!=-1) Sound.stopSound(Speed[i].m_noiseID); //Stop Sound (should be off, to be safe)
                    Logfile.log("SPEED <broke down> id:",i);
                    explosionAt(Speed[i].m_pos[0]+0.5,Speed[i].m_pos[2]+0.5,Speed[i].m_pos[1]+1,1,100,9);
                }
            }
            if (checkRadarBox(Speed[i].m_pos[0],Speed[i].m_pos[1],Speed[i].m_pos[2]))
            {
                Speed[i].drawSpeed();
            }
        }
    }
    //Draw Tanks
    for (int i=0;i<g_maxTanks;i++)
    {
        if (Tank[i].m_active)
        {
            if (Tank[i].m_inUse)
            {
                int unit=Tank[i].m_user;
                float data[]={Unit[unit].m_xpos,
                              Unit[unit].m_ypos,
                              Unit[unit].m_zpos,
                              Unit[unit].m_zhed,
                              Unit[unit].m_yheadhed,
                              Unit[unit].m_transportFloat,
                              Unit[unit].m_jumpState
                             };
                int status=Tank[i].update(data,(float)CycleTime);
                if (status==1) //Death
                {
                    Unit[unit].m_carries=false;
                    Unit[unit].m_carryType=0;
                    Unit[unit].m_inTransportType=0;
                    Sound.stopSound(Tank[i].m_noiseID);
                    Logfile.log("TANK <broke down> id:",i);
                    explosionAt(Tank[i].m_pos[0]+0.5,Tank[i].m_pos[2]+0.5,Tank[i].m_pos[1]+1,1,100,9);
                }

                if (Tank[i].m_noiseID!=-1) //Play Motor sound
                {
                    float vel=Unit[unit].m_avgvel/0.040;
                    if (vel<0.2) vel=0.2;
                    float soundData[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                                       g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                                       Tank[i].m_pos[0],Tank[i].m_pos[1],Tank[i].m_pos[2],
                                       0,0,0,   vel,2,0};
                    Sound.updateSound(Tank[i].m_noiseID,soundData);
                }
            }
            else
            {
                int status=Tank[i].update();
                if (status==1) //Death
                {
                    if (Tank[i].m_noiseID!=-1) Sound.stopSound(Tank[i].m_noiseID); //Stop Sound (should be off, to be safe)
                    Logfile.log("TANK <broke down> id:",i);
                    explosionAt(Tank[i].m_pos[0]+0.5,Tank[i].m_pos[2]+0.5,Tank[i].m_pos[1]+1,1,100,9);
                }
            }
            if (checkRadarBox(Tank[i].m_pos[0],Tank[i].m_pos[1],Tank[i].m_pos[2]))
            {
                if (Unit[g_playerID].m_inTransportType==3 && Unit[g_playerID].m_transportID==i && g_view==vFPS) Tank[i].drawFPSTank();
                else Tank[i].drawTank();
            }
        }
    }
    //Draw Beams
    for (int i=0;i<g_maxBeams;i++)
    {
        if (Beam[i].m_active)
        {
            Beam[i].update((float)CycleTime);
            if (checkRadarBox(Beam[i].m_start_pos[0],Beam[i].m_start_pos[1],Beam[i].m_start_pos[2]) ||
                checkRadarBox(Beam[i].m_end_pos[0],Beam[i].m_end_pos[1],Beam[i].m_end_pos[2]))
            {
                Beam[i].drawBeam();
            }
        }
    }
    //Draw Radars
    for (int i=0;i<g_maxRadars;i++)
    {
        if (Radar[i].m_active)
        {
            int status=Radar[i].update(CycleTime);
            if (status==1) //Death
            {
                int xpos=(int)Radar[i].m_pos[0];
                int zpos=(int)Radar[i].m_pos[2];
                for (int x=xpos;x<xpos+4;x++)
                for (int z=zpos;z<zpos+4;z++)
                {
                    World.setObjectHeight(x,z,0);
                }
                Logfile.log("RADAR <broke down> id:",i);
                explosionAt(Radar[i].m_pos[0]+1,Radar[i].m_pos[2]+1,Radar[i].m_pos[1]+1,1,100,9);
                explosionAt(Radar[i].m_pos[0]+1,Radar[i].m_pos[2]+3,Radar[i].m_pos[1]+1,1,100,9);
                explosionAt(Radar[i].m_pos[0]+3,Radar[i].m_pos[2]+1,Radar[i].m_pos[1]+1,1,100,9);
                explosionAt(Radar[i].m_pos[0]+3,Radar[i].m_pos[2]+3,Radar[i].m_pos[1]+1,1,100,9);
                //Destroy Outposts
                for (int c=0;c<99;c++)
                {
                    if (Radar[i].m_outpostID[c]!=-1)
                    {
                        Outpost[Radar[i].m_outpostID[c]].m_HP=0; //Set HP to 0;
                        Radar[i].m_outpostID[c]=-1; //Remove from ID storage
                    }
                }
            }
            if (checkRadarBox(Radar[i].m_pos[0],Radar[i].m_pos[1],Radar[i].m_pos[2]) ||
                checkRadarBox(Radar[i].m_pos[0]+4,Radar[i].m_pos[1],Radar[i].m_pos[2]) ||
                checkRadarBox(Radar[i].m_pos[0],Radar[i].m_pos[1],Radar[i].m_pos[2]+4) ||
                checkRadarBox(Radar[i].m_pos[0]+4,Radar[i].m_pos[1],Radar[i].m_pos[2]+4)) //Test FC
            {
                float pixelColor[4800];
                calcRadarDisplay(pixelColor,i);
                Radar[i].drawRadar(pixelColor,Spawner[Radar[i].m_owner].m_content);
            }
        }
    }
    //Draw Outposts
    for (int i=0;i<g_maxOutposts;i++)
    {
        if (Outpost[i].m_active)
        {
            int status;
            if (Outpost[i].m_moving)
            {
                int carrier=Outpost[i].m_carrier;
                status=Outpost[i].update(Unit[carrier].m_xpos,Unit[carrier].m_ypos,Unit[carrier].m_zpos,
                                         Unit[carrier].m_ybodyhed);
            }
            else status=Outpost[i].update();

            switch (status)
            {
                case 1:{//Death
                        int xpos=(int)Radar[i].m_pos[0];
                        int zpos=(int)Radar[i].m_pos[2];
                        World.setObjectHeight(xpos,zpos,0);
                        Logfile.log("OUTPOST <broke down> id:",i);
                        explosionAt(Outpost[i].m_pos[0],Outpost[i].m_pos[2]+1,Outpost[i].m_pos[1],1,100,9);
                        Radar[Outpost[i].m_connected_radarID].removeOutpost(i); //Tell Radar
                       }break;
            }
            if (checkRadarBox(Outpost[i].m_pos[0],Outpost[i].m_pos[1],Outpost[i].m_pos[2])) //Test FC
            {
                Outpost[i].drawOutpost();
            }
        }
    }

/*    //XXXXXXXXXXXXXXXXXXXXXX
	//Draw the guy
	glEnable(GL_LIGHTING);
	if (_model != NULL)
	{
		glPushMatrix();
		glTranslatef(500, 5, 450);

		glScalef(0.1f, 0.1f, 0.1f);
        _model->draw();
        _model->advance(0.004f);
		glPopMatrix();
	}
	glDisable(GL_LIGHTING);
	//XXXXXXXXXXXXXXXXXX*/
}

void showHUD(void)
{
    if (g_showHUD)
    {
        if (g_HUDtimer<300) g_HUDtimer+=6;
        spawnSeeker();
        showPlayerName();
    }
    else g_HUDtimer-=6;
    if (g_HUDtimer<0)
    {
        g_HUDtimer=0;
    }
}

void flash(float x, float y, float z)
{
    if (!checkFrustumBox(x,y,z)) return;
    g_flashTimer=5;
    g_xFlashPos=x;
    g_yFlashPos=y;
    g_zFlashPos=z;
}

void gameOverTest(void)
{
    int playersAlive=0;
    for (int i=0;i<g_maxPlayers;i++) //Count Players with Online Spawners
    {
        if (!Unit[i].m_online) continue;
        if (Unit[i].m_spawnerOnline) playersAlive++;
    }
    if (g_gameMode==1) //FFA
    {
        if (playersAlive<2) //Game Over
        {
             g_gameOver=true;
             if (Unit[g_playerID].m_spawnerOnline) g_victory=true;
             else g_victory=false;
        }
    }
    else if (g_gameMode==2) //Teams
    {
        int team1=0;
        int team2=0;
        for (int i=0;i<g_maxPlayers;i++)
        {
            if (Unit[i].m_online && Unit[i].m_spawnerOnline)
            {
                if (Unit[i].m_team==1) team1++;
                else team2++;
            }
        }
        if (team1==0) //Team 1 lost
        {
            if (Unit[g_playerID].m_team==2) {g_victory=true; Sound.playSimpleSound(Swin,1);}
            else Sound.playSimpleSound(Slost,1);
            g_gameOver=true;
        }
        if (team2==0) //Team 2 lost
        {
            if (Unit[g_playerID].m_team==1) {g_victory=true; Sound.playSimpleSound(Swin,1);}
            else Sound.playSimpleSound(Slost,1);
            g_gameOver=true;
        }
    }
}

void spawnSeeker(void)
{
    float dist=sqrt((g_xpos-Spawner[g_playerID].m_pos[0])*(g_xpos-Spawner[g_playerID].m_pos[0])+
                    (g_ypos-Spawner[g_playerID].m_pos[1])*(g_ypos-Spawner[g_playerID].m_pos[1])+
                    (g_zpos-Spawner[g_playerID].m_pos[2])*(g_zpos-Spawner[g_playerID].m_pos[2]));
    float tempDist=sqrt((g_xpos+g_xhed-Spawner[g_playerID].m_pos[0])*(g_xpos+g_xhed-Spawner[g_playerID].m_pos[0])+
                    (g_ypos+g_yhed-Spawner[g_playerID].m_pos[1])*(g_ypos+g_yhed-Spawner[g_playerID].m_pos[1])+
                    (g_zpos+g_zhed-Spawner[g_playerID].m_pos[2])*(g_zpos+g_zhed-Spawner[g_playerID].m_pos[2]));
    float dDist=dist-tempDist+1;
    g_dDist=dDist;
    g_beepDelay+=dDist;
    if (g_beepDelay>100)
    {
        g_beepDelay=0;
        Sound.playSimpleSound(Sbeep,1);
    }
}

void zoomingOff(void)
{
    g_zooming=false;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.01f,500.0f);
    glMatrixMode(GL_MODELVIEW);
    glFogf(GL_FOG_END, 150.0f);
}

void drawText(int ID)
{
    glPushMatrix();
    glTranslatef(-10,-5,-25);
    glColor4f(1,1,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    switch (ID)
    {
        case  1: {glVertexPointer(3, GL_FLOAT, 0, text5VertexArray); glDrawArrays(GL_LINES, 0, text5v/3);} break;
        case  2: {glVertexPointer(3, GL_FLOAT, 0, text15VertexArray); glDrawArrays(GL_LINES, 0, text15v/3);} break;
        case 20: {glVertexPointer(3, GL_FLOAT, 0, text20VertexArray); glDrawArrays(GL_LINES, 0, text20v/3);} break;
        case 21: {glVertexPointer(3, GL_FLOAT, 0, text21VertexArray); glDrawArrays(GL_LINES, 0, text21v/3);} break;
        case 22: {glVertexPointer(3, GL_FLOAT, 0, text22VertexArray); glDrawArrays(GL_LINES, 0, text22v/3);} break;
        case 23: {glVertexPointer(3, GL_FLOAT, 0, text23VertexArray); glDrawArrays(GL_LINES, 0, text23v/3);} break;
        case 24: {glVertexPointer(3, GL_FLOAT, 0, text24VertexArray); glDrawArrays(GL_LINES, 0, text24v/3);} break;
        case 25: {glVertexPointer(3, GL_FLOAT, 0, text25VertexArray); glDrawArrays(GL_LINES, 0, text25v/3);} break;
        case 26: {glVertexPointer(3, GL_FLOAT, 0, text26VertexArray); glDrawArrays(GL_LINES, 0, text26v/3);} break;
        case 27: {glVertexPointer(3, GL_FLOAT, 0, text27VertexArray); glDrawArrays(GL_LINES, 0, text27v/3);} break;
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void extraSound(void)
{
    //Unit Step sound
    for (int i=0;i<g_maxPlayers;i++)
    {
        if (Unit[i].m_online && Unit[i].m_spawnerOnline)
        {
            if (Unit[i].m_stepTimer>99)
            {
                Unit[i].m_stepTimer=0;
                float data[]={g_xpos-g_xhed,g_ypos-g_yhed,g_zpos-g_zhed,    g_xvel,g_yvel,g_zvel,
                              g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                              Unit[i].m_xpos,
                              Unit[i].m_ypos,
                              Unit[i].m_zpos,
                              0,0,0,   2,0.3,0};
                Sound.playWAVE(Sstep,data);
            }
        }
    }
}

void doDamage(int hurtingPlayer, int hurtObject, int against)
{
    enum objectTypes{oTerrain=1,oUnit,oSpawner,oTurret,oMine,oWall,oBuilding,oHeli,oSpeed,oTank,
                     oProj,oBBox,oBBlock,oPowerup,oRadar,oOutpost};
    float damage=0;
    int currWeapon;
    if (hurtingPlayer<100) //Unit with weapon
    {
        currWeapon=Unit[hurtingPlayer].m_currWeapon;
        if (Unit[hurtingPlayer].m_inTransportType==3)
        {
            currWeapon=10;
        }
    }
    else //Turret shooting
    {
        switch (Turret[hurtingPlayer-100].m_type)
        {
            case 1:{//Gattling
                    currWeapon=101;
                   }break;
            case 3:{//Laser
                    currWeapon=103;
                   }break;
        }
    }
    //Invunerable inside Tank
    if (against==oUnit) //Unit
    {
        if (Unit[hurtObject].m_inTransportType==3) //In tank, redirect damage to tank
        {
            against=oTank; //Tank
            hurtObject=Unit[hurtObject].m_transportID; //TankID
        }
    }
    switch (currWeapon)
    {
        case 0:  damage=30; if (against!=oUnit) damage=5; break;
        case 1:  damage=20; break;
        case 2:  damage=10; break;
        case 3:  damage=100; if (against!=oUnit) damage=20; break;
        case 4:  damage=50; if (against!=oUnit) damage=100; break;
        case 10: damage=20; break;
        case 101:damage=5; break;
        case 103:damage=200; break;
        default: damage=0; break;
    }
    switch (against)
    {
        case oUnit:     Unit[hurtObject].m_HP-=damage;
                        Unit[hurtObject].m_hitCount++;
                        Logfile.log(g_playerNames[hurtObject],"HP:",Unit[hurtObject].m_HP);
                        Logfile.log(" Position:",Unit[hurtObject].m_xpos+Unit[hurtObject].m_ypos+Unit[hurtObject].m_zpos);
                        break;
        case oSpawner:  Spawner[hurtObject].m_HP-=damage;
                        Spawner[hurtObject].m_hitCount++;
                        Logfile.log(" SPAWNER ID:",hurtObject,"HP:",Spawner[hurtObject].m_HP);
                        Logfile.log(" Position:",Spawner[hurtObject].m_pos[0]+Spawner[hurtObject].m_pos[1]+Spawner[hurtObject].m_pos[2]);
                        break;
        case oTurret:   Turret[hurtObject].m_HP-=damage;
                        Logfile.log(" TURRET ID:",hurtObject,"HP:",Turret[hurtObject].m_HP);
                        Logfile.log(" Position:",Turret[hurtObject].m_pos[0]+Turret[hurtObject].m_pos[1]+Turret[hurtObject].m_pos[2]);
                        break;
        case oMine:     Mine[hurtObject].m_HP-=damage;
                        Logfile.log(" MINE ID:",hurtObject,"HP:",Mine[hurtObject].m_HP);
                        Logfile.log(" Position:",Mine[hurtObject].m_pos[0]+Mine[hurtObject].m_pos[1]+Mine[hurtObject].m_pos[2]);
                        break;
        case oWall:     Wall[hurtObject].m_HP-=damage;
                        Logfile.log(" WALL ID:",hurtObject,"HP:",Wall[hurtObject].m_HP);
                        Logfile.log(" Position:",Wall[hurtObject].m_pos[0]+Wall[hurtObject].m_pos[1]+Wall[hurtObject].m_pos[2]);
                        break;
        case oBuilding: Building[hurtObject].m_HP-=damage;
                        Logfile.log(" BUILDING ID:",hurtObject,"HP:",Building[hurtObject].m_HP);
                        Logfile.log(" Position:",Building[hurtObject].m_pos[0]+Building[hurtObject].m_pos[1]+Building[hurtObject].m_pos[2]);
                        break;
        case oHeli:     Heli[hurtObject].m_HP-=damage;
                        Logfile.log(" HELI ID:",hurtObject,"HP:",Heli[hurtObject].m_HP);
                        Logfile.log(" Position:",Heli[hurtObject].m_pos[0]+Heli[hurtObject].m_pos[1]+Heli[hurtObject].m_pos[2]);
                        break;
        case oSpeed:    Speed[hurtObject].m_HP-=damage;
                        Logfile.log(" SPEED ID:",hurtObject,"HP:",Speed[hurtObject].m_HP);
                        Logfile.log(" Position:",Speed[hurtObject].m_pos[0]+Speed[hurtObject].m_pos[1]+Speed[hurtObject].m_pos[2]);
                        break;
        case oTank:     if (currWeapon==4) Tank[hurtObject].m_HP-=damage;
                        Logfile.log(" TANK ID:",hurtObject,"HP:",Tank[hurtObject].m_HP);
                        Logfile.log(" Position:",Tank[hurtObject].m_pos[0]+Tank[hurtObject].m_pos[1]+Tank[hurtObject].m_pos[2]);
                        break;
        case oBBox:     Buildbox[hurtObject].m_HP-=damage;
                        Logfile.log(" BUILDBOX ID:",hurtObject,"HP:",Buildbox[hurtObject].m_HP);
                        Logfile.log(" Position:",Buildbox[hurtObject].m_pos[0]+Buildbox[hurtObject].m_pos[1]+Buildbox[hurtObject].m_pos[2]);
                        break;
        case oBBlock:   Buildblock[hurtObject].m_HP-=damage;
                        Logfile.log(" BUILDBLOCK ID:",hurtObject,"HP:",Buildblock[hurtObject].m_HP);
                        Logfile.log(" Position:",Buildblock[hurtObject].m_pos[0]+Buildblock[hurtObject].m_pos[1]+Buildblock[hurtObject].m_pos[2]);
                        break;
        case oPowerup:  Powerup[hurtObject].m_HP-=damage;
                        Logfile.log(" POWERUP ID:",hurtObject,"HP:",Powerup[hurtObject].m_HP);
                        Logfile.log(" Position:",Powerup[hurtObject].m_pos[0]+Powerup[hurtObject].m_pos[1]+Powerup[hurtObject].m_pos[2]);
                        break;
        case oRadar:    Radar[hurtObject].m_HP-=damage;
                        Logfile.log(" RADAR ID:",hurtObject,"HP:",Radar[hurtObject].m_HP);
                        Logfile.log(" Position:",Radar[hurtObject].m_pos[0]+Radar[hurtObject].m_pos[1]+Radar[hurtObject].m_pos[2]);
                        break;
        case oOutpost:  Outpost[hurtObject].m_HP-=damage;
                        Logfile.log(" OUTPOST ID:",hurtObject,"HP:",Outpost[hurtObject].m_HP);
                        Logfile.log(" Position:",Outpost[hurtObject].m_pos[0]+Outpost[hurtObject].m_pos[1]+Outpost[hurtObject].m_pos[2]);
                        break;
    }
    if (against==0 && hurtObject==g_playerID) //You got hit, fall back and chainge aim
    {
        g_xvel+=Unit[hurtingPlayer].m_xdir*damage*0.01;
        g_yvel+=Unit[hurtingPlayer].m_ydir*damage*0.01;
        g_zvel+=Unit[hurtingPlayer].m_zdir*damage*0.01;
        //Aim-shift?
    }
}

void exitGame(void)
{
    if (g_gameState==gLobby)
    {
        g_lobbyStatus=0;
        g_wantToLeave=true;
        infoSwapLobby();
    }

    if (g_gameState==gInGame)
    {
    	g_online=false;
    	if (serverFound) sendPlayerData();
    }
    Sound.playSimpleSound(Sexit,0.1);
    Sleep(1500);
    WSACleanup (); //Close WinSock2
    Logfile.log("Game Shutdown");
    Logfile.closeFile();
}

void fireWeapon(int primSec,int unit)
{
    int currWeapon=Unit[unit].m_currWeapon;
    if (Unit[unit].m_inTransportType==3) currWeapon=10; //Tank
    if (primSec==1) //Primary Fire
    {
        int soundID=0;
        float volume=0;
        switch (currWeapon) //Play Sound
        {
            case 0: soundID=SshovelMiss;volume=0.5;
                    Unit[unit].m_meleeAttackProg=0.01;
                    break; //Melee
            case 1: soundID=Srevolver;volume=2;
                    Unit[unit].m_rifleAttackProg=0.01;
                    flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                    break; //Revolver
            case 2: soundID=Suzi;volume=2;
                    Unit[unit].m_rifleAttackProg=0.01;
                    flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                    break; //Uzi
            case 3: soundID=Ssniper;volume=2;
                    Unit[unit].m_rifleAttackProg=0.01;
                    flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                    break; //Sniper
            case 4:{
                    soundID=SrocketLaunch;volume=2;
                    Unit[unit].m_rifleAttackProg=0.01;
                    flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                    int slot=-1;
                    for (int p=0;p<g_maxProj;p++)
                    {
                        if (!Proj[p].m_active)
                        {
                            slot=p; break;
                        }
                    }
                    if (slot!=-1)
                    {
                        float data[7]={Unit[unit].m_xpos,Unit[unit].m_ypos+0.6,Unit[unit].m_zpos,
                                       Unit[unit].m_yheadhed,Unit[unit].m_zhed,unit,1};
                        Proj[slot].newProj(data);
                    }
                   }break; //RL
            case 5: break; //Radar
            case 10:soundID=Stank_cannon;volume=2;
                    Tank[Unit[unit].m_transportID].m_cannonProg=0.01;
                    flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                    int slot=-1;
                    for (int p=0;p<g_maxProj;p++)
                    {
                        if (!Proj[p].m_active)
                        {
                            slot=p; break;
                        }
                    }
                    if (slot!=-1)
                    {
                        int objectType=bulletHitTest(unit);
                        int color=9; //Black debris
                        if (objectType==1) color=20; //Terrain impact
                        if (objectType!=0) explosionAt(g_tempPos[0],g_tempPos[2],g_tempPos[1],1,100,color);
                    }
                    break; //Tank Cannon
        }
        if (soundID!=0) makeSound(soundID,unit,volume,2);
        if (unit==g_playerID) //Set fireDelay
        {
            switch (currWeapon)
            {
                case 0: g_fireDelay=100;g_muzzleFlash=0;break; //Melee
                case 1: g_fireDelay=100;g_muzzleFlash=10;break; //Revolver
                case 2: g_fireDelay=25;g_muzzleFlash=5;break; //Uzi
                case 3: g_fireDelay=200;g_muzzleFlash=10;break; //Sniper
                case 4: g_fireDelay=150;g_muzzleFlash=15;break; //RL
                case 5: break; //Radar
                case 10:g_fireDelay=150;g_muzzleFlash=15;break; //Tank Cannon
            }
        }
        if (currWeapon<4) //Shootable Weapon
        {
            int iHitObject=bulletHitTest(unit); //Hit Test
            if (iHitObject!=0) impact(iHitObject,unit); //Something was hit
            Logfile.log(g_playerNames[unit],"did not hit anything");
        }
        else //ex. Placeble Weapon
        {

        }
    }
    if (primSec==2) //Secondary Fire
    {
        switch (currWeapon)
        {
            case 0:{//Melee
                    interaction(20,unit);
                    Unit[unit].m_interactionProg=0.01;
                    if (unit==g_playerID) g_fireDelay=40;
                   }break;
            case 1:{//Revolver

                   }break;
            case 2:{//Uzi

                   }break;
            case 3:{//Sniper Zoom
                    if (unit==g_playerID)
                    {
                        if (!g_zooming && g_view==vFPS)
                        {
                            g_zooming=true;
                            glMatrixMode(GL_PROJECTION);
                            glLoadIdentity();
                            gluPerspective(10.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.01f,800.0f);
                            glMatrixMode(GL_MODELVIEW);
                            glFogf(GL_FOG_END, 200.0f);
                        }
                        else
                        {
                            g_zooming=false;
                            glMatrixMode(GL_PROJECTION);
                            glLoadIdentity();
                            gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.01f,500.0f);
                            glMatrixMode(GL_MODELVIEW);
                            glFogf(GL_FOG_END, 150.0f);
                        }
                        g_fireDelay=50;
                    }
                    Unit[unit].m_zooming=!Unit[unit].m_zooming;
                    break;}
            case 10:{//Tank Machinegun
                     Tank[Unit[unit].m_transportID].m_machinegunProg=0.01;
                     flash(Unit[unit].m_xpos,Unit[unit].m_ypos+1,Unit[unit].m_zpos);
                     makeSound(Stank_gun,unit,3,2);
                     int iHitObject=bulletHitTest(unit); //Hit Test
                     if (iHitObject!=0) impact(iHitObject,unit); //Something was hit
                     if (unit==g_playerID) {g_fireDelay=30;g_muzzleFlash=5;}
                    }break;
        }
    }
}

void randomEvent(int NewRandom,int LastRandom)
{
    if ( NewRandom-10000 < 60 )
    {
        srand(LastRandom);
        //Make Power-up
        if (g_numOfPowerups<g_maxNumOfPowerups)
        {
            //Find location
            int xpos=rand()%(g_terrainWidth-101)+50;
            int zpos=rand()%(g_terrainLength-101)+50;
            //Check if pos is OK
            if (World.getObjectHeight(xpos,zpos)==0)
            {
                int nextPowerupID=-1;
                for (int ID=0;ID<g_maxNumOfPowerups;ID++)
                {
                    if (!Powerup[ID].m_active) {nextPowerupID=ID; break;}
                }
                if (nextPowerupID!=-1)
                {
                    Powerup[nextPowerupID].newPowerup(xpos,World.getHeight(xpos,zpos),zpos);
                    float data[]={g_xpos-g_xhed,g_ypos-g_yhed,g_zpos-g_zhed,    g_xvel,g_yvel,g_zvel,
                                  g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                                  Powerup[nextPowerupID].m_pos[0],
                                  Powerup[nextPowerupID].m_pos[1],
                                  Powerup[nextPowerupID].m_pos[2],
                                  0,0,0,   1,2,0};
                    Sound.playWAVE(SpowerupSpawn,data);
                    g_numOfPowerups++;
                    Logfile.log("POWERUP created at",xpos,zpos);
                }
            }
        }
    }
}

int testCollision(float x,float y,float z)
{
    //Player-terrain
    if (y<World.getHeight(x,z)+World.getObjectHeight(x,z)) return -10;
    //Player-Player
    for (int i=0;i<g_maxPlayers;i++)
    {
        if (Unit[i].m_online)
        {
            if (i==g_playerID) continue;
            if (!Unit[i].m_spawning && Unit[i].m_spawnerOnline) //No col with spawning Units
            {
                if (Unit[i].unitBulletHitTest(x,y,z))
                return -i-1;
            }
        }
    }
    //Player-Spawner
    for (int i=0;i<g_maxPlayers;i++)
    {
        if (Spawner[i].m_active)
        {
            if (Unit[g_playerID].m_carrySpawner)
            {
                if (Unit[g_playerID].m_carryID==i) continue; //Do not collide with Spawner on back
            }
            if (Spawner[i].spawnerBulletHitTest(x,y,z))
            return i+1;
        }
    }
    //Player-Heli
    for (int i=0;i<g_maxHelis;i++)
    {
        if (Heli[i].m_active)
        {
            if (Heli[i].heliBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-Speed
    for (int i=0;i<g_maxSpeeds;i++)
    {
        if (Speed[i].m_active)
        {
            if (Speed[i].speedBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-Tank
    for (int i=0;i<g_maxTanks;i++)
    {
        if (Tank[i].m_active)
        {
            if (Tank[i].tankBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-BBox
    for (int i=0;i<g_maxBuildboxes;i++)
    {
        if (Buildbox[i].m_active)
        {
            if (Buildbox[i].buildboxBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-BBlock
    for (int i=0;i<g_maxBuildblocks;i++)
    {
        if (Buildblock[i].m_active)
        {
            if (Buildblock[i].buildblockBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-Powerup
    for (int i=0;i<g_maxNumOfPowerups;i++)
    {
        if (Powerup[i].m_active)
        {
            if (Powerup[i].powerupBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Player-Outpost
    for (int i=0;i<g_maxOutposts;i++)
    {
        if (Outpost[i].m_active)
        {
            if (Outpost[i].outpostBulletHitTest(x,y,z))
            return 10; //Does not matter, just not 0
        }
    }
    //Pos OK
    return 0;
}

void gameInit(void)
{
    SetCursorPos(300,240);
    ShowCursor(false);
    if (!serverFound)
    {
        g_seed=1;
        g_numOfPlayers=1;
        g_playerID=0;
        g_gameMode=1;
        //Store the Name
        for (int i=0;i<20;i++) g_playerNames[0][i]=g_playerName[i];
        float* VertexArray;
        VertexArray=getTextVertexArray(g_playerName, 15);
        for (int i=0;i<VertexArray[0];i++) text20VertexArray[i]=VertexArray[i+1];
        text20v=(int)VertexArray[0]-1;
        //Erase Lobby Data
        for (int i=0;i<g_maxPlayers;i++)
        {
            g_allPlayerLobbyData[i][0]=0;
        }
        g_allPlayerLobbyData[0][0]=2; //Not yourself
        g_allPlayerLobbyData[0][1]=1; //On team 1
        Unit[0].m_online=true;
        Unit[0].m_spawnerOnline=true;
    }
    else //Count players online
    {
        int numOfPlayers=0;
        for (int i=0;i<g_maxPlayers;i++)
        {
            if (g_allPlayerLobbyData[i][0]==2) numOfPlayers++;
        }
        g_numOfPlayers=numOfPlayers;
        Logfile.log("Number of Players:",numOfPlayers);
    }
    //Log GameMode
    if (g_gameMode==1) Logfile.log("GameMode: Free For All");
    else if (g_gameMode==2) Logfile.log("GameMode: Teams");

    srand(g_seed);
    genSpawnerPos();
    World.setPerlinNoise(g_seed);
    switch (g_gameMode) //gen start chunks
    {
        case gmFFA: World.firstChunks(g_spawnXpos[g_playerID],g_spawnZpos[g_playerID]); break;
        case gmTEAM:
            {
                if (g_allPlayerLobbyData[g_playerID][1]==1) World.firstChunks(g_spawnXpos[0],g_spawnZpos[0]);
                else World.firstChunks(g_spawnXpos[1],g_spawnZpos[1]);
            }break;
    }

    genStars();
    //genSomeStartObjects();
    if (serverFound)
    {
        //Set teams
        for (int i=0;i<g_maxPlayers;i++)
        {
            if (g_allPlayerLobbyData[i][0]==2) //Player was ready
            {
                Unit[i].m_online=true;
                Unit[i].m_spawnerOnline=true;
                Unit[i].m_playerID=i;
                if (g_gameMode==2) //Teams
                {
                    Unit[i].m_team=g_allPlayerLobbyData[i][1];
                }
                if (g_gameMode==1) //FFA
                {
                    Unit[i].m_team=0;
                }
            }
        }
        //Place Spawners
        if (!g_testServer) //Normal Mode
        {
            if (g_gameMode==2) //Teams
            {
                int team1Offset=0;
                int team2Offset=0;
                for (int i=0;i<g_maxPlayers;i++)
                {
                    if (Unit[i].m_online)
                    {
                        if (Unit[i].m_team==1)
                        {
                            Spawner[i].newSpawner(Unit[i].m_team,i,g_spawnXpos[0]+team1Offset,g_spawnZpos[0],World.getHeight(g_spawnXpos[0]+team1Offset,g_spawnZpos[0]));
                            team1Offset+=2;
                        }
                        else //if (Unit[i].m_team==2)
                        {
                            Spawner[i].newSpawner(Unit[i].m_team,i,g_spawnXpos[1]+team2Offset,g_spawnZpos[1],World.getHeight(g_spawnXpos[1]+team2Offset,g_spawnZpos[1]));
                            team2Offset+=2;
                        }
                    }
                }
            }
            else if (g_gameMode==1) //FFA
            {
                for (int i=0;i<g_maxPlayers;i++)
                {
                    if (Unit[i].m_online)
                    {
                        Spawner[i].newSpawner(Unit[i].m_team,i,g_spawnXpos[i],g_spawnZpos[i],World.getHeight(g_spawnXpos[i],g_spawnZpos[i]));
                    }
                }
            }
        }
        else //Test Mode
        {
            int offset=0;
            for (int i=0;i<g_maxPlayers;i++)
            {
                if (Unit[i].m_online)
                {
                    Spawner[i].newSpawner(Unit[i].m_team,i,g_spawnXpos[0]+offset,g_spawnZpos[0],World.getHeight(g_spawnXpos[0]+offset,g_spawnZpos[0]));
                    offset+=3;
                }
            }
        }
    }
    else //Offline Play
    {
        g_testServer=true;

        Unit[0].m_team=0;
        g_xpos=g_spawnXpos[g_playerID]; g_ypos=10; g_zpos=g_spawnZpos[g_playerID];
        Spawner[0].newSpawner(0,0,g_xpos,g_zpos,World.getHeight(g_xpos,g_zpos));
        Spawner[0].m_open=true;

        Powerup[0].newPowerup(g_xpos+10,World.getHeight(g_xpos+10,g_zpos),g_zpos);

        Building[0].newBuilding(g_xpos+30,World.getHeight(g_xpos+30,g_zpos-20),g_zpos-20,1,0);
        Building[1].newBuilding(g_xpos+30,World.getHeight(g_xpos+30,g_zpos-10),g_zpos-10,2,0);
        Building[2].newBuilding(g_xpos+30,World.getHeight(g_xpos+30,g_zpos),g_zpos,3,0);
        Building[3].newBuilding(g_xpos+30,World.getHeight(g_xpos+30,g_zpos+10),g_zpos+10,6,0);

        Heli[0].newHeli(g_xpos,World.getHeight(g_xpos,g_zpos+10),g_zpos+10);
        Speed[0].newSpeed(g_xpos+5,World.getHeight(g_xpos+5,g_zpos+10),g_zpos+10);
        Tank[0].newTank(g_xpos+10,World.getHeight(g_xpos+10,g_zpos+10),g_zpos+10);

        Radar[0].newRadar(g_xpos+10,World.getHeight(g_xpos+10,g_zpos-10),g_zpos-10,0);
    }
    g_specUnit=g_playerID;
    g_xpos=Spawner[g_playerID].m_pos[0]+0.5;
    g_ypos=Spawner[g_playerID].m_pos[1]+5;
    g_zpos=Spawner[g_playerID].m_pos[2]+0.5;
}
bool checkRadarBox(float x,float y,float z)
{
    float tempx;
    float tempy;
    float tempz;
    float tempdist;
    float dist;
    //check other planes
    tempx=g_pointNear[0];
    tempy=g_pointNear[1];
    tempz=g_pointNear[2];
    dist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    //check right plane
    tempx=g_pointNear[0]+g_planeRight[0];
    tempy=g_pointNear[1]+g_planeRight[1];
    tempz=g_pointNear[2]+g_planeRight[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check left plane
    tempx=g_pointNear[0]+g_planeLeft[0];
    tempy=g_pointNear[1]+g_planeLeft[1];
    tempz=g_pointNear[2]+g_planeLeft[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check top plane
    tempx=g_pointNear[0]+g_planeTop[0];
    tempy=g_pointNear[1]+g_planeTop[1];
    tempz=g_pointNear[2]+g_planeTop[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check bottom plane
    tempx=g_pointNear[0]+g_planeBottom[0];
    tempy=g_pointNear[1]+g_planeBottom[1];
    tempz=g_pointNear[2]+g_planeBottom[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    return true;
}

bool checkFrustumBox(float x,float y,float z)
{
    //check far plane
    float tempx=g_pointFar[0];
    float tempy=g_pointFar[1];
    float tempz=g_pointFar[2];
    float tempdist;
    float dist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    tempx=g_pointFar[0]+g_planeFar[0];
    tempy=g_pointFar[1]+g_planeFar[1];
    tempz=g_pointFar[2]+g_planeFar[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check near plane
    tempx=g_pointNear[0];
    tempy=g_pointNear[1];
    tempz=g_pointNear[2];
    dist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    tempx=g_pointNear[0]+g_planeNear[0];
    tempy=g_pointNear[1]+g_planeNear[1];
    tempz=g_pointNear[2]+g_planeNear[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check other planes
    tempx=g_pointNear[0];
    tempy=g_pointNear[1];
    tempz=g_pointNear[2];
    dist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    //check right plane
    tempx=g_pointNear[0]+g_planeRight[0];
    tempy=g_pointNear[1]+g_planeRight[1];
    tempz=g_pointNear[2]+g_planeRight[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check left plane
    tempx=g_pointNear[0]+g_planeLeft[0];
    tempy=g_pointNear[1]+g_planeLeft[1];
    tempz=g_pointNear[2]+g_planeLeft[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check top plane
    tempx=g_pointNear[0]+g_planeTop[0];
    tempy=g_pointNear[1]+g_planeTop[1];
    tempz=g_pointNear[2]+g_planeTop[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    //check bottom plane
    tempx=g_pointNear[0]+g_planeBottom[0];
    tempy=g_pointNear[1]+g_planeBottom[1];
    tempz=g_pointNear[2]+g_planeBottom[2];
    tempdist=((x-tempx)*(x-tempx)+(y-tempy)*(y-tempy)+(z-tempz)*(z-tempz));
    if (tempdist>dist) return false;
    return true;
}

void loadSound(void)
{
    for (int i=0;i<20;i++) g_soundArr[i]=-1; //Clear sound list

    if (g_soundOK) return;
    if (Sound.error==0)
    if (Sound.loadWAVE(Serase,"Res/Sound/erase.wav"))
    if (Sound.loadWAVE(Sexplosion,"Res/Sound/explosion.wav"))
    if (Sound.loadWAVE(ShitUnit,"Res/Sound/hitUnit.wav"))
    if (Sound.loadWAVE(Sjump,"Res/Sound/jump.wav"))
    if (Sound.loadWAVE(Sland,"Res/Sound/land.wav"))
    if (Sound.loadWAVE(Snohost,"Res/Sound/nohost.wav"))
    if (Sound.loadWAVE(Sselect,"Res/Sound/select.wav"))
    if (Sound.loadWAVE(Sselected,"Res/Sound/selected.wav"))
    if (Sound.loadWAVE(Sshot,"Res/Sound/shot.wav"))
    if (Sound.loadWAVE(ShitSpawn,"Res/Sound/hitSpawn.wav"))
    if (Sound.loadWAVE(ShitSpawnShield,"Res/Sound/hitSpawnShield.wav"))
    if (Sound.loadWAVE(SrocketLaunch,"Res/Sound/rocketLaunch.wav"))
    if (Sound.loadWAVE(Srevolver,"Res/Sound/revolver.wav"))
    if (Sound.loadWAVE(Suzi,"Res/Sound/uzi.wav"))
    if (Sound.loadWAVE(Ssniper,"Res/Sound/sniper.wav"))
    if (Sound.loadWAVE(SshovelMiss,"Res/Sound/shovelMiss.wav"))
    if (Sound.loadWAVE(Sexit,"Res/Sound/exit.wav"))
    if (Sound.loadWAVE(Sdeath,"Res/Sound/death.wav"))
    if (Sound.loadWAVE(Sspawning,"Res/Sound/spawning.wav"))
    if (Sound.loadWAVE(Sdig,"Res/Sound/dig.wav"))
    if (Sound.loadWAVE(Sundig,"Res/Sound/undig.wav"))
    if (Sound.loadWAVE(Sstep,"Res/Sound/step.wav"))
    if (Sound.loadWAVE(Sbeep,"Res/Sound/beep.wav"))
    if (Sound.loadWAVE(Swin,"Res/Sound/win.wav"))
    if (Sound.loadWAVE(Slost,"Res/Sound/lost.wav"))
    if (Sound.loadWAVE(SpowerupDelivery,"Res/Sound/powerupDelivery.wav"))
    if (Sound.loadWAVE(SpowerupPickup,"Res/Sound/powerupPickup.wav"))
    if (Sound.loadWAVE(SpowerupSpawn,"Res/Sound/powerupSpawn.wav"))
    if (Sound.loadWAVE(Smine,"Res/Sound/mine.wav"))
    if (Sound.loadWAVE(SplaceObject,"Res/Sound/placeObject.wav"))
    if (Sound.loadWAVE(Stank_gun,"Res/Sound/tank_gun.wav"))
    if (Sound.loadWAVE(Stank_cannon,"Res/Sound/tank_cannon.wav"))
    if (Sound.loadWAVE(Sturret_laser,"Res/Sound/turret_laser.wav"))
    if (Sound.loadWAVE(Sturret_gattling,"Res/Sound/turret_gattling.wav"))
    if (Sound.loadWAVE(Stank_motor,"Res/Sound/tank_motor.wav"))
    if (Sound.loadWAVE(Sheli_motor,"Res/Sound/heli_motor.wav"))
    if (Sound.loadWAVE(Sspeed_motor,"Res/Sound/speed_motor.wav"))
    {g_soundOK=true;return;}
    g_soundError=Sound.error;
}

void explosionAt(float xf, float zf, float y, int type, int size, int color)
{
    int x=(int)xf; int z=(int)zf;
    //Type: 0 non-destructive,  1 Destructive,  2 non-destructive,Soundless
    //Find nextExplosionID
    int slot=-1;
    for (int i=0;i<g_maxExplosion;i++)
    {
        if (!Explosion[i].m_active)
        {
            slot=i;
            break;
        }
        //if (i==g_maxExplosion-1) g_nextExplosionID=-1;
    }
    if (slot!=-1)
    {
         if (color<9) //Player related color debris
         {
             if (g_gameMode==2) //Teams
             {
                 color=Unit[color].m_team-1;
             }
         }
         else if (color==20) //Terain related debris
         {
//             if (g_terrTex[x][z]!=0) color=12; //Mountain Texture
             if (World.getHeight(x,z)<0) color=11; //Stone Texture
//             if (g_terrTex[x][z]==0 && World.getHeight(x,z)>0) color=10; //Sand Texture
         }
         Explosion[slot].newExplosion(xf,y,zf,size,color);
         if (type==2) return; //No sound
         //Sound
         float data[]={g_xpos-g_xhed,g_ypos-g_yhed,g_zpos-g_zhed,    g_xvel,g_yvel,g_zvel,
                       g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                       x,y,z,  0,0,0,   1,10,0};
         Sound.playWAVE(Sexplosion,data);
         if (type==0) return; //No destruction
         if (type==1)
         {
             //Destruction
             float expPower=0.01*(float)size;
             if (World.getHeight(x,z)>y) lowerGround(x,z,expPower*1);

             if (World.getHeight(x-1,z)>y) lowerGround(x-1,z,expPower*0.8);
             if (World.getHeight(x-1,z-1)>y) lowerGround(x-1,z-1,expPower*0.7);
             if (World.getHeight(x,z-1)>y) lowerGround(x,z-1,expPower*0.8);
             if (World.getHeight(x+1,z)>y) lowerGround(x+1,z,expPower*0.8);
             if (World.getHeight(x+1,z+1)>y) lowerGround(x+1,z+1,expPower*0.7);
             if (World.getHeight(x,z+1)>y) lowerGround(x,z+1,expPower*0.8);
             if (World.getHeight(x+1,z-1)>y) lowerGround(x+1,z-1,expPower*0.7);
             if (World.getHeight(x-1,z+1)>y) lowerGround(x-1,z+1,expPower*0.7);

             if (World.getHeight(x+2,z-1)>y) lowerGround(x+2,z-1,expPower*0.5);
             if (World.getHeight(x+2,z)>y) lowerGround(x+2,z,expPower*0.5);
             if (World.getHeight(x+2,z+1)>y) lowerGround(x+2,z+1,expPower*0.5);
             if (World.getHeight(x-2,z-1)>y) lowerGround(x-2,z-1,expPower*0.5);
             if (World.getHeight(x-2,z)>y) lowerGround(x-2,z,expPower*0.5);
             if (World.getHeight(x-2,z+1)>y) lowerGround(x-2,z+1,expPower*0.5);
             if (World.getHeight(x-1,z+2)>y) lowerGround(x-1,z+2,expPower*0.5);
             if (World.getHeight(x,z+2)>y) lowerGround(x,z+2,expPower*0.5);
             if (World.getHeight(x+1,z+2)>y) lowerGround(x+1,z+2,expPower*0.5);
             if (World.getHeight(x-1,z-2)>y) lowerGround(x-1,z-2,expPower*0.5);
             if (World.getHeight(x,z-2)>y) lowerGround(x,z-2,expPower*0.5);
             if (World.getHeight(x+1,z-2)>y) lowerGround(x+1,z-2,expPower*0.5);
             //HP Damage
             float blastRadius=0.04*(float)size;
             for (int i=0;i<g_maxPlayers;i++)
             {
                 if (Unit[i].m_online)
                 {
                     //Players
                     if (Unit[i].m_xpos<x+blastRadius && Unit[i].m_xpos>x-blastRadius &&
                         Unit[i].m_ypos<y+blastRadius && Unit[i].m_ypos>y-blastRadius &&
                         Unit[i].m_zpos<z+blastRadius && Unit[i].m_zpos>z-blastRadius)
                     {
                         if (Unit[i].m_inTransportType==3) continue;
                         Unit[i].m_HP-=50;
                         if (size>100) Unit[i].m_HP-=50;
                         if (i==g_playerID) g_yvel+=0.1;
                         Logfile.log(g_playerNames[i],"was harmed in EXPLOSION, HP:",Unit[i].m_HP);
                     }
                     //Spawners
                     if (Unit[i].m_spawning)
                     {
                         if (Spawner[i].m_pos[0]<x+blastRadius && Spawner[i].m_pos[0]>x-blastRadius &&
                             Spawner[i].m_pos[1]<y+blastRadius && Spawner[i].m_pos[1]>y-blastRadius &&
                             Spawner[i].m_pos[2]<z+blastRadius && Spawner[i].m_pos[2]>z-blastRadius)
                         {
                             Spawner[i].m_HP-=40;
                             Logfile.log(g_playerNames[i],"'s SPAWNER was harmed in EXPLOSION, HP:",Spawner[i].m_HP);
                         }
                     }
                 }
             }
             //Turrets
             for (int i=0;i<g_maxTurrets;i++)
             {
                 if (Turret[i].m_active)
                 {
                     if (Turret[i].m_pos[0]<x+blastRadius && Turret[i].m_pos[0]>x-blastRadius &&
                         Turret[i].m_pos[1]<y+blastRadius && Turret[i].m_pos[1]>y-blastRadius &&
                         Turret[i].m_pos[2]<z+blastRadius && Turret[i].m_pos[2]>z-blastRadius)
                     {
                         Turret[i].m_HP-=50;
                         Logfile.log("TURRET ID:",i,"was harmed in EXPLOSION, HP:",Turret[i].m_HP);
                     }
                 }
             }
             //Mines
             for (int i=0;i<g_maxMines;i++)
             {
                 if (Mine[i].m_active)
                 {
                     if (Mine[i].m_pos[0]<x+blastRadius && Mine[i].m_pos[0]>x-blastRadius &&
                         Mine[i].m_pos[1]<y+blastRadius && Mine[i].m_pos[1]>y-blastRadius &&
                         Mine[i].m_pos[2]<z+blastRadius && Mine[i].m_pos[2]>z-blastRadius)
                     {
                         Mine[i].m_HP-=50;
                         Logfile.log("MINE ID:",i,"was harmed in EXPLOSION, HP:",Mine[i].m_HP);
                     }
                 }
             }
             //Walls
             for (int i=0;i<g_maxWalls;i++)
             {
                 if (Wall[i].m_active)
                 {
                     if (Wall[i].m_pos[0]<x+blastRadius && Wall[i].m_pos[0]>x-blastRadius &&
                         Wall[i].m_pos[1]<y+blastRadius && Wall[i].m_pos[1]>y-blastRadius &&
                         Wall[i].m_pos[2]<z+blastRadius && Wall[i].m_pos[2]>z-blastRadius)
                     {
                         Wall[i].m_HP-=50;
                         Logfile.log("WALL ID:",i,"was harmed in EXPLOSION, HP:",Wall[i].m_HP);
                     }
                 }
             }
             //Buildings
             for (int i=0;i<g_maxBuildings;i++)
             {
                 if (Building[i].m_active)
                 {
                     if (Building[i].m_pos[0]<x+blastRadius && Building[i].m_pos[0]>x-blastRadius &&
                         Building[i].m_pos[1]<y+blastRadius && Building[i].m_pos[1]>y-blastRadius &&
                         Building[i].m_pos[2]<z+blastRadius && Building[i].m_pos[2]>z-blastRadius)
                     {
                         Building[i].m_HP-=50;
                         Logfile.log("BUILDING ID:",i,"was harmed in EXPLOSION, HP:",Building[i].m_HP);
                     }
                 }
             }
             //Powerups
             for (int i=0;i<g_maxNumOfPowerups;i++)
             {
                 if (Powerup[i].m_active)
                 {
                     if (Powerup[i].m_pos[0]<x+blastRadius && Powerup[i].m_pos[0]>x-blastRadius &&
                         Powerup[i].m_pos[1]<y+blastRadius && Powerup[i].m_pos[1]>y-blastRadius &&
                         Powerup[i].m_pos[2]<z+blastRadius && Powerup[i].m_pos[2]>z-blastRadius)
                     {
                         Powerup[i].m_HP-=50;
                         Logfile.log("POWERUP ID:",i,"was harmed in EXPLOSION, HP:",Powerup[i].m_HP);
                     }
                 }
             }
             //BBox
             for (int i=0;i<g_maxBuildboxes;i++)
             {
                 if (Buildbox[i].m_active)
                 {
                     if (Buildbox[i].m_pos[0]<x+blastRadius && Buildbox[i].m_pos[0]>x-blastRadius &&
                         Buildbox[i].m_pos[1]<y+blastRadius && Buildbox[i].m_pos[1]>y-blastRadius &&
                         Buildbox[i].m_pos[2]<z+blastRadius && Buildbox[i].m_pos[2]>z-blastRadius)
                     {
                         Buildbox[i].m_HP-=50;
                         Logfile.log("BUILDBOX ID:",i,"was harmed in EXPLOSION, HP:",Buildbox[i].m_HP);
                     }
                 }
             }
             //BBlock
             for (int i=0;i<g_maxBuildblocks;i++)
             {
                 if (Buildblock[i].m_active)
                 {
                     if (Buildblock[i].m_pos[0]<x+blastRadius && Buildblock[i].m_pos[0]>x-blastRadius &&
                         Buildblock[i].m_pos[1]<y+blastRadius && Buildblock[i].m_pos[1]>y-blastRadius &&
                         Buildblock[i].m_pos[2]<z+blastRadius && Buildblock[i].m_pos[2]>z-blastRadius)
                     {
                         Buildblock[i].m_HP-=50;
                         Logfile.log("BUILDBLOCK ID:",i,"was harmed in EXPLOSION, HP:",Buildblock[i].m_HP);
                     }
                 }
             }
             //Heli
             for (int i=0;i<g_maxHelis;i++)
             {
                 if (Heli[i].m_active)
                 {
                     if (Heli[i].m_pos[0]<x+blastRadius && Heli[i].m_pos[0]>x-blastRadius &&
                         Heli[i].m_pos[1]<y+blastRadius && Heli[i].m_pos[1]>y-blastRadius &&
                         Heli[i].m_pos[2]<z+blastRadius && Heli[i].m_pos[2]>z-blastRadius)
                     {
                         Heli[i].m_HP-=50;
                         Logfile.log("HELI ID:",i,"was harmed in EXPLOSION, HP:",Heli[i].m_HP);
                     }
                 }
             }
             //Speed
             for (int i=0;i<g_maxSpeeds;i++)
             {
                 if (Speed[i].m_active)
                 {
                     if (Speed[i].m_pos[0]<x+blastRadius && Speed[i].m_pos[0]>x-blastRadius &&
                         Speed[i].m_pos[1]<y+blastRadius && Speed[i].m_pos[1]>y-blastRadius &&
                         Speed[i].m_pos[2]<z+blastRadius && Speed[i].m_pos[2]>z-blastRadius)
                     {
                         Speed[i].m_HP-=50;
                         Logfile.log("SPEED ID:",i,"was harmed in EXPLOSION, HP:",Speed[i].m_HP);
                     }
                 }
             }
             //Tank
             for (int i=0;i<g_maxTanks;i++)
             {
                 if (Tank[i].m_active)
                 {
                     if (Tank[i].m_pos[0]<x+blastRadius && Tank[i].m_pos[0]>x-blastRadius &&
                         Tank[i].m_pos[1]<y+blastRadius && Tank[i].m_pos[1]>y-blastRadius &&
                         Tank[i].m_pos[2]<z+blastRadius && Tank[i].m_pos[2]>z-blastRadius)
                     {
                         Tank[i].m_HP-=50;
                         Logfile.log("TANK ID:",i,"was harmed in EXPLOSION, HP:",Tank[i].m_HP);
                     }
                 }
             }
             //Radar
             for (int i=0;i<g_maxRadars;i++)
             {
                 if (Radar[i].m_active)
                 {
                     if (Radar[i].m_pos[0]<x+blastRadius && Radar[i].m_pos[0]>x-blastRadius &&
                         Radar[i].m_pos[1]<y+blastRadius && Radar[i].m_pos[1]>y-blastRadius &&
                         Radar[i].m_pos[2]<z+blastRadius && Radar[i].m_pos[2]>z-blastRadius)
                     {
                         Radar[i].m_HP-=50;
                         Logfile.log("RADAR ID:",i,"was harmed in EXPLOSION, HP:",Radar[i].m_HP);
                     }
                 }
             }
             //Outpost
             for (int i=0;i<g_maxOutposts;i++)
             {
                 if (Outpost[i].m_active)
                 {
                     if (Outpost[i].m_pos[0]<x+blastRadius && Outpost[i].m_pos[0]>x-blastRadius &&
                         Outpost[i].m_pos[1]<y+blastRadius && Outpost[i].m_pos[1]>y-blastRadius &&
                         Outpost[i].m_pos[2]<z+blastRadius && Outpost[i].m_pos[2]>z-blastRadius)
                     {
                         Outpost[i].m_HP-=50;
                         Logfile.log("OUTPOST ID:",i,"was harmed in EXPLOSION, HP:",Outpost[i].m_HP);
                     }
                 }
             }
         }
    }
    //Flash
    flash(x,y+1,z);
}

void collisionDetection(void)
{
     //Proj Collision
     for (int i=0;i<g_maxProj;i++)
     {
         if (Proj[i].m_active)
         {
             //Terrain
             {
                 if (World.getHeight((int)Proj[i].m_xpos,(int)Proj[i].m_zpos)>Proj[i].m_ypos)
                 {
                     explosionAt(Proj[i].m_xpos,Proj[i].m_zpos,Proj[i].m_ypos,1,100,20);
                     Proj[i].m_active=false;
                 }
             }
             //Unit-proj
             for (int j=0;j<g_maxPlayers;j++)
             if (Unit[j].m_online)
             {
                 if (Unit[j].unitBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     if (Proj[i].m_fromPlayer==j) continue;
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }

             //Spawner-proj
             for (int j=0;j<g_maxPlayers;j++)
             if (Spawner[j].m_active)
             {
                 if (Spawner[j].spawnerBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }

             //Turret-proj
             for (int j=0;j<g_maxTurrets;j++)
             if (Turret[j].m_active)
             {
                 if (Turret[j].turretBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     if (Proj[i].m_fromPlayer-100==j) continue;
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }

             //Wall-proj
             for (int j=0;j<g_maxWalls;j++)
             if (Wall[j].m_active)
             {
                 if (Wall[j].wallBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
             //Building-proj
             for (int j=0;j<g_maxBuildings;j++)
             if (Building[j].m_active)
             {
                 if (Building[j].buildingBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
             //Heli-proj
             for (int j=0;j<g_maxHelis;j++)
             if (Heli[j].m_active)
             {
                 if (Heli[j].heliBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
             //Speed-proj
             for (int j=0;j<g_maxSpeeds;j++)
             if (Speed[j].m_active)
             {
                 if (Speed[j].speedBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
             //Tank-proj
             for (int j=0;j<g_maxTanks;j++)
             if (Tank[j].m_active)
             {
                 if (Tank[j].tankBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     if (Tank[j].m_inUse) //Tank proj dont colide with its tank
                     {
                         if (Proj[i].m_fromPlayer==Tank[j].m_user) continue;
                     }
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
             //Radar-proj
             for (int j=0;j<g_maxRadars;j++)
             if (Radar[j].m_active)
             {
                 if (Radar[j].radarBulletHitTest(Proj[i].m_xpos,Proj[i].m_ypos,Proj[i].m_zpos))
                 {
                     Proj[i].m_active=false;
                     explosionAt((int)Proj[i].m_xpos,(int)Proj[i].m_zpos,Proj[i].m_ypos,1,100,9);
                 }
             }
         }
     }
     //Player-Mine
     for (int i=0;i<g_maxMines;i++)
     {
         if (Mine[i].m_active)
         {
             if (Mine[i].m_startDelay<1 && !Mine[i].m_trigged)
             {
                 for (int j=0;j<g_maxPlayers;j++)
                 {
                     if (Unit[j].m_online)
                     {
                         if (Mine[i].m_pos[0]>Unit[j].m_xpos-1-0.5 && Mine[i].m_pos[0]<Unit[j].m_xpos+1-0.5 &&
                             Mine[i].m_pos[1]>Unit[j].m_ypos-1-1.5 && Mine[i].m_pos[1]<Unit[j].m_ypos+1-1.5 &&
                             Mine[i].m_pos[2]>Unit[j].m_zpos-1-0.5 && Mine[i].m_pos[2]<Unit[j].m_zpos+1-0.5)
                         {
                             g_makeSound=Smine;
                             Mine[i].m_trigged=true;
                         }
                     }
                 }
             }
         }
     }
}

void drawHUD(void)
{
    //draw HUD
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    if (g_debugMode)
    {
        glPushMatrix();
            glTranslatef(15,15,-40);
            glColor4f(0,1,0,1);
            //numbers
            drawTal(avgFPS); glTranslatef(0,-2.5,0);
            drawTal(Unit[g_playerID].m_carries); glTranslatef(0,-2.5,0);
            drawTal(g_headingXZ); glTranslatef(0,-2.5,0);
            drawTal(g_headingY); glTranslatef(0,-2.5,0);
            glTranslatef(0,-1,0);
            drawTal(g_xpos/100); glTranslatef(0,-2.5,0);
            drawTal(g_ypos); glTranslatef(0,-2.5,0);
            drawTal(g_zpos/100); glTranslatef(0,-2.5,0);
            glTranslatef(0,-1,0);
            drawTal(g_debug0/100); glTranslatef(0,-2.5,0);
            drawTal(0); glTranslatef(0,-2.5,0);
            drawTal(0); glTranslatef(0,-2.5,0);
            drawTal(0); glTranslatef(0,-2.5,0);
            drawTal(0); glTranslatef(0,-2.5,0);
        glPopMatrix();
    }
    if (g_showPlayerData)
    {
        displayPlayerData();
    }
    if (g_HUDtimer>1 && Unit[g_playerID].m_inTransportType==0)
    {
        glPushMatrix();
            glTranslatef(-0.5,-0.5,-3.5);
            glTranslatef(0.5,0,0);
            glColor4f(0.2,0.2,0.2,1);
            float progress=(float)g_HUDtimer/300;
            float color=0;
            //right - spawner content
            glPushMatrix();
                glTranslatef(2.0-progress,-0.25,0);
                glColor4f(0.25,0.25,0.25,1);
                glBegin(GL_QUADS);
                glVertex4f(0.0,0.0,0.0,1);
                glColor4f(0.1,0.1,0.1,1);
                glVertex4f(1.0,0.0,0.0,1);
                glVertex4f(1.0,1.5,0.0,1);
                glColor4f(0.25,0.25,0.25,1);
                glVertex4f(0.0,1.5,0.0,1);
                glEnd();
                //square
                glColor4f(0.1,0.6,0.15,1);
                glBegin(GL_QUADS);
                glVertex4f(0.15,0.65,0.0,1);
                glVertex4f(0.8,0.65,0.0,1);
                glColor4f(0.2,0.7,0.2,1);
                glVertex4f(0.49,1.3,0.0,1);
                glVertex4f(0.46,1.3,0.0,1);
                glEnd();
                glColor4f(0.9,0.9,0.9,1);
                glTranslatef(-0.05,0.3,0);
                glScalef(0.15,0.15,0.15);
                drawTal(Spawner[g_playerID].m_content);
            glPopMatrix();
            //left - beeper
            glPushMatrix();
                glTranslatef(-3.0+progress,-0.25,0.0);
                glColor4f(0.1,0.1,0.1,1);
                glBegin(GL_QUADS);
                glVertex4f(0.0,0.0,0.0,1);
                glColor4f(0.25,0.25,0.25,1);
                glVertex4f(1.0,0.0,0.0,1);
                glVertex4f(1.0,1.5,0.0,1);
                glColor4f(0.1,0.1,0.1,1);
                glVertex4f(0.0,1.5,0.0,1);
                glEnd();
                //square
                if (g_beepDelay<10) glColor4f(0.7,0.7,0.4,1);
                else glColor4f(0.3,0.3,0.4,1);
                glBegin(GL_QUADS);
                glVertex4f(0.85,0.25,0.0,1);
                glVertex4f(0.85,1.3,0.0,1);
                if (g_beepDelay<10) glColor4f(0.8,0.8,0.3,1);
                else glColor4f(0.2,0.2,0.2,1);
                glVertex4f(0.15,1.3,0.0,1);
                glVertex4f(0.15,0.25,0.0,1);
                glEnd();
                //icon
                glColor4f(0.9,0.9,0.9,1);
                glBegin(GL_LINE_STRIP);
                glVertex4f(0.3,0.5,0,1);
                glVertex4f(0.5,1.0,0,1);
                glVertex4f(0.7,0.5,0,1);
                glEnd();
            glPopMatrix();
            //up - spawner HP
            glPushMatrix();
                glTranslatef(-0.75,2.0-progress,0.0);
                glColor4f(0.25,0.25,0.25,1);
                //back square
                glBegin(GL_QUADS);
                glVertex4f(0.0,0.0,0.0,1);
                glVertex4f(1.5,0.0,0.0,1);
                glColor4f(0.1,0.1,0.1,1);
                glVertex4f(1.5,1.0,0.0,1);
                glVertex4f(0.0,1.0,0.0,1);
                glEnd();
                //square
                color=(float)Spawner[g_playerID].m_HP/400;
                glColor4f(0.4,color+0.4*color,0.6*color,1);
                glBegin(GL_QUADS);
                glVertex4f(0.2,0.1,0.0,1);
                glVertex4f(1.3,0.1,0.0,1);
                glColor4f(0.2,color+0.2*color,0.4*color,1);
                glVertex4f(1.3,0.85,0.0,1);
                glVertex4f(0.2,0.85,0.0,1);
                glEnd();
                //icon
                glColor4f(0.9,0.9,0.9,1);
                glBegin(GL_LINE_STRIP);
                glVertex4f(0.35,0.2,0,1);
                glVertex4f(0.75,0.8,0,1);
                glVertex4f(1.15,0.2,0,1);
                glEnd();
            glPopMatrix();
            //down - player HP
            glPushMatrix();
                glTranslatef(-0.75,-2.0+progress,0.0);
                glColor4f(0.1,0.1,0.1,1);
                glBegin(GL_QUADS);
                glVertex4f(0.0,0.0,0.0,1);
                glVertex4f(1.5,0.0,0.0,1);
                glColor4f(0.25,0.25,0.25,1);
                glVertex4f(1.5,1.0,0.0,1);
                glVertex4f(0.0,1.0,0.0,1);
                glEnd();
                //square
                color=(float)Unit[g_playerID].m_HP/200;
                glColor4f(0.2,color+0.2*color,0.4*color,1);
                glBegin(GL_QUADS);
                glVertex4f(0.2,0.1,0.0,1);
                glVertex4f(1.3,0.1,0.0,1);
                glColor4f(0.4,color+0.6*color,0.4*color,1);
                glVertex4f(1.3,0.85,0.0,1);
                glVertex4f(0.2,0.85,0.0,1);
                glEnd();
                //Draw Player Symbol
                glColor4f(0.9,0.9,0.9,1);
                glTranslatef(0.45,0.2,0);
                glScalef(1.5,1.5,1.5);
                glBegin(GL_LINE_STRIP);
                //Legs
                glVertex4f(0.05,0.0,0.0,1);
                glVertex4f(0.15,0.04,0.0,1);
                glVertex4f(0.2,0.1,0.0,1);
                glVertex4f(0.35,0.08,0.0,1);
                glVertex4f(0.3,0.0,0.0,1);
                glEnd();
                //Arms
                glBegin(GL_LINE_STRIP);
                glVertex4f(0.05,0.15,0.0,1);
                glVertex4f(0.09,0.19,0.0,1);
                glVertex4f(0.2,0.2,0.0,1);
                glVertex4f(0.3,0.18,0.0,1);
                glVertex4f(0.4,0.2,0.0,1);
                glEnd();
                //Head
                glBegin(GL_LINE_STRIP);
                glVertex4f(0.2,0.1,0.0,1);
                glVertex4f(0.2,0.25,0.0,1);
                glVertex4f(0.1,0.25,0.0,1);
                glVertex4f(0.1,0.35,0.0,1);
                glVertex4f(0.3,0.35,0.0,1);
                glVertex4f(0.3,0.25,0.0,1);
                glVertex4f(0.2,0.25,0.0,1);
                glEnd();
            glPopMatrix();
        glPopMatrix();
    }
    //Text
    glEnableClientState(GL_VERTEX_ARRAY);
    if (!serverFound)
    {
        glPushMatrix();
        glTranslatef(-7,10,-30);
        glVertexPointer(3, GL_FLOAT, 0, text1VertexArray);
        glColor4f(0.0,1.0,0,1);
        glDrawArrays(GL_LINES, 0, text1v/3);
        glPopMatrix();
    }
    if (g_soundError!=0)
    {
        glPushMatrix();
        if (serverFound) glTranslatef(-7,10,-30);
        else glTranslatef(-7,8,-30);
        glVertexPointer(3, GL_FLOAT, 0, text2VertexArray);
        glDrawArrays(GL_LINES, 0, text2v/3);
        glPopMatrix();
    }
    if (!Unit[g_playerID].m_spawnerOnline)
    {
        glPushMatrix();
        glTranslatef(-9,-3,-25);
        glVertexPointer(3, GL_FLOAT, 0, text4VertexArray);
        glColor4f(0.7,0.1,0,1);
        glDrawArrays(GL_LINES, 0, text4v/3);
        glPopMatrix();
    }
    if (g_gameOver)
    {
        glPushMatrix();
        glTranslatef(-4,-4,-15);
        if (g_victory)
        {
            glVertexPointer(3, GL_FLOAT, 0, text6VertexArray);
            glColor4f(0.2,0.9,0,2);
            glDrawArrays(GL_LINES, 0, text6v/3);
        }
        else
        {
            glVertexPointer(3, GL_FLOAT, 0, text7VertexArray);
            glColor4f(0.9,0.2,0,2);
            glDrawArrays(GL_LINES, 0, text7v/3);
        }
        glPopMatrix();
    }
    if (!activeSwitch)
    {
        glPushMatrix();
        glTranslatef(-4,0.5,-10);
        //Draw sign
        glBegin(GL_QUADS);
        glColor4f(0.7,0.3,0.3,1);
        glVertex3f(0,0,0);
        glColor4f(0.5,0.1,0.1,1);
        glVertex3f(0,-1,0);
        glVertex3f(8,-1,0);
        glColor4f(1,0.4,0.3,1);
        glVertex3f(8,0,0);
        glEnd();
        //Draw Text
        glScalef(0.3,0.4,0.3);
        glTranslatef(2,-1.7,0);
        glColor4f(0.2,0.1,0.3,1);
        glVertexPointer(3, GL_FLOAT, 0, text3VertexArray);
        glDrawArrays(GL_LINES, 0, text3v/3);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    if (g_view!=vTop) glEnable(GL_FOG);
}

int interactionHitTest(int unit,int range) //Accuracy 1 = Straight, higher value lowers accuracy
{
    //Hit detection
    enum objectTypes{oTerrain=1,oUnit,oSpawner,oTurret,oMine,oWall,oBuilding,oHeli,oSpeed,oTank,
                     oProj,oBBox,oBBlock,oPowerup,oRadar,oOutpost};

    float xpos,ypos,zpos;
    float xhed=Unit[unit].m_xdir;
    float yhed=Unit[unit].m_ydir;
    float zhed=Unit[unit].m_zdir;
    for (int i=1;i<range;i++)
    {
        xpos=Unit[unit].m_xpos+xhed*(float)i*0.3;
        ypos=Unit[unit].m_ypos+yhed*(float)i*0.3;
        zpos=Unit[unit].m_zpos+zhed*(float)i*0.3;

        //Hit terrain (1)
        if (World.getHeight(int(xpos),int(zpos))>ypos)
        {
            Logfile.log(" InteractionHitTest with TERRAIN");
            g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
            Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
            return oTerrain;
        }
        //Hit Player (2)
        for (int player=0;player<g_maxPlayers;player++)
        {
            if (Unit[player].m_online && Unit[player].m_spawnerOnline && Unit[player].m_HP>0)
            {
                //NOT itself,if already dead,if in tank
                if (player!=unit && Unit[player].m_HP>0 && Unit[player].m_inTransportType!=3)
                {
                    if (Unit[player].unitBulletHitTest(xpos,ypos,zpos))
                    {
                        Logfile.log(" InteractionHitTest with PLAYER id:",player);
                        g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                        Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                        g_tempID=player;
                        return oUnit;
                    }
                }
            }
        }
        //Hit Spawner (3)
        for (int spawn=0;spawn<g_maxPlayers;spawn++)
        {
            if (Spawner[spawn].m_active)
            {
                if (Spawner[spawn].spawnerBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with SPAWNER id:",spawn);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=spawn;
                    return oSpawner;
                }
            }
        }
        //Hit Turret (4)
        for (int turretID=0;turretID<g_maxTurrets;turretID++)
        {
            if (Turret[turretID].m_active)
            {
                if (Turret[turretID].turretBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with TURRET id:",turretID);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=turretID;
                    return oTurret;
                }
            }
        }
        //Hit Mine (5)
        for (int mineID=0;mineID<g_maxMines;mineID++)
        {
            if (Mine[mineID].m_active)
            {
                if (Mine[mineID].mineBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with MINE id:",mineID);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=mineID;
                    return oMine;
                }
            }
        }
        //Hit Wall (6)
        for (int wallID=0;wallID<g_maxWalls;wallID++)
        {
            if (Wall[wallID].m_active)
            {
                if (Wall[wallID].wallBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with WALL id:",wallID);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=wallID;
                    return oWall;
                }
            }
        }
        //Hit Building (7)
        for (int buildingID=0;buildingID<g_maxBuildings;buildingID++)
        {
            if (Building[buildingID].m_active)
            {
                if (Building[buildingID].buildingBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with BUILDING id:",buildingID);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=buildingID;
                    return oBuilding;
                }
            }
        }
        //Hit Heli (8)
        for (int heli=0;heli<g_maxHelis;heli++)
        {
            if (Heli[heli].m_active)
            {
                if (Heli[heli].heliBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with HELI id:",heli);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=heli;
                    return oHeli;
                }
            }
        }
        //Hit Speed (9)
        for (int speed=0;speed<g_maxSpeeds;speed++)
        {
            if (Speed[speed].m_active)
            {
                if (Speed[speed].speedBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with SPEED id:",speed);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=speed;
                    return oSpeed;
                }
            }
        }
        //Hit Tank (10)
        for (int tank=0;tank<g_maxTanks;tank++)
        {
            if (Tank[tank].m_active)
            {
                if (Tank[tank].tankBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with TANK id:",tank);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=tank;
                    return oTank;
                }
            }
        }
        //proj (11) NO INTERACTION
        //oProj

        //Hit BuildBox (12)
        for (int bbox=0;bbox<g_maxBuildboxes;bbox++)
        {
            if (Buildbox[bbox].m_active)
            {
                if (Buildbox[bbox].buildboxBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with BUILDBOX id:",bbox);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=bbox;
                    return oBBox;
                }
            }
        }
        //Hit BuildBlock (13)
        for (int bblock=0;bblock<g_maxBuildblocks;bblock++)
        {
            if (Buildblock[bblock].m_active)
            {
                if (Buildblock[bblock].buildblockBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with BUILDBLOCK id:",bblock);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=bblock;
                    return oBBlock;
                }
            }
        }
        //Hit Powerup (14)
        for (int pow=0;pow<g_maxNumOfPowerups;pow++)
        {
            if (Powerup[pow].m_active)
            {
                if (Powerup[pow].powerupBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with POWERUP id:",pow);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=pow;
                    return oPowerup;
                }
            }
        }
        //Hit Radar (15)
        for (int radar=0;radar<g_maxRadars;radar++)
        {
            if (Radar[radar].m_active)
            {
                if (Radar[radar].radarInteractionHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with RADAR id:",radar);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=radar;
                    return oRadar;
                }
            }
        }
        //Hit Outpost (16)
        for (int outpost=0;outpost<g_maxOutposts;outpost++)
        {
            if (Outpost[outpost].m_active)
            {
                if (Outpost[outpost].outpostBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(" InteractionHitTest with OUTPOST id:",outpost);
                    g_tempPos[0]=xpos-xhed*0.3; g_tempPos[1]=ypos-yhed*0.3; g_tempPos[2]=zpos-zhed*0.3;
                    Logfile.log("  at:",g_tempPos[0]+g_tempPos[1]+g_tempPos[2]);
                    g_tempID=outpost;
                    return oOutpost;
                }
            }
        }
    }
    return 0;
}

int bulletHitTest(int unit) //Accuracy 1 = Straight, higher value lowers accuracy
{
    Logfile.log("BulletHitTest rand. seed:",g_random);
    srand(g_random);
    float xpos,ypos,zpos;
    float sxpos,sypos,szpos;
    int range=0; int accuracy=1;
    int xabs,yabs,zabs;
    float xhed,yhed,zhed;
    if (unit<100) //Unit shooting
    {
        switch (Unit[unit].m_currWeapon)
        {
            case 0:range=5; accuracy=1; break; //Melee
            case 1:range=50; accuracy=10; break; //Rev
            case 2:range=40; accuracy=100; break; //Uzi
            case 3:range=200; if (Unit[unit].m_zooming) accuracy=1; else accuracy=200; break; //Sniper
            default:range=5; accuracy=1; break; //else
        }
        if (Unit[unit].m_inTransportType==3) //In Tank
        {
            range=60;
            accuracy=70;
        }
        xabs=rand()%2; if (xabs==0) xabs=-1;
        yabs=rand()%2; if (yabs==0) yabs=-1;
        zabs=rand()%2; if (zabs==0) zabs=-1;
        xhed=Unit[unit].m_xdir+float(rand()%accuracy)/1000*xabs;
        yhed=Unit[unit].m_ydir+float(rand()%accuracy)/1000*yabs;
        zhed=Unit[unit].m_zdir+float(rand()%accuracy)/1000*zabs;
        sxpos=Unit[unit].m_xpos;
        sypos=Unit[unit].m_ypos;
        szpos=Unit[unit].m_zpos;
        Logfile.log("Aiming vectors are:",xhed+yhed+zhed);
    }
    else //Turret shooting
    {
        int ID=unit-100;
        switch (Turret[ID].m_type)
        {
            case 1:{//Gattling
                    makeSound(Sturret_gattling,ID,2,4);
                    range=60;
                    accuracy=60;
                   }break;
            case 2:{//Rocket
                    //error
                   }break;
            case 3:{//Laser
                    makeSound(Sturret_laser,ID,2,4);
                    range=200;
                    accuracy=1;
                   }break;
        }
        xabs=rand()%2; if (xabs==0) xabs=-1;
        yabs=rand()%2; if (yabs==0) yabs=-1;
        zabs=rand()%2; if (zabs==0) zabs=-1;
        //Calculate Heading Vectors
        float yrot=180-Turret[ID].m_yrot;
        float xrot=Turret[ID].m_xrot;
        float piover180=0.0174532925;
        xhed=sinf(xrot*piover180)*cosf(yrot*piover180) +float(rand()%accuracy)/1000*xabs;
        yhed=cosf(xrot*piover180)                      +float(rand()%accuracy)/1000*yabs;
        zhed=-sinf(xrot*piover180)*sinf(yrot*piover180)+float(rand()%accuracy)/1000*zabs;
        sxpos=Turret[ID].m_pos[0]+0.5;
        sypos=Turret[ID].m_pos[1]+1;
        szpos=Turret[ID].m_pos[2]+0.5;
        Logfile.log("Turret aiming vectors are:",xhed+yhed+zhed);
    }
    //Hit detection
    enum objectTypes{oTerrain=1,oUnit,oSpawner,oTurret,oMine,oWall,oBuilding,oHeli,oSpeed,oTank,
                     oProj,oBBox,oBBlock,oPowerup,oRadar,oOutpost};

    for (int i=1;i<range;i++)
    {
        xpos=sxpos+xhed*(float)i;
        ypos=sypos+yhed*(float)i;
        zpos=szpos+zhed*(float)i;

        //Hit terrain (1)
        if (World.getHeight(int(xpos),int(zpos))>ypos)
        {
            Logfile.log(g_playerNames[unit],"HIT TERRAIN");
            Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
            g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
            return oTerrain;
        }
        //Hit Player (2)
        for (int player=0;player<g_maxPlayers;player++)
        {
            if (Unit[player].m_online)
            {
                if (player!=unit && Unit[player].m_HP>0 && Unit[player].m_spawnerOnline)
                {
                    if (Unit[player].unitBulletHitTest(xpos,ypos,zpos))
                    {
                        Logfile.log(g_playerNames[unit],"HIT PLAYER",g_playerNames[player]);
                        Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                        g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                        g_tempID=player;
                        return oUnit;
                    }
                }
            }
        }
        //Hit Spawner (3)
        for (int spawn=0;spawn<g_maxPlayers;spawn++)
        {
            if (Spawner[spawn].m_active)
            {
                if (Spawner[spawn].spawnerBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT SPAWNER ID:",spawn);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=spawn;
                    return oSpawner;
                }
            }
        }
        //Hit Turret (4)
        for (int turretID=0;turretID<g_maxTurrets;turretID++)
        {
            if (Turret[turretID].m_active)
            {
                if (Turret[turretID].turretBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT TURRET ID:",turretID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=turretID;
                    return oTurret;
                }
            }
        }
        //Hit Mine (5)
        for (int mineID=0;mineID<g_maxMines;mineID++)
        {
            if (Mine[mineID].m_active)
            {
                if (Mine[mineID].mineBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT MINE ID:",mineID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=mineID;
                    return oMine;
                }
            }
        }
        //Hit Wall (6)
        for (int wallID=0;wallID<g_maxWalls;wallID++)
        {
            if (Wall[wallID].m_active)
            {
                if (Wall[wallID].wallBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT WALL ID:",wallID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=wallID;
                    return oWall;
                }
            }
        }
        //Hit Building (7)
        for (int buildingID=0;buildingID<g_maxBuildings;buildingID++)
        {
            if (Building[buildingID].m_active)
            {
                if (Building[buildingID].buildingBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT BUILDING ID:",buildingID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=buildingID;
                    return oBuilding;
                }
            }
        }
        //Hit Heli (8)
        for (int heli=0;heli<g_maxHelis;heli++)
        {
            if (Heli[heli].m_active)
            {
                if (Heli[heli].heliBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT HELI ID:",heli);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=heli;
                    return oHeli;
                }
            }
        }
        //Hit Speed (9)
        for (int speed=0;speed<g_maxSpeeds;speed++)
        {
            if (Speed[speed].m_active)
            {
                if (Speed[speed].speedBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT SPEED ID:",speed);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=speed;
                    return oSpeed;
                }
            }
        }
        //Hit Tank (10)
        for (int tank=0;tank<g_maxTanks;tank++)
        {
            if (Tank[tank].m_active)
            {
                if (Tank[tank].tankBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT TANK ID:",tank);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    if (Unit[unit].m_inTransportType==3 && Unit[unit].m_transportID==tank) continue; //cant shoot yourself
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=tank;
                    return oTank;
                }
            }
        }
        //Hit Proj (11)
        for (int proj=0;proj<g_maxProj;proj++)
        {
            if (Proj[proj].m_active)
            {
                if (Proj[proj].projHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT PROJECTILE ID:",proj);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=proj;
                    return oProj;
                }
            }
        }
        //Hit BuildBox (12)
        for (int bbox=0;bbox<g_maxBuildboxes;bbox++)
        {
            if (Buildbox[bbox].m_active)
            {
                if (Buildbox[bbox].buildboxBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT BUILDBOX ID:",bbox);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=bbox;
                    return oBBox;
                }
            }
        }
        //Hit BuildBlock (13)
        for (int bblock=0;bblock<g_maxBuildblocks;bblock++)
        {
            if (Buildblock[bblock].m_active)
            {
                if (Buildblock[bblock].buildblockBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT BUILDBLOCK ID:",bblock);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=bblock;
                    return oBBlock;
                }
            }
        }
        //Hit Powerup (14)
        for (int pow=0;pow<g_maxNumOfPowerups;pow++)
        {
            if (Powerup[pow].m_active)
            {
                if (Powerup[pow].powerupBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT POWERUP ID:",pow);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=pow;
                    return oPowerup;
                }
            }
        }
        //Hit Radar (15)
        for (int radarID=0;radarID<g_maxRadars;radarID++)
        {
            if (Radar[radarID].m_active)
            {
                if (Radar[radarID].radarBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT RADAR ID:",radarID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=radarID;
                    return oRadar;
                }
            }
        }
        //Hit Outpost (16)
        for (int outpostID=0;outpostID<g_maxOutposts;outpostID++)
        {
            if (Outpost[outpostID].m_active)
            {
                if (Outpost[outpostID].outpostBulletHitTest(xpos,ypos,zpos))
                {
                    Logfile.log(g_playerNames[unit],"HIT OUTPOST ID:",outpostID);
                    Logfile.log(" Shooter position:",Unit[unit].m_xpos+Unit[unit].m_ypos+Unit[unit].m_zpos);
                    g_tempPos[0]=xpos; g_tempPos[1]=ypos; g_tempPos[2]=zpos;
                    g_tempID=outpostID;
                    return oOutpost;
                }
            }
        }
    }
    return 0;
}
/*
void drawSurface(float campos)
{
    float xpos=g_xpos;
    float ypos=g_ypos;
    float zpos=g_zpos;
    if (g_view==v3rd)
    {
        xpos=g_xpos-g_xhed*campos;
        ypos=g_ypos-g_yhed*campos+1;
        zpos=g_zpos-g_zhed*campos;
    }
    //Calculate view square
    int maxx,minx,maxz,minz;
    if (g_zooming) g_viewDist=150;
    else g_viewDist=100;
    if (g_xhed>0)
    {
        maxx=int(xpos+g_viewDist+g_xhed*g_viewDist);   if (maxx>g_terrainLength-1) maxx=g_terrainLength-1;
        minx=maxx-g_viewDist*2;                        if (minx<1) minx=1;
    }
    else if (g_xhed<=0)
    {
        minx=int(xpos-g_viewDist+g_xhed*g_viewDist);     if (minx<1) minx=1;
        maxx=minx+g_viewDist*2;                          if (maxx>g_terrainWidth-1) maxx=g_terrainWidth-1;
    }

    if (g_zhed>0)
    {
        maxz=int(zpos+g_viewDist+g_zhed*g_viewDist);   if (maxz>g_terrainLength-1) maxz=g_terrainLength-1;
        minz=maxz-g_viewDist*2;                        if (minz<1) minz=1;
    }
    else if (g_zhed<=0)
    {
        minz=int(zpos-g_viewDist+g_zhed*g_viewDist);     if (minz<1) minz=1;
        maxz=minz+g_viewDist*2;                          if (maxz>g_terrainWidth-1) maxz=g_terrainWidth-1;
    }

    if (Unit[g_playerID].m_spawning && Unit[g_playerID].m_spawnerMoving) //Can not spawn, topview
    {
        int carrier=Spawner[g_playerID].m_carrier;
        maxx=(int)Unit[carrier].m_xpos+30;
        maxz=(int)Unit[carrier].m_zpos+30;
        minx=(int)Unit[carrier].m_xpos-30;
        minz=(int)Unit[carrier].m_zpos-30;
    }

    //Draw view square
    if (g_debugMode)
    {
        glColor4f(0.1,0.8,0.2,1);
        glBegin(GL_LINE_LOOP);
        glVertex3f(minx-1,20,minz-1);
        glVertex3f(maxx+1,20,minz-1);
        glVertex3f(maxx+1,20,maxz+1);
        glVertex3f(minx-1,20,maxz+1);
        glEnd();
    }

    //Draw surface
    float mat_ambi[]={0.2, 0.2, 0.2, 1.0};
    float mat_diff[]={0.8, 0.8, 0.8, 1.0};
    float mat_spec[]={0.1, 0.1, 0.1, 1.0};
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambi);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diff);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
    glEnable(GL_CULL_FACE);
    glPushMatrix();
    g_terPoly=0;

    if (Unit[g_playerID].m_spawning && Unit[g_playerID].m_spawnerMoving) //Can not spawn, topview
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
        glBegin(GL_QUADS);
        for (int z=minz;z<maxz;z++) //g_terrainWidth-1
        for (int x=minx;x<maxx;x++) //g_terrainLength-1
        {
            //Top Side
            //if (g_ypos>g_height[x][z]) Always
            {
                glNormal3f(0,1,0);
                glTexCoord2f(0,0);
                glVertex3f(x,g_height[x][z],z);
                glNormal3f(0,1,0);
                glTexCoord2f(0.1,0);
                glVertex3f(x,g_height[x][z],z+1);
                glNormal3f(0,1,0);
                glTexCoord2f(0.1,1);
                glVertex3f(x+1,g_height[x][z],z+1);
                glNormal3f(0,1,0);
                glTexCoord2f(0,1);
                glVertex3f(x+1,g_height[x][z],z);
                g_terPoly+=1;
            }

            //Right Side +x
            if (g_height[x][z]>g_height[x+1][z] && xpos>x)
            {
                glNormal3f(1,0,0);
                glVertex3f(x+1, g_height[x][z], z+1);
                glNormal3f(1,0,0);
                glVertex3f(x+1, g_height[x+1][z], z+1);
                glNormal3f(1,0,0);
                glVertex3f(x+1, g_height[x+1][z], z);
                glNormal3f(1,0,0);
                glVertex3f(x+1, g_height[x][z], z);
                g_terPoly+=1;
            }
            //Left Side -x
            if (g_height[x][z]>g_height[x-1][z] && xpos<x)
            {
                glNormal3f(-1,0,0);
                glVertex3f(x, g_height[x][z], z);
                glNormal3f(-1,0,0);
                glVertex3f(x, g_height[x-1][z], z);
                glNormal3f(-1,0,0);
                glVertex3f(x, g_height[x-1][z], z+1);
                glNormal3f(-1,0,0);
                glVertex3f(x, g_height[x][z], z+1);
                g_terPoly+=1;
            }
            //Back Side +z
            if (g_height[x][z]>g_height[x][z+1] && zpos>z)
            {
                glNormal3f(0,0,1);
                glVertex3f(x, g_height[x][z], z+1);
                glNormal3f(0,0,1);
                glVertex3f(x, g_height[x][z+1], z+1);
                glNormal3f(0,0,1);
                glVertex3f(x+1, g_height[x][z+1], z+1);
                glNormal3f(0,0,1);
                glVertex3f(x+1, g_height[x][z], z+1);
                g_terPoly+=1;
            }
            //Front Side -z
            if (g_height[x][z]>g_height[x][z-1] && zpos<z)
            {
                glNormal3f(0,0,-1);
                glVertex3f(x+1, g_height[x][z], z);
                glNormal3f(0,0,-1);
                glVertex3f(x+1, g_height[x][z-1], z);
                glNormal3f(0,0,-1);
                glVertex3f(x, g_height[x][z-1], z);
                glNormal3f(0,0,-1);
                glVertex3f(x, g_height[x][z], z);
                g_terPoly+=1;
            }
        }
        glEnd();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }
    else //Normal view
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
        float diff,texID;
        glBegin(GL_QUADS);
        for (int z=minz;z<maxz;z++) //g_terrainWidth-1
        for (int x=minx;x<maxx;x++) //g_terrainLength-1
        if (checkFrustumBox(x,g_height[x][z],z)) //Frustum Culling switch
        {
            //Set Texture
            texID=g_terrTex[x][z]+g_terrTexShift[x][z];
            if (g_height[x][z]>60) texID=0.1+g_terrTexShift[x][z];
            else if (g_height[x][z]>40) texID=0.2+g_terrTexShift[x][z];
            if (g_height[x][z]<0) texID=0.3+g_terrTexShift[x][z];
            if (g_height[x][z]<-30) texID=0.5+g_terrTexShift[x][z];

            //Top Side
            if (ypos>g_height[x][z])
            {
                glNormal3f(0,1,0);
                glTexCoord2f(0.001+texID,0);
                glVertex3f(x,g_height[x][z],z);
                glNormal3f(0,1,0);
                glTexCoord2f(0.049+texID,0);
                glVertex3f(x,g_height[x][z],z+1);
                glNormal3f(0,1,0);
                glTexCoord2f(0.049+texID,1);
                glVertex3f(x+1,g_height[x][z],z+1);
                glNormal3f(0,1,0);
                glTexCoord2f(0.001+texID,1);
                glVertex3f(x+1,g_height[x][z],z);
                g_terPoly+=1;
            }

            //Right Side +x
            if (g_height[x][z]>g_height[x+1][z] && xpos>x)
            {
                diff=g_height[x][z]-g_height[x+1][z];
                glNormal3f(1,0,0);
                glTexCoord2f(0.001+texID,0);
                glVertex3f(x+1, g_height[x][z], z+1);
                glNormal3f(1,0,0);
                glTexCoord2f(0.001+texID,-diff);
                glVertex3f(x+1, g_height[x+1][z], z+1);
                glNormal3f(1,0,0);
                glTexCoord2f(0.049+texID,-diff);
                glVertex3f(x+1, g_height[x+1][z], z);
                glNormal3f(1,0,0);
                glTexCoord2f(0.049+texID,0);
                glVertex3f(x+1, g_height[x][z], z);
                g_terPoly+=1;
            }
            //Left Side -x
            if (g_height[x][z]>g_height[x-1][z] && xpos<x)
            {
                diff=g_height[x][z]-g_height[x-1][z];
                glNormal3f(-1,0,0);
                glTexCoord2f(0.001+texID,0);
                glVertex3f(x, g_height[x][z], z);
                glNormal3f(-1,0,0);
                glTexCoord2f(0.001+texID,-diff);
                glVertex3f(x, g_height[x-1][z], z);
                glNormal3f(-1,0,0);
                glTexCoord2f(0.049+texID,-diff);
                glVertex3f(x, g_height[x-1][z], z+1);
                glNormal3f(-1,0,0);
                glTexCoord2f(0.049+texID,0);
                glVertex3f(x, g_height[x][z], z+1);
                g_terPoly+=1;
            }
            //Back Side +z
            if (g_height[x][z]>g_height[x][z+1] && zpos>z)
            {
                diff=g_height[x][z]-g_height[x][z+1];
                glNormal3f(0,0,1);
                glTexCoord2f(0.001+texID,0);
                glVertex3f(x, g_height[x][z], z+1);
                glNormal3f(0,0,1);
                glTexCoord2f(0.001+texID,-diff);
                glVertex3f(x, g_height[x][z+1], z+1);
                glNormal3f(0,0,1);
                glTexCoord2f(0.049+texID,-diff);
                glVertex3f(x+1, g_height[x][z+1], z+1);
                glNormal3f(0,0,1);
                glTexCoord2f(0.049+texID,0);
                glVertex3f(x+1, g_height[x][z], z+1);
                g_terPoly+=1;
            }
            //Front Side -z
            if (g_height[x][z]>g_height[x][z-1] && zpos<z)
            {
                diff=g_height[x][z]-g_height[x][z-1];
                glNormal3f(0,0,-1);
                glTexCoord2f(0.001+texID,0);
                glVertex3f(x+1, g_height[x][z], z);
                glNormal3f(0,0,-1);
                glTexCoord2f(0.001+texID,-diff);
                glVertex3f(x+1, g_height[x][z-1], z);
                glNormal3f(0,0,-1);
                glTexCoord2f(0.049+texID,-diff);
                glVertex3f(x, g_height[x][z-1], z);
                glNormal3f(0,0,-1);
                glTexCoord2f(0.049+texID,0);
                glVertex3f(x, g_height[x][z], z);
                g_terPoly+=1;
            }
        }
        glEnd();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}
*/
void inputIP(void)
{
     if (inputDelay>130)
     {
        if (currNumber<20)
        {
            if (keys[48] || keys[96]) {cIPnumber[currNumber]='0'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[49] || keys[97]) {cIPnumber[currNumber]='1'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[50] || keys[98]) {cIPnumber[currNumber]='2'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[51] || keys[99]) {cIPnumber[currNumber]='3'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[52] || keys[100]) {cIPnumber[currNumber]='4'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[53] || keys[101]) {cIPnumber[currNumber]='5'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[54] || keys[102]) {cIPnumber[currNumber]='6'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[55] || keys[103]) {cIPnumber[currNumber]='7'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[56] || keys[104]) {cIPnumber[currNumber]='8'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[57] || keys[105]) {cIPnumber[currNumber]='9'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
            if (keys[190] || keys[110]) {cIPnumber[currNumber]='.'; currNumber++; inputDelay=0;Sound.playSimpleSound(Sselect,1);}
        }
        if (keys[8] && currNumber>0) {currNumber--; inputDelay=0;Sound.playSimpleSound(Serase,1);}
        if (keys[13]) //enter
        {
            cIPnumber[currNumber]='\0';
            haveIP=true; inputDelay=0;
            Sound.playSimpleSound(Sselected,0.4);
            Logfile.log("Connecting to IP:",cIPnumber);
        }
     }
     else inputDelay+=CycleTime;
}

void drawIP(void)
{
     glPushMatrix();
     glColor4f(1,1,1,1);
     glTranslatef(-1,0.3,-3);
     glScalef(0.1,0.1,0.1);
     //Draw Text
     glEnableClientState(GL_VERTEX_ARRAY);
     glVertexPointer(3, GL_FLOAT, 0, text0VertexArray);
     glDrawArrays(GL_LINES, 0, text0v/3);
     glDisableClientState(GL_VERTEX_ARRAY);
     //Draw IP
     glTranslatef(0,-3,0);
     glColor4f(0.7,0.7,0.3,1);
     for (int i=0;i<currNumber;i++)
     {
         glTranslatef(1.1,0,0);
         switch (cIPnumber[i])
         {
                case '0': {rita0(); break;}
                case '1': {rita1(); break;}
                case '2': {rita2(); break;}
                case '3': {rita3(); break;}
                case '4': {rita4(); break;}
                case '5': {rita5(); break;}
                case '6': {rita6(); break;}
                case '7': {rita7(); break;}
                case '8': {rita8(); break;}
                case '9': {rita9(); break;}
                case '.': {ritaMinus(); break;}
         }
     }
     if (toggle>700) {onoff=!onoff; toggle=0;}
     else toggle++;
     if (onoff)
     {
         glBegin(GL_LINES);
         glVertex3f(1.1,-0.3,0);
         glVertex3f(2.1,-0.3,0);
         glEnd();
     }

     glPopMatrix();
}

void sendPlayerLobbyData(void)
{                      //            0           1           2     3 4 5 6 7 8 9
     int playerLobbyData[20]={g_lobbyStatus,g_lobbyTeam,g_playerID,0,0,0,0,0,0,0,
                       //      10111213141516171819
                                0,0,0,0,0,0,0,0,0,0};
     for (int i=0;i<15;i++)
     {
         playerLobbyData[i+5]=g_playerName[i];
     }
     send (SServer, (const char*)&playerLobbyData, sizeof(playerLobbyData), 0);
}

void getOtherPlayerLobbyData(void)
{
     int allOtherPlayerLobbyData[9][20];
     // om [ID][0]==3 -> ny spelare DVS namn incoming, annars:
     // om [ID][0]==0 finns inte eller lämnat -> sätt XXXXXXX till false, eller inte 0 räcker för att inte rita...
     // om [ID][0]==1 fortfarande redo -> rita namn
     // ID==9 är serverns meddelande
     // if [9][0] server kommer stängas av
     // if [9][1] server väntar på spelare/ready
     // if [9][2] spel startas
     recv (SServer, (char*)&allOtherPlayerLobbyData, sizeof(allOtherPlayerLobbyData), 0);
     for (int i=0;i<9;i++)
     for (int j=0;j<20;j++)
     {
         g_allPlayerLobbyData[i][j]=allOtherPlayerLobbyData[i][j]; //Store all data
     }
     //Get all names
     char text[9][20];
     for (int i=0;i<9;i++)
     {
         if (allOtherPlayerLobbyData[i][0]!=0)
         {
             for (int letter=0;letter<15;letter++)
             {
                 text[i][letter]=g_playerNames[i][letter]=allOtherPlayerLobbyData[i][letter+5];
             }
         }
     }
     //Make text arrays
     float* VertexArray;
     int i=0;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text20VertexArray[i]=VertexArray[i+1];
     text20v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text21VertexArray[i]=VertexArray[i+1];
     text21v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text22VertexArray[i]=VertexArray[i+1];
     text22v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text23VertexArray[i]=VertexArray[i+1];
     text23v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text24VertexArray[i]=VertexArray[i+1];
     text24v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text25VertexArray[i]=VertexArray[i+1];
     text25v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text26VertexArray[i]=VertexArray[i+1];
     text26v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text27VertexArray[i]=VertexArray[i+1];
     text27v=(int)VertexArray[0]-1;
     i++;
     VertexArray=getTextVertexArray(text[i], 15);
     for (int i=0;i<VertexArray[0];i++) text28VertexArray[i]=VertexArray[i+1];
     text28v=(int)VertexArray[0]-1;

     if (allOtherPlayerLobbyData[8][0]==2) {g_timeToStart=true; drawText(2);} //Have server started the game
     g_gameMode=allOtherPlayerLobbyData[8][1]; //get gamemode
}

void sendPlayerData(void)
{
                  //         0     1      2          3         4            5          6          7          8         9
     float playerData[20]={g_xpos,g_ypos,g_zpos,g_headingY,g_headingXZ,g_transportFloat,g_moving,g_playerID,g_online,g_avgvel,
                  //            10           11          12           13           14         15          16     (17)         18               19
                           g_jumpState,g_projCreated,g_makeSound,g_fireWeapon,g_checkSum,g_action,g_animation,g_random,g_extraFloatA,g_extraFloatB};

 //    if (!Unit[g_playerID].m_spawnerOnline) playerData[0]=playerData[1]=playerData[2]=0; //Reset player pos if spawner is offline
     if (Unit[g_playerID].m_spawning) {g_ypos=playerData[1]=Unit[g_playerID].m_ypos;} //If spawning ypos comes from Unit

     calcCheckSum(); playerData[14]=g_checkSum;
     send (SServer, (const char*)&playerData, sizeof(playerData), 0);
}

void getOtherPlayerData(void)
{
     float allOtherPlayerData[8][20];
     if (serverFound)
     {
         recv (SServer, (char*)&allOtherPlayerData, sizeof(allOtherPlayerData), 0);
     }
     else //Offline Play
     {
         allOtherPlayerData[0][0]= g_xpos;
         allOtherPlayerData[0][1]= g_ypos;
         allOtherPlayerData[0][2]= g_zpos;
         allOtherPlayerData[0][3]= g_headingY;
         allOtherPlayerData[0][4]= g_headingXZ;
         allOtherPlayerData[0][5]= g_transportFloat;
         allOtherPlayerData[0][6]= g_moving;
         allOtherPlayerData[0][7]= g_playerID;
         allOtherPlayerData[0][8]= g_online;
         allOtherPlayerData[0][9]= g_avgvel;
         allOtherPlayerData[0][10]=g_jumpState;
         allOtherPlayerData[0][11]=g_projCreated;
         allOtherPlayerData[0][12]=g_makeSound;
         allOtherPlayerData[0][13]=g_fireWeapon;
         // [14] checksum value
         allOtherPlayerData[0][15]=g_action;
         allOtherPlayerData[0][16]=g_animation;
         allOtherPlayerData[0][17]=g_random;
         allOtherPlayerData[0][18]=g_extraFloatA;
         allOtherPlayerData[0][19]=g_extraFloatB;

         if (Unit[g_playerID].m_spawning) {g_ypos=allOtherPlayerData[0][1]=Unit[g_playerID].m_ypos;} //If spawning ypos comes from Unit

         calcCheckSum();
         allOtherPlayerData[0][14]=g_checkSum;
     }

     for (int i=0;i<g_maxPlayers;i++)
     {
         if (Unit[i].m_online) Unit[i].getData(allOtherPlayerData[i]);
         else continue;

         //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
         //Check Sum
         float sum=allOtherPlayerData[i][0]+allOtherPlayerData[i][1]+allOtherPlayerData[i][2]+
                   allOtherPlayerData[i][3]+allOtherPlayerData[i][4]+allOtherPlayerData[i][5]+
                   allOtherPlayerData[i][6]+allOtherPlayerData[i][7]+allOtherPlayerData[i][8]+
                   allOtherPlayerData[i][9]+allOtherPlayerData[i][10]+allOtherPlayerData[i][11]+
                   allOtherPlayerData[i][12]+allOtherPlayerData[i][13]+allOtherPlayerData[i][15]+
                   allOtherPlayerData[i][16]+allOtherPlayerData[i][18]+allOtherPlayerData[i][19]; //[17] not included (g_random)
         if (allOtherPlayerData[i][14]!=sum) {Logfile.log("ERROR Checksum for:",g_playerNames[i]); g_gameOver=true;} //Value error
         //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

         //Set random number from server
         if (i==g_playerID)
         {
             g_randomLast=g_random;
             g_random=(int)allOtherPlayerData[i][17];
         }

         //Player left
         if (allOtherPlayerData[i][8]==0)
         {
             Unit[i].m_online=false;
             Unit[i].m_spawnerOnline=false;
             Spawner[i].m_active=false;
             if (Unit[i].m_carries)
             {
                 if (Unit[i].m_carrySpawner)
                 {
                     allOtherPlayerData[i][15]=-10; //Spawner drop
                 }
                 if (Unit[i].m_carryPow)
                 {
                     allOtherPlayerData[i][15]=-22; //Pow drop
                 }
                 if (Unit[i].m_carryBBox)
                 {
                     allOtherPlayerData[i][15]=-20; //BBox drop
                 }
                 if (Unit[i].m_carryBBlock)
                 {
                     allOtherPlayerData[i][15]=-21; //BBlock drop
                 }
             }
             Logfile.log(g_playerNames[i],"Left the Game");
         }
         //Fire Weapon
         if (allOtherPlayerData[i][13]!=0)
         {
             if (allOtherPlayerData[i][13]==1 || allOtherPlayerData[i][13]==2)
             {
                 Logfile.log(g_playerNames[i],"pressed action key:", (int)allOtherPlayerData[i][13]);
                 fireWeapon((int)allOtherPlayerData[i][13],i);
                 if (i==g_playerID && allOtherPlayerData[i][13]==1) fireRecoil(i); //If its you and MB1, calc recoil
                 Unit[i].m_ybodyhed=Unit[i].m_yheadhed;
                 Unit[i].m_fireCount++;
             }
             if (allOtherPlayerData[i][13]==3) //Object Pickup (1)Spawner,(2)BBox,(3)BBlock,(4)Powerup,(5)Outpost
             {
                 int type=objectBehindTest(0,i); //Returns type, ID stored in g_tempID
                 if (type!=-1)
                 {
                     int ID=g_tempID;
                     switch (type)
                     {
                         case 1:{//Spawner
                                 Unit[i].m_carries=true;
                                 Unit[i].m_carrySpawner=true;
                                 Unit[i].m_carryID=ID;
                                 Unit[ID].m_spawnerMoving=true;
                                 Unit[i].m_currWeapon=0;
                                 Spawner[ID].m_moving=true;
                                 Spawner[ID].m_carrier=i;
                                 Logfile.log(g_playerNames[i],"picked up (e) SPAWNER:", ID);
                                }break;
                         case 2:{//BBox
                                 Unit[i].m_carries=true;
                                 Unit[i].m_carryBBox=true;
                                 Unit[i].m_carryID=ID;
                                 Unit[i].m_currWeapon=0;
                                 Buildbox[ID].m_moving=true;
                                 Buildbox[ID].m_carrier=i;
                                 Logfile.log(g_playerNames[i],"picked up (e) BUILDBOX:", ID);
                                }break;
                         case 3:{//BBlock
                                 Unit[i].m_carries=true;
                                 Unit[i].m_carryBBlock=true;
                                 Unit[i].m_carryID=ID;
                                 Unit[i].m_currWeapon=0;
                                 Buildblock[ID].m_moving=true;
                                 Buildblock[ID].m_carrier=i;
                                 World.removeBuildblock(Buildblock[ID].m_pos[0],Buildblock[ID].m_pos[2]);
                                 Logfile.log(g_playerNames[i],"picked up (e) BUILDBLOCK:", ID);
                                }break;
                         case 4:{//Powerup
                                 Unit[i].m_carries=true;
                                 Unit[i].m_carryPow=true;
                                 Unit[i].m_carryID=ID;
                                 Unit[i].m_currWeapon=0;
                                 Powerup[ID].m_moving=true;
                                 Powerup[ID].m_carrier=i;
                                 Logfile.log(g_playerNames[i],"picked up (e) POWERUP:", ID);
                                }break;
                         case 5:{//Outpost
                                 Unit[i].m_carries=true;
                                 Unit[i].m_carryID=ID;
                                 Unit[i].m_currWeapon=0;
                                 Outpost[ID].m_moving=true;
                                 Outpost[ID].m_carrier=i;
                                 Logfile.log(g_playerNames[i],"picked up (e) OUTPOST:", ID);
                                }break;
                     }
                     if (i==g_playerID)
                     {
                         g_view=v3rd;
                         g_pushDelay=100;
                     }
                 }
             }
         }

         //Play Sound
         if (allOtherPlayerData[i][12]!=0)
         {
             Logfile.log(g_playerNames[i],"made sound:", (int)allOtherPlayerData[i][12]);
             float data[]={g_xpos-g_xhed*2,g_ypos-g_yhed*2,g_zpos-g_zhed*2,    g_xvel,g_yvel,g_zvel,
                           g_xhed,g_yhed,g_zhed,    g_xupp,g_yupp,g_zupp,
                           allOtherPlayerData[i][0],allOtherPlayerData[i][1],allOtherPlayerData[i][2],
                           0,0,0,   1,1,0};
             Sound.playWAVE((int)allOtherPlayerData[i][12],data);
             if ((int)allOtherPlayerData[i][12]==Sland) flattenGround(i); //Flatten ground
         }

         //Action
         // 1000+type -> Pick Up
         // -10 -20 -21 -22 -23 -24 -25 -26 -> Drop
         if (allOtherPlayerData[i][15]!=0)
         {
             //Spawner Put down -10
             if (int(allOtherPlayerData[i][15])==-10)
             {
                 Unit[i].m_carries=false;
                 Unit[i].m_carrySpawner=false;
                 int ID=Unit[i].m_carryID;
                 Unit[ID].m_spawnerMoving=false;
                 Spawner[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=3; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"put down (e) SPAWNER:", ID);
                 Logfile.log(" on pos: x =",(int)Spawner[ID].m_pos[0],", z =", (int)Spawner[ID].m_pos[2]);
             }
             //BBox drop -20
             if (int(allOtherPlayerData[i][15])==-20)
             {
                 Unit[i].m_carries=false;
                 Unit[i].m_carryBBox=false;
                 int ID=Unit[i].m_carryID;
                 Buildbox[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=2; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"put down (e) BUILDBOX:", ID);
                 Logfile.log(" on pos: x =",(int)Buildbox[ID].m_pos[0],", z =", (int)Buildbox[ID].m_pos[2]);
             }
             //BBlock drop -21
             if (int(allOtherPlayerData[i][15])==-21)
             {
                 Unit[i].m_carries=false;
                 Unit[i].m_carryBBlock=false;
                 int ID=Unit[i].m_carryID;
                 Buildblock[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 World.setBuildblockType(Buildblock[ID].m_pos[0],Buildblock[ID].m_pos[2],Buildblock[ID].m_type);
                 if (i==g_playerID) {g_ypos+=3; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"put down (e) BUILDBLOCK:", ID);
                 Logfile.log(" on pos: x =",(int)Buildblock[ID].m_pos[0],", z =", (int)Buildblock[ID].m_pos[2]);
             }
             //Powerup drop -22
             if (int(allOtherPlayerData[i][15])==-22)
             {
                 Unit[i].m_carries=false;
                 Unit[i].m_carryPow=false;
                 int ID=Unit[i].m_carryID;
                 Powerup[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=1.5; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"put down (e) POWERUP:", ID);
                 Logfile.log(" on pos: x =",(int)Powerup[ID].m_pos[0],", z =", (int)Powerup[ID].m_pos[2]);
             }
             //Exit Heli -23
             if (int(allOtherPlayerData[i][15])==-23)
             {
                 Unit[i].m_moving=false;
                 Unit[i].m_inTransportType=0;
                 int ID=Unit[i].m_transportID;
                 Sound.stopSound(Heli[ID].m_noiseID);
                 Heli[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=2.5; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"dismounted HELI:", ID);
                 Logfile.log(" on pos: x =",(int)Heli[ID].m_pos[0],", z =", (int)Heli[ID].m_pos[2]);
             }
             //Exit Speed -24
             if (int(allOtherPlayerData[i][15])==-24)
             {
                 Unit[i].m_moving=false;
                 Unit[i].m_inTransportType=0;
                 int ID=Unit[i].m_transportID;
                 Sound.stopSound(Speed[ID].m_noiseID);
                 Speed[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=2.5; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"dismounted SPEED:", ID);
                 Logfile.log(" on pos: x =",(int)Speed[ID].m_pos[0],", z =", (int)Speed[ID].m_pos[2]);
             }
             //Exit Tank -25
             if (int(allOtherPlayerData[i][15])==-25)
             {
                 Unit[i].m_moving=false;
                 Unit[i].m_inTransportType=0;
                 Unit[i].m_jumpState=0;
                 int ID=Unit[i].m_transportID;
                 Sound.stopSound(Tank[ID].m_noiseID);
                 Tank[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=2.5; g_view=vFPS; g_jumpState=0;}
                 Logfile.log(g_playerNames[i],"dismounted TANK:", ID);
                 Logfile.log(" on pos: x =",(int)Tank[ID].m_pos[0],", z =", (int)Tank[ID].m_pos[2]);
             }
             //Outpost drop
             if (int(allOtherPlayerData[i][15])==-26)
             {
                 Unit[i].m_carries=false;
                 int ID=Unit[i].m_carryID;
                 Outpost[ID].place(Unit[i].m_xpos,World.getHeight(Unit[i].m_xpos,Unit[i].m_zpos),Unit[i].m_zpos);
                 if (i==g_playerID) {g_ypos+=1.5; g_view=vFPS;}
                 Logfile.log(g_playerNames[i],"put down (e) OUTPOST:", ID);
                 Logfile.log(" on pos: x =",(int)Outpost[ID].m_pos[0],", z =", (int)Outpost[ID].m_pos[2]);
             }
         }

         //Unit Animation
         if (allOtherPlayerData[i][16]!=0)
         {
             Unit[i].m_ybodyhed=Unit[i].m_yheadhed;
             Unit[i].m_zooming=false;
             if (allOtherPlayerData[i][16]==1) //Dig
             {
                 Unit[i].m_loadedShovel=true;
             }
             if (allOtherPlayerData[i][16]==2) //Undig
             {
                 Unit[i].m_loadedShovel=false;
             }
             if (allOtherPlayerData[i][16]==3) //Melee Attack
             {
                 Unit[i].m_meleeAttackProg=0.01;
                 Unit[i].m_loadedShovel=false;
             }
             if (allOtherPlayerData[i][16]==4) //Muzzle Flash
             {
                 Unit[i].m_rifleAttackProg=0.01;
                 if (i!=g_playerID) flash(Unit[i].m_xpos,Unit[i].m_ypos+1,Unit[i].m_zpos);
             }
             if (allOtherPlayerData[i][16]==5) //Place Mine
             {
                 Unit[i].m_rifleAttackProg=0.01;
             }
             if (allOtherPlayerData[i][16]==6) //Place Turret
             {
                 Unit[i].m_rifleAttackProg=0.01;
             }
             //Change Weapon
             if (allOtherPlayerData[i][16]==10) //Melee
             {
                 Unit[i].m_nextWeapon=0;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==11) //Revolver
             {
                 Unit[i].m_nextWeapon=1;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==12) //Uzi
             {
                 Unit[i].m_nextWeapon=2;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==13) //Sniper
             {
                 Unit[i].m_nextWeapon=3;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==14) //Rocketlauncher
             {
                 Unit[i].m_nextWeapon=4;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==15) //Radar
             {
                 Unit[i].m_nextWeapon=5;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==16) //Mine
             {
                 Unit[i].m_nextWeapon=6;
                 Unit[i].m_weaponSwapProg=0.01;
             }
             if (allOtherPlayerData[i][16]==17) //Turret
             {
                 Unit[i].m_nextWeapon=7;
                 Unit[i].m_weaponSwapProg=0.01;
             }
         }

         //ExtraFloat A
         if (allOtherPlayerData[i][18]!=0)
         {
             if (Unit[i].m_inTransportType==3) //Tank
             {
                 Tank[Unit[i].m_transportID].m_xrot+=allOtherPlayerData[i][18];
             }
             if (Unit[i].m_inTransportType==1) //Heli
             {
                 Heli[Unit[i].m_transportID].m_yrot+=allOtherPlayerData[i][18];
                 if (g_playerID==i)
                 {
                     g_headingY=Heli[Unit[i].m_transportID].m_yrot;
                 }
             }
             if (Unit[i].m_inTransportType==2) //Speed
             {
                 Speed[Unit[i].m_transportID].m_yrot+=allOtherPlayerData[i][18];
                 if (g_playerID==i)
                 {
                     g_headingY=Speed[Unit[i].m_transportID].m_yrot;
                 }
             }
         }
         //ExtraFloat B
         if (allOtherPlayerData[i][19]!=0)
         {
             if (Unit[i].m_inTransportType==3) //Tank
             {
                 Tank[Unit[i].m_transportID].m_zrot+=allOtherPlayerData[i][19];
             }
         }
     }
     //randomEvent(g_random,g_randomLast);
}
/*
void genHeights(void)
{
    //Hills
    int tempRandom;
    for (int x=0;x<g_terrainWidth;x+=10)
    for (int z=0;z<g_terrainLength;z+=10)
    {
        tempRandom=rand()%30;
        if (tempRandom==0)
        g_height[x][z]=rand()%10+10;//30+1
        else
        g_height[x][z]=rand()%5+5;//10+1
    }
    //Plains
    int plains=rand()%50+50;
    int length=rand()%50+50;
    int width=rand()%50+50;
    int height,x,z;
    for (int i=plains;i>0;i--)
    {
        x=int((rand()%(g_terrainWidth-300)+100)/10)*10;
        z=int((rand()%(g_terrainLength-300)+100)/10)*10;
        height=rand()%10;
        length=rand()%50+50;
        width=rand()%50+50;
        for (int p=0;p<length;p+=10)
        for (int l=0;l<width;l+=10)
        {
            if (p>length-10) g_height[x+p][z+l]=(height+g_height[x+p+10][z+l])/2;
            else if (p<10) g_height[x+p][z+l]=(height+g_height[x+p-10][z+l])/2;
            else if (l>width-10) g_height[x+p][z+l]=(height+g_height[x+p][z+l+10])/2;
            else if (l<10) g_height[x+p][z+l]=(height+g_height[x+p][z+l-10])/2;
            else g_height[x+p][z+l]=height;
        }
    }
    //Mountain Chain
    int mountChains=rand()%10+5;
    float xhed,zhed,vectorLength;
    int xnew,znew,heightDiff;
    for (int i=mountChains;i>0;i--)
    {
        //Start Pos
        x=rand()%(g_terrainWidth-300)+100;
        z=rand()%(g_terrainLength-300)+100;
        height=rand()%30+20;
        length=rand()%40+10;
        //Direction
        xhed=float((rand()%21-10))/10;
        zhed=float((rand()%21-10))/10;
        vectorLength=sqrt((xhed)*(xhed)+(zhed)*(zhed));
        if (vectorLength==0) continue;
        xhed=xhed/vectorLength;
        zhed=zhed/vectorLength;
        for (int j=0;j<length;j++)
        {
            xnew=int((x+xhed*j*10)/10)*10; znew=int((z+zhed*j*10)/10)*10;
            if (xnew<10 || xnew>g_terrainWidth-10 || znew<10 || znew>g_terrainLength-10) break;//Outside Height Array
            else
            {
                //Change height
                heightDiff=rand()%7-3;
                height+=heightDiff;
                if (height<10) height=10; if (height>60) height=60;
                g_height[xnew][znew]=height;
                g_height[xnew+10][znew]=height*0.8;
                g_height[xnew][znew+10]=height*0.8;
                g_height[xnew-10][znew]=height*0.8;
                g_height[xnew][znew-10]=height*0.8;
                g_height[xnew+10][znew+10]=height*0.6;
                g_height[xnew+10][znew-10]=height*0.6;
                g_height[xnew-10][znew+10]=height*0.6;
                g_height[xnew-10][znew-10]=height*0.6;
                //Set Terrain Texture
                for (int p=xnew-19;p<xnew+19;p++)
                for (int q=znew-19;q<znew+19;q++)
                {
                    g_terrTex[p][q]=0.4;
                }
                tempRandom=rand()%5;
                if (tempRandom==0) //Change Direction
                {
                    xhed+=float((rand()%21-10))/50;
                    zhed+=float((rand()%21-10))/50;
                    vectorLength=sqrt((xhed)*(xhed)+(zhed)*(zhed));
                    if (vectorLength==0) break;
                    xhed=xhed/vectorLength;
                    zhed=zhed/vectorLength;
                }
            }
        }
    }
    //Ravines
    int ravines=rand()%7+1;
    for (int i=ravines;i>0;i--)
    {
        //Start Pos
        x=rand()%(g_terrainWidth-300)+100;
        z=rand()%(g_terrainLength-300)+100;
        height=rand()%30-50;
        length=rand()%40+10;
        //Direction
        xhed=float((rand()%21-10))/10;
        zhed=float((rand()%21-10))/10;
        vectorLength=sqrt((xhed)*(xhed)+(zhed)*(zhed));
        if (vectorLength==0) continue;
        xhed=xhed/vectorLength;
        zhed=zhed/vectorLength;
        for (int j=0;j<length;j++)
        {
            xnew=int((x+xhed*j*10)/10)*10; znew=int((z+zhed*j*10)/10)*10;
            if (xnew<10 || xnew>g_terrainWidth-10 || znew<10 || znew>g_terrainLength-10) break;//Outside Height Array
            else
            {
                //Change height
                heightDiff=rand()%9-3;
                height+=heightDiff;
                if (height<-50) height=-50; if (height>0) height=0;
                g_height[xnew][znew]=height;
                g_height[xnew+10][znew]=height*0.8;
                g_height[xnew][znew+10]=height*0.8;
                g_height[xnew-10][znew]=height*0.8;
                g_height[xnew][znew-10]=height*0.8;
                g_height[xnew+10][znew+10]=height*0.6;
                g_height[xnew+10][znew-10]=height*0.6;
                g_height[xnew-10][znew+10]=height*0.6;
                g_height[xnew-10][znew-10]=height*0.6;
                tempRandom=rand()%5;
                if (tempRandom==0) //Change Direction
                {
                    xhed+=float((rand()%21-10))/50;
                    zhed+=float((rand()%21-10))/50;
                    vectorLength=sqrt((xhed)*(xhed)+(zhed)*(zhed));
                    if (vectorLength==0) break;
                    xhed=xhed/vectorLength;
                    zhed=zhed/vectorLength;
                }
            }
        }
    }
    //Mountains
    int mountains=rand()%10;
    int steps,maxx,minx,maxz,minz;
    enum directions{up=0,right=1,down=2,left=3};
    int dir=0;
    for (int i=mountains;i>0;i--)
    {
        minx=maxx=x=int((rand()%(g_terrainWidth-300)+100)/10)*10;
        minz=maxz=z=int((rand()%(g_terrainLength-300)+100)/10)*10;
        height=rand()%50+50;
        steps=rand()%10+10;
        g_height[x][z]=height;
        int jumps=1;
        for (int s=steps;s>0;s--)
        {
            if (x<10 || x>g_terrainWidth-10 || z<10 || z>g_terrainLength-10) break;//Outside Height Array
            if (height<10) break; //Break if to low
            for (int j=0;j<jumps;j++)
            {
                //Add Direction
                switch (dir)
                {
                    case up:    z+=10; break;
                    case right: x+=10; break;
                    case down:  z-=10; break;
                    case left:  x-=10; break;
                }
                if (x<10 || x>g_terrainWidth-10 || z<10 || z>g_terrainLength-10) break;//Outside Height Array
                height-=rand()%15-5;  //Height Decrese
                if (height<10) break; //Break if to low
                g_height[x][z]=height;//Set Height
            }
            //Change Direction
            dir++;
            if (dir>3) dir=0;

            for (int j=0;j<jumps;j++)
            {
                //Add Direction
                switch (dir)
                {
                    case up:    z+=10; break;
                    case right: x+=10; break;
                    case down:  z-=10; break;
                    case left:  x-=10; break;
                }
                if (x<10 || x>g_terrainWidth-10 || z<10 || z>g_terrainLength-10) break;//Outside Height Array
                height-=rand()%10-1; //Height Decrese
                if (height<10) break; //Break if to low
                g_height[x][z]=height;//Set Height
            }
            //Change Direction
            dir++;
            if (dir>3) dir=0;
            jumps++; //One more jump next round
            //Remember Mountain position for terrain texture array
            if (x<minx) minx=x; if (x>maxx) maxx=x;
            if (z<minz) minz=z; if (z>maxz) maxz=z;
        }
        //Set Mountain Texture for Mountain
        for (int p=minx-9;p<maxx+9;p++)
        for (int q=minz-9;q<maxz+9;q++)
        {
            g_terrTex[p][q]=0.4;
        }
    }
    //Walls
    for (int x=0;x<g_terrainWidth;x+=10)
    {
        g_height[x][0]=rand()%50+50;
        g_height[x][10]=rand()%50+50;
        g_height[x][20]=rand()%40+30;
        g_height[x][30]=rand()%30+20;
        g_height[x][g_terrainWidth-1]=rand()%50+50;
        g_height[x][g_terrainWidth-11]=rand()%50+50;
        g_height[x][g_terrainWidth-21]=rand()%10+30;
        g_height[x][g_terrainWidth-31]=rand()%30+20;
    }
    for (int z=0;z<g_terrainLength;z+=10)
    {
        g_height[0][z]=rand()%50+50;
        g_height[10][z]=rand()%50+50;
        g_height[20][z]=rand()%40+30;
        g_height[30][z]=rand()%30+20;
        g_height[g_terrainLength-1][z]=rand()%50+50;
        g_height[g_terrainLength-11][z]=rand()%50+50;
        g_height[g_terrainLength-21][z]=rand()%40+30;
        g_height[g_terrainLength-31][z]=rand()%30+20;
    }
    //interpolation
    //interpolera och skapa punkter mellan i x-led för första och sista z-värde
    for (int z=0;z<g_terrainLength;z+=10)
    for (int x=0;x<g_terrainWidth-1;x+=10)
    {
        for (int i=0;i<10;i++)
        {
            float f = (1 - cos(i*3.1415927/10)) * .5;
            g_height[x+i][z]=(g_height[x][z]*(1-f) + g_height[x+10][z]*f);
        }
    }

    //interpolera och skapa punkter mellan i z-led
    for (int x=0;x<g_terrainWidth-1;x++)
    for (int z=0;z<g_terrainLength;z+=10)
    {
     for (int i=0;i<10;i++)
     {
         float f = (1 - cos(i*3.1415927/10)) * .5;
         g_height[x][z+i]=(g_height[x][z]*(1-f) + g_height[x][z+10]*f);
     }
    }
    //Random Height Adjustment
    for (int x=1;x<g_terrainWidth-1;x++)
    for (int z=1;z<g_terrainLength-1;z++)
    g_height[x][z]+=float(rand()%5)/50;
}
*/
bool fixConnection(void)
{
    WSADATA w;
    int error = WSAStartup (0x0202, &w);
    if (error)
    { // there was an error
      return false;
    }
    if (w.wVersion != 0x0202)
    { // wrong WinSock version!
      WSACleanup (); // unload ws2_32.dll
      return false;
    }

    //=====================================================
    SServer = socket (AF_INET, SOCK_STREAM, 0); // Create socket
    //========================================================

    sIPnumber.assign( cIPnumber, cIPnumber+currNumber );

    sockaddr_in target;
    target.sin_family = AF_INET;       	// address family Internet
    target.sin_port = htons (666);    	// set server’s port number
    target.sin_addr.s_addr = inet_addr ((const char *)sIPnumber.c_str());  // set server’s IP

    if (connect(SServer, (SOCKADDR*) &target, sizeof(target)) == SOCKET_ERROR)
    { // an error connecting has occurred!
        WSACleanup ();
        return false;
    }

    return true;
}

void loadText(void)
{
     float* VertexArray;

     char text0[]={"ENTER HOST IP"};
     VertexArray=getTextVertexArray(text0, 13);
     for (int i=0;i<VertexArray[0];i++) text0VertexArray[i]=VertexArray[i+1];
     text0v=(int)VertexArray[0]-1;

     char text1[]={"NO CONNECTION"};
     VertexArray=getTextVertexArray(text1, 13);
     for (int i=0;i<VertexArray[0];i++) text1VertexArray[i]=VertexArray[i+1];
     text1v=(int)VertexArray[0]-1;

     char text2[]={"SOUND ERROR"};
     VertexArray=getTextVertexArray(text2, 11);
     for (int i=0;i<VertexArray[0];i++) text2VertexArray[i]=VertexArray[i+1];
     text2v=(int)VertexArray[0]-1;

     char text3[]={"PRESS <P> TO RESUME"};
     VertexArray=getTextVertexArray(text3, 19);
     for (int i=0;i<VertexArray[0];i++) text3VertexArray[i]=VertexArray[i+1];
     text3v=(int)VertexArray[0]-1;

     char text4[]={"SPAWNER OFFLINE"};
     VertexArray=getTextVertexArray(text4, 15);
     for (int i=0;i<VertexArray[0];i++) text4VertexArray[i]=VertexArray[i+1];
     text4v=(int)VertexArray[0]-1;

     char text5[]={"WAITING FOR HOST"};
     VertexArray=getTextVertexArray(text5, 16);
     for (int i=0;i<VertexArray[0];i++) text5VertexArray[i]=VertexArray[i+1];
     text5v=(int)VertexArray[0]-1;

     char text6[]={"YOU WON"};
     VertexArray=getTextVertexArray(text6, 7);
     for (int i=0;i<VertexArray[0];i++) text6VertexArray[i]=VertexArray[i+1];
     text6v=(int)VertexArray[0]-1;

     char text7[]={"YOU LOST"};
     VertexArray=getTextVertexArray(text7, 8);
     for (int i=0;i<VertexArray[0];i++) text7VertexArray[i]=VertexArray[i+1];
     text7v=(int)VertexArray[0]-1;

     char text8[]={"PRESS ENTER"};
     VertexArray=getTextVertexArray(text8, 11);
     for (int i=0;i<VertexArray[0];i++) text8VertexArray[i]=VertexArray[i+1];
     text8v=(int)VertexArray[0]-1;

     char text9[]={"LOBBY"};
     VertexArray=getTextVertexArray(text9, 5);
     for (int i=0;i<VertexArray[0];i++) text9VertexArray[i]=VertexArray[i+1];
     text9v=(int)VertexArray[0]-1;

     char text10[]={"ENTER NAME"};
     VertexArray=getTextVertexArray(text10, 10);
     for (int i=0;i<VertexArray[0];i++) text10VertexArray[i]=VertexArray[i+1];
     text10v=(int)VertexArray[0]-1;

     char text11[]={"                    "}; //Your Name
     VertexArray=getTextVertexArray(text11, 20);
     for (int i=0;i<VertexArray[0];i++) text11VertexArray[i]=VertexArray[i+1];
     text11v=(int)VertexArray[0]-1;

     char text12[]={"<R> EADY"};
     VertexArray=getTextVertexArray(text12, 8);
     for (int i=0;i<VertexArray[0];i++) text12VertexArray[i]=VertexArray[i+1];
     text12v=(int)VertexArray[0]-1;

     char text13[]={"<L> EAVE"};
     VertexArray=getTextVertexArray(text13, 8);
     for (int i=0;i<VertexArray[0];i++) text13VertexArray[i]=VertexArray[i+1];
     text13v=(int)VertexArray[0]-1;

     char text14[]={"<T> EAMSWAP"};
     VertexArray=getTextVertexArray(text14, 11);
     for (int i=0;i<VertexArray[0];i++) text14VertexArray[i]=VertexArray[i+1];
     text14v=(int)VertexArray[0]-1;

     char text15[]={"GAME STARTING"};
     VertexArray=getTextVertexArray(text15, 13);
     for (int i=0;i<VertexArray[0];i++) text15VertexArray[i]=VertexArray[i+1];
     text15v=(int)VertexArray[0]-1;
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//Windows functions

void KillGLWindow(void)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("TEST",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
    wc.hIcon            = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_TEST));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "TEST";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"TEST",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

        case WM_MOUSEMOVE:
        {
            cursorX=LOWORD(lParam);
            cursorY=HIWORD(lParam);
            if (wParam==MK_LBUTTON) mouse[0]=true; else mouse[0]=false;
            if (wParam==MK_RBUTTON) mouse[1]=true; else mouse[1]=false;
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            g_scroll=HIWORD(wParam);
            return 0;
        }

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
//	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
//	{
//		fullscreen=FALSE;							// Windowed Mode
//	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("TEST",windowWidth,windowHeight,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From GameCycle()
//			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
                    exitGame();
					done=TRUE;						// ESC Signalled A Quit
				}
				else                                // Not Time To Quit, Update Screen
				{
                    LastTime=CurrTime;
                    CurrTime=clock();
                    CycleTime=CurrTime-LastTime;
                    if (CycleTime!=0) FPS=float(1000/CycleTime);
                    if (iFPSdelay>49)
                    {
                        avgCycleTime=((float)CurrTime-(float)iTimeSomeCyclesAgo)/50;
                        if (CurrTime!=iTimeSomeCyclesAgo) avgFPS=(50000/(CurrTime-iTimeSomeCyclesAgo));
                        iTimeSomeCyclesAgo=CurrTime;
                        iFPSdelay=0;
                    }
                    else iFPSdelay++;

    				GameCycle();					// Draw The Scene
    				SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (fullscreen)
				{
    				if (!CreateGLWindow("TEST",windowWidth*2,windowHeight*2,16,fullscreen)) return 0;
                }
                else
				{
    				if (!CreateGLWindow("TEST",windowWidth,windowHeight,16,fullscreen)) return 0;
                }
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
