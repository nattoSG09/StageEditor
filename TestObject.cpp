#include "TestObject.h"
#include "Engine/ResourceManager/Model.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/DirectX/Input.h"

TestObject::TestObject(GameObject* parent)
	:GameObject(parent,"TestObject")
{
}

void TestObject::Initialize()
{
	hModel_ = Model::Load("DebugCollision/BoxCollider.fbx");
}

void TestObject::Update()
{
	XMVECTOR q0 = XMLoadFloat3(&transform_.rotate_);
	XMVECTOR q1 = XMVectorSet(0,-180,0,1);

	static float t = 0;
	ImGui::Text("t = %f", t);

	// 現在の方向を指すベクトル
	XMVECTOR result = XMQuaternionSlerp(q0, q1, t);

	XMFLOAT3 output;
	XMStoreFloat3(&output, result);

	ImGui::Text("result = %f,%f,%f", output.x,output.y,output.z);
	transform_.rotate_ = output;
	if (Input::IsKey(DIK_SPACE)) {
		if (t <= 1)t += 0.0001f;
	}

	
}

void TestObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObject::Release()
{
}
