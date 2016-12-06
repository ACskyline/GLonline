#include "Ball.h"
#include <math.h>
#define Pi 3.14
#define COFFSET 0.05

Ball::Ball()
{
	colliderType = COL2D_BALL;
	ball_slice = 50;
	ball_radius = 0.1;
	color = Color(1, 0, 0);
}

Ball::Ball(float x_pos, float y_pos, float _speed, float x_dir, float y_dir, float radius_ball, int slice_ball, Color _color)
{
	colliderType = COL2D_BALL;
	pos_x = x_pos;
	pos_y = y_pos;
	dir_x = x_dir;
	dir_y = y_dir;
	speed = _speed;
	ball_radius = radius_ball;
	ball_slice = slice_ball;
	color = _color;
}


Ball::~Ball()
{
}

void Ball::draw()
{
	glPushMatrix();
	glTranslatef(pos_x,pos_y,0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= ball_slice+1; i++)
		glVertex3f(ball_radius * cos(2 * Pi / ball_slice * i), ball_radius * sin(2 * Pi / ball_slice * i), 0);
	glEnd();
	glPopMatrix();
}

bool Ball::testCollision(Collider2D* collider2d_other)
{
	Ball* ball_other = (Ball*)collider2d_other;
	Wall* wall_other = (Wall*)collider2d_other;
	Box* box_other = (Box*)collider2d_other;
	bool collide = false;
	float ex = collider2d_other->pos_x - pos_x;
	float ey = collider2d_other->pos_y - pos_y;
	float ex_Ab = abs(ex);
	float ey_Ab = abs(ey);
	switch (collider2d_other->colliderType){
	case COL2D_BALL:
		if (!away(ball_other)){
			if ((ball_other->ball_radius + ball_radius)*(ball_other->ball_radius + ball_radius) >= ((ball_other->pos_x) - (pos_x))*((ball_other->pos_x) - (pos_x)) + ((ball_other->pos_y) - (pos_y))*((ball_other->pos_y) - (pos_y))){
				//float rel_dirX = ball_other->pos_x - pos_x;
				//float rel_dirY = ball_other->pos_y - pos_y;
				normalize(&ex, &ey);
				float rel_speedU = ex*dir_x*speed + ey*dir_y*speed;
				//对另一个碰撞体相对方向应该取相反数
				float rel_speedU_other = -ex*ball_other->dir_x*ball_other->speed - ey*ball_other->dir_y*ball_other->speed;
				//更改自己的方向和速度
				dir_x = speed*dir_x - ex*rel_speedU - ex*rel_speedU_other;
				dir_y = speed*dir_y - ey*rel_speedU - ey*rel_speedU_other;
				speed = magnitude(dir_x, dir_y);
				normalize(&dir_x, &dir_y);
				//更改另一个碰撞体的方向和速度
				ball_other->dir_x = ball_other->speed*ball_other->dir_x + ex*rel_speedU + ex*rel_speedU_other;
				ball_other->dir_y = ball_other->speed*ball_other->dir_y + ey*rel_speedU + ey*rel_speedU_other;
				ball_other->speed = magnitude(ball_other->dir_x, ball_other->dir_y);
				normalize(&ball_other->dir_x, &ball_other->dir_x);
				//返回真
				collide = true;
			}
		}
		break;
	case COL2D_POLYGON:
		break;
	case COL2D_BOX:
		/*if (collideBoxVertex(box_other)){
			normalize(&ex, &ey);
			float VUx = ex*(ex*dir_x*speed + ey*dir_y*speed);
			float VUy = ey*(ex*dir_x*speed + ey*dir_y*speed);
			dir_x = dir_x*speed - 2 * VUx;
			dir_y = dir_y*speed - 2 * VUy;
			speed = magnitude(dir_x, dir_y);
			normalize(&dir_x, &dir_y);
			collide = collide || true;
		}
		else*/ 
		if ((ex_Ab>ey_Ab)&&(pos_x - box_other->pos_x >= 0) && ((pos_x+dir_x*speed) - (box_other->pos_x+box_other->dir_x*box_other->speed) <= box_other->left + ball_radius + COFFSET) && (speed*dir_x-box_other->speed*box_other->dir_x < 0) && abs(pos_y-box_other->pos_y)<=ball_radius+box_other->up){//此圆球在正方体的右侧且发生碰撞
			if (box_other->dir_x > 0 && box_other->speed*box_other->dir_x > abs(speed*dir_x)){
				dir_x = box_other->speed*box_other->dir_x;
				dir_y = speed*dir_y;
				speed = magnitude(dir_x, dir_y);
				normalize(&dir_x, &dir_y);
			}
			else{
				dir_x = -dir_x;
			}
			pos_x = box_other->pos_x + box_other->left + ball_radius + COFFSET;
			collide = collide || true;
			if (!color.equalTo(box_other->color)){
				box_other->life--;
			}
		}
		else if ((ex_Ab>ey_Ab) && (box_other->pos_x - pos_x >= 0) && ((box_other->pos_x + box_other->dir_x*box_other->speed) - (pos_x + dir_x*speed) <= box_other->left + ball_radius + COFFSET) && (speed*dir_x - box_other->speed*box_other->dir_x > 0) && abs(pos_y - box_other->pos_y) <= ball_radius + box_other->up){//此圆球在正方体的左侧且发生碰撞
			if (box_other->dir_x < 0 && -box_other->speed*box_other->dir_x > abs(speed*dir_x)){
				dir_x = box_other->speed*box_other->dir_x;
				dir_y = speed*dir_y;
				speed = magnitude(dir_x, dir_y);
				normalize(&dir_x, &dir_y);
			}
			else{
				dir_x = -dir_x;
			}
			pos_x = box_other->pos_x - box_other->left - ball_radius - COFFSET;
			collide = collide || true; 
			if (!color.equalTo(box_other->color)){
				box_other->life--;
			}
		}
		else if ((ex_Ab <= ey_Ab) && (pos_y - box_other->pos_y >= 0) && ((pos_y + dir_y*speed) - (box_other->pos_y + box_other->dir_y*box_other->speed) <= box_other->up + ball_radius + COFFSET) && (speed*dir_y - box_other->speed*box_other->dir_y < 0) && abs(pos_x - box_other->pos_x) <= ball_radius + box_other->left){//此圆球在正方体的上侧且发生碰撞
			if (box_other->dir_y > 0 && box_other->speed*box_other->dir_y > abs(speed*dir_y)){
				dir_y = box_other->speed*box_other->dir_y;
				dir_x = speed*dir_x;
				speed = magnitude(dir_x, dir_y);
				normalize(&dir_x, &dir_y);
			}
			else{
				dir_y = -dir_y;
			}
			pos_y = box_other->pos_y + box_other->up + ball_radius + COFFSET;
			collide = collide || true;
			if (!color.equalTo(box_other->color)){
				box_other->life--;
			}
		}
		else if ((ex_Ab <= ey_Ab) && (box_other->pos_y - pos_y >= 0) && ((box_other->pos_y + box_other->dir_y*box_other->speed) - (pos_y + dir_y*speed) <= box_other->up + ball_radius + COFFSET) && (speed*dir_y - box_other->speed*box_other->dir_y > 0) && abs(pos_x - box_other->pos_x) <= ball_radius + box_other->left){//此圆球在正方体的下侧且发生碰撞
			if (box_other->dir_y < 0 && -box_other->speed*box_other->dir_y > abs(speed*dir_y)){
				dir_y = box_other->speed*box_other->dir_y;
				dir_x = speed*dir_x;
				speed = magnitude(dir_x, dir_y);
				normalize(&dir_x, &dir_y);
			}
			else{
				dir_y = -dir_y;
			}
			pos_y = box_other->pos_y - box_other->up - ball_radius-COFFSET;
			collide = collide || true;
			if (!color.equalTo(box_other->color)){
				box_other->life--;
			}
		}
		break;
	case COL2D_WALL:
		if ((pos_x - wall_other->pos_x >= 0) && (pos_y + ball_radius <= wall_other->pos_y + wall_other->up&&pos_y - ball_radius >= wall_other->pos_y - wall_other->up) && (pos_x - wall_other->pos_x <= wall_other->left + ball_radius + COFFSET) && dir_x<0){//此圆球在墙的右侧且发生碰撞
			pos_x = wall_other->pos_x + wall_other->left + ball_radius;
			dir_x = -dir_x;
			collide = collide || true;
		}
		else if ((wall_other->pos_x - pos_x >= 0) && (pos_y + ball_radius <= wall_other->pos_y + wall_other->up&&pos_y - ball_radius >= wall_other->pos_y - wall_other->up) && (wall_other->pos_x - pos_x <= wall_other->left + ball_radius + COFFSET) && dir_x>0){//此圆球在墙的左侧且发生碰撞
			pos_x = wall_other->pos_x - wall_other->left - ball_radius;
			dir_x = -dir_x;
			collide = collide || true;
		}
		else if ((pos_y - wall_other->pos_y >= 0) && (pos_x + ball_radius <= wall_other->pos_x + wall_other->left&&pos_x - ball_radius >= wall_other->pos_x - wall_other->left) && (pos_y - wall_other->pos_y <= wall_other->up + ball_radius + COFFSET) && dir_y<0){//此圆球在墙的上侧且发生碰撞
			pos_y = wall_other->pos_y + wall_other->up + ball_radius;
			dir_y = -dir_y;
			collide = collide || true;
		}
		else if ((wall_other->pos_y - pos_y >= 0) && (pos_x + ball_radius <= wall_other->pos_x + wall_other->left&&pos_x - ball_radius >= wall_other->pos_x - wall_other->left) && (wall_other->pos_y - pos_y <= wall_other->up + ball_radius + COFFSET) && dir_y>0){//此圆球在墙的下侧且发生碰撞
			pos_y = wall_other->pos_y - wall_other->up - ball_radius;
			dir_y = -dir_y;
			collide = collide || true;
		}
		break;
	case COL2D_UNDEFINED:
		break;
	default:
		break;
	}
	return collide;
}

bool Ball::collideBoxVertex(Box* box_other){
	if ((magnitude(box_other->pos_x + box_other->left - pos_x, box_other->pos_y + box_other->up - pos_y) <= ball_radius && !awayVertex(box_other->pos_x + box_other->left, box_other->pos_y + box_other->up)) 
		|| (magnitude(box_other->pos_x + box_other->left - pos_x, box_other->pos_y - box_other->up - pos_y) <= ball_radius && !awayVertex(box_other->pos_x + box_other->left, box_other->pos_y - box_other->up))
		|| (magnitude(box_other->pos_x - box_other->left - pos_x, box_other->pos_y - box_other->up - pos_y) <= ball_radius && !awayVertex(box_other->pos_x - box_other->left, box_other->pos_y - box_other->up))
		|| (magnitude(box_other->pos_x - box_other->left - pos_x, box_other->pos_y + box_other->up - pos_y) <= ball_radius && !awayVertex(box_other->pos_x - box_other->left, box_other->pos_y + box_other->up)))
	{
		return true;
	}
	else{
		return false;
	}
}

