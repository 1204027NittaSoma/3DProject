#pragma once

class Field :public KdGameObject
{
public:
	Field() { Init(); }
	~Field() { ReleaseDangeon(); }

	void Init()override;
	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void CreateDangeon();
	void ReleaseDangeon();

	void SetEntryDangeon(const bool _entry) { bEntryDangeon = _entry; }

private:
	std::shared_ptr<KdModelWork> m_model = nullptr;


	Math::Vector3 m_pos = {};
	Math::Vector3 m_entryPos = {};

	static const int MaxSetPos = 16;
	Math::Vector3 m_setPos[MaxSetPos] = {};
	Math::Matrix m_dangeonMat[MaxSetPos];
	std::string SetNum[MaxSetPos];
	
	bool bSetDangeonMat = false;
	bool bEntryNextDangeon = false;
	bool bCreateDangeon = false;
	bool bEntryDangeon = false;

	std::list<std::shared_ptr<KdGameObject>> m_roomList;
	std::shared_ptr<KdGameObject> m_room;

};