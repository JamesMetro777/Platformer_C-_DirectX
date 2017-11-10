#include "pch.h"
#include "CollisionBox.h"
#include <SimpleMath.h>

CollisionBox::CollisionBox(DirectX::SimpleMath::Vector2 centre, DirectX::SimpleMath::Vector2 extents) 
	: centre{ centre }
	, extents{extents}
{

}

CollisionBox::~CollisionBox()
{

}

bool CollisionBox::DoesIntersect(const CollisionBox& otherAabb) const
{
	//Test whether the two rectangles are disjoint (do not intersect)
	// build the rectangles...
	float thisTop = this->centre.y - this->extents.y;
	float thisLeft = this->centre.x - this->extents.x;
	float thisBottom = this->centre.y + this->extents.y;
	float thisRight = this->centre.x + this->extents.x;

	float otherTop = otherAabb.centre.y - otherAabb.extents.x;
	float otherLeft = otherAabb.centre.x - otherAabb.extents.x;
	float otherBottom = otherAabb.centre.y + otherAabb.extents.y;
	float otherRight = otherAabb.centre.x + otherAabb.extents.x;


	// if the two rectangles are disjoint then dont do anything...
	if ((thisTop > otherBottom) 
		|| (thisLeft > otherRight) 
		|| (thisBottom < otherTop)
		|| (thisRight < otherLeft))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CollisionBox::CollidesBottomTop(const CollisionBox& otherAabb) const
{
	//Test whether the two rectangles are disjoint (do not intersect)
	// build the rectangles...
	float thisTop = this->centre.y - this->extents.y;
	float thisLeft = this->centre.x - this->extents.x;
	float thisBottom = this->centre.y + this->extents.y;
	float thisRight = this->centre.x + this->extents.x;

	float otherTop = otherAabb.centre.y - otherAabb.extents.x;
	float otherLeft = otherAabb.centre.x - otherAabb.extents.x;
	float otherBottom = otherAabb.centre.y + otherAabb.extents.y;
	float otherRight = otherAabb.centre.x + otherAabb.extents.x;

	if (thisTop == otherBottom)
	{
		return false;
	}

	float scaleDifference = sqrt((extents.y - otherAabb.extents.y) * (extents.y - otherAabb.extents.y));
	DirectX::SimpleMath::Vector2 otherCentre = otherAabb.centre;

	double distance = sqrt((centre.y - otherCentre.y) *(centre.y - otherCentre.y));

	if (distance <= scaleDifference)
	{
		return false;
	}

	if (distance < this->extents.y + otherAabb.extents.y)
	{
		return true;
	}
}

// Check to see if the player is coliiding with the side of another
// collidable object
bool CollisionBox::CollidesRightLeft(const CollisionBox& otherAabb) const
{
	//Test whether the two rectangles are disjoint (do not intersect)
	// build the rectangles...
	float thisTop = this->centre.y - this->extents.y;
	float thisLeft = this->centre.x - this->extents.x;
	float thisBottom = this->centre.y + this->extents.y;
	float thisRight = this->centre.x + this->extents.x;

	float otherTop = otherAabb.centre.y - otherAabb.extents.x;
	float otherLeft = otherAabb.centre.x - otherAabb.extents.x;
	float otherBottom = otherAabb.centre.y + otherAabb.extents.y;
	float otherRight = otherAabb.centre.x + otherAabb.extents.x;


	if (thisBottom == otherTop)
	{
		return false;
	}

	float totalLength = (this->extents.x + otherAabb.extents.x) * (this->extents.x + otherAabb.extents.x);

	float distanceBetween = (this->centre.x - otherAabb.centre.x) * (this->centre.x - otherAabb.centre.x);

	if (distanceBetween > totalLength)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CollisionBox::WalkingOn(const CollisionBox& otherAabb) const
{
	float thisBottom = this->centre.y + this->extents.y;
	float otherTop = otherAabb.centre.y - otherAabb.extents.x;

	if (thisBottom == otherTop)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CollisionBox::SetAABB(const DirectX::SimpleMath::Vector2& topLeft)
{
	// we receive a reference to the top left corner of our AABB
	this->centre = topLeft + extents;
}

DirectX::SimpleMath::Vector2 CollisionBox::getCentre()
{
	return this->centre;
}