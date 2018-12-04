#pragma once

#include "headfileSet.h"
using namespace cocos2d;

class keyboardControl : public Node
{
	Node* node;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	void initKeyboardListener(Node*);
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	void keyPressedDuration(EventKeyboard::KeyCode code);
	void keyboardTrigger();
};