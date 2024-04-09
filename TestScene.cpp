#include "TestScene.h"
#include "Game/Objects/Stages/Stage.h"
#include "Game/Objects/Stages/StageObject.h"
#include "Game/Objects/SkySphere/SkySphere.h"
#include "Game/Cameras/DebugCamera.h"
#include "Game/Cameras/TPSCamera.h"
#include "Engine/DirectX/Input.h"
#include "Engine/GameObject/Camera.h"
#include "Game/Objects/Player/Player.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	// �X�J�C�X�t�B�A��p��
	SkySphere* pSkySphere = Instantiate<SkySphere>(this);

	// �X�e�[�W��p��
	Stage* pStage = Instantiate<Stage>(this);


#ifdef _DEBUG
	// debug�p�G�f�B�^�[�J������p��
	DebugCamera* pDebugCamera = Instantiate<DebugCamera>(this);
	pDebugCamera->SetPosition(XMFLOAT3(20, 40, -80));
#else
	// �v���C���[��p��
	Player* pPlayer = Instantiate<Player>(this);
	pPlayer->SetPosition(0, 10, 0);

	// release�p�v���C���[�J������p��
	TPSCamera* pTpsCamera = Instantiate<TPSCamera>(this);
	pTpsCamera->SetTarget(pPlayer);
#endif // _DEBUG


}

//�X�V
void TestScene::Update()
{
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
