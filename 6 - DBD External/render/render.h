#pragma once

class Render {
private:
	ID3D11Device* g_device = nullptr;
	ID3D11DeviceContext* g_device_context = nullptr;
	IDXGISwapChain* g_swap_chain = nullptr;
	ID3D11RenderTargetView* g_render_target = nullptr;

	void InputHandler();

public:
	// Device
	bool InitDevice(HWND hwnd);
	void CleanupDevice();

	// Render Target
	void CreateRenderTarget();
	void CleanupRenderTarget();

	// ImGui
	void InitImGui(HWND hwnd);
	void SetupStyle();
	void CleanupImGui();

	// Frame
	void BeginFrame();
	void EndFrame();

	// Utils
	void DrawMenu(HWND hwnd);
	void DrawCornerBox(ImDrawList* draw, double x, double y, double w, double h,
		ImColor color, double thickness, double cornerFraction);
};
