#pragma once

enum class TrollState
{
	LeftToRight,
	RightToLeft
};

#include "Common\DeviceResources.h"
#include "Common\SpriteResources.h"
#include "SimpleMath.h"
#include "Content\Player.h"
#include "Common\CollisionBox.h"
class Troll
{
public:
	Troll::Troll(std::wstring textureId,
		int tileHeight,
		int tileWidth,
		int frameCount,
		int boardColumnCount,
		int boardRowCount,
		DirectX::SimpleMath::Vector2 offset);
	~Troll();

private:
	std::wstring textureId;
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 velocity;

	DirectX::SimpleMath::Vector2 boardOffset;

	CollisionBox aabb;

	// for animation
	RECT sourceRectangle;
	double lastFrameChange;
	double animationRate;
	const int height;
	const int width;
	const int frameCount;
	int frame;
	int boardColumnCount;
	int boardRowCount;
	DirectX::SimpleMath::Color tint;
	
	DirectX::SpriteEffects spriteEffect;
	TrollState state;

public:
	void Update(double totalTime, double timeDelta, const std::shared_ptr<Player> thePlayer);
	void Render(const std::shared_ptr<SpriteResources>& resources);
private:
	void Move(double timeDelta);
	void Animate(double totalTime);
};

