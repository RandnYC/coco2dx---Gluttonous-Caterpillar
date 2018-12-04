#include "Robot.h"

Robot::Robot( const Size& _sz) 
	: Snake() , area(_sz) , cnt(0) ,last_action(0),risk(0.0f)
{}

Robot* Robot::create(const Size& _sz)
{
	auto sprite = new Robot(_sz);
	if (sprite)
	{
		sprite->Snake::autorelease();
		//sprite->Snake::scheduleUpdate();

		sprite->setBodySize(DEFAULT_BODY_SIZE);
		return sprite;
	}
	else
	{
		CC_SAFE_DELETE(sprite);
		CC_ASSERT("Create Player Fail");
	}
	return nullptr;
}

void Robot::update()
{
	Snake::update();
	risk = evalueWall(0.0f);

	if (risk <= CONST_WARNING) move();
	if (cnt >= 150 || risk > CONST_WARNING) moveTowardCenter();
	if (risk >= CONST_DANGEROUS) log("Robot In Dangerous Area");
}

void Robot::update(float dt)
{
	update();
}

Action* Robot::runAction(RotateBy* _act , int)
{
	last_action = _act->clone();
	return this->Snake::runAction(_act);
}

void Robot::predictPosition(degree rot , queue<PR>& que ,float speed)const
{
	while (!que.empty()) que.pop();		//重置清空队列

	degree head_rot = this->getHead()->getRotation() + rot;
	auto head_pos = this->getPosition() + Vec2(head_rot)*speed;
	que.push(make_pair(head_pos, head_rot));	//头节点入队

	for (int j = 1; j < bodyNumber(); j++)
	{
		auto pre = que.back();
		auto nd = body.at(j);

		degree rot_pre = pre.second;
		auto v_pre = Vec2(rot_pre);

		auto pre_back = pre.first - this->getRadius() * Vec2(pre.second);
		auto now_back = nd->back;
		auto v_now = Vec2(pre_back - now_back);
		degree rot_now(v_now);


		if (rot_now.angle(rot_pre) > MAX_ROT)
		{
			rot_now = degree(rot_pre +
				(v_now.x*v_pre.y - v_now.y*v_pre.x > 0 ? MAX_ROT : -MAX_ROT));
		}
		que.push(make_pair(pre_back, rot_now));
	}

}

Robot::PFF Robot::distanceWall(degree rot , float speed)const
{
	float min_d = INF * 1.0f;
	queue<PR> que;
	predictPosition(rot, que , speed);
	float head_dist = -1.0f;

	if (bodyNumber())
	{
		auto r = this->getRadius();
		
		auto head_pos = que.front().first;
		head_dist = MIN(MIN(head_pos.x - r, area.width - r - head_pos.x),
			MIN(head_pos.y - r, area.height - r - head_pos.y));

		while(!que.empty())
		{
			auto pos = que.front().first;

			float d1 = MIN(pos.x - r, area.width -r - pos.x);
			float d2 = MIN(pos.y - r, area.height -r - pos.y);

			min_d = MIN( MIN(d1, d2) , min_d );

			que.pop();
		}
	}
	return make_pair(head_dist , min_d);
}

float Robot::distanceMyself(degree rot , float speed)const
{
	int i = 360 / MAX_ROT -1;
	if (bodyNumber() < i) return 0;	//头碰不到i之前的节点, 无风险

	
	queue<PR> que;	
	predictPosition(rot, que);
	for (int j = 0; j < i; j++) que.pop();	//头碰不到i之前的节点

	float dist = 0.0f;
	auto head_pos = this->getHead()->getPosition() + Vec2(rot)*speed;
	while(!que.empty())
	{
		auto nd = que.front();
		dist = MIN(dist, head_pos.getDistance(nd.first));
		que.pop();
	}
	return dist;
}

float Robot::evalueWall(degree rot , float speed)const
{
	auto r = this->getRadius();
	auto dist = distanceWall(rot, speed);
	return CONST_VALUE + COEF_WALL1 * dist.first/r
						+ COEF_WALL2* dist.second/r;
}

