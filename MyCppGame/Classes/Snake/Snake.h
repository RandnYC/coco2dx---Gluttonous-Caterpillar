#pragma once

#include "headfileSet.h"
#include "BodyNode.h"

using namespace cocos2d;

class Snake : public Node
{
protected:

	Vector<BodyNode*> body;
	void bodyConnect();	//用于衔接身体节点
	Color4F color;
	int cnt;	//食物
public:
	Snake();	//用于初始化
	static Snake* create();
	void update(float) override;
	void update();
	void setPosition(const Vec2&) override;
	const Vec2& getPosition()const override;
	Action* runAction(Action*) override;
	void setRotation(float) override;
	float getRotation() const override;
	void setScale(float) override;
	float getScale() const override;

	void removeBody(int = 1);
	void addBody(int= 1);
	void reset();
	void setBodySize(int);
	BodyNode* getHead()const;

	int bodyNumber()const;
	virtual const Vec2 getCenterPosition()const;
	virtual float getLength()const;
	virtual float getRadius()const;
	virtual bool collide(const Snake*)const;	//判断自己是否撞到对方
	virtual bool collide()const;	//判断自己是否撞到自己
	virtual bool eatFood(BodyNode*);
	virtual bool outofRange(const Size&)const;	//判断是否出界
	virtual void disappear();

};