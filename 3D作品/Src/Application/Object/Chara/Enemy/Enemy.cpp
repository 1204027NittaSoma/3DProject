#include "Enemy.h"
#include "../../../Scene/SceneManager.h"
#include "../../Weapon/BulletWeapon.h"

void Enemy::Init()
{
	m_pos = { -10,0,10 };

	m_status.hp = 10;

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Chara/Enemy/Enemy.gltf"));

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeBump | KdCollider::TypeSight | KdCollider::TypeEnemy);

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Enemy::Update()
{



	Math::Vector3 hitPos = {};


	//=======================
	// 球判定
	//=======================
	// 球判定用の変数
	KdCollider::SphereInfo sphereInfo;
	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 2.0f, 0);
	// 球の半径を設定
	sphereInfo.m_sphere.Radius = 1.0f;
	// 当たり判定をしたいタイプの設定
	sphereInfo.m_type = KdCollider::TypeGround | KdCollider::TypePlayer;

	Math::Vector3 hitDir = {};
	float maxOverLap = 0;


	if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
	{
		m_pos += hitDir * maxOverLap;
	}


	// ダメージ判定========================================
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報に格納するリスト
	std::list<KdCollider::CollisionResult> retSightList;

	// 球と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphereInfo, &retSightList))
		{
			m_hitObj = obj;
		}
	}

	// 球に当たったリスト情報一番近いオブジェクトを検出する
	maxOverLap = 0;
	hitDir = {};
	hitPos = {};
	bool hit = false;
	for (auto& ret : retSightList)
	{
		// 一番近くで当たったものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			hitPos = ret.m_hitPos;
			hit = true;
		}
	}

	if(hit)
	{
		//if (m_status.invincible <= 0)
		{
			m_status.hp -= 1;
			m_status.invincible = m_status.HitDelay;
			m_hitObj->OnHit();
		}

	}
	// =====================================================


	m_status.invincible--;
	if (m_status.invincible <= 0)
	{
		m_status.invincible = 0;
	}

	if (m_status.hp <= 0)
	{
		m_isExpired = true;
	}


	// 敵同士の当たり判定=======================================

	// 当たり判定をしたいタイプの設定
	//sphereInfo.m_type = KdCollider::TypeBump;

	//if (CollisionOther(sphereInfo, SceneManager::Instance().GetObjList(), GetMe(), maxOverLap, hitDir, hitPos))
	//{
	//	m_pos += hitDir * maxOverLap;
	//}

	// 視野の当たり判定================
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.5f, 0);
	// 球の半径を設定
	sphereInfo.m_sphere.Radius = 15;
	// 当たり判定をしたいタイプの設定
	sphereInfo.m_type = KdCollider::TypePlayer;

	if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
	{
		CharaRotetion(-hitDir);
	}
}

void Enemy::PostUpdate()
{

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat;
	rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = rotMat * transMat;
}

void Enemy::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Enemy::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}
