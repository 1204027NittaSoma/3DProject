#pragma once
#include "../BaseChara.h"

class BulletWeapon;

class Enemy :public BaseChara
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Init()override;
	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void SetPos(const Math::Vector3 _pos) { m_pos = _pos; }

private:
	
	std::shared_ptr<KdGameObject> m_hitObj = nullptr;
	bool m_bulletFlg = false;
};