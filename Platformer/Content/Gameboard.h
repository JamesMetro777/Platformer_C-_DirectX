#pragma once
#include <Content\Tile.h>
#include <vector>
#include <Content\Troll.h>
#include <Common\GameController.h>
#include <Content\Player.h>
class Gameboard
{
public:
	Gameboard(std::shared_ptr<DX::DeviceResources> resources);
	~Gameboard();

private:
	std::vector<std::shared_ptr<Tile>> gameboard;
	std::vector<std::shared_ptr<Tile>> collidable;

public:
	void Update(double totalTime, double timeDelta, std::shared_ptr<GameController> gamepad);
	void Render(const std::shared_ptr<SpriteResources>& spriteResources);

private:
	void GenerateGameBoard(std::shared_ptr<DX::DeviceResources> resources);
	std::shared_ptr<Troll> troll;
	std::shared_ptr<Player> player;
	int boardColCount;
	int boardRowCount;
};

