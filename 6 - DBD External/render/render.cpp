#include "../include.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Device

bool Render::InitDevice(HWND hwnd) {
	RECT rect;
	GetClientRect(hwnd, &rect);

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION,
		&sd, &g_swap_chain, &g_device, nullptr, &g_device_context
	);

	if (FAILED(hr))
		return false;

	CreateRenderTarget();
	return true;
}

void Render::CleanupDevice() {
	CleanupRenderTarget();

	if (g_swap_chain) { g_swap_chain->Release(); g_swap_chain = nullptr; }
	if (g_device_context) { g_device_context->Release(); g_device_context = nullptr; }
	if (g_device) { g_device->Release(); g_device = nullptr; }
}

// Render Target

void Render::CreateRenderTarget() {
	ID3D11Texture2D* back_buffer = nullptr;
	g_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	g_device->CreateRenderTargetView(back_buffer, nullptr, &g_render_target);
	back_buffer->Release();
}

void Render::CleanupRenderTarget() {
	if (g_render_target) { g_render_target->Release(); g_render_target = nullptr; }
}

// ImGui

void Render::InitImGui(HWND hwnd) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_device, g_device_context);

	SetupStyle();
}

void Render::SetupStyle() {
	ImGuiStyle& style = ImGui::GetStyle();

	// Arrondis
	style.WindowRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.ChildRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;

	// Padding / Spacing
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(8, 4);
	style.ItemSpacing = ImVec2(8, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);

	// Bordures
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.TabBorderSize = 0.0f;
	style.ChildBorderSize = 1.0f;

	// Taille
	style.ScrollbarSize = 12.0f;
	style.GrabMinSize = 8.0f;

	// Couleurs noir / violet
	ImVec4* c = style.Colors;

	// Violet accent
	ImVec4 purple       = ImVec4(0.55f, 0.28f, 0.86f, 1.00f);
	ImVec4 purpleHover  = ImVec4(0.65f, 0.38f, 0.95f, 1.00f);
	ImVec4 purpleActive = ImVec4(0.45f, 0.20f, 0.75f, 1.00f);
	ImVec4 purpleDim    = ImVec4(0.35f, 0.18f, 0.55f, 1.00f);

	// Fond
	c[ImGuiCol_WindowBg]             = ImVec4(0.06f, 0.06f, 0.08f, 0.96f);
	c[ImGuiCol_ChildBg]              = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
	c[ImGuiCol_PopupBg]              = ImVec4(0.07f, 0.07f, 0.09f, 0.96f);

	// Bordures
	c[ImGuiCol_Border]               = ImVec4(0.20f, 0.16f, 0.28f, 0.60f);
	c[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	// Titre
	c[ImGuiCol_TitleBg]              = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
	c[ImGuiCol_TitleBgActive]        = ImVec4(0.10f, 0.08f, 0.14f, 1.00f);
	c[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.06f, 0.06f, 0.08f, 0.60f);

	// Frames (checkbox bg, input bg, slider bg)
	c[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.10f, 0.16f, 1.00f);
	c[ImGuiCol_FrameBgHovered]       = ImVec4(0.18f, 0.14f, 0.24f, 1.00f);
	c[ImGuiCol_FrameBgActive]        = ImVec4(0.22f, 0.16f, 0.30f, 1.00f);

	// Tabs
	c[ImGuiCol_Tab]                  = ImVec4(0.10f, 0.08f, 0.14f, 1.00f);
	c[ImGuiCol_TabHovered]           = purple;
	c[ImGuiCol_TabActive]            = purpleDim;
	c[ImGuiCol_TabUnfocused]         = ImVec4(0.08f, 0.06f, 0.10f, 1.00f);
	c[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.14f, 0.10f, 0.20f, 1.00f);

	// Boutons
	c[ImGuiCol_Button]               = purple;
	c[ImGuiCol_ButtonHovered]        = purpleHover;
	c[ImGuiCol_ButtonActive]         = purpleActive;

	// Headers (CollapsingHeader, Selectable)
	c[ImGuiCol_Header]               = ImVec4(0.16f, 0.12f, 0.22f, 1.00f);
	c[ImGuiCol_HeaderHovered]        = ImVec4(0.22f, 0.16f, 0.32f, 1.00f);
	c[ImGuiCol_HeaderActive]         = purple;

	// Checkmark / Slider / Resize
	c[ImGuiCol_CheckMark]            = purple;
	c[ImGuiCol_SliderGrab]           = purple;
	c[ImGuiCol_SliderGrabActive]     = purpleHover;
	c[ImGuiCol_ResizeGrip]           = ImVec4(0.55f, 0.28f, 0.86f, 0.25f);
	c[ImGuiCol_ResizeGripHovered]    = ImVec4(0.55f, 0.28f, 0.86f, 0.65f);
	c[ImGuiCol_ResizeGripActive]     = ImVec4(0.55f, 0.28f, 0.86f, 0.90f);

	// Separator
	c[ImGuiCol_Separator]            = ImVec4(0.28f, 0.20f, 0.40f, 0.50f);
	c[ImGuiCol_SeparatorHovered]     = purple;
	c[ImGuiCol_SeparatorActive]      = purpleActive;

	// Scrollbar
	c[ImGuiCol_ScrollbarBg]          = ImVec4(0.06f, 0.06f, 0.08f, 0.60f);
	c[ImGuiCol_ScrollbarGrab]        = ImVec4(0.20f, 0.16f, 0.28f, 1.00f);
	c[ImGuiCol_ScrollbarGrabHovered] = purpleDim;
	c[ImGuiCol_ScrollbarGrabActive]  = purple;

	// Texte
	c[ImGuiCol_Text]                 = ImVec4(0.92f, 0.90f, 0.96f, 1.00f);
	c[ImGuiCol_TextDisabled]         = ImVec4(0.45f, 0.42f, 0.50f, 1.00f);
}

void Render::CleanupImGui() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// Frame

void Render::BeginFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Render::EndFrame() {
	ImGui::Render();

	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_device_context->OMSetRenderTargets(1, &g_render_target, nullptr);
	g_device_context->ClearRenderTargetView(g_render_target, clear_color);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	g_swap_chain->Present(1, 0);
}

// Utils
void Render::InputHandler() {
	for (int i = 0; i < 5; i++) {
		ImGui::GetIO().MouseDown[i] = false;
	}

	int Button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) {
		Button = 0;
	}

	if (Button != -1) {
		ImGui::GetIO().MouseDown[Button] = true;
	}
}

