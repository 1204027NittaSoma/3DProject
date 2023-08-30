#include "Player.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/TPSCamera/TPSCamera.h"
#include "../../Weapon/BulletWeapon.h"


void Player::Init()
{
	m_pos = {};
	m_speed = 0.2f;
	m_ang = 0;

	m_status.hp = 5;

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat;
	rotMat = Math::Matrix::CreateRotationZ(m_ang);

	m_mWorld = rotMat * transMat;



	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Chara/Player/player.gltf"));

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypePlayer);

	// �f�o�b�O�p
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	//===================
	// ���[���h���ŊǗ�
	//===================
	// �i�s�������Ǘ�����x�N�g��
	Math::Vector3 moveVec = {};
	// �ړ����Ă��邩�ǂ������Ǘ�����ϐ�
	bool moveFlg = false;
	// �J�����̉�]�s����擾
	Math::Matrix camRotMat;

	camRotMat = m_camera.lock()->GetRotationYMatrix();

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Math::Vector3 v = Math::Vector3::TransformNormal(Math::Vector3(0, 0, 1), camRotMat);

		moveVec += v;
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Math::Vector3 v = Math::Vector3::TransformNormal(Math::Vector3(0, 0, -1), camRotMat);

		moveVec += v;
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		Math::Vector3 v = Math::Vector3::TransformNormal(Math::Vector3(-1, 0, 0), camRotMat);

		moveVec += v;
		moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		Math::Vector3 v = Math::Vector3::TransformNormal(Math::Vector3(1, 0, 0), camRotMat);

		moveVec += v;
		moveFlg = true;
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_pos.y += 0.5f;
	}

	moveVec.Normalize();
	m_pos += moveVec * m_speed;
	//m_pos.y -= m_gravity;
	//m_gravity += 0.005f;






	//==============
//�����蔻��
//==============

//���C����p�ϐ����쐬
	KdCollider::RayInfo rayInfo;
	//���C�̔��ˈʒu�i���W�j��ݒ�
	rayInfo.m_pos = m_pos;
	//���C�̕�����ݒ�
	rayInfo.m_dir = { 0,-1,0 };//�����x�N�g���͒����P
	//�i���̋��e�͈͂�ݒ�
	static float enableStepHigh = 0.3f;
	rayInfo.m_pos.y += enableStepHigh;
	//���C�̒�����ݒ�
	rayInfo.m_range = m_gravity + enableStepHigh;
	//�����蔻����������^�C�v��ݒ�
	rayInfo.m_type = KdCollider::TypeGround;

	Math::Vector3 hitPos = {};

	//�������Ă�����
	if (CollisionRay(rayInfo, SceneManager::Instance().GetObjList(), hitPos))
	{
		m_pos = hitPos;
		m_gravity = 0;
	}




	{
		//=======================
		// ������
		//=======================
		// ������p�̕ϐ�
		KdCollider::SphereInfo sphereInfo;
		// ���̒��S�ʒu��ݒ�
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		// ���̔��a��ݒ�
		sphereInfo.m_sphere.Radius = 0.6f;
		// �����蔻����������^�C�v�̐ݒ�
		sphereInfo.m_type = KdCollider::TypeGround | KdCollider::TypeBump;

		Math::Vector3 hitDir = {};
		float maxOverLap = 0;


		if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
		{
			// ���Ƌ����������Ă���
			m_pos += hitDir * maxOverLap;
		}

		// �_���[�W����
		sphereInfo.m_type = KdCollider::TypeEnemyDamage;

		if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
		{
			//if (m_status.invincible <= 0)
			{
				m_status.hp -= 1;
				m_status.invincible = m_status.HitDelay;
			}

		}

		m_status.invincible--;
		if (m_status.invincible <= 0)
		{
			m_status.invincible = 0;
		}

		if (m_status.hp <= 0)
		{
			m_isExpired = true;
		}


		// ����̓����蔻��================================================
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		// ���̔��a��ݒ�
		sphereInfo.m_sphere.Radius = 20;
		// �����蔻����������^�C�v�̐ݒ�
		sphereInfo.m_type = KdCollider::TypeSight;

		if (CollisionSight(sphereInfo, SceneManager::Instance().GetObjList(), hitDir, hitPos))
		{

			lockOn = true;
			hitDir *= Math::Vector3(-1, -1, -1);
			CharaRotetion(hitDir);

			m_bulletVec = hitPos - m_pos;
			m_bulletVec.Normalize();
		}
		else
		{
			lockOn = false;
			CharaRotetion(moveVec);
		}
	}
	if (lockOn)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_shootDelay <= 0)
			{				
				m_bullet = std::make_shared<BulletWeapon>();
				m_bullet->SetOwnerMat(m_shootMat);
				m_bullet->SetMoveVec(m_bulletVec);
				SceneManager::Instance().AddObject(m_bullet);
				m_shootDelay = 20;
			}
		}

		m_shootDelay--;
		if (m_shootDelay <= 0)
		{
			m_shootDelay = 0;
		}
	}

}

