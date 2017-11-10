#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Common\SpriteResources.h"
#include "Content\Gameboard.h"
#include "Common\GameController.h"




// Renders Direct2D and 3D content on the screen.
namespace Platformer
{
	class PlatformerMain : public DX::IDeviceNotify
	{
	public:
		PlatformerMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~PlatformerMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::shared_ptr<SpriteResources> spriteResources;
		std::unique_ptr<Gameboard> gameboard;
		std::shared_ptr<GameController> gamepad;
		
	

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}