#include "TPSCamera.h"
#include "../../../Scene/SceneManager.h"

void TPSCamera::Init()
{
	// �����_
	m_LocalPos = Math::Matrix::CreateTranslation(0, 2.0f, -10.0f);

	// ��ʒ���
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;


	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �f�o�b�O�p
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	CameraBase::Init();
}

void TPSCamera::Update()
{


	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	//=======================
	// ������
	// ������p�̕ϐ�
	//=======================

	
	// �J������]
	UpdateRotateByMouse();
	m_Rotation = GetRotationMatrix();
	m_mWorld = m_LocalPos * m_Rotation * targetMat;

	CameraBase::Update();
}

void TPSCamera::UpdateRotateByMouse()
{
	// �}�E�X�ŃJ��������]�����鏈��
	POINT newPos;
	GetCursorPos(&newPos);

	POINT mouseMove;
	mouseMove.x = newPos.x - m_FixMousePos.x;
	mouseMove.y = newPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]�����鏈��
	m_DegAng.x += mouseMove.y * 0.15f;
	m_DegAng.y += mouseMove.x * 0.15f;

	// ��]����
	m_DegAng.x = std::clamp(m_DegAng.x, -FLT_MAX, FLT_MAX);
}
