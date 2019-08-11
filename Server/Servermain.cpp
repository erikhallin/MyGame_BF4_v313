#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <ctime>
#include <math.h>

using namespace std;

int           g_terrainWidth=101;
int           g_terrainLength=101;
//static float  g_height[101][101];
SOCKET        SClient[8];
SOCKET        SServer;
int           numOfClients=0;
int           maxNumOfClients=8;
int           numOfPlayers=8;
static float  playerData[8][20];
int           seed=1;
int           CurrTime=0;
int           LastTime=0;
int           ping;
int           displayPingDelay=0;
int           spawnXpos[8];
int           spawnZpos[8];
int           gameMode=0;
string        g_serverStringName;
char          g_serverName[20];
int           g_random;

//Lobby
bool   g_playerActive[8]={false,false,false,false,false,false,false,false};
bool   g_playerReady[8]={false,false,false,false,false,false,false,false};
int    playerLobbyData[9][20];

void sendStartData(SOCKET,int);
void getPlayerData(SOCKET,int);
void sendPlayerData(SOCKET,int);
void setSpawnerPositions(void);
void getPlayerLobbyData(SOCKET,int);
void sendPlayerLobbyData(SOCKET,int);
void sendServerLobbyData(SOCKET,int);

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
//    srand(time(0));
//    seed=rand()%1000;
//    cout << seed << "\n";
//    cout << time(0) << "\n";

    cout << "SERVER \n";
    WSADATA w;
    int error = WSAStartup (0x0202, &w);
    if (error)
    { // there was an error
      return 0;
    }
    if (w.wVersion != 0x0202)
    { // wrong WinSock version!
      WSACleanup (); // unload ws2_32.dll
      return 0;
    }
    cout << "WSADATA OK \n";
//=====================================================
    SServer = socket (AF_INET, SOCK_STREAM, 0); // Create socket
//    cout << "Created SOCKET SServer \n";
//========================================================
    sockaddr_in addr; // the address structure for a TCP socket
    addr.sin_family = AF_INET;  	// Address family Internet
    addr.sin_port = htons (666);   // Assign port 5001 to this socket
    addr.sin_addr.s_addr = htonl (INADDR_ANY);   // No destination
//    cout << "sockaddr_in created and filled\n";
//=========================================================================
    //Show IP
    char hostName[80];
    if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR) {
        cerr << "Error " << WSAGetLastError() <<
                " when getting local host name." << endl;
        return 0;
    }
    cout << "Host name is " << hostName << "." << endl;

    struct hostent* hostInfo = gethostbyname(hostName);
    if (hostInfo == 0) {
        cerr << "Yow! Bad host lookup." << endl;
        return 0;
    }

    for (int i = 0; hostInfo->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, hostInfo->h_addr_list[i], sizeof(struct in_addr));
        cout << "Server IP Address: " << inet_ntoa(addr) << endl;
    }
//=========================================================================
    if (bind(SServer, (LPSOCKADDR) &addr, sizeof(addr)) == SOCKET_ERROR)
    { // error
      WSACleanup ();  // unload WinSock
      return 0;     	// quit
    }
//    cout << "Bound IP to port \n";
//=======================================================================
    //Choose server name
    bool done=false;
    while (!done)
    {
        cout << "\nEnter Game Name:";
        cin.getline (g_serverName,256);
        if (strlen(g_serverName)>10) cout << "\nName too long, max 10 letters";
        else done=true;
    }
    g_serverStringName.assign( g_serverName, 15 );
    //Change to uppercase
    for (int i=0;i<15;i++)
    {
        if (g_serverName[i]>95) g_serverName[i]-=32;
    }
//=======================================================================
    //Choose GameMode
    done=false;
    while (!done) //Get gamemode
    {
        int input=0;
        cout << "\nChoose GameMod: (1) Free for All\n                (2) Teams\n                 ";
        cin >> input;
        if (input==1)
        {
            gameMode=1;
            done=true;
        }
        else if (input==2)
        {
            gameMode=2;
            done=true;
        }
        else cout << "\nChoose 1 or 2\n";
    }
//=====================================================
    //Choose Seed
    cout << "\nEnter Seed:";
    cin >> seed;
//=====================================================
    if (listen(SServer,10)==SOCKET_ERROR)
    { // error!  unable to listen
      WSACleanup ();
      return 0;
    }
    cout << "\nNow Listening and Accepting for " << numOfPlayers << " Clients\n";
