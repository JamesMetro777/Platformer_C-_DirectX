#include "pch.h"
#include "Troll.h"


Troll::Troll(std::wstring textureId,
	int tileHeight,
	int tileWidth,
	int frameCount,
	int boardColumnCount,
	int boardRowCount,
	DirectX::SimpleMath::Vector2 offset)
	: boardOffset{ offset }
	, boardColumnCount{ boardColumnCount }
	, boardRowCount{ boardRowCount }
	, height{ tileHeight }
	, width{ tileWidth }
	, frameCount{ frameCount }
	, frame{ 0 }
	, tint{ 1.0f, 1.0f, 1.0f, 1.0f }
	, position{ offset + DirectX::SimpleMath::Vector2(static_cast<float>((boardColumnCount - 2) * tileWidth), static_cast<float>((boardRowCount - 1) * tileHeight)) }
	, lastFrameChange{ 0.0 }
	, animationRate{ 1.0 / 6.0 }
	, spriteEffect{ DirectX::SpriteEffects_FlipHorizontally }
	, velocity{ -32.0f, 0.0f }
	, state{TrollState::RightToLeft}
	, aabb{ DirectX::SimpleMath::Vector2(position.x + (this->width / 2), position.y + (this->height / 2)), DirectX::SimpleMath::Vector2(20.0f, 20.0f) }
{
	this->textureId = textureId;
	
	//Initial source rectangle defines the first frame of the sprite sheet
	this->sourceRectangle.left = 0L;
	this->sourceRectangle.top = 0L;
	this->sourceRectangle.right = static_cast<LONG>(this->width);
	this->sourceRectangle.bottom = static_cast<LONG>(this->height);
}


Troll::~Troll()
{
}

void Troll::Update(double totalTime, double timeDelta, const std::shared_ptr<Player> thePlayer)
{
	this->Animate(totalTime);
	this->Move(timeDelta);
	this->aabb.SetAABB(this->position);

	// check collisions with main player
	if (this->aabb.DoesIntersect(thePlayer->getAABB()))
	{
		// Kill the player
		this->tint = DirectX::Colors::Black.v;
		thePlayer->KillPlayer();
	}
}

void Troll::Move(double timeDelta)
{
	DirectX::SimpleMath::Vector2 distanceToMove = static_cast<float>(timeDelta)* this->velocity;
	DirectX::SimpleMath::Vector2 newPosition = this->position + distanceToMove;
	if (this->state == TrollState::LeftToRight)
	{
		//Patrolling left-to-right, so check that newPosition has not moved beyond 
		//  the bottom right corner tile.

		//  This is tricky, since there is no Tile::GetPosition() method. Our Troll
		//  class does know the board offset, the tile width (a Troll is one tile wide)
		//  and the number of columns in the board.  
		//  The Troll should turn when its *right hand edge* hits the right edge of 
		//  the game board.  I.e. when its *left hand edge* is in line with the left
		//  edge of the last column of tiles.  So we sant to find the position of the
		//  *left edge* of the final column.
		float rightPatrolLimit
			= this->boardOffset.x + ((this->boardColumnCount - 2) * this->width);

		if (newPosition.x >= rightPatrolLimit)
		{
			//Turn the Troll around
			this->spriteEffect = DirectX::SpriteEffects_FlipHorizontally;
			this->velocity.x = -1.0f * this->velocity.x;  //Change direction of motion only
			this->state = TrollState::RightToLeft;
		}
		else
		{
			//Move the Troll to the new position
			this->position = newPosition;
		}
	}
	else
	{
		//Patrolling right-to-left, so check that newPosition has not moved beyond 
		//  the fourth tile from the right.

		float leftPatrolLimit
			= this->boardOffset.x + ((this->boardColumnCount - 5) * this->width);

		if (newPosition.x <= leftPatrolLimit)
		{
			//Turn the Troll around
			this->spriteEffect = DirectX::SpriteEffects_None;
			this->velocity.x = -1.0f * this->velocity.x;  //Change direction of motion only
			this->state = TrollState::LeftToRight;
		}
		else
		{
			//Move the Troll to the new position
			this->position = newPosition;
		}

	}
}

void Troll::Animate(double totalTime)
{
	if (totalTime >= this->lastFrameChange + this->animationRate)
	{
		//Time to show the next frame
		this->frame = this->frame + 1;

		//Check whether we have gone past the last frame
		if (this->frame >= this->frameCount)
		{
			this->frame = 0;
		}

		this->sourceRectangle.left = this->frame * this->width;
		this->sourceRectangle.top = 0L;
		this->sourceRectangle.right = sourceRectangle.left + this->width;
		this->sourceRectangle.bottom = this->height;

		//Record the time when we changed the frame
		this->lastFrameChange = totalTime;
	}
}

void Troll::Render(const std::shared_ptr<SpriteResources>& spriteResources)
{
	//Get the DirectX::SpriteBatch from the SpriteResources object
	spriteResources->GetSpriteBatch()->Draw(
		spriteResources->GetTexture(this->textureId).Get(),
		this->position,
		&sourceRectangle,  //Need the raw C++ pointer
		this->tint,
		0,  //Angle of rotation in radians
		DirectX::SimpleMath::Vector2(0, 0), //Origin for the rotation
		1.0f, //Scale factor
		this->spriteEffect, //Reflections about the horizontal and verticle axes
		0.0f  //Layer depth; not required as we use DirectX::SpriteSortMode_Deferred
		);
}