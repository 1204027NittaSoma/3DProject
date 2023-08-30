#pragma once
#include "../BaseChara.h"

class TPSCamera;
class BulletWeapon;

class Player :public BaseChara
{
public:
	Player() { Init(); }
	~Player() {}

	void Init()override;
	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void SetPos(const Math::Vector3 _pos) { m_pos = _pos; }

	void SetCamera(std::shared_ptr<TPSCamera> _camera)
	{
		m_camera = _camera;
	}

	bool CollisionSight(KdCollider::SphereInfo _sphereInfo,
		std::list<std::shared_ptr<KdGameObject>> _objList,
		Math::Vector3& _dir, Math::Vector3& _pos);

private:

	std::weak_ptr<TPSCamera> m_camera;

	std::weak_ptr<KdGameObject> m_target;

	bool pushKey = false;
	bool pushEnKey = false;
	bool m_lockonFlg = false;
	bool lockOn = false;

	int itCnt = 0;
	int m_shootDelay = 0;

	std::shared_ptr<BulletWeapon> m_bullet = nullptr;

	Math::Vector3 m_bulletVec = {};
	Math::Vector3 m_shootPos = {};
	Math::Matrix m_shootMat;
};