#pragma once

class BulletWeapon :public KdGameObject
{
public:
	BulletWeapon() { Init(); }
	~BulletWeapon() {}

	void Init()override;
	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void SetMoveVec(const Math::Vector3 _moveVec) { m_moveVec = _moveVec; }

	void SetOwnerMat(const Math::Matrix _mat) { m_ownerMat = _mat; }

	void OnHit()override;

private:
	std::shared_ptr<KdModelWork> m_model = nullptr;
	std::weak_ptr<KdModelWork> m_ownerModel;

	Math::Matrix m_mat;
	Math::Matrix m_ownerMat;

	Math::Vector3 m_pos = {};
	Math::Vector3 m_moveVec = {};

	float m_speed = 0.0f;
	int m_limit = 0;
};