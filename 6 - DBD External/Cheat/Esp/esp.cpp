#include "../../include.h"

using namespace global;

void Esp::Draw2DBox(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color)
{
	SDK::FVector head = { pos.X, pos.Y, pos.Z + 100.f };
	pos.Z -= 100.f;

	SDK::FVector head2D, feet2D;
	head2D = overlay->WorldToScreen(cam, head);
	feet2D = overlay->WorldToScreen(cam, pos);

	double boxHeight = feet2D.Y - head2D.Y;
	double boxWidth = boxHeight / 2.4f;

	double topX = head2D.X - boxWidth / 2.f;
	double topY = head2D.Y;

	// Box
	render->DrawCornerBox(ImGui::GetBackgroundDrawList(), topX, topY, boxWidth, boxHeight, color, 1.5f, 0.25f);
	//Fill
	ImGui::GetBackgroundDrawList()->AddRectFilled(
		ImVec2(topX, topY),
		ImVec2(topX + boxWidth, topY + boxHeight),
		ImColor(0, 0, 0, 60)
	);

}

void Esp::DrawLines(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color) {
	SDK::FVector head = { pos.X, pos.Y, pos.Z + 100.f };

	SDK::FVector head2D;
	head2D = overlay->WorldToScreen(cam, head);

	SDK::FVector startPos = { overlay->ScreenCenterX, overlay->ScreenCenterY * 2, 0 };

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(startPos.X, startPos.Y), ImVec2(head2D.X, head2D.Y), color, 1.f);
}

void Esp::DrawName(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, const std::string& name, ImColor color) {
	SDK::FVector head = { pos.X, pos.Y, pos.Z + 100.f };
	pos.Z -= 100.f;

	SDK::FVector head2D, feet2D;
	head2D = overlay->WorldToScreen(cam, head);
	feet2D = overlay->WorldToScreen(cam, pos);

	double boxHeight = feet2D.Y - head2D.Y;
	double boxWidth = boxHeight / 2.4f;

	float textWidth = ImGui::CalcTextSize(name.c_str()).x;
	float drawX = (float)(head2D.X - textWidth / 2.f);
	float drawY = (float)(head2D.Y - 16.f);

	// Ombre
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawX + 1, drawY + 1), ImColor(0, 0, 0, 200), name.c_str());
	// Texte
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawX, drawY), color, name.c_str());
}

void Esp::DrawSkeleton(const Entity& entity, const SDK::FMinimalViewInfo& cam, ImColor color) {
	for (auto& pair : skeletonPairs) {
		SDK::FVector from2D = overlay->WorldToScreen(cam, entity.bones[pair.from]);
		SDK::FVector to2D = overlay->WorldToScreen(cam, entity.bones[pair.to]);

		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2((float)from2D.X, (float)from2D.Y),
			ImVec2((float)to2D.X, (float)to2D.Y),
			color, 1.5f
		);
	}
}

void Esp::Draw() {
	LocalPlayer lp = m_cheat->GetLocalPlayer();
	std::lock_guard<std::mutex> lock(m_cheat->actorMutex);
	for (auto& actor : m_cheat->actors) {
		if (config::g_drawEspCornerBox)
			Draw2DBox(actor.position, lp.FMinimalViewInfo, ImColor(config::g_colorEspCornerBox[0], config::g_colorEspCornerBox[1], config::g_colorEspCornerBox[2]));

		if (config::g_drawLines)
			DrawLines(actor.position, lp.FMinimalViewInfo, ImColor(config::g_colorLines[0], config::g_colorLines[1], config::g_colorLines[2]));

		if (config::g_drawName)
			DrawName(actor.position, lp.FMinimalViewInfo, actor.Name, ImColor(config::g_colorName[0], config::g_colorName[1], config::g_colorName[2]));

		if (config::g_drawSkeleton)
			DrawSkeleton(actor, lp.FMinimalViewInfo, ImColor(config::g_colorSkeleton[0], config::g_colorSkeleton[1], config::g_colorSkeleton[2]));
	}
}