void Robot::move()
{
	srand(time(0));

	degree rot(0.0f);

	float h = -1.0f;
	float h1[2] = { -1.0f , -1.0f },
		h2[2] = { -1.0f ,-1.0f };	//h1向左, h2向右, 下标0只转弯, 下标1转弯+直走


	bool rnd = rand() % 2;
	do
	{
		if (!rnd && (h1[0] <= 0 && h1[1] <= 0))
		{
			rot = -SPEED_ROT;

			h1[0] = evalueWall(rot); h1[1] = evalueWall(rot, SPEED);
			h = MIN(h1[0], h1[1]);
		}
		else if (h2[0] <= 0 && h2[1] <= 0)
		{
			rnd = 0;
			rot = SPEED_ROT;

			h2[0] = evalueWall(rot); h2[1] = evalueWall(rot, SPEED);
			h = MIN(h2[0], h2[1]);
		}
		else	//向左和向右都试过	//或者通过h1[1]和h2[1]的大小评判
		{
			h1[0] = h1[1] = -1.0f;
			h2[0] = h2[1] = -1.0f;
			break;
		}

	} while (h >= risk && h > CONST_SAFE);


	//判断是否卡死
	static Vec2 rec_pos = this->getPosition();
	if (rec_pos == this->getPosition())
		cnt++;

	//跑动作
	//直走
	if (fmod(rand(), CONST_VALUE) >= h || h <= CONST_SAFE
		|| h < risk && (h1[1] < h1[0] || h2[1] < h2[0]))
	{
		auto angle = this->getRotation() / 180 * PI;
		auto toward = Vec2(SPEED*sin(angle), SPEED* cos(angle));

		auto up = MoveBy::create(0.0f, toward);
		this->Snake::runAction(up);
	}

	//拐弯
	if (h > CONST_SAFE || h <= CONST_SAFE && rand() % 2)
	{
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action , 0);
	}

	rec_pos = this->getPosition();
	if (risk <= CONST_SAFE) cnt = 0;

}


void Robot:: disappear()
{
	while (bodyNumber())
		body.popBack();
	cnt = 0;
}

void Robot::moveTowardCenter()
{

	Vec2 v1 = getPosition() - area/2;
	Vec2 v2 = degree(getRotation());

	degree rot = SPEED_ROT;
	if (v1.dot(v2)<0)
	{
		auto angle = this->getRotation() / 180 * PI;
		auto toward = Vec2(SPEED*sin(angle), SPEED* cos(angle));

		auto up = MoveBy::create(0.0f, toward);
		this->Snake::runAction(up);


		rot = v1.cross(v2) > 0 ? -rot : rot;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action , 0);

	}
	else if(v1.dot(v2)>0)
	{
		rot = v1.cross(v2) > 0 ? -rot : rot;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action , 0);
	}
	else
	{
		rot = !last_action ? SPEED : -SPEED;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action , 0);
	}

	if (risk <= CONST_SAFE)
		cnt = 0;

}

bool Robot::evalueFood(const BodyNode* food )const
{
	auto v = food->getPosition() - this->getHead()->getPosition();
	auto r = this->getRadius();
	float dist = 0.0f;
	for (auto nd : body)
	{
		auto pos = nd->getPosition() + v;
		float d1 = MIN(pos.x - r, area.width - r - pos.x);
		float d2 = MIN(pos.y - r, area.height - r - pos.y);

		dist = MIN(MIN(d1, d2), dist);
	}

	return CONST_VALUE + COEF_WALL1 * dist / r
		+ COEF_WALL2 * dist / r;
}

BodyNode* Robot::getFoodInfomation(const list<BodyNode*>& lst)const
{
	list<BodyNode*>::const_iterator iter;
	for ( iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (evalueFood(*iter) <= CONST_SAFE)
			break;
	}
	if(iter!=lst.end())
		return *iter;
	else return NULL;
}


void Robot::headForFood(const BodyNode* food)
{
	if (!food) return;

	Vec2 v1 = getPosition() - food->getPosition();
	Vec2 v2 = degree(getRotation());

	degree rot = SPEED_ROT;
	if (v1.dot(v2)<0)
	{
		auto angle = this->getRotation() / 180 * PI;
		auto toward = Vec2(SPEED*sin(angle), SPEED* cos(angle));

		auto up = MoveBy::create(0.0f, toward);
		this->Snake::runAction(up);


		rot = v1.cross(v2) > 0 ? -rot : rot;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action, 0);

	}
	else if (v1.dot(v2)>0)
	{
		rot = v1.cross(v2) > 0 ? -rot : rot;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action, 0);
	}
	else
	{
		rot = !last_action ? SPEED : -SPEED;
		auto rot_action = RotateBy::create(0, rot);
		this->runAction(rot_action, 0);
	}

}