//=====================================================================

    //LobbyLoop values
    SOCKET acceptSocket;
    bool doneLobby=false;
    numOfClients=0;
    u_long mode=1;
    for (int i=0;i<maxNumOfClients+1;i++) //Clear data
    for (int j=0;j<20;j++)
    {
        playerLobbyData[i][j]=0;
    }
    for (int i=0;i<15;i++) //Insert Server name
    {
        playerLobbyData[8][i+5]=g_serverName[i];
    }
    for (int i=0;i<maxNumOfClients;i++) //Insert Player ID
    {
        playerLobbyData[i][2]=i;
    }
    playerLobbyData[8][0]=1; //ServerStatus
    playerLobbyData[8][1]=gameMode;
    playerLobbyData[8][2]=seed;
    mode=1;
    ioctlsocket(SServer, FIONBIO, &mode); //Socket is now Non-Blocking
    //LobbyLoop
    //playerLobbyData[20] -> [0]-(0=left game,1=active,2=ready)
    while (!doneLobby)
    {
        //Get Lobby data from active players
        if (numOfClients>0)
        {
            for (int i=0;i<maxNumOfClients;i++)
            {
                if (g_playerActive[i])
                {
                    getPlayerLobbyData(SClient[i], i);
                    //Left or ready?
                    if (playerLobbyData[i][0]==0) {g_playerActive[i]=false; g_playerReady[i]=false;} //Left
                    if (playerLobbyData[i][0]==1) {g_playerReady[i]=false;} //Not ready
                    if (playerLobbyData[i][0]==2) {g_playerReady[i]=true;} //Ready
                }
            }
        }
        //Count players online
        numOfClients=0;
        for (int i=0;i<maxNumOfClients;i++)
        {
            if (g_playerActive[i]) numOfClients++;
        }
        //New Clients?
        if (numOfClients<8)
        {
  //          cout << "innan accept\n";
            acceptSocket = accept(SServer, NULL, NULL );
   //         cout << "efter accept\n";
            if (acceptSocket != SOCKET_ERROR)
            {

                //Find empty slot
                int emptySlot=-1;
                for (int i=0;i<maxNumOfClients;i++)
                {
                    if (!g_playerActive[i]) {emptySlot=i; g_playerActive[i]=true; break;}
                }
                if (emptySlot!=-1)
                {
                    SClient[emptySlot] = acceptSocket;
                    mode=0;
                    ioctlsocket(SClient[emptySlot], FIONBIO, &mode); //Socket is now Blocking
                    numOfClients++;
                    cout << "Accepted new Client\n";
                    getPlayerLobbyData(SClient[emptySlot], emptySlot);
                    sendServerLobbyData(SClient[emptySlot], emptySlot);
                    getPlayerLobbyData(SClient[emptySlot], emptySlot);
                }
            }
        }
        //Everyone ready?
        if (numOfClients>0)
        {
            int playersReady=0;
            for (int i=0;i<maxNumOfClients;i++)
            {
                if (g_playerReady[i]) playersReady++;
            }
            if (numOfClients==playersReady) //Everyone ready!
            {
                doneLobby=true;
                playerLobbyData[8][0]=2;
            }
        }
        //skicka ut info till alla andra
        if (numOfClients>0)
        {
            for (int i=0;i<maxNumOfClients;i++)
            {
                if (g_playerActive[i])
                {
                    sendPlayerLobbyData(SClient[i], i);
                }
            }
        }
//        cout << numOfClients << "\n";
    }
    //-------------------------------------------------------------------
    mode=0;
    ioctlsocket(SServer, FIONBIO, &mode); //Blocking again
    cout << "\nStarting Game\n";
    cout << "-------------------------------------\n";
