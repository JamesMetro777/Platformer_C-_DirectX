#pragma once
#include <SimpleMath.h>
#include <string>
#include "Common\DeviceResources.h"
#include "Common\SpriteResources.h"
#include "Common\CollisionBox.h"
///<summary>Indicates what sort of tile this is.</summary>
///<remarks>
///For this simple demo a tile has two possible states: empty, so is is neither 
///rendered nor updated; or it is a platform, so is rendered but not updated.  
///In a more complex game a tile may change state, e.g. to show a collectible.
///</remarks>
enum class TileState
{
	Empty,
	Platform
};

class Tile
{
public:
	Tile(std::wstring textureId,
		DirectX::SimpleMath::Vector2 offset,
		std::pair<int, int> columnRow,
		float scale);
	~Tile();

public:
	void Update();
	void Render(const std::shared_ptr<SpriteResources>& spriteResources);
	TileState GetState();
	CollisionBox& getAABB();
	DirectX::SimpleMath::Vector2 getPosition();
private:
	std::wstring textureId;
	DirectX::SimpleMath::Color spriteTint;
	DirectX::SimpleMath::Vector2 spritePosition;
	std::unique_ptr<RECT> spriteSourceRectangle;
	const float textureHeight;
	const float textureWidth;
	DirectX::SimpleMath::Vector2 boardOffset;
	float scale;
	std::pair<int, int> columnRow;
	TileState state;
	CollisionBox collisionBox;
};

