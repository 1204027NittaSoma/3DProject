#pragma once

class CurveRoom :public KdGameObject
{
public:
	CurveRoom() { Init(); }
	~CurveRoom() {}

	void Init()override;


	void DrawLit()override;
	void GenerateDepthMapFromLight()override;



private:
	std::shared_ptr<KdModelWork> m_model = nullptr;

	Math::Vector3 m_pos = {};
	Math::Vector3 m_entryPos = {};

	bool bSetEntry = false;
	bool bEntryNextDangeon = false;
};