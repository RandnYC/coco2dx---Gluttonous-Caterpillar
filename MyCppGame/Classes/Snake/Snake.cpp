#include "Snake.h"

Snake::Snake() : cnt(0)
{
	srand(time(0));
	color = Color4F(Color3B(rand() % 256, rand() % 256, rand() % 256), 0.8);
	auto head = BodyNode::create(color);
	body.pushBack(head);
	head->setPosition(Vec2(0, 0));

	this->addChild(head, 0);
}
Snake* Snake::create()
{
	auto sprite = new Snake();
	if (sprite)
	{
		sprite->autorelease();
		sprite->scheduleUpdate();
		return sprite;
	}
	else
	{
		CC_SAFE_DELETE(sprite);
		CC_ASSERT("Snake Create Fail");
	}
	return nullptr;
}

void Snake::update(float dt)
{
	Node::update(dt);
	this->update();
}
void Snake::update()
{
	if (bodyNumber())
	{
		for (auto nd : body)
			nd->update();

		bodyConnect();
	}
	else
		CC_ASSERT("Snake With No Body In Snake Update");
}
void Snake::addBody(int num)
{
	auto scl = this->getScale();
	if (bodyNumber())
	{

		for (int i = 1; i <= num; i++)
		{
			auto bd = BodyNode::create(color);
			bd->setScale(scl);
			auto pre = body.back();
			bd->attachTo(pre);
			body.pushBack(bd);

			this->addChild(bd, i);
		}
	}
	else 
		CC_ASSERT("Snake With No Body In Snake AddBody");
}

void Snake::removeBody(int num)
{
	if (num < bodyNumber())
	{
		while (num--)
		{
			auto tail = body.back();
			body.popBack();
			this->removeChild(tail);
		}
	}
	else if (num == bodyNumber())
		CC_ASSERT("Warning: You are going to remove all the body of the snake in Snake RemoveBody");
	else
		CC_ASSERT("Error With exceeding Remove Number in Snake RemoveBody");
}

int Snake::bodyNumber()const
{
	if(this) 
		return body.size();
	else
		CC_ASSERT("Snake With NULL PTR in BodyNumber");
	return -1;
}

void Snake::setPosition(const Vec2& v)
{
	auto head = getHead();
	head->setPosition(v);
	bodyConnect();
}

const Vec2& Snake::getPosition()const
{
	return getHead()->getPosition();
}

void Snake::reset()
{
	if(bodyNumber() == 0) 
		CC_ASSERT("Body Number=0 in Snake Reset");
	if (bodyNumber() == 1)
	{
		this->getHead()->setRotation(0);
		return;
	}
		
	for (int i = 1; i < bodyNumber(); i++)
	{
		auto pre = body.at(i - 1);
		auto now = body.at(i);

		pre->setRotation(0);
		now->setRotation(0);
		now->attachTo(pre);
	}
}

void Snake::bodyConnect()
{
	for (int i = 1; i < bodyNumber(); i++)
	{
		auto pre = body.at(i - 1);
		auto now = body.at(i);

		degree rot_pre = pre->getRotation();
		auto v_pre = Vec2(rot_pre);

		auto v_now = Vec2(pre->back - now->back);
		degree rot_now(v_now);


		//auto dir = v_now.cross(v_pre);
		if (rot_now.angle(rot_pre) > MAX_ROT)
		{
			rot_now = degree(rot_pre +
				(v_now.x*v_pre.y - v_now.y*v_pre.x > 0 ? MAX_ROT : -MAX_ROT));
		}

		now->setRotation(rot_now);
		now->attachTo(pre);
	}
}

const Vec2 Snake::getCenterPosition()const
{
	return (getHead()->getPosition() +
			body.back()->getPosition()) / 2;
}

bool Snake::collide(const Snake* other)const	//撞到了other的身体(不包括头)
{
	if (other)
	{
		auto head = this->getHead();
		for (auto nd : other->body)
			if (nd != other->getHead() && head->collide(nd))
				return 1;
		return 0;
	}
	else
		CC_ASSERT("Collide With NULL PRT of Other in Snake");
	return 0;
}

bool Snake::collide()const
{

	auto head = getHead();
	int i = 360 / MAX_ROT -1;
	for (; i < bodyNumber(); i++)	//前两节和头部相连
	{
		auto nd = body.at(i);
		if (head->collide(nd))
			return true;
	}
	return false;

}

void Snake::disappear()
{
	//this->removeBody(this->bodyNumber()-1);
	this->reset();
	cnt = 0;
}

float Snake::getLength()const
{
	auto r = getHead()->radius;
	return r * bodyNumber() + r;
}


float Snake::getRadius()const
{
	auto r = getHead()->radius;
	if (r < EPS) CC_ASSERT("Invalid Radius In Robot Evalue");
	return r;
}

bool Snake::outofRange(const Size& _sz)const
{
	for (auto x : body)
	{
		auto pos = x->getPosition();
		float r = getRadius();
		if (pos.x + r >= _sz.width || pos.x - r <= 0
			|| pos.y + r >= _sz.height || pos.y - r <= 0)
			return true;
	}
	return false;
}

Action* Snake::runAction(Action* atn)
{
	auto head = getHead();
	head->runAction(atn);
	return atn;
}

BodyNode* Snake::getHead()const
{
	if (bodyNumber())
		return body.front();
	else
		CC_ASSERT("Snake With No Body");
	return NULL;
}

void Snake::setRotation(float _r)
{
	getHead()->setRotation(_r);
}

float Snake::getRotation()const
{
	return getHead()->getRotation();
}

void Snake::setBodySize(int n)
{
	if (n > 0)
	{
		while (bodyNumber() < n)
			this->addBody();
		while (bodyNumber() > n)
			this->removeBody(1);
	}
	else
	{
		CC_ASSERT("Invaild BodyNumber");
	}
}

bool Snake::eatFood(BodyNode* food)
{
	auto head = this->getHead();

	if (head->collide(food))
	{
		this->addBody();
		if (cnt++ %2 == 1)
		{
			this->setScale(pow(SCALE, cnt/2));
		}
		return true;
	}
	return false;
}

void Snake::setScale(float _scl)
{
	for (auto nd : body)
	{
		nd->setScale(_scl);
	}
}

float Snake::getScale() const
{
	return this->getHead()->getScale();
}