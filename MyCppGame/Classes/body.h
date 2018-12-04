#pragma once
#include"cocos2d.h"
NS_CC_BEGIN;
class Body :public Sprite
{
private:
	Vec2 rot;
	float speed;
public:
	static Body* create(std::string filename);
	bool initWithFilename(std::string filename);
	void initKeyListener();
	void chgRotation(float dRot);
	virtual void update(float dt) override;
};

NS_CC_END