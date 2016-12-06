#include "GameManager.h"
#include <GL\glut.h>
#include <iostream>
#include <sstream>

using namespace std;

GameManager::GameManager()
{
	//lock = true;
	for (int i = 0; i < MAX_PLAYER; i++){
		player[i] = 0;
	}
	objectNumber = 0;
	playerNumber = 0;
	Color One(1, 0, 0);
	Color Two(0, 1, 0);
	add(new Ball(-1, 3, 0.05, 0, -1.5, 0.1,50,One)); 
	add(new Ball(1, 3, 0.05, 0, -1.5, 0.1, 50, One));
	add(new Ball(-2, -1, 0.05, 0, -1.5, 0.1,50,Two));
	add(new Ball(1, 0, 0.05, 0, 1.5, 0.1, 50, Two));
	add(new Ball(3, 0, 0.05, 0, 4, 0.1, 50, Two));
	add(new Ball(0, 0, 0.05, 0, -3, 0.1, 50,One));
	player[HOST] = new Box(-3, 0, 0.08, 0, 0, 0.2, 0.2,One,3);
	add(player[HOST]);
	player[GUEST] = new Box(3, 0, 0.08, 0, 0, 0.2, 0.2,Two,3);
	add(player[GUEST]);
	add(new Wall(0, 3, 4, 0.5));
	add(new Wall(0, -3, 4, 0.5));
	add(new Wall(-4, 0, 0.5, 2.5));
	add(new Wall(4, 0, 0.5, 2.5));
	//lock = false;
}


GameManager::~GameManager()
{
	if (collider_head != 0){
		Collider2D_Node*q = collider_head;
		while(q != 0){
			Collider2D_Node* temp = q;
			q = q->next;
			if (temp->content != 0){
				delete temp->content;
			}
			delete temp;
		}
	}
}

void GameManager::draw()
{
	if (collider_head != 0){
		for (Collider2D_Node*q = collider_head; q != 0;q=q->next){
			if (q->content != 0){
				q->content->draw();
			}
		}
	}
}

void GameManager::drawUI()
{
	float left = 3.7;
	float up = 2.8;
	Color color;
	int life = 0;
	for (int i = 0; i < MAX_PLAYER; i++){
		if (player[i]){
			color = player[i]->color;
			life = player[i]->life;
			switch (i){
			case 0:
				for (int i = 0; i < life; i++){
					drawLife(-left + i*LIFEGAP, up, color);
				}
				break;
			case 1:
				for (int i = 0; i < life; i++){
					drawLife(left - i*LIFEGAP, up, color);
				}
				break;
			case 2:
				for (int i = 0; i < life; i++){
					drawLife(-left + i*LIFEGAP, -up, color);
				}
				break;
			case 3:
				for (int i = 0; i < life; i++){
					drawLife(left - i*LIFEGAP, up, color);
				}
				break;
			}
		}
	}
}

void GameManager::drawLife(float pos_x, float pos_y, Color color)
{
	int ball_slice = 10;
	float ball_radius = 0.05;
	glPushMatrix();
	glTranslatef(pos_x, pos_y, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= ball_slice + 1; i++)
		glVertex3f(ball_radius * cos(2 * 3.14 / ball_slice * i), ball_radius * sin(2 * 3.14 / ball_slice * i), 0);
	glEnd();
	glPopMatrix();
}

void GameManager::moveOutside(Collider2D* collider)
{
	if (collider){
		collider->pos_x = 100;
		collider->pos_y = 100;
	}
}

void GameManager::freezeSpeed(Collider2D* collider)
{
	if (collider){
		collider->speed = 0;
		collider->dir_x = 0;
		collider->dir_y = 0;
	}
}

void GameManager::update()
{
	if (collider_head != 0){
		for (Collider2D_Node*q = collider_head; q != 0; q = q->next){
			if (q->content != 0){
				q->content->update();
				if (q->content->colliderType == COL2D_BOX){
					Box* tempPlayer = (Box*)q->content;
					if (tempPlayer->life <= 0){
						freezeSpeed(tempPlayer);
						moveOutside(tempPlayer);
					}
				}
			}
		}
	}
	if (collider_head != 0){
		for (Collider2D_Node*q = collider_head; q != 0; q = q->next){
			if (q->content != 0){
				for (Collider2D_Node*j = collider_head; j != 0; j = j->next){
					if (j->content != 0){
						q->content->testCollision(j->content);
						/*if (q->content->speed > MAX_SPEED)
						{
							q->content->speed = MAX_SPEED;
						}
						if (j->content->speed > MAX_SPEED)
						{
							j->content->speed = MAX_SPEED;
						}*/
					}
				}
			}
		}
	}
}

void GameManager::add(Collider2D* collider)
{
	Collider2D_Node* n = collider_head;
	if (n == 0){
		collider_head = new Collider2D_Node();
		collider_head->content = collider;
		collider_head->next = 0;
	}
	else{
		while (n->next != 0){
			n = n->next;
		}
		n->next = new Collider2D_Node();
		n->next->content = collider;
		n->next->next = 0;
	}
	if (collider->colliderType == COL2D_BOX){
		playerNumber++;
	}
	objectNumber++;
}

bool GameManager::deletePlayer(int number)
{
	if (collider_head!=0&&player[number] != 0){
		Collider2D_Node* p = collider_head;
		Collider2D_Node* p_last = 0;
		while (p->content!=player[number]&&p!= 0){
			p_last = p;
			p = p->next;
		}
		if (p == 0){//未找到该玩家
			return false;
		}
		else if (p->content == player[number]){//找到了该玩家
			//重新连接链表并删除玩家
			if (p == collider_head){//p指向head
				collider_head = p->next;
				delete p->content;
				delete p;
			}
			else{
				p_last->next = p->next;
				delete p->content;
				delete p;
			}
		}
	}
	else{
		return false;
	}
	player[number] = 0;
	playerNumber--;
	objectNumber--;
	return true;
}

void GameManager::printfAll()
{
	stringstream sstream;
	int length = 6 * MAX_OBJECT * CHARPERFLOAT;
	char* cArray = new char[length];
	Collider2D_Node* p = collider_head;
	while (p){
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
		p = p->next;
	}
	sstream.getline(cArray, length);
	cArray[strlen(cArray) + 1] = '\0';
	printf("%s\n",cArray);
	delete cArray;
}