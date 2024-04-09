#include "Gravity.h"
#include "../../../../Engine/ImGui/imgui.h"

namespace {
	const float acceleration = 9.8f;
	const float FPS = 60;
}

Gravity::Gravity(StageObject* myObj)
    :StageObjectComponent(myObj,"Gravity"),speed_(0.0f),time_(0.1f)
{
}

void Gravity::Initialize()
{
	

}

void Gravity::Update()
{
	// 移動する速度を更新
	speed_ += acceleration * time_ / FPS;

	// 位置に適応
	myObject->Move(XMVectorSet(0, -1, 0, 0), speed_);
}

json Gravity::Save()
{
	json data;
	data["type"] = COMP_TESTMOVE;
	// 他のデータも保存する場合はここに追加
	return data;
}

void Gravity::DrawDatails()
{
	// 速度を入力
	ImGui::InputFloat("speed", &speed_);

	// 経過時間を入力
	ImGui::InputFloat("time", &time_);
}
