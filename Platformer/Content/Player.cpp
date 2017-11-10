#include "pch.h"
#include "Player.h"


Player::Player(std::wstring textureId,
	float tileHeight,
	float tileWidth,
	float scaledHeight,
	float scaledWidth,
	int frameCount,
	int boardColumnCount,
	int boardRowCount,
	DirectX::SimpleMath::Vector2 offset)
	: boardOffset{ offset }
	, boardColumnCount{ boardColumnCount }
	, boardRowCount{ boardRowCount }
	, scale{ 0.75f }
	, height{ tileHeight }
	, width{ tileWidth }
	, scaledHeight{scaledHeight}
	, scaledWidth{scaledWidth}
	, frameCount{ frameCount }
	, frame{ 0 }
	, tint{ 1.0f, 1.0f, 1.0f, 1.0f }
	, position{ offset + DirectX::SimpleMath::Vector2(0.0f, static_cast<float>((boardRowCount - 1) * 64.0f)) }
	, lastFrameChange{ 0.0 }
	, animationRate{ 1.0 / 6.0 }
	, spriteEffect{ DirectX::SpriteEffects_FlipHorizontally }
	, velocity{ 0.0f, 0.0f }
	, aabb{ DirectX::SimpleMath::Vector2(position.x + (this->width / 2), position.y + (this->height / 2)), DirectX::SimpleMath::Vector2((scaledWidth / 2), (scaledWidth / 2)) }
	, playerState{ PlayerState::alive }
	, movingState{ MoveState::IdleWalking }
	, accelerationDueToGravity{ 115.0f }
{
	this->textureId = textureId;
	this->scaleOffset = height - scaledHeight;
	//Initial source rectangle defines the first frame of the sprite sheet
	this->sourceRectangle.left = 0L;
	this->sourceRectangle.top = 0L;
	this->sourceRectangle.right = static_cast<LONG>(this->width);
	this->sourceRectangle.bottom = static_cast<LONG>(this->height);

	this->position.y += scaleOffset;
	this->aabb.SetAABB(position);
	baseLevel = position.y;
	currentHeight = baseLevel;
}


Player::~Player()
{
}

void Player::Update(double totalTime, double timeDelta, std::shared_ptr<GameController> gamepad, std::vector<std::shared_ptr<Tile>> collidable)
{
	if (playerState == PlayerState::alive)
	{
		// check for collision with platforms!
		this->DetectPlatformCollision(collidable, totalTime, timeDelta, gamepad);
		this->aabb.SetAABB(this->position);
		
		SetValues(gamepad);

		this->Animate(totalTime);
		this->Move(timeDelta);

		if (movingState == MoveState::IdleWalking)
		{
			if (gamepad->WasButtonReleaseDetected(XINPUT_GAMEPAD_Y))
			{
				movingState = MoveState::Jumping;
				this->velocity.y = -128.0f;
			}
		}
		else if (movingState == MoveState::Falling)
		{
			this->Fall(timeDelta);
		}
		else
		{
			this->Jump(timeDelta);
		}
	}
}

void Player::Fall(double timeDelta)
{
	this->velocity.x = 0.0f;
	DirectX::SimpleMath::Vector2 initialVelocity = this->velocity;
	acceleration.y = accelerationDueToGravity;
	this->velocity.y = initialVelocity.y + (timeDelta * this->acceleration.y);
	//Second calculate the distance moved
	DirectX::SimpleMath::Vector2 distanceTravelled = 0.5f * (initialVelocity + this->velocity) * timeDelta;

	//Third, calculate the new position of the sprite
	this->position = this->position + distanceTravelled;

	if (this->position.y > baseLevel)
	{
		this->position.y = baseLevel;
		movingState = MoveState::IdleWalking;
		this->velocity.y = 0.0f;
	}
	this->aabb.SetAABB(this->position);
}

void Player::Jump(double timeDelta)
{
	this->velocity.x = 0.0f;
	DirectX::SimpleMath::Vector2 initialVelocity = this->velocity;
	acceleration.y = accelerationDueToGravity;
	this->velocity = initialVelocity + (timeDelta * this->acceleration);
	//Second calculate the distance moved
	DirectX::SimpleMath::Vector2 distanceTravelled = 0.5f * (initialVelocity + this->velocity) * timeDelta;

	//Third, calculate the new position of the sprite
	this->position = this->position + distanceTravelled;

	if (this->position.y > baseLevel)
	{
		this->position.y = baseLevel;
		movingState = MoveState::IdleWalking;
		this->velocity.y = 0.0f;
	}
	this->aabb.SetAABB(this->position);
}

