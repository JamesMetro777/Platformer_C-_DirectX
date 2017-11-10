#include "pch.h"
#include "Gameboard.h"
#include <fstream>
#include <string>
#include <iostream>
Gameboard::Gameboard(std::shared_ptr<DX::DeviceResources> resources)
{
	GenerateGameBoard(resources);
	troll = std::shared_ptr<Troll>(new Troll(L"Assets/TrollWalkSide.png", 64.0f, 64.0f, 6, boardColCount, boardRowCount, DirectX::SimpleMath::Vector2(10, 10)));
	player = std::shared_ptr<Player>(new Player(L"Assets/PlayerIdleStance.png",64.0f,64.0f, 64.0f * 0.75f, 64.0f * 0.75f, 6, boardColCount, boardRowCount, DirectX::SimpleMath::Vector2(10, 10)));
}


Gameboard::~Gameboard()
{
}


void Gameboard::GenerateGameBoard(std::shared_ptr<DX::DeviceResources> resources)
{
	// load in the file 
	std::ifstream myFile("Assets/LevelOne.csv");
	std::string rowStr;

	std::wstring imageFilePathName;
	int rowCount = 0;
	int colCount = 0;

	// set up the paramaters
	DirectX::SimpleMath::Vector2 offset(10, 10);
	
	// get the first line and store in the variable "line"
	while (std::getline(myFile, rowStr))
	{
		std::pair<int, int>colRow;
		float scale = 1.0f;
		for (int col = 0; col < rowStr.size(); col++)
		{
			if (rowStr[col] == '0')
			{
				imageFilePathName = L"Assets/TileEmpty.png";
				colRow.first = colCount;
				colRow.second = rowCount;
				colCount++;
			}
			else if (rowStr[col] == '1')
			{
				imageFilePathName = L"Assets/Street.png";
				colRow.first = colCount;
				colRow.second = rowCount;
				colCount++;
			}
			else if (rowStr[col] == '2')
			{
				imageFilePathName = L"Assets/Pit.png";
				colRow.first = colCount;
				colRow.second = rowCount;
				colCount++;
			}
			else if (rowStr[col] == '3')
			{
				imageFilePathName = L"Assets/Background.png";
				colRow.first = colCount;
				colRow.second = rowCount;
				colCount++;
			}

			if (rowStr[col] != ',')
			{
				std::shared_ptr<Tile> toadd = std::shared_ptr<Tile>(new Tile(imageFilePathName,offset,colRow,scale));
				gameboard.push_back(toadd);
				

				if (imageFilePathName == L"Assets/Street.png")
				{
					collidable.push_back(toadd);
				}

			}
			
		}
		boardColCount = colCount;
		colCount = 0;
		rowCount++;
	}
	boardRowCount = rowCount;
	myFile.close();
}

void Gameboard::Update(double totalTime, double timeDelta, std::shared_ptr<GameController> gamepad)
{
	player->Update(totalTime, timeDelta, gamepad, collidable);
	troll->Update(totalTime, timeDelta, this->player);

	if (player->getState() == PlayerState::alive)
	{
		// contunue
	}
	else
	{
		// end
	}

}

void Gameboard::Render(const std::shared_ptr<SpriteResources>& spriteResources)
{
	for (int i = 0; i < gameboard.size(); i++)
	{
		gameboard[i]->Render(spriteResources);
	}

	troll->Render(spriteResources);
	player->Render(spriteResources);
	
}

