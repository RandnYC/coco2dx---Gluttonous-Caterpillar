#include "body.h"

#define img_rot -90
#define tar_spd 10
USING_NS_CC;
Body * Body::create(std::string filename)
{
	Body* body = new (std::nothrow)Body();
	if (body&&body->initWithFilename(filename))
	{
		body->autorelease();
		return body;
	}
	CC_SAFE_DELETE(body);
	return nullptr;
}

bool Body::initWithFilename(std::string filename)
{
	if(!Sprite::initWithFile(filename))
		goto wrong;
	rot = Vec2(0, 1);
	speed = tar_spd;
	this->setRotation(90+img_rot);
	this->scheduleUpdate();
	this->initKeyListener();
	return true;

wrong:
	return false;
}

void Body::initKeyListener()
{
	EventListenerKeyboard* listener=EventListenerKeyboard::create();
	listener->onKeyPressed= [=](EventKeyboard::KeyCode code, Event*)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			this->chgRotation(-5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			this->chgRotation(5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
			speed += 5;
			break;
		}
	};
		
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

void Body::chgRotation(float dRot)
{
	auto newrot = this->getRotation() + dRot;
	this->setRotation(newrot);//设置图片旋转方向
	newrot = -(newrot+img_rot) / 360 * 2 * 3.1415926;//设置移动旋转方向,注意这两种方向的坐标系方向相反
	rot = { cos(newrot),sin(newrot) };
	return;
}

void Body::update(float dt)
{
	Sprite::update(dt);
	this->setPosition(this->getPosition() + speed * rot*dt);
	if (speed != tar_spd)speed -= (speed - tar_spd) / 3 * dt;
}
