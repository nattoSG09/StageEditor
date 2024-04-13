#include "TestScene.h"
#include "Engine/DirectX/Input.h"
#include "Engine/ResourceManager/Model.h"
#include "Engine/ImGui/imgui.h"


//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	hModel = Model::Load("Model/Player/Walking.fbx");
	
}

//更新
void TestScene::Update()
{
	// ワールド座標によって変換されてるか確認用
	ImGui::SliderFloat("x", &transform_.position_.x, -10, 10);

	int i = 0;
	// 頂点座標のローカル座標を取得
	vs_ = Model::GetVertices(hModel);
	for (auto v : vs_) {
		// ワールド行列で変換
		XMVECTOR vecPosition = XMVector3Transform(XMLoadFloat3(&v), transform_.GetWorldMatrix());
		XMStoreFloat3(&v, vecPosition);
		ImGui::Text("vPos[%d] = %f,%f,%f",i, v.x, v.y, v.z);
		i++;
	}

	// インデックス情報を取得

}

//描画
void TestScene::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

//開放
void TestScene::Release()
{
}
