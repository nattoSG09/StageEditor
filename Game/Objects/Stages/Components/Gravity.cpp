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
	// �ړ����鑬�x���X�V
	speed_ += acceleration * time_ / FPS;

	// �ʒu�ɓK��
	myObject->Move(XMVectorSet(0, -1, 0, 0), speed_);
}

json Gravity::Save()
{
	json data;
	data["type"] = COMP_TESTMOVE;
	// ���̃f�[�^���ۑ�����ꍇ�͂����ɒǉ�
	return data;
}

void Gravity::DrawDatails()
{
	// ���x�����
	ImGui::InputFloat("speed", &speed_);

	// �o�ߎ��Ԃ����
	ImGui::InputFloat("time", &time_);
}
