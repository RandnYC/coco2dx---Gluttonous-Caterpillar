#include "Player.h"

Player::Player() : Snake()	
{
	control.initKeyboardListener(this);
}

Player* Player::create(const Size& area)
{
	auto sprite = new Player();
	if (sprite)
	{
		sprite->autorelease();
		sprite->scheduleUpdate();

		sprite->setPosition(area/2);
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

void Player::disappear()
{
	this->setBodySize(DEFAULT_BODY_SIZE);

	auto scl = this->getScale();
	this->setScale(1.0f);
	cnt = 0;
	//log("%f", scl);
}

void Player::update(float dt)
{
	if (bodyNumber())
	{
		Snake::update();
		control.keyboardTrigger();
	}
}