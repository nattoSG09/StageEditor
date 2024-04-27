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
    // 他のデータも保存する場合はここに追加
    return data;
}

void KeyComponent::OnCollision(GameObject* pTarget)
{
    // プレイヤーが`myObject_`に触れたら...
    if (pTarget->GetObjectName() == "Player") {
        
        // プレイヤーは`key`を取得
        ((Player*)myObject->FindObject("Player"))->GetKey();

        // ステージ上から`myObject_`を削除
        ((Stage*)myObject->FindObject("Stage"))->Delete(myObject);
    }
}