void Player::PostUpdate()
{
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat;
	rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = rotMat * transMat;


	const KdModelWork::Node* pEntryNode;
	pEntryNode = m_model->FindNode("Point");
	if (pEntryNode)
	{
		m_shootMat = pEntryNode->m_worldTransform * m_mWorld;
	}
}

void Player::DrawLit()
{
	//Math::Color color = { 0,0,0 };

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);

}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);

}

bool Player::CollisionSight(KdCollider::SphereInfo _sphereInfo, std::list<std::shared_ptr<KdGameObject>> _objList, Math::Vector3& _dir, Math::Vector3& _pos)
{
	// �f�o�b�O�p
	m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// ���ɓ��������I�u�W�F�N�g���Ɋi�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSightList;
	std::list<std::weak_ptr<KdGameObject>> retHitObjList;
	std::list<std::weak_ptr<KdGameObject>>::iterator retHitObjIt;

	// ���Ɠ����蔻��
	bool hitEnemy = false;
	for (auto& obj : _objList)
	{
		if (obj->Intersects(_sphereInfo, &retSightList))
		{
			hitEnemy = true;
			retHitObjList.push_back(obj);
		}
	}

	// ���ɓ����������X�g����ԋ߂��I�u�W�F�N�g�����o����
	_dir = {};
	_pos = {};
	bool hit = false;
	retHitObjIt = retHitObjList.begin();

	for (auto& ret : retSightList)
	{
		retHitObjIt->lock()->SetRetHitObj(ret);
		retHitObjIt++;
	}

	retHitObjIt = retHitObjList.begin();

	if (hitEnemy)
	{
		if (GetAsyncKeyState('R') & 0x8000)
		{
			if (!pushEnKey)
			{
				if (!m_lockonFlg)
				{
					m_target = *retHitObjIt;
					m_lockonFlg = true;
					pushEnKey = true;
				}
				else
				{
					m_target.reset();
					m_lockonFlg = false;
					pushEnKey = true;
				}
			}
		}
		else
		{
			pushEnKey = false;
		}

		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			if (m_lockonFlg)
			{
				if (!pushKey)
				{

					while (m_target.lock() != (*retHitObjIt).lock())
					{
						retHitObjIt++;
					}

					retHitObjIt++;

					if (retHitObjIt == retHitObjList.end())
					{
						retHitObjIt = retHitObjList.begin();
					}

					m_target = *retHitObjIt;
					pushKey = true;
				}
			}
		}
		else
		{
			pushKey = false;
		}
	}

	if (m_target.expired() == false)
	{
		_dir = m_target.lock()->GetRetHitObj().m_hitDir;
		_pos = m_target.lock()->GetPos();
		hit = true;

		if ((m_target.lock()->GetRetHitObj().m_hitPos - m_pos).Length() > _sphereInfo.m_sphere.Radius)
		{
			retHitObjIt = retHitObjList.begin();
			itCnt = 0;
			m_lockonFlg = false;
			m_target.reset();
		}
	}
	else
	{
		retHitObjIt = retHitObjList.begin();
		itCnt = 0;
		m_lockonFlg = false;
		m_target.reset();
	}
	return hit;
}
