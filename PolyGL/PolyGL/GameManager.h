#pragma once
#include "Ball.h"
#define MAX_PLAYER 4
#define HOST 0
#define GUEST 1
#define LIFEGAP 0.3
#define MAX_SPEED 0.08
#define MAX_OBJECT 20
#define CHARPERFLOAT 20

struct Collider2D_Node{
	Collider2D* content = 0;
	Collider2D_Node* next = 0;
};
class GameManager
{
private:
	void freezeSpeed(Collider2D* collider);
	void moveOutside(Collider2D* collider);
	void GameManager::drawLife(float pos_x, float pos_y, Color color);
public:
	Collider2D_Node* collider_head;
	Box* player[MAX_PLAYER];
	int playerNumber;
	int objectNumber;
	//bool lock;
	GameManager();
	~GameManager();
	void draw();
	void update();
	void add(Collider2D* collider);
	bool deletePlayer(int number);
	void drawUI();
	void printfAll();
};