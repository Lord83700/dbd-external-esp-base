#include "../include.h"

using namespace global;


void Overlay::UpdatePosition(HWND* hwnd, HWND game) {
    RECT rect;
    GetWindowRect(game, &rect);

    int x = rect.left;
    int y = rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    MoveWindow(*hwnd, x, y, width, height, TRUE);

    ScreenCenterX = width / 2.0f;
    ScreenCenterY = height / 2.0f;
}

void Overlay::Unhook(HWND hwnd) {
    MARGINS margin = { 0 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);
    SetWindowLong(hwnd, GWL_EXSTYLE, 0);
    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);
}

void Overlay::SetupHook(HWND *hwnd) {
    *hwnd = NULL; //Implemente your own way to hook a window
    if (*hwnd == NULL)
        return;

    overlay->UpdatePosition(hwnd, game_hwnd);

    EnableWindow(*hwnd, true);
    ShowWindow(*hwnd, SW_SHOW);

    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(*hwnd, &margin);

    SetWindowLong(*hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
    UpdateWindow(*hwnd);
}


D3DMATRIX Overlay::CreateMatrix(SDK::FRotator rot, SDK::FVector origin) {
    const float DEG_TO_RAD = static_cast<float>(3.14159265358979323846) / 180.f;
    const float radPitch = rot.Pitch * DEG_TO_RAD;
    const float radYaw = rot.Yaw * DEG_TO_RAD;
    const float radRoll = rot.Roll * DEG_TO_RAD;

    const float SP = sinf(radPitch);
    const float CP = cosf(radPitch);
    const float SY = sinf(radYaw);
    const float CY = cosf(radYaw);
    const float SR = sinf(radRoll);
    const float CR = cosf(radRoll);

    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;

    matrix.m[3][0] = origin.X;
    matrix.m[3][1] = origin.Y;
    matrix.m[3][2] = origin.Z;
    matrix.m[3][3] = 1.f;

    return matrix;
}

SDK::FVector Overlay::WorldToScreen(SDK::FMinimalViewInfo camera, SDK::FVector WorldLocation)
{
    SDK::FVector Screenlocation(0, 0, 0);
    const D3DMATRIX tempMatrix = CreateMatrix(camera.Rotation, SDK::FVector(0, 0, 0));

    SDK::FVector vAxisX(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    SDK::FVector vAxisY(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    SDK::FVector vAxisZ(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    SDK::FVector vDelta = WorldLocation - camera.Location;
    SDK::FVector vTransformed = SDK::FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

    if (vTransformed.Z < 1.f)
        vTransformed.Z = 1.f;

    const float FOV_DEG_TO_RAD = static_cast<float>(3.14159265358979323846) / 360.f;

    Screenlocation.X = ScreenCenterX + vTransformed.X * (ScreenCenterX / tanf(
        camera.FOV * FOV_DEG_TO_RAD)) / vTransformed.Z;
    Screenlocation.Y = ScreenCenterY - vTransformed.Y * (ScreenCenterX / tanf(
        camera.FOV * FOV_DEG_TO_RAD)) / vTransformed.Z;

    return Screenlocation;
}