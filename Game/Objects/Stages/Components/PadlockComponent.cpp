#include "PadlockComponent.h"
#include "../../../../Engine/Collider/SphereCollider.h"
#include "../../Player/Player.h"
#include "../Stage.h"

PadlockComponent::PadlockComponent(StageObject* myObj)
	:StageObjectComponent(myObj,"PadlockComponent")
{
}

void PadlockComponent::Initialize()
{
	myObject->AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
}

void PadlockComponent::Update()
{
}

json PadlockComponent::Save()
{
    json data;
    data["type"] = COMP_PADLOCK;
    // ‘¼‚Ìƒf[ƒ^‚à•Û‘¶‚·‚éê‡‚Í‚±‚±‚É’Ç‰Á
    return data;
}

void PadlockComponent::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "Player") {
        ((Player*)myObject->FindObject("Player"))->LostKey();

    }
}
