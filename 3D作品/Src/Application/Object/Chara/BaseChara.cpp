#include "BaseChara.h"

void BaseChara::SetMe(std::shared_ptr<KdGameObject> _me)
{
	m_me = _me;
}

const std::shared_ptr<KdGameObject>& BaseChara::GetMe()
{
	if (!m_me.expired())
	{
		std::shared_ptr<KdGameObject> me = m_me.lock();
		return me;
	}
	else
	{
		return nullptr;
	}
}

void BaseChara::CharaRotetion(Math::Vector3 _moveVec)
{
	if (_moveVec.LengthSquared() == 0.0f) { return; }

	// キャラクターの正面方向のベクトル
	Math::Vector3 nowDir = GetMatrix().Backward();

	//　移動方向のベクトル
	Math::Vector3 targetDir = _moveVec;

	nowDir.Normalize();
	targetDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	//角度の差分
	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -12.0f, 12.0f);
	m_worldRot.y += rotateAng;
}

bool BaseChara::CollisionOther(KdCollider::SphereInfo _sphereInfo, std::list<std::shared_ptr<KdGameObject>> _objList, std::shared_ptr<KdGameObject> _me, float& _maxOverLap, Math::Vector3& _dir, Math::Vector3& _pos)
{
	// デバッグ用
	m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報に格納するリスト
	std::list<KdCollider::CollisionResult> retSightList;

	// 球と当たり判定
	for (auto& obj : _objList)
	{
		if (obj == m_me.lock()) { break; }
		obj->Intersects(_sphereInfo, &retSightList);
	}

	// 球に当たったリスト情報一番近いオブジェクトを検出する
	_maxOverLap = 0;
	_dir = {};
	_pos = {};
	bool hit = false;
	for (auto& ret : retSightList)
	{
		// 一番近くで当たったものを探す
		if (_maxOverLap < ret.m_overlapDistance)
		{
			_maxOverLap = ret.m_overlapDistance;
			_dir = ret.m_hitDir;
			_pos = ret.m_hitPos;
			hit = true;
		}
	}
	return hit;
}