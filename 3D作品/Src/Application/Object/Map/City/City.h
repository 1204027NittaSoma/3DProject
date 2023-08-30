#pragma once

class City :public KdGameObject
{
public:
	City() { Init(); }
	~City() {}

	void Init()override;
	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	bool GetEntryDangeon() { return bEntryDangeon; }

	void ReleaseCity() { m_isExpired = true; }

private:

	std::shared_ptr<KdModelWork> m_model = nullptr;

	Math::Vector3 m_pos = {};
	Math::Vector3 m_entryPos = {};

	bool bSetEntry = false;
	bool bEntryDangeon = false;
};