#pragma once

#include "headfileSet.h"
#include "Snake\Snake.h"
using namespace cocos2d;
using namespace std;

class Robot :public Snake
{
	//enum judgementTag { safe, warning, dangerous };
	Size area;
	int cnt;	//是否流畅
	float risk;
	RotateBy* last_action;	//记录是左转还是右转

	typedef pair<Vec2, degree> PR;
	typedef pair<float, float>PFF;

public:

	Robot(const Size&);

	static Robot* create(const Size&);

	void disappear() override;

	virtual void move();
	virtual void moveTowardCenter();
	virtual void headForFood(const BodyNode*);
	virtual BodyNode* getFoodInfomation(const list<BodyNode*>&)const;
	virtual bool evalueFood(const BodyNode*)const;

	virtual Action* runAction(RotateBy* , int);	//区分原本的runAction

	virtual float evalueWall(degree,float =0.0) const;	//风险评估
	//virtual float evalueMyself(const Vec2&) const;

	virtual void predictPosition(degree, queue<PR>&, float = 0.0)const;
	virtual PFF distanceWall(degree , float = 0.0) const;	//预测移动向量v之后距离最近的那堵墙的距离
	virtual float distanceMyself(degree , float = 0.0) const;	//预测移动向量v之后头离最近身体节点的距离
	void update();
	void update(float)override;
};