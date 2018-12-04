#ifndef __SCHEDULER_TEST__
#define __SCHEDULER_TEST__

#include"cocos2d.h"
USING_NS_CC;

class SchedulerTest :public Scene {
public:
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(SchedulerTest);

	void update(float dt);
	void Own_Update(float dt);

	//准备状态的函数
	void ReadyFunc(float dt);

private:
	Size visibleSize;
};
#endif