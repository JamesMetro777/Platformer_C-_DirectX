#include "pch.h"
#include "Tile.h"


Tile::Tile(std::wstring textureId,DirectX::SimpleMath::Vector2 offset,std::pair<int, int> columnRow, float scale)
	: spriteTint{ 1.0f, 1.0f, 1.0f, 1.0f }  //Tint solid white; i.e. use original colours at original saturation
	, spritePosition{}  //Zero vector
	, spriteSourceRectangle{ nullptr }
	, textureHeight{ 64.0f }
	, textureWidth{ 64.0f }
	, boardOffset{ offset }
	, scale{ scale }
	, columnRow{ columnRow }
	, state{ TileState::Empty }
	, collisionBox{DirectX::SimpleMath::Vector2(0.0f,0.0f),DirectX::SimpleMath::Vector2(32.0f,32.0f)}
{
	this->textureId = textureId;
	
	//column gives the x position and row gives the y position
	this->spritePosition
		= this->boardOffset + DirectX::SimpleMath::Vector2(columnRow.first * (this->textureWidth * this->scale),
		columnRow.second * (this->textureHeight * this->scale));

	//if (textureId == L"Assets/TilePlatform.png")
	//{
	//	this->spritePosition.y -= 1.0f;
	//}
	//this->spritePosition.y -= 1.0f;

	collisionBox.SetAABB(spritePosition);

	this->spriteSourceRectangle = std::unique_ptr<RECT>(new RECT());
	this->spriteSourceRectangle->left = 0;
	this->spriteSourceRectangle->top = 0;
	this->spriteSourceRectangle->right = static_cast<LONG>(textureWidth);
	this->spriteSourceRectangle->bottom = static_cast<LONG>(textureHeight);
}


Tile::~Tile()
{
}

void Tile::Update()
{
	
}

void Tile::Render(const std::shared_ptr<SpriteResources>& spriteResources)
{
	//Get the DirectX::SpriteBatch from the SpriteResources object
	spriteResources->GetSpriteBatch()->Draw(
		spriteResources->GetTexture(this->textureId).Get(),
		this->spritePosition,
		this->spriteSourceRectangle.get(),  //Need the raw C++ pointer
		this->spriteTint,
		0,  //Angle of rotation in radians
		DirectX::SimpleMath::Vector2(0, 0), //Origin for the rotation
		this->scale, //Scale factor
		DirectX::SpriteEffects_None, //Reflections about the horizontal and verticle axes
		0.0f  //Layer depth; not required as we use DirectX::SpriteSortMode_Deferred
		);
}

TileState Tile::GetState()
{
	return this->state;
}

CollisionBox& Tile::getAABB()
{
	return this->collisionBox;
}

DirectX::SimpleMath::Vector2 Tile::getPosition()
{
	return this->spritePosition;
}