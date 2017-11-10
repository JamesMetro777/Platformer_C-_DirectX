#include "pch.h"
#include "GameController.h"
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

GameController::GameController()
{
}


GameController::~GameController()
{
}

void GameController::Update()
{
	//Store the previous state of the gamepad for edge detection algorithm
	this->gamepadStatePrevious = this->gamepadState;

	//Poll the Xbox gamepad to get its current state
	ZeroMemory(&this->gamepadState, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &this->gamepadState);
	if (dwResult == ERROR_SUCCESS)
	{
		this->gamepadConnected = true;
		this->UpdateAnalog();
	}
	else
	{
		this->gamepadConnected = false;
	}

}

void GameController::UpdateAnalog()
{
	float LX = gamepadState.Gamepad.sThumbLX;
	float LY = gamepadState.Gamepad.sThumbLY;

	// determine the magnitude (sum of squares)
	// c^2 = a^2 + b^2
	float magnitude = sqrt(LX*LX + LY*LY);

	// what direction we are pointing in...
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767)
		{
			magnitude = 32767;
		}

		// adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		// normalized between 0 and 1
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}
}

bool GameController::IsButtonDown(unsigned short int button)
{
	if (this->gamepadState.Gamepad.wButtons & button)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameController::WasButtonPressDetected(unsigned short int button)
{
	if (!(this->gamepadStatePrevious.Gamepad.wButtons & button)
		&& (this->gamepadState.Gamepad.wButtons & button))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameController::WasButtonReleaseDetected(unsigned short int button)
{
	if ((this->gamepadStatePrevious.Gamepad.wButtons & button)
		&& !(this->gamepadState.Gamepad.wButtons & button))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameController::SetVibration(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION vibration;

	// Zeroise the Vibration
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	vibration.wLeftMotorSpeed = leftVal;
	vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(0, &vibration);
}

float GameController::getLeftNormalized()
{
	return this->normalizedMagnitude;
}

float GameController::getLeftXNormalized()
{
	float LX = gamepadState.Gamepad.sThumbLX;
	//clip the magnitude at its expected maximum value

	// normalized between 0 and 1
	normalizedMagnitude = LX / 32767;

	return normalizedMagnitude;
}