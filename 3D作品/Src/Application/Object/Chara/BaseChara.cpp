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

	// �L�����N�^�[�̐��ʕ����̃x�N�g��
	Math::Vector3 nowDir = GetMatrix().Backward();

	//�@�ړ������̃x�N�g��
	Math::Vector3 targetDir = _moveVec;

	nowDir.Normalize();
	targetDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	//�p�x�̍���
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
	// �f�o�b�O�p
	m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// ���ɓ��������I�u�W�F�N�g���Ɋi�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSightList;

	// ���Ɠ����蔻��
	for (auto& obj : _objList)
	{
		if (obj == m_me.lock()) { break; }
		obj->Intersects(_sphereInfo, &retSightList);
	}

	// ���ɓ����������X�g����ԋ߂��I�u�W�F�N�g�����o����
	_maxOverLap = 0;
	_dir = {};
	_pos = {};
	bool hit = false;
	for (auto& ret : retSightList)
	{
		// ��ԋ߂��œ����������̂�T��
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