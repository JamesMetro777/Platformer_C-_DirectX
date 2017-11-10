#pragma once
#include <Xinput.h>
class GameController
{
public:
	GameController();
	~GameController();

	void Update();
	bool GamepadConnected();
	bool IsButtonDown(unsigned short int button);
	bool WasButtonPressDetected(unsigned short int button);
	bool WasButtonReleaseDetected(unsigned short int button);
	float getLeftXNormalized();

	void UpdateAnalog();

	void SetVibration(int left = 0, int right = 0);

	float getLeftNormalized();

private:
	XINPUT_STATE gamepadState;
	XINPUT_STATE gamepadStatePrevious;
	XINPUT_VIBRATION vibration;
	bool gamepadConnected;
	float normalizedMagnitude;
};
