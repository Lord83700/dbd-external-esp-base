#pragma once

class Esp {
private:
	Cheat* m_cheat;

public:
	Esp(Cheat* cheat) : m_cheat(cheat) {}

	void Draw();
	void Draw2DBox(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color);
	void DrawName(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, const std::string& name, ImColor color);
	void DrawLines(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color);
	void DrawSkeleton(const Entity& entity, const SDK::FMinimalViewInfo& cam, ImColor color);

};

inline Esp* esp = new Esp(cheat);