void Player::SetValues(std::shared_ptr<GameController> gamepad)
{
	float yvelocity = velocity.y;
	if (gamepad->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		velocity = DirectX::SimpleMath::Vector2(120.0f, yvelocity);
		this->textureId = L"Assets/PlayerWalkSide.png";
		this->spriteEffect = DirectX::SpriteEffects::SpriteEffects_None;
	}
	else if (gamepad->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		velocity = DirectX::SimpleMath::Vector2(-120.0f, yvelocity);
		this->textureId = L"Assets/PlayerWalkSide.png";
		this->spriteEffect = DirectX::SpriteEffects::SpriteEffects_FlipHorizontally;
	}
	else
	{
		velocity = DirectX::SimpleMath::Vector2(0.0f, yvelocity);
		this->textureId = L"Assets/PlayerIdleStance.png";
		this->spriteEffect = DirectX::SpriteEffects::SpriteEffects_None;
	}
}

void Player::Move(double timeDelta)
{
	DirectX::SimpleMath::Vector2 toMove = timeDelta * this->velocity;
	this->position = this->position + toMove;

	// make sure we are within the limit of the gameboard
	if (this->position.x < this->boardOffset.x)
	{
		this->position.x = this->boardOffset.x;
	}
	else if (this->position.x > this->boardOffset.x + (this->width * (this->boardColumnCount - 1)))
	{
		this->position.x = this->boardOffset.x + (this->width * (this->boardColumnCount - 1));
	}
}

void Player::DetectPlatformCollision(std::vector<std::shared_ptr<Tile>> collidable, double totalTime, double timeDelta, std::shared_ptr<GameController> gamepad)
{
	bool collided = false;
	bool hitTop = false;

	for (int i = 0; i < collidable.size(); i++)
	{
		if (this->getAABB().DoesIntersect(collidable[i]->getAABB()))
		{
			collided = true;

			if (this->getAABB().CollidesBottomTop(collidable[i]->getAABB()))
			{
				DirectX::SimpleMath::Vector2 pRect = this->getAABB().getCentre();
				DirectX::SimpleMath::Vector2 oRect = collidable[i]->getAABB().getCentre();

				if (pRect.y > oRect.y)
				{
						this->movingState = MoveState::Falling;
						this->velocity.y = 128.0f;
						this->velocity.x = 0.0f;
						this->currentHeight = this->position.y;
						hitTop = true;
				}
				else
				{
					if (this->position.y != currentHeight)
					{
						this->position.y = (collidable[i]->getPosition().y) - scaledHeight;
						this->velocity.y = 0.0f;
						movingState = MoveState::IdleWalking;
						currentHeight = this->position.y;
						break;
					}
				}
			}
			
			if (!hitTop)
			{
				if (this->getAABB().CollidesRightLeft(collidable[i]->getAABB()))
				{

					DirectX::SimpleMath::Vector2 pRect = this->getAABB().getCentre();
					DirectX::SimpleMath::Vector2 oRect = collidable[i]->getAABB().getCentre();

					double hd = sqrt((pRect.x - oRect.x) * (pRect.x - oRect.x));

					float toAdd = (32.0f + 24.0f) - hd;

					if (pRect.x < oRect.x)
					{
						// right side of player
						this->velocity.x = 0.0f;
						this->position.x = this->position.x - toAdd;
						this->aabb.SetAABB(this->position);
					}
					else if (pRect.x > oRect.x)
					{
						// left side of the player
						this->velocity.x = 0.0f;
						this->position.x = this->position.x + toAdd;
						break;
					}
					else
					{

					}
				}
			}
		}
	}


	this->aabb.SetAABB(this->position);

	if (!collided)
	{
		if (this->position.y < (baseLevel - this->height))
		{
			if (movingState != MoveState::Jumping)
			{
				this->movingState = MoveState::Falling;
				this->velocity.y = 128.0f;
			}
		}
	}
}

void Player::Animate(double totalTime)
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

		this->sourceRectangle.left = this->frame * (64.0f);
		this->sourceRectangle.top = 0L;
		this->sourceRectangle.right = sourceRectangle.left + (64.0f);
		this->sourceRectangle.bottom = 64.0f;;

		//Record the time when we changed the frame
		this->lastFrameChange = totalTime;
	}
}

void Player::Render(const std::shared_ptr<SpriteResources>& spriteResources)
{
	//Get the DirectX::SpriteBatch from the SpriteResources object
	spriteResources->GetSpriteBatch()->Draw(
		spriteResources->GetTexture(this->textureId).Get(),
		this->position,
		&sourceRectangle,  //Need the raw C++ pointer
		this->tint,
		0,  //Angle of rotation in radians
		DirectX::SimpleMath::Vector2(0, 0), //Origin for the rotation
		scale, //Scale factor
		this->spriteEffect, //Reflections about the horizontal and verticle axes
		0.0f  //Layer depth; not required as we use DirectX::SpriteSortMode_Deferred
		);
}

CollisionBox& Player::getAABB()
{
	return this->aabb;
}

void Player::KillPlayer()
{
	this->playerState = PlayerState::dead;
}

PlayerState Player::getState()
{
	return this->playerState;
}