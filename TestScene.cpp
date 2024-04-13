#include "TestScene.h"
#include "Engine/DirectX/Input.h"
#include "Engine/ResourceManager/Model.h"
#include "Engine/ImGui/imgui.h"


//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	hModel = Model::Load("Model/Player/Walking.fbx");
	
}

//�X�V
void TestScene::Update()
{
	// ���[���h���W�ɂ���ĕϊ�����Ă邩�m�F�p
	ImGui::SliderFloat("x", &transform_.position_.x, -10, 10);

	int i = 0;
	// ���_���W�̃��[�J�����W���擾
	vs_ = Model::GetVertices(hModel);
	for (auto v : vs_) {
		// ���[���h�s��ŕϊ�
		XMVECTOR vecPosition = XMVector3Transform(XMLoadFloat3(&v), transform_.GetWorldMatrix());
		XMStoreFloat3(&v, vecPosition);
		ImGui::Text("vPos[%d] = %f,%f,%f",i, v.x, v.y, v.z);
		i++;
	}

	// �C���f�b�N�X�����擾

}

//�`��
void TestScene::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

//�J��
void TestScene::Release()
{
}
