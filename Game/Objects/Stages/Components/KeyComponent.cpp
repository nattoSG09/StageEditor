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
    // �v���C���[��`myObject_`�ɐG�ꂽ��...
    if (pTarget->GetObjectName() == "Player") {
        
        // �v���C���[��`key`���擾
        ((Player*)myObject->FindObject("Player"))->GetKey();

        // �X�e�[�W�ォ��`myObject_`���폜
        ((Stage*)myObject->FindObject("Stage"))->Delete(myObject);
    }
}
