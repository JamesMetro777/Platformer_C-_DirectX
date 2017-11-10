//  (c) Martyn Prigmore 2014 and beyond. All rights reserved.

//When you add this include statement to this .cpp file it makes the Microsoft::WRL 
//namespace visible to code in the .h file.  
#include "pch.h"  

#include "SpriteResources.h"
#include "Common\DirectXHelper.h"
#include <WICTextureLoader.h>

using std::shared_ptr;
using std::unique_ptr;
using std::wstring;
using DirectX::SpriteBatch;
using DirectX::CommonStates;
using DirectX::SpriteFont;
using Microsoft::WRL::ComPtr;

SpriteResources::SpriteResources(shared_ptr<DX::DeviceResources> deviceResources)
  : deviceResources{ deviceResources }
  , spriteBatch{ nullptr }
  , commonStates{ nullptr }
  , spriteFont{ nullptr }
  , textures{}  //An empty collection of key-value pairs
{
  //Initialize the DirectX Tool Kit classes to manage sprites
  this->spriteBatch
    = std::unique_ptr<SpriteBatch>(new SpriteBatch(deviceResources->GetD3DDeviceContext()));
  this->commonStates
    = std::unique_ptr<CommonStates>(new CommonStates(deviceResources->GetD3DDevice()));
   this->spriteFont
    = std::unique_ptr<SpriteFont>(new SpriteFont(deviceResources->GetD3DDevice(), L"Assets/italic.spritefont"));
}

SpriteResources::~SpriteResources()
{

}

std::unique_ptr<SpriteBatch>& SpriteResources::GetSpriteBatch()
{
  return this->spriteBatch;
}

std::unique_ptr<CommonStates>& SpriteResources::GetCommonStates()
{
  return this->commonStates;
}

std::unique_ptr<SpriteFont>& SpriteResources::GetSpriteFont()
{
  return this->spriteFont;
}

void SpriteResources::LoadTexture(wstring imageFilePathname)
{
  //Load the image file into a local texture
  ComPtr<ID3D11ShaderResourceView> texture = nullptr; 
  DX::ThrowIfFailed(
    DirectX::CreateWICTextureFromFile(
    this->deviceResources->GetD3DDevice(),
    this->deviceResources->GetD3DDeviceContext(),
    imageFilePathname.c_str(),
    0,
    &texture
    )
    );
  //Add this texture to our map, identified by its file pathname
  this->textures[imageFilePathname] = texture;
}

ComPtr<ID3D11ShaderResourceView> SpriteResources::GetTexture(wstring imageFilePathname)
{
	ComPtr<ID3D11ShaderResourceView> texture = this->textures[imageFilePathname];
	return texture;
}
