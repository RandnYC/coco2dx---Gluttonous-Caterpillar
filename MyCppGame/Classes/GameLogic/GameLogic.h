#pragma once

#include "Player\Player.h"
#include "Robot\Robot.h"
#include "headfileSet.h"
using namespace cocos2d;



class GameLogic : public Node
{
	typedef list<BodyNode*>::iterator iterFood;
	typedef list<Robot*>::iterator iterRobot;

protected:
	enum scoreTag { eat, kill };
	unsigned int score;
	Size area;

	list<BodyNode*> food;
	list<Robot*> robot;

	Player* major;
public:

	GameLogic(Scene*, const Size&);
	static GameLogic* create(Scene*, const Size&);

	virtual void rule();
	virtual void rulePlayer();
	virtual void ruleRobot();
	virtual void eatFood();
	virtual void robotLose(Snake*);
	virtual void playerLose();
	virtual void addRobot();
	virtual void addPlayer();
	virtual void getPoint(scoreTag);
	virtual int getScore()const;

	virtual bool outofRange(const Snake*) const;
	virtual void addFood();
	virtual void robotSearchFood();
	virtual void removeRobot();
	virtual int robotNumber()const;
	virtual void addFoodTimely(float);
	virtual void addRobotTimely(float);
	void update(float) override;
	void clearScore();
};