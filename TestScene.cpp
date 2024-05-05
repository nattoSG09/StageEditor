#include "TestScene.h"
#include "Game/Objects/Stages/Stage.h"
#include "Game/Objects/Stages/StageObject.h"
#include "Game/Objects/SkySphere/SkySphere.h"
#include "Game/Cameras/DebugCamera.h"
#include "Game/Cameras/TPSCamera.h"
#include "Engine/DirectX/Input.h"
#include "Engine/GameObject/Camera.h"
#include "Game/Objects/Player/Player.h"


#include "Engine/ResourceManager/Model.h"
#include "Engine/ImGui/imgui.h"
#include <algorithm>

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	// �Q�[��
	modelHandle_ = Model::Load("DebugCollision/BoxCollider.fbx");

	// �X�J�C�X�t�B�A��p��
	SkySphere* pSkySphere = Instantiate<SkySphere>(this);

	// �X�e�[�W��p��
	Stage* pStage = Instantiate<Stage>(this);

	// �v���C���[��p��
	Player* pPlayer = Instantiate<Player>(this);
	pPlayer->SetPosition(0, 10, 0);

#ifdef _DEBUG
	// debug�p�G�f�B�^�[�J������p��
	DebugCamera* pDebugCamera = Instantiate<DebugCamera>(this);
	pDebugCamera->SetPosition(XMFLOAT3(20, 40, -80));
#else
	// release�p�v���C���[�J������p��
	TPSCamera* pTpsCamera = Instantiate<TPSCamera>(this);
	pTpsCamera->SetTarget(pPlayer);
#endif // _DEBUG
}

float OutCubic(float x,float min,float max) {
	x = std::clamp(x, min, max);
	return (float)(1 - pow(1 - x, 3));
}

float OutCubic(float x) {
	return OutCubic(x, 0.f, 1.f);
}

//�X�V
void TestScene::Update()
{
	//// ��]����
	//{
	//	// ���s���
	//	static bool isActive = false;

	//	// ��]�p�x�i���W�A���j
	//	static float angle = 0;

	//	// �ړI�̊p�x�i���W�A���j
	//	constexpr float target = XMConvertToRadians(90) ;

	//	// �t���[�����[�g
	//	const float fps = 60.f;

	//	// ��]�Ɋ|���鎞�ԁi�b�j
	//	static float rotationDuration = 0.5f;

	//	// ��]�� = �ړI�̊p�x�i���W�A���j / (�t���[�����[�g * ��]�ɂ����鎞�ԁi�b�j)
	//	float rotationValue = target / (fps * rotationDuration);

	//	// ��]����
	//	if(isActive)if (angle <= target)angle += rotationValue;

	//	// `OutCubic`�C�[�W���O�֐���K�p ���Q�l<https://easings.net/ja#easeOutCubic>
	//	float easedAngle = target * OutCubic(angle);

	//	// GUI�\��
	//	ImGui::Begin("rotation Test");{
	//		if (ImGui::Button("start")) { angle = 0; isActive = true; }
	//		ImGui::DragFloat("duration", &rotationDuration, 0.1f);
	//		ImGui::DragFloat("angle", &angle,0.1f);
	//	}
	//	ImGui::End();

	//	// ���f������]
	//	transform_.rotate_.y = XMConvertToDegrees(easedAngle);
	//}

}

//�`��
void TestScene::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

//�J��
void TestScene::Release()
{
}
