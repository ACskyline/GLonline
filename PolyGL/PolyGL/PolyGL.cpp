#include "GameManager.h"
#include <math.h>
#include <time.h>
#include "stdio.h"
#include <cstdio> 
#include <Winsock2.h>
#include <sstream>
#include <fstream>
#define MAX_FPS 60
#define MAX_LAG 3
#define MAX_COUNT 2
#pragma comment(lib,"ws2_32.lib")

using namespace std;

GameManager GM;
clock_t startFrame = clock();
clock_t endFrame = 0;
SOCKET sockCom;
int player;
bool logout = false;
//HANDLE hMutex = NULL;/////////////////////////////////////////////////////////////

void ServerUpdateOther(int other_player,char key,int state)
{
	switch (key){
	case 'w':
		switch (state){
		case 1:
			GM.player[other_player]->dir_y = 1;
			break;
		case 0:
			GM.player[other_player]->dir_y = 0;
			break;
		default:
			break;
		}
		break;
	case 'a':
		switch (state){
		case 1:
			GM.player[other_player]->dir_x = -1;
			break;
		case 0:
			GM.player[other_player]->dir_x = 0;
			break;
		default:
			break;
		}
		break;
	case 's':
		switch (state){
		case 1:
			GM.player[other_player]->dir_y = -1;
			break;
		case 0:
			GM.player[other_player]->dir_y = 0;
			break;
		default:
			break;
		}
		break;
	case 'd':
		switch (state){
		case 1:
			GM.player[other_player]->dir_x = 1;
			break;
		case 0:
			GM.player[other_player]->dir_x = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void ServerRecvThread()
{
	while (true){
		char recvBuf[100];
		if (recv(sockCom, recvBuf, 100, 0) != SOCKET_ERROR)
		{
			int other_player = recvBuf[0] - '0';
			char key = recvBuf[1];
			int state = recvBuf[2] - '0';
			ServerUpdateOther(other_player,key,state);
		}
		else{
			printf("Lost connection!\n");
			logout = true;
			return;
		}
	}
}

void ServerSendThread()///////////////////////////////////////打包并发送位置信息
{
	int result = -4;
	while (true){
		stringstream sstream;
		int length = 6 * MAX_OBJECT * CHARPERFLOAT;
		char* cArray = new char[length];
		Collider2D_Node* p = GM.collider_head;
		if (p){
			while(p){
				if (p->content){/////////////////////////////////////////////////
					if (p->content->colliderType != COL2D_WALL){
						sstream << p->content->pos_x << ' ';
						sstream << p->content->pos_y << ' ';
						sstream << p->content->dir_x << ' ';
						sstream << p->content->dir_y << ' ';
						sstream << p->content->speed << ' ';
						if (p->content->colliderType == COL2D_BOX){
							Box* tempBox = (Box*)p->content;
							sstream << tempBox->life << ' ';
						}
					}
				}
				else{
					printf("no content\n");
				}
				p = p->next;
			}
			sstream.getline(cArray, length);
			cArray[strlen(cArray) + 1] = '\0';
			for (int i = 0; i < 3; i++){////////////////////////////////////////////////////发三次
				result = send(sockCom, cArray, sizeof(char)*length, 0);
				if (result == SOCKET_ERROR)//发送信息
				{
					printf("Lost connection!\n");
					logout = true;
					return;
				}
			}
			//printf("%s\n", cArray);
		}
		delete cArray;
		Sleep(1000/MAX_FPS/2);//
	}
}

void ClientRecvThread()////////////////////////////////////////接收并更新位置信息
{
	int result = -4;
	int trump = 0;//
	while (true){
		stringstream sstream;
		int length = 6 * MAX_OBJECT * CHARPERFLOAT;
		char* cArray = new char[length];
		char* cArray_temp = new char[length];
		Collider2D_Node* p = GM.collider_head;
		if (p){
			//do{
				//do{
					result = recv(sockCom, cArray, sizeof(char)*length, 0);
					if (result == SOCKET_ERROR){
						printf("You are disconnected!\n");
						logout = true;
						return;
					}
				//} while (result != sizeof(char)*length);
				//do{
					result = recv(sockCom, cArray_temp, sizeof(char)*length, 0);
					if (result == SOCKET_ERROR){
						printf("You are disconnected!\n");
						logout = true;
						return;
					}
				//} while (result != sizeof(char)*length);
			//} while (strcmp(cArray, cArray_temp)!=0);
			//printf("%s\n", cArray);
					if (strcmp(cArray, cArray_temp) != 0){
						trump = 1;
					}
			if (trump == 0){//
				sstream.str(cArray);
				while (p&&!sstream.eof()){//更新位置
					if (p->content){/////////////////////////////////////////////////
						if (p->content->colliderType != COL2D_WALL){
							float posX;
							float posY;
							float dirX;
							float dirY;
							float speeD;
							sstream >> posX;//p->content->pos_x;
							sstream >> posY;//p->content->pos_y;
							sstream >> dirX;// p->content->dir_x;
							sstream >> dirY;// p->content->dir_y;
							sstream >> speeD;// p->content->speed;
							/*if (p->content->distance(posX, posY, p->content->pos_x, p->content->pos_y) >= MAX_LAG)
							{
								printf("recv:%s\nnow:", cArray);
								GM.printfAll();
								break;
							}
							else{*/
								p->content->pos_x = posX;//;
								p->content->pos_y = posY;//;
								p->content->dir_x = dirX;// ;
								p->content->dir_y = dirY;// ;
								p->content->speed = speeD;//;
								if (p->content->colliderType == COL2D_BOX)
								{
									Box* tempBox = (Box*)p->content;
									sstream >> tempBox->life;
								}
							//}
						}
					}
					else{
						printf("no content\n");
					}
					p = p->next;
				}
				trump = 1;//
			}
			trump--;//
		}
		delete cArray;
		delete cArray_temp;
	}
}

int initServerSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);//

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//

	listen(sockSrv, 10);//

	SOCKADDR_IN  addrClient;
	int len = sizeof(SOCKADDR);
	sockCom = accept(sockSrv, (SOCKADDR*)&addrClient, &len);//

	return 0;
}

int initClientSocket(char* serverAddr)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(1, 1);

	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}

	sockCom = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(serverAddr);      // 本地回路地址是127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	while (connect(sockCom, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR){}
	return 0;
}

