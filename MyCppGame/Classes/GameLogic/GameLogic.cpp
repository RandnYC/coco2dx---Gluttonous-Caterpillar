#include "GameLogic.h"

GameLogic::GameLogic( Scene* _scene ,const Size& _sz)
	:score(0),major(0) , area(_sz)
{}

GameLogic* GameLogic::create(Scene* _scene, const Size& _sz)
{
	auto gl = new GameLogic(_scene,_sz);
	if (gl)
	{
		gl->scheduleUpdate();
		gl->schedule(schedule_selector(addFoodTimely), 4.0f);
		gl->schedule(schedule_selector(addRobotTimely), 5.0f);

		gl->autorelease();
		gl->addPlayer();

		return gl;
	}
	else
		CC_ASSERT("Create GameLogic Failed");
	
	return NULL;
}

void GameLogic::getPoint(scoreTag st)
{
	switch (st)
	{
	case eat:
		score += 1;
		break;
	case kill:
		score += 10;
		break;
	default:
		log("unknown scoreTag!");
		break;
	}
}

void GameLogic::clearScore()
{
	score = 0;
}

void GameLogic::playerLose()
{
	major->disappear();
	major->setPosition(area / 2);
}


void GameLogic::robotLose(Snake* rbt)
{
	rbt->disappear();
}

void GameLogic::rulePlayer()
{
	if (outofRange(major) || major->collide())
	{
		major->disappear();
		major->setPosition(area / 2);
	}

	for (auto iter = robot.begin(); iter != robot.end(); )
	{
		if (major->collide(*iter))
		{
			major->disappear();
			major->setPosition(area / 2);
			score = 0;
			break;
		}
		else iter++;
	}
}

void GameLogic::ruleRobot()
{
	for (auto iter = robot.begin(); iter != robot.end(); )
	{
		if ((*iter)->collide(major))
		{
			int num = (*iter)->bodyNumber();
			(*iter)->disappear();
			this->removeChild(*iter);
			
			while (num--)
				addFood();

			getPoint(kill);
			robot.erase(iter++);
		}
		else iter++;
	}

}

int GameLogic::getScore()const
{
	return score;
}

void GameLogic::rule()
{
	rulePlayer();
	ruleRobot();
}

void GameLogic::robotSearchFood()
{
	for (auto iter = robot.begin(); iter != robot.end(); iter++)
	{
		auto x= (*iter)->getFoodInfomation(food);
		(*iter)->headForFood(x);
		(*iter)->update();
	}
}


void GameLogic::update(float dt)
{
	Node::update(dt);
	robotSearchFood();
	eatFood();
	rule();

	auto pos = &(this->getPosition());
	log("%f %f", this->getPosition().x, this->getPosition().y);
}

void GameLogic:: addRobotTimely(float)
{
	addRobot();
}

void GameLogic::addRobot()
{
	auto rbt = Robot::create(area);
	robot.push_back(rbt);

	float x = fmod(rand(), area.width - 10 * RADIUS)+ 5*RADIUS;
	float y = fmod(rand(), area.height - 10 * RADIUS) + 5 * RADIUS;;
	rbt->setPosition(Vec2(x,y));

	this->addChild(rbt);
}


bool GameLogic::outofRange(const Snake* obj)const
{
	if (obj->outofRange(area))
		return true;
	return false;
}

void GameLogic::addFoodTimely(float)
{
	addFood();
}


void GameLogic::addFood()
{
	auto color = Color4F(Color3B(rand() % 256, rand() % 256, rand() % 256), 1);
	auto fd = BodyNode::create(color);
	auto scl = fmod(rand(),RADIUS*2) /(2* RADIUS);
	fd->setScale(scl);

	auto r = fd->radius;
	food.push_back(fd);

	float x = fmod(rand(), area.width-2*r);
	float y = fmod(rand(), area.height-2*r);
	fd->setPosition(Vec2(x, y));

	this->addChild(fd);
}


void GameLogic::addPlayer()
{
	major = Player::create(area);
	this->addChild(major);
}

void GameLogic::eatFood()
{
	for (iterFood iter = food.begin(); iter != food.end(); )
	{
		if (major->eatFood(*iter))
		{
			this->removeChild(*iter);
			food.erase(iter++);
			getPoint(eat);
		}
		else ++iter;
	}

	for (iterRobot iter1 = robot.begin(); iter1 != robot.end(); iter1++)
	{
		for (iterFood iter2 = food.begin(); iter2 != food.end(); )
		{
			if ((*iter1)->eatFood(*iter2))
			{
				this->removeChild(*iter2);
				food.erase(iter2++);
			}
			else ++iter2;
		}
	}
}

void GameLogic::removeRobot()
{
	auto rbt = robot.back();
	this->removeChild(rbt);
	robot.pop_back();
}

int GameLogic::robotNumber()const
{
	if (this)
		return robot.size();
	else
		CC_ASSERT("Gamelogic unexisted");
	return -1;
}