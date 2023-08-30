#include "KdGameObject.h"

void KdGameObject::DrawDebug()
{
	if (!m_pDebugWire)return;

	m_pDebugWire->Draw();
}

void KdGameObject::SetScale(float scalar)
{
	Math::Vector3 scale(scalar);

	SetScale(scale);
}

void KdGameObject::SetScale(const Math::Vector3& scale)
{
	Math::Vector3 vecX = m_mWorld.Right();
	Math::Vector3 vecY = m_mWorld.Up();
	Math::Vector3 vecZ = m_mWorld.Backward(); 
	
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();

	m_mWorld.Right(vecX * scale.x);
	m_mWorld.Up(vecY * scale.y);
	m_mWorld.Backward(vecZ * scale.z);
}

Math::Vector3 KdGameObject::GetScale() const
{
	return Math::Vector3(m_mWorld.Right().Length(), m_mWorld.Up().Length(), m_mWorld.Backward().Length());
}

void KdGameObject::CalcDistSqrFromCamera(const Math::Vector3& camPos)
{
	m_distSqrFromCamera = (m_mWorld.Translation() - camPos).LengthSquared();
}

bool KdGameObject::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}

bool KdGameObject::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}

bool KdGameObject::CollisionRay(KdCollider::RayInfo _rayInfo, std::list<std::shared_ptr<KdGameObject>> _objList, Math::Vector3& _pos)
{
	// デバッグ用==============
	m_pDebugWire->AddDebugLine(_rayInfo.m_pos, _rayInfo.m_dir, _rayInfo.m_range);

	//レイに当たったオブジェクト情報
	std::list<KdCollider::CollisionResult> retRayList;

	//レイと当たり判定をする	
	for (auto& obj : _objList)
	{
		obj->Intersects(_rayInfo, &retRayList);
	}
	//レイに当たったリストから一番近いオブジェクトを検出
	_pos = {};// 遮断された座標
	float maxOverLap = 0;
	bool hit = false;
	for (auto& ret : retRayList)
	{
		//レイを遮断しオーバーした長さが一番長いもの
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			_pos = ret.m_hitPos;
			hit = true;
		}
	}
	return hit;
}

bool KdGameObject::CollisionSphere(const KdCollider::SphereInfo& _sphereInfo, std::list<std::shared_ptr<KdGameObject>> _objList, float& _maxOverLap, Math::Vector3& _dir, Math::Vector3& _pos)
{
	// デバッグ用
	//m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報に格納するリスト
	std::list<KdCollider::CollisionResult> retSightList;

	// 球と当たり判定
	for (auto& obj : _objList)
	{
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

bool KdGameObject::CollisionSphere(const KdCollider::SphereInfo& _sphereInfo, std::list<std::shared_ptr<KdGameObject>> _objList, float& _maxOverLap, Math::Vector3& _dir, Math::Vector3& _pos, std::weak_ptr<KdGameObject> _hitObj)
{
	// デバッグ用
	//m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報に格納するリスト
	std::list<KdCollider::CollisionResult> retSightList;

	// 球と当たり判定
	for (auto& obj : _objList)
	{
		if (obj->Intersects(_sphereInfo, &retSightList))
		{
			_hitObj = obj;
		}
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
