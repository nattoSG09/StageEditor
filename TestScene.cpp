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

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	// ゲーム
	modelHandle_ = Model::Load("DebugCollision/BoxCollider.fbx");

	// スカイスフィアを用意
	SkySphere* pSkySphere = Instantiate<SkySphere>(this);

	// ステージを用意
	Stage* pStage = Instantiate<Stage>(this);

	// プレイヤーを用意
	Player* pPlayer = Instantiate<Player>(this);
	pPlayer->SetPosition(0, 10, 0);

#ifdef _DEBUG
	// debug用エディターカメラを用意
	DebugCamera* pDebugCamera = Instantiate<DebugCamera>(this);
	pDebugCamera->SetPosition(XMFLOAT3(20, 40, -80));
#else
	// release用プレイヤーカメラを用意
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

//更新
void TestScene::Update()
{
	//// 回転処理
	//{
	//	// 実行状態
	//	static bool isActive = false;

	//	// 回転角度（ラジアン）
	//	static float angle = 0;

	//	// 目的の角度（ラジアン）
	//	constexpr float target = XMConvertToRadians(90) ;

	//	// フレームレート
	//	const float fps = 60.f;

	//	// 回転に掛かる時間（秒）
	//	static float rotationDuration = 0.5f;

	//	// 回転量 = 目的の角度（ラジアン） / (フレームレート * 回転にかかる時間（秒）)
	//	float rotationValue = target / (fps * rotationDuration);

	//	// 回転する
	//	if(isActive)if (angle <= target)angle += rotationValue;

	//	// `OutCubic`イージング関数を適用 ※参考<https://easings.net/ja#easeOutCubic>
	//	float easedAngle = target * OutCubic(angle);

	//	// GUI表示
	//	ImGui::Begin("rotation Test");{
	//		if (ImGui::Button("start")) { angle = 0; isActive = true; }
	//		ImGui::DragFloat("duration", &rotationDuration, 0.1f);
	//		ImGui::DragFloat("angle", &angle,0.1f);
	//	}
	//	ImGui::End();

	//	// モデルを回転
	//	transform_.rotate_.y = XMConvertToDegrees(easedAngle);
	//}

}

//描画
void TestScene::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

//開放
void TestScene::Release()
{
}
