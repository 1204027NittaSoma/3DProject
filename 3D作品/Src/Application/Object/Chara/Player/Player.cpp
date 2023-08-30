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

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	//===================
	// ワールド軸で管理
	//===================
	// 進行方向を管理するベクトル
	Math::Vector3 moveVec = {};
	// 移動しているかどうかを管理する変数
	bool moveFlg = false;
	// カメラの回転行列を取得
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
//当たり判定
//==============

//レイ判定用変数を作成
	KdCollider::RayInfo rayInfo;
	//レイの発射位置（座標）を設定
	rayInfo.m_pos = m_pos;
	//レイの方向を設定
	rayInfo.m_dir = { 0,-1,0 };//方向ベクトルは長さ１
	//段差の許容範囲を設定
	static float enableStepHigh = 0.3f;
	rayInfo.m_pos.y += enableStepHigh;
	//レイの長さを設定
	rayInfo.m_range = m_gravity + enableStepHigh;
	//当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	Math::Vector3 hitPos = {};

	//当たっていたら
	if (CollisionRay(rayInfo, SceneManager::Instance().GetObjList(), hitPos))
	{
		m_pos = hitPos;
		m_gravity = 0;
	}




	{
		//=======================
		// 球判定
		//=======================
		// 球判定用の変数
		KdCollider::SphereInfo sphereInfo;
		// 球の中心位置を設定
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		// 球の半径を設定
		sphereInfo.m_sphere.Radius = 0.6f;
		// 当たり判定をしたいタイプの設定
		sphereInfo.m_type = KdCollider::TypeGround | KdCollider::TypeBump;

		Math::Vector3 hitDir = {};
		float maxOverLap = 0;


		if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
		{
			// 球と球が当たっている
			m_pos += hitDir * maxOverLap;
		}

		// ダメージ判定
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


		// 視野の当たり判定================================================
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		// 球の半径を設定
		sphereInfo.m_sphere.Radius = 20;
		// 当たり判定をしたいタイプの設定
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
	// デバッグ用
	m_pDebugWire->AddDebugSphere(_sphereInfo.m_sphere.Center, _sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報に格納するリスト
	std::list<KdCollider::CollisionResult> retSightList;
	std::list<std::weak_ptr<KdGameObject>> retHitObjList;
	std::list<std::weak_ptr<KdGameObject>>::iterator retHitObjIt;

	// 球と当たり判定
	bool hitEnemy = false;
	for (auto& obj : _objList)
	{
		if (obj->Intersects(_sphereInfo, &retSightList))
		{
			hitEnemy = true;
			retHitObjList.push_back(obj);
		}
	}

	// 球に当たったリスト情報一番近いオブジェクトを検出する
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