void init()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}

void display()
{
	//设置摄像机位置和二维平面位置
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0, 0, -5);
	
	//WaitForSingleObject(hMutex, INFINITE);/////////////////////////////////////////////////////////////
	//绘制所有物体
	GM.draw();

	GM.drawUI();

	//服务器更新所有物体的位置，断线后每个玩家都和服务器的行为一样
	//if (player == 1 || logout == true){
		//WaitForSingleObject(hMutex, INFINITE);/////////////////////////////////////////////////////////////
		GM.update();
		//服务器判断所有玩家的生命
		/*for (int i = 0; i < MAX_PLAYER; i++){
			if (GM.player[i] != 0 && GM.player[i]->life <= 0){
				//if (!GM.lock){
					printf("delete %d\n", i);
					//GM.lock = true;
					GM.deletePlayer(i);//删除玩家//////////////////////////////////////////////////////////////////////////////////////////////////
					//GM.lock = false;
				//}
			}
		}*/
		//ReleaseMutex(hMutex);/////////////////////////////////////////////////////////////
	//}
	//ReleaseMutex(hMutex);/////////////////////////////////////////////////////////////
	

	//刷新频率最大MAX_FPS
	endFrame = clock(); 
	while (endFrame - startFrame < 1000 / MAX_FPS){ endFrame = clock();}//Sleep(5);
	startFrame = clock();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	if (0 == h)
		h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	if (GM.player[player]){
		char sendBuf[100];
		if (player == HOST || logout == true){//主
			switch (key){
			case 'w':
				GM.player[player]->dir_y = 1;
				break;
			case 'a':
				GM.player[player]->dir_x = -1;
				break;
			case 's':
				GM.player[player]->dir_y = -1;
				break;
			case 'd':
				GM.player[player]->dir_x = 1;
				break;
			default:
				break;
			}
		}
		else{//从
			switch (key){
			case 'w':
				GM.player[player]->dir_y = 1;
				sprintf(sendBuf, "%dw1\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 'a':
				GM.player[player]->dir_x = -1;
				sprintf(sendBuf, "%da1\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 's':
				GM.player[player]->dir_y = -1;
				sprintf(sendBuf, "%ds1\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 'd':
				GM.player[player]->dir_x = 1;
				sprintf(sendBuf, "%dd1\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			default:
				break;
			}
		}
	}
}

void keyboardup(unsigned char key, int x, int y)
{
	if (GM.player[player]){
		char sendBuf[100];
		if (player == HOST || logout == true){//主
			switch (key){
			case 'w':
				GM.player[player]->dir_y = 0;
				break;
			case 'a':
				GM.player[player]->dir_x = 0;
				break;
			case 's':
				GM.player[player]->dir_y = 0;
				break;
			case 'd':
				GM.player[player]->dir_x = 0;
				break;
			default:
				break;
			}
		}
		else{//从
			switch (key){
			case 'w':
				GM.player[player]->dir_y = 0;
				sprintf(sendBuf, "%dw0\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 'a':
				GM.player[player]->dir_x = 0;
				sprintf(sendBuf, "%da0\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 's':
				GM.player[player]->dir_y = 0;
				sprintf(sendBuf, "%ds0\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			case 'd':
				GM.player[player]->dir_x = 0;
				sprintf(sendBuf, "%dd0\0", player);
				send(sockCom, sendBuf, strlen(sendBuf) + 1, 0);//发送信息
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char** argv)
{
	HANDLE hThread_ServerRecv, hThread_ServerSend, hThread_ClientRecv;
	DWORD ThreadID_ServerRecv, ThreadID_ServerSend,ThreadID_ClientRecv;
	//hMutex = CreateMutex(NULL, NULL, NULL);
	printf("Are you server or client? 1:server 2:client\n");
	char SC = getchar();
	int result = 1;
	if (SC == '1'){
		player = HOST;
		printf("Waiting\n");
		result = initServerSocket();
	}
	else if (SC == '2'){
		player = GUEST;
		printf("Enter server address:\n");
		char serverAddr[17];
		fflush(stdin);
		gets(serverAddr);
		printf("Waiting\n");
		result = initClientSocket(serverAddr);
	}
	else{//输入既不是1也不是2
		printf("Invalid\n");
		system("pause");
		return 0;
	}
	if (result == 0){
		printf("Succeed\n");
		if (player == HOST){//主
			hThread_ServerRecv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerRecvThread, NULL, 0, &ThreadID_ServerRecv);
			hThread_ServerSend = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerSendThread, NULL, 0, &ThreadID_ServerSend);
		}
		else{//从
			hThread_ClientRecv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientRecvThread, NULL, 0, &ThreadID_ClientRecv);
		}
		srand((unsigned)time(NULL));
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize(800, 600);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("PolyGL");
		init();
		glutDisplayFunc(display);//服务端更新数据并绘制，客户端只绘制
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardup);
		glutMainLoop();
	}
	else{
		printf("Fail\n");
	}
	closesocket(sockCom);
	WSACleanup();
	system("pause");
	return 0;
}