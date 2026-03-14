#pragma once

class Overlay {
private:
	D3DMATRIX CreateMatrix(SDK::FRotator rot, SDK::FVector origin);
public:
	float ScreenCenterX = 0;
	float ScreenCenterY = 0;

	void SetupHook(HWND* hwnd);
	void Unhook(HWND hwnd);
	void UpdatePosition(HWND* hwnd, HWND game);

	SDK::FVector WorldToScreen(SDK::FMinimalViewInfo camera, SDK::FVector WorldLocation);
};