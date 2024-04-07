#include "StageObject.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "Components/StageObjectComponent.h"

const string ComponentTypeToString(ComponentType type)
{
	switch (type)
	{
	case COMP_TESTMOVE: return "TestMove";
	case COMP_INPUTMOVE: return "InputMove";
	}

	return "none";
}

StageObject::StageObject(string name,string modelFilePath,GameObject* parent)
	:GameObject(parent, name),modelHandle_(-1),modelFilePath_(modelFilePath)
{
}

void StageObject::Initialize()
{
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// Component�̏���������
	for (auto comp : components_) {
		comp->Initialize();
	}
}

void StageObject::Update()
{
	// Component�̍X�V����
	for (auto comp : components_) {
		comp->Update();
	}
}

void StageObject::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void StageObject::Release()
{
}

void StageObject::Move(XMVECTOR dir,float speed)
{
	XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_	) + XMVector3Normalize(dir) * speed);
}

void StageObject::AddComponent(StageObjectComponent* comp)
{
	components_.push_back(comp);
	comp->Initialize();
}

void StageObject::DeleteComponent(StageObjectComponent* comp)
{
	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(components_.begin(), components_.end(), comp);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != components_.end()) components_.erase(it);

	// �폜
	delete comp;
}

StageObject* CreateStageObject(string name, string modelFilePath, GameObject* parent)
{
	StageObject* obj = new StageObject(name, modelFilePath, parent);
	obj->Initialize();
	if (parent != nullptr)parent->PushBackChild(obj);
	return obj;
}
