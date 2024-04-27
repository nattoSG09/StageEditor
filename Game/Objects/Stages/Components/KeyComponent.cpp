#include "KeyComponent.h"
#include "../../../../Engine/Collider/SphereCollider.h"
#include "../Stage.h"

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
    // ‘¼‚Ìƒf[ƒ^‚à•Û‘¶‚·‚éê‡‚Í‚±‚±‚É’Ç‰Á
    return data;
}

void KeyComponent::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "Player") {
        int a = 0;
        ((Stage*)myObject->FindObject("Stage"))->Delete(myObject);
    }
}