/*    //Debug: display data
    for (int i=0;i<maxNumOfClients;i++)
    {
        if (g_playerActive[i])
        {
            cout << i << " - ";
            for (int j=0;j<20;j++)
            cout << playerLobbyData[i][j] << " ";
            cout << "\n\n";
        }
    }
*/
    numOfPlayers=numOfClients;

    //reset playerData
    for (int i=0;i<8;i++)
    for (int j=0;j<20;j++)
    playerData[i][j]=0;

    //Get data from all clients first time
    for (int i=0;i<maxNumOfClients;i++)
    {
        if (g_playerActive[i]) playerData[i][8]=1;
    }

    cout << "Sending and Recieving player data\n";
    int playersOnline=1;
    int playersLastOnline=numOfPlayers;
    int iUpdatePingDelay=100;
    LastTime=clock();
    while (playersOnline>0)
    {
        //Get data
        for (int i=0;i<maxNumOfClients;i++)
        {
            if (playerData[i][8]) //If Player Online, get data
            getPlayerData(SClient[i], i);
        }
        //New random value
        int iRandom=rand()%10000+10000;
        for (int i=0;i<maxNumOfClients;i++) playerData[i][17]=iRandom;
        //send data
        for (int i=0;i<maxNumOfClients;i++)
        {
            if (playerData[i][8]) //If Player Online, send data
            sendPlayerData(SClient[i], i);
        }
        //check if any players left
        playersOnline=0;
        for (int i=0;i<maxNumOfClients;i++)
        {
            if (playerData[i][8]) playersOnline++;
        }
        if (playersOnline<playersLastOnline)
        {
            cout << "\nPlayer has left the game\n";
            playersLastOnline--;
        }
        //Check ping
        if (iUpdatePingDelay<1)
        {
            CurrTime=clock();
            ping=CurrTime-LastTime;
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                      \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                 << "Response Time: "<< ping/100 << " ms";
            LastTime=CurrTime;
            iUpdatePingDelay=100;
        }
        else iUpdatePingDelay--;
    }
    cout << "\nNo Players left\n\n";
    WSACleanup ();

    system("PAUSE");
    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------

void getPlayerData(SOCKET Sclient, int currPlayer)
{
    float currPlayerData[20];
    if (recv (Sclient, (char*)&currPlayerData, sizeof(currPlayerData), 0)==SOCKET_ERROR)
    {
        cout << "\nError recv data from player " << currPlayer+1 << "\n";
        playerData[currPlayer][8]=0;
    }
    else
    {
        for (int i=0;i<20;i++)
        {
            playerData[currPlayer][i]=currPlayerData[i];
        }
    }
}

void getPlayerLobbyData(SOCKET Sclient, int currPlayer)
{
//    cout << "innan recv\n";
    int currPlayerLobbyData[20];
    if (recv (Sclient, (char*)&currPlayerLobbyData, sizeof(currPlayerLobbyData), 0)==SOCKET_ERROR)
    {
        cout << "\nError recv lobby data from player " << currPlayer+1 << "\n";
        playerLobbyData[currPlayer][0]=0;
        g_playerActive[currPlayer]=false;
        g_playerReady[currPlayer]=false;
    }
    else
    {
//        cout << "Succsesfull recv \n";
        for (int i=0;i<20;i++)
        {
            playerLobbyData[currPlayer][i]=currPlayerLobbyData[i];
        }
        playerLobbyData[currPlayer][2]=currPlayer;
    }
//    cout << "recv klar \n";
}

void sendPlayerData(SOCKET Sclient, int currPlayer)
{
    if (playerData[currPlayer][8]) //if online
    {
        if (send (Sclient, (const char*)&playerData, sizeof(playerData), 0)==SOCKET_ERROR)
        {
             cout << "\nError send data to player " << currPlayer+1 << "\n";
             playerData[currPlayer][8]=0; //set offline
        }
    }
}

void sendPlayerLobbyData(SOCKET Sclient, int currPlayer)
{
//    cout << "innan send \n";
    if (send (Sclient, (const char*)&playerLobbyData, sizeof(playerLobbyData), 0)==SOCKET_ERROR)
    {
         cout << "\nError send lobby data to player " << currPlayer+1 << "\n";
         playerLobbyData[currPlayer][0]=0;
         g_playerActive[currPlayer]=false;
         g_playerReady[currPlayer]=false;
    }
//    else cout << "Succsesfull send \n";
//    cout << "efter send \n";
}

void sendServerLobbyData(SOCKET Sclient, int currPlayer)
{
//    cout << "innan send \n";
    playerLobbyData[8][3]=currPlayer;
    if (send (Sclient, (const char*)&playerLobbyData[8], sizeof(playerLobbyData[8]), 0)==SOCKET_ERROR)
    {
         cout << "\nError send lobby data to player " << currPlayer+1 << "\n";
         playerLobbyData[currPlayer][0]=0;
         g_playerActive[currPlayer]=false;
         g_playerReady[currPlayer]=false;
    }
//    else cout << "Succsesfull send \n";
//    cout << "efter send \n";
}
