#pragma once

#include "headfileSet.h"
#include "Snake\Snake.h"
#include "keyboardControl.h"
using namespace cocos2d;

class Player : public Snake
{
protected:
	keyboardControl control;
public:
	Player();
	static Player* create(const Size&);
	void disappear()override;
	void update(float) override;
};