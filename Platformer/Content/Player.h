#pragma once

#include "Common\DeviceResources.h"
#include "Common\SpriteResources.h"
#include "SimpleMath.h"
#include "Common\GameController.h"
#include "Common\CollisionBox.h"
#include "Content\Tile.h"
#include <vector>
enum PlayerState
{
	alive,
	dead
};

enum MoveState
{
	IdleWalking,
	Jumping,
	Falling
};
class Player
{
public:
	Player(std::wstring textureId,
		float tileHeight,
		float tileWidth,
		float scaledHeight,
		float scaledWidth,
		int frameCount,
		int boardColumnCount,
		int boardRowCount,
		DirectX::SimpleMath::Vector2 offset);
	~Player();

private:
	std::wstring textureId;
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 velocity;

	DirectX::SimpleMath::Vector2 boardOffset;
	PlayerState playerState;
	MoveState movingState;
	CollisionBox aabb;

	// for animation
	RECT sourceRectangle;
	double lastFrameChange;
	double animationRate;
	float height;
	float width;
	float scaleOffset;
	float scaledHeight;
	float scaledWidth;
	int frameCount;
	int frame;
	int boardColumnCount;
	int boardRowCount;
	float baseLevel;
	float currentHeight;
	DirectX::SimpleMath::Color tint;
	DirectX::SpriteEffects spriteEffect;
	float scale;

	float accelerationDueToGravity; //In pixels per second per second
	DirectX::SimpleMath::Vector2 acceleration;
	bool jumping;

public:
	void Update(double totalTime, double timeDelta, std::shared_ptr<GameController> gamepad, std::vector<std::shared_ptr<Tile>> collidable);
	void Render(const std::shared_ptr<SpriteResources>& resources);
	void KillPlayer();
	void Fall(double totalTime, double timeDelta);
	
	PlayerState getState();
	// return a constant reference of the CollisionBox
	CollisionBox& getAABB();
private:
	void Move(double timeDelta);
	void Animate(double totalTime);
	void SetValues(std::shared_ptr<GameController> controller);
	void Jump(double timeDelta);
	void Fall(double timeDelta);
	void DetectPlatformCollision(std::vector<std::shared_ptr<Tile>> collidable, double totalTime, double timeDelta, std::shared_ptr<GameController> controllerS);
};

