#include "degreeHandle.h"
using namespace cocos2d;

degree::degree(float _d ) : d(degree::standardize(_d))	{}
degree::degree(const Vec2& _v)
{
	if (!_v.x && !_v.y)
		CC_ASSERT("Degree Error");

	float rot = 0;
	float ans = _v.x / (float)sqrt(_v.x * _v.x + _v.y*_v.y);	//Îó²îÅÐ¶Ï
	if (ans >= 1)	rot = 90;
	else if (ans <= -1)	rot = -90;
	else rot = asin(ans) / PI * 180;

	if (_v.y < 0)
	{
		if (rot > 0)
			rot = _v.x >= 0 ? 180 - rot : 180 + rot;
		else if (rot < 0)
			rot = _v.x >= 0 ? -180 + rot : -180 - rot;
		else rot = 180;
	}
	d = standardize(rot);
}

degree::operator float() const
{
	return this->d;
}

degree::operator Vec2() const	
{
	return Vec2(sin(d/180 *PI), cos(d/180 *PI));
}

float degree::standardize(float _d)
{
	while (_d < 0) _d += 360;
	while (_d >= 360) _d -= 360;
	return _d;
}

degree degree::angle(const degree _d) const
{
	degree ans = this->d - _d;
	return ans > 180 ? 360 - ans : ans;
}