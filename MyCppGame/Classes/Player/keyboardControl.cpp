#include "keyboardControl.h"

void keyboardControl::initKeyboardListener(Node* nd)
{
	//键盘事件监听器
	auto listener = EventListenerKeyboard::create();
	node = nd;

	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = true;
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, nd);
}

void keyboardControl::keyPressedDuration(EventKeyboard::KeyCode code)
{
	auto angle = node->getRotation()/180 * PI;
	auto toward = Vec2(SPEED*sin(angle), SPEED* cos(angle));	//三角函数弧度制

	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		auto rotleft = RotateBy::create(0, -SPEED_ROT);
		node ->runAction(rotleft);
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		auto rotright = RotateBy::create(0, SPEED_ROT);
		node->runAction(rotright);
		break ;
	}
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		auto up = MoveBy::create(MOVE_DELY,toward);
		node->runAction(up);
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		break;
	}

	}
}

bool keyboardControl::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
	if (keys[keyCode])
		return true;
	else
		return false;
}

void keyboardControl::keyboardTrigger() 
{
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
		rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
		upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
		downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;

	if (isKeyPressed(leftArrow))
		keyPressedDuration(leftArrow);
	if (isKeyPressed(rightArrow))
		keyPressedDuration(rightArrow);
	if (isKeyPressed(upArrow))
		keyPressedDuration(upArrow);
	if (isKeyPressed(downArrow))
		keyPressedDuration(downArrow);
}