void Render::DrawMenu(HWND hwnd) {
	if (GetAsyncKeyState(VK_INSERT)) {
		Sleep(200);
		config::g_showMenu = !config::g_showMenu;
	}

	ImGui::GetIO().MouseDrawCursor = config::g_showMenu;

	if (config::g_showMenu) {
		InputHandler();

		SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
		UpdateWindow(hwnd);
		SetFocus(hwnd);

		ImGui::SetNextWindowSize(ImVec2(480, 340), ImGuiCond_FirstUseEver);
		ImGui::Begin("Dead By Daylight", &config::g_showMenu,
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

		// Ligne violette sous le titre
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		float titleH = ImGui::GetFrameHeight() + ImGui::GetStyle().FramePadding.y;
		ImGui::GetWindowDrawList()->AddLine(
			ImVec2(winPos.x + 1, winPos.y + titleH),
			ImVec2(winPos.x + winSize.x - 1, winPos.y + titleH),
			ImColor(0.55f, 0.28f, 0.86f, 1.00f), 2.0f);

		ImGui::Spacing();

		if (ImGui::BeginTabBar("##MainTabs")) {

			if (ImGui::BeginTabItem("  ESP  ")) {
				ImGui::Spacing();
				ImGui::BeginChild("##esp_child", ImVec2(0, 0), true);

			if (ImGui::CollapsingHeader("Box ESP", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Spacing();
				ImGui::Checkbox("Corner Box", &config::g_drawEspCornerBox);
				ImGui::SameLine(220);
				ImGui::SetNextItemWidth(150);
				ImGui::ColorEdit3("##BoxColor", config::g_colorEspCornerBox,
					ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::Checkbox("Name", &config::g_drawName);
				ImGui::SameLine(220);
				ImGui::SetNextItemWidth(150);
				ImGui::ColorEdit3("##NameColor", config::g_colorName,
					ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::CollapsingHeader("Skeleton", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					ImGui::Checkbox("Draw Skeleton", &config::g_drawSkeleton);
					ImGui::SameLine(220);
					ImGui::SetNextItemWidth(150);
					ImGui::ColorEdit3("##SkelColor", config::g_colorSkeleton,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::CollapsingHeader("Lines", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					ImGui::Checkbox("Draw Lines", &config::g_drawLines);
					ImGui::SameLine(220);
					ImGui::SetNextItemWidth(150);
					ImGui::ColorEdit3("##LineColor", config::g_colorLines,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("  Misc  ")) {
				ImGui::Spacing();
				ImGui::BeginChild("##misc_child", ImVec2(0, 0), true);

				ImGui::TextDisabled("Not Implemented");

				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
	else {
		SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
		UpdateWindow(hwnd);
	}
}

void Render::DrawCornerBox(ImDrawList* draw, double x, double y, double w, double h,
	ImColor color, double thickness = 1.5f, double cornerFraction = 0.25f)
{
	float cW = w * cornerFraction;
	float cH = h * cornerFraction;

	// Top-left
	draw->AddLine(ImVec2(x, y), ImVec2(x + cW, y), color, thickness);
	draw->AddLine(ImVec2(x, y), ImVec2(x, y + cH), color, thickness);
	// Top-right
	draw->AddLine(ImVec2(x + w, y), ImVec2(x + w - cW, y), color, thickness);
	draw->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + cH), color, thickness);
	// Bottom-left
	draw->AddLine(ImVec2(x, y + h), ImVec2(x + cW, y + h), color, thickness);
	draw->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - cH), color, thickness);
	// Bottom-right
	draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - cW, y + h), color, thickness);
	draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - cH), color, thickness);
}