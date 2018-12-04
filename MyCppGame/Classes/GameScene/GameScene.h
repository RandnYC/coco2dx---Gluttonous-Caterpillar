#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "GameLogic\GameLogic.h"
#include "headfileSet.h"
using namespace cocos2d;

class GameScene : public cocos2d::Scene
{
	GameLogic* gameLogic;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void initLabels();
	void initMenuClose();

	void menuLablel();

	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float) override;
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};


#endif // __HELLOWORLD_SCENE_H__
