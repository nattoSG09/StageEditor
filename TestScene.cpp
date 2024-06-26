#include "TestScene.h"
#include "Game/Objects/Stages/Stage.h"
#include "Game/Objects/Stages/StageObject.h"
#include "Game/Objects/SkySphere/SkySphere.h"
#include "Game/Cameras/DebugCamera.h"
#include "Game/Cameras/TPSCamera.h"
#include "Engine/DirectX/Input.h"
#include "Engine/GameObject/Camera.h"
#include "Game/Objects/Player/Player.h"

#include "TestObject.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	// スカイスフィアを用意
	SkySphere* pSkySphere = Instantiate<SkySphere>(this);

	Instantiate<TestObject>(this);

//
//	// ステージを用意
//	Stage* pStage = Instantiate<Stage>(this);
//
//	// プレイヤーを用意
//	Player* pPlayer = Instantiate<Player>(this);
//	pPlayer->SetPosition(0, 10, 0);
//
//#ifdef _DEBUG
//	// debug用エディターカメラを用意
//	DebugCamera* pDebugCamera = Instantiate<DebugCamera>(this);
//	pDebugCamera->SetPosition(XMFLOAT3(20, 40, -80));
//#else
//
//	// release用プレイヤーカメラを用意
//	TPSCamera* pTpsCamera = Instantiate<TPSCamera>(this);
//	pTpsCamera->SetTarget(pPlayer);
//#endif // _DEBUG


}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
