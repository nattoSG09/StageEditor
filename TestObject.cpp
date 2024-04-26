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
	// Œ»Žž“_‚Ì‰ñ“]Šp“x
	XMVECTOR rotate = XMLoadFloat3(&transform_.rotate_);

	float t = 0.1f;
	static float angle = 0;

	if (Input::IsKey(DIK_W)) { angle = 0.f; ImGui::Text("input key = W"); }
	if (Input::IsKey(DIK_A)) { angle = -90.f; ImGui::Text("input key = A");}
	if (Input::IsKey(DIK_S)) { angle = 180.f; ImGui::Text("input key = S");}
	if (Input::IsKey(DIK_D)) { angle = 90.f; ImGui::Text("input key = D");}

	XMVECTOR result = XMQuaternionSlerp(rotate, XMVectorSet(0,angle,0,1), t);

	XMFLOAT3 output;
	XMStoreFloat3(&output, result);

	ImGui::Text("result = %f,%f,%f", output.x,output.y,output.z);
	transform_.rotate_ = output;
}

void TestObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObject::Release()
{
}
