#include "KeyComponent.h"
#include "../../../../Engine/Collider/SphereCollider.h"
#include "../Stage.h"
#include "../../Player/Player.h"

KeyComponent::KeyComponent(StageObject* myObj)
	:StageObjectComponent(myObj, "KeyComponent")
{
}

void KeyComponent::Initialize()
{
    myObject->AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
}

void KeyComponent::Update()
{
}

json KeyComponent::Save()
{
    json data;
    data["type"] = COMP_KEY;
    // ���̃f�[�^���ۑ�����ꍇ�͂����ɒǉ�
    return data;
}

void KeyComponent::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "Player") {
        ((Stage*)myObject->FindObject("Stage"))->Delete(myObject);
        ((Player*)((Stage*)myObject->FindObject("Player")))->GetKey();
    }
}
