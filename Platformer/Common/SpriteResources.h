//  (c) Martyn Prigmore 2014 and beyond. All rights reserved.
//
//  Declares a class to manage sprite resources
//
//  Design simplifications for a teaching tool
//  ******************************************
//  This code is intended as a teaching tool for first year students of games 
//  programming.  The design makes some simplyfying assumptions that are not 
//  realistic for a proper game, but are reasonable in a teaching tool.
#pragma once
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <SpriteFont.h>
#include <map>
#include <string>
#include "Common\DeviceResources.h"


class SpriteResources
{
public:
	SpriteResources(std::shared_ptr<DX::DeviceResources> deviceResources);
	~SpriteResources();

private:
	std::shared_ptr<DX::DeviceResources> deviceResources;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::CommonStates> commonStates;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	//A map (also called an associative array or dictionary) of textures
	//Each element consists of a name and a value.  In this case the name is the
	//pathname of the .png file we use to load the texture, and the value is the 
	//smart pointer to the actual texture object stored in memory.
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;  //See comment

public:
	std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch();
	std::unique_ptr<DirectX::CommonStates>& GetCommonStates();
	std::unique_ptr<DirectX::SpriteFont>& GetSpriteFont();
	void LoadTexture(std::wstring imageFilePathname);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(std::wstring imageFilePathname);

};