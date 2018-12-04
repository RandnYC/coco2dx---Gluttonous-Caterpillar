#include "BodyNode.h"

BodyNode* BodyNode::create(Color4F color)
{
	auto sprite = new BodyNode();
	if (sprite)
	{
		sprite->autorelease();

		auto dot = DrawNode::create();
		dot->drawDot(Vec2(0,0), RADIUS, color);
		dot->setTag(1);
		sprite->addChild(dot ,1 );
		sprite->radius = RADIUS;
		sprite->update();
		return sprite;
	}
	else
	{
		CC_SAFE_DELETE(sprite);
		CC_ASSERT("BodyNode Create Fail");
	}
	return nullptr;
}


const Size& BodyNode::getContentSize()const
{
	return Node::getContentSize();
}

void BodyNode::setContentSize(const Size& _sz)	{}

void BodyNode::attachTo(BodyNode* pre)
{
	pre->update();
	this->setPosition(pre->back);
}

void BodyNode::update()
{
	auto pos = this->getPosition();
	auto rot = this->getRotation();

	front.y = pos.y + cos(rot / 180 * PI)*radius;
	back.y = pos.y - cos(rot / 180 * PI)*radius;

	front.x = pos.x + sin(rot / 180 * PI)*radius;
	back.x = pos.x - sin(rot / 180 * PI)*radius;

}


void BodyNode::setPosition(const Vec2& v)
{
	Node::setPosition(v);
	this->update();
}

void BodyNode::setRotation(float _d)
{
	Node::setRotation( degree(_d));
	this->update();
}

float BodyNode::getRotation() const
{
	degree ans = Node::getRotation();
	return ans;
}

void BodyNode::setScale(float scl)
{
	radius = RADIUS * scl;
	Node::setScale(scl);
	this->update();
}

float BodyNode::getScale() const
{
	if (RADIUS)
		return radius / RADIUS;
	else
		CC_ASSERT("Default Radius =0 in BodyNode GetScale");
}

void BodyNode::update(float dt)
{
	Node::update(dt);
	this->update();
}


bool BodyNode::collide(const BodyNode* other)const
{
	auto dist = this->getPosition().getDistance(other->getPosition());
	return dist <= this->radius + other->radius ? true : false;
}

float BodyNode::getDistance(BodyNode* other)const
{
	return this->getPosition().getDistance(other->getPosition());
}