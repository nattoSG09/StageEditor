#include "Player.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/ImGui/ImGui.h"
#include "../../Cameras/TPSCamera.h"
#include "../../../Engine/GameObject/Camera.h"
#include "../Stages/Stage.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player")
{
}

void Player::Initialize()
{
	modelHandle_ = Model::Load("Model/Player/Walking.fbx");
	assert(modelHandle_ >= 0);

}

void Player::Update()
{
	// �A�j���[�V�������s�����ǂ���
	bool isAnim = false;

	// �ړ�����
	{
		TPSCamera* cam = (TPSCamera*)FindObject("TPSCamera");
		float angle = cam->GetAngle().y;
		// ������ݒ�
		XMVECTOR dir{}; {
			// �����x�N�g�����擾
			XMVECTOR sightline = XMVector3Normalize(XMVectorSetY(Camera::GetSightline(), 0));

			// �O���Ɉړ�
			if (Input::IsKey(DIK_W)) {
				dir += sightline; isAnim = true;
				transform_.rotate_.y = angle - 25;

			}
			// ����Ɉړ�
			if (Input::IsKey(DIK_S)) {
				dir += -sightline; isAnim = true;
				transform_.rotate_.y = (angle - 25) + 180;
			}
			// �����Ɉړ�
			if (Input::IsKey(DIK_A)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90))); isAnim = true;
				transform_.rotate_.y = (angle - 25) - 90;
			}
			// �E���Ɉړ�
			if (Input::IsKey(DIK_D)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90))); isAnim = true;
				transform_.rotate_.y = (angle - 25) + 90;
			}

			// ���K������
			dir = XMVector3Normalize(dir);
		}

		// ���x��ݒ�
		float speed = 0.1f;

		// �ړ�
		Move(dir, speed);
	}

	// �A�j���[�V�������s��
	{
		static bool prevAnim = false;
		if (isAnim == true) {
			if (prevAnim == false)Model::SetAnimFrame(modelHandle_, 0, 60, 1);
			prevAnim = true;
		}
		else {
			Model::SetAnimFrame(modelHandle_, 0, 0, 0);
			prevAnim = false;
		}
	}
	
	// �ڒn����
	//{
	//	// �X�e�[�W�̏����擾����
	//	Stage* pStage = (Stage*)FindObject("Stage");
	//	
	//	
	//	vector<StageObject*> objects = pStage->GetObjects();



	//	// �X�e�[�W�̃I�u�W�F�N�g���ׂĂɑ΂��Ă����蔻����s��
	//	RayCastData playerToStageRay;
	//	for (auto obj : objects) {

	//		// ���C�L���X�g�f�[�^�̏�����
	//		RayCastData tmp; {
	//			tmp.start = transform_.position_;
	//			tmp.dir = XMFLOAT3(0, -1, 0);
	//		}

	//		// ���C�L���X�g���������ɕ���
	//		Model::RayCast(obj->GetModelHandle(), &tmp);
	//		ImGui::Text("%s.hit = %s,dist = %f",obj->GetObjectName().c_str(), tmp.hit ? "true" : "false",playerToStageRay.dist);

	//		// ���C�L���X�g����x�ł��q�b�g�����烌�C�L���X�g��ł̂���߂�
	//		if (tmp.hit == true) { 
	//			playerToStageRay = tmp;break; 
	//		}
	//	}
	//	
	//	// 
	//	if (playerToStageRay.dist >= 0.1f) {
	//		Move(XMLoadFloat3(&playerToStageRay.dir), playerToStageRay.dist);
	//	}

	//}

}

void Player::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void Player::Release()
{
}

void Player::Move(XMVECTOR dir, float speed)
{
	XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + (dir * speed));
}
