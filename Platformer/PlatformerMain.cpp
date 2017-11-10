#include "pch.h"
#include "PlatformerMain.h"
#include "Common\DirectXHelper.h"

using namespace Platformer;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
PlatformerMain::PlatformerMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.


	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	spriteResources = std::shared_ptr<SpriteResources>(new SpriteResources(deviceResources));
	gamepad = std::shared_ptr<GameController>(new GameController());
	spriteResources->LoadTexture(L"Assets/PlayerIdleStance.png");
	spriteResources->LoadTexture(L"Assets/PlayerWalkSide.png");
	spriteResources->LoadTexture(L"Assets/TileEmpty.png");
	spriteResources->LoadTexture(L"Assets/Street.png");
	spriteResources->LoadTexture(L"Assets/Pit.png");
	spriteResources->LoadTexture(L"Assets/Background.png");
	spriteResources->LoadTexture(L"Assets/TrollWalkSide.png");

	gameboard = std::unique_ptr<Gameboard>(new Gameboard(deviceResources));
}

PlatformerMain::~PlatformerMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void PlatformerMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
}

// Updates the application state once per frame.
void PlatformerMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		this->gamepad->Update();
		// TODO: Replace this with your app's content update functions.
		gameboard->Update(m_timer.GetTotalSeconds(), m_timer.GetElapsedSeconds(),this->gamepad);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool PlatformerMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::DarkOrange);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.

	spriteResources->GetSpriteBatch()->Begin(
		DirectX::SpriteSortMode_Deferred,
		spriteResources->GetCommonStates()->NonPremultiplied());

	gameboard->Render(spriteResources); 
	
	
	spriteResources->GetSpriteBatch()->End();

	return true;
}

// Notifies renderers that device resources need to be released.
void PlatformerMain::OnDeviceLost()
{

}

// Notifies renderers that device resources may now be recreated.
void PlatformerMain::OnDeviceRestored()
{

	CreateWindowSizeDependentResources();
}
