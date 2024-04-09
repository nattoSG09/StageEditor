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
	// アニメーションを行うかどうか
	bool isAnim = false;

	// 移動処理
	{
		TPSCamera* cam = (TPSCamera*)FindObject("TPSCamera");
		float angle = cam->GetAngle().y;
		// 方向を設定
		XMVECTOR dir{}; {
			// 視線ベクトルを取得
			XMVECTOR sightline = XMVector3Normalize(XMVectorSetY(Camera::GetSightline(), 0));

			// 前方に移動
			if (Input::IsKey(DIK_W)) {
				dir += sightline; isAnim = true;
				transform_.rotate_.y = angle - 25;

			}
			// 後方に移動
			if (Input::IsKey(DIK_S)) {
				dir += -sightline; isAnim = true;
				transform_.rotate_.y = (angle - 25) + 180;
			}
			// 左方に移動
			if (Input::IsKey(DIK_A)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90))); isAnim = true;
				transform_.rotate_.y = (angle - 25) - 90;
			}
			// 右方に移動
			if (Input::IsKey(DIK_D)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90))); isAnim = true;
				transform_.rotate_.y = (angle - 25) + 90;
			}

			// 正規化して
			dir = XMVector3Normalize(dir);
		}

		// 速度を設定
		float speed = 0.1f;

		// 移動
		Move(dir, speed);
	}

	// アニメーションを行う
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
	
	// 接地処理
	//{
	//	// ステージの情報を取得する
	//	Stage* pStage = (Stage*)FindObject("Stage");
	//	
	//	
	//	vector<StageObject*> objects = pStage->GetObjects();



	//	// ステージのオブジェクトすべてに対してあたり判定を行う
	//	RayCastData playerToStageRay;
	//	for (auto obj : objects) {

	//		// レイキャストデータの初期化
	//		RayCastData tmp; {
	//			tmp.start = transform_.position_;
	//			tmp.dir = XMFLOAT3(0, -1, 0);
	//		}

	//		// レイキャストを下方向に放つ
	//		Model::RayCast(obj->GetModelHandle(), &tmp);
	//		ImGui::Text("%s.hit = %s,dist = %f",obj->GetObjectName().c_str(), tmp.hit ? "true" : "false",playerToStageRay.dist);

	//		// レイキャストが一度でもヒットしたらレイキャストを打つのをやめる
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
