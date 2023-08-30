#pragma once

class Ground :public KdGameObject
{
public:
	Ground() { Init(); }
	~Ground() {}

	void DrawLit()override;
	void GenerateDepthMapFromLight()override;
	void Init()override;

private:
	std::shared_ptr<KdModelWork> m_model = nullptr;
	Math::Vector3 m_pos;
};