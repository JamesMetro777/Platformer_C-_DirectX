#pragma once
#include <SimpleMath.h>
class CollisionBox
{
public:
	CollisionBox(DirectX::SimpleMath::Vector2 centre, DirectX::SimpleMath::Vector2 extents);
	~CollisionBox();

public:
	bool DoesIntersect(const CollisionBox& otherAabb) const;
	void SetAABB(const DirectX::SimpleMath::Vector2& topLeft);
	DirectX::SimpleMath::Vector2 getCentre();

	// functions for detecting specific platform collisions.
	bool CollidesBottomTop(const CollisionBox& otherAabb) const;
	bool CollidesRightLeft(const CollisionBox& otherAABB) const;
	bool CollidesTopBottom(const CollisionBox& otherAABB) const;

	bool WalkingOn(const CollisionBox& otherAABB) const;

private:
	DirectX::SimpleMath::Vector2 centre;
	DirectX::SimpleMath::Vector2 extents;

};

