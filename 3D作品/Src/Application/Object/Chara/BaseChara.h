#pragma once

class TPSCamera;

struct Status
{
	int hp = 0;
	int mp = 0;
	static const int HitDelay = 60;
	int invincible = HitDelay;
};

class BaseChara :public KdGameObject
{
public:
	BaseChara() {}
	~BaseChara() {}

	void SetMe(std::shared_ptr<KdGameObject> _me);

	const std::shared_ptr<KdGameObject>& GetMe();

	void CharaRotetion(Math::Vector3 _moveVec);

	bool CollisionOther(KdCollider::SphereInfo _sphereInfo,
		std::list<std::shared_ptr<KdGameObject>> _objList, std::shared_ptr<KdGameObject> _me,
		float& _maxOverLap, Math::Vector3& _dir, Math::Vector3& _pos);

protected:

	std::weak_ptr<KdGameObject> m_me;

	std::shared_ptr<KdModelWork> m_model = nullptr;

	Math::Vector3 m_worldRot = {};

	Math::Vector3 m_pos = {};

	float m_speed = 0;
	float m_gravity = 0;
	float m_ang = 0;

	Status m_status;
};