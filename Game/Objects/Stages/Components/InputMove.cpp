#include "InputMove.h"
#include "../../../../Engine/DirectX/Input.h"
#include "../../../../Engine/GameObject/Camera.h"

InputMove::InputMove(StageObject* myObj)
    :StageObjectComponent(myObj,"InputMove")
{
}

void InputMove::Initialize()
{
}

void InputMove::Update()
{
    // ������ݒ�
    XMVECTOR dir{}; {
        // �����x�N�g�����擾
        XMVECTOR sightline = XMVector3Normalize(XMVectorSetY(Camera::GetSightline(),0));

        // �O���Ɉړ�
        if (Input::IsKey(DIK_W)) {
            dir += sightline;
        }
        // ����Ɉړ�
        if (Input::IsKey(DIK_S)) {
            dir += -sightline;
        }

        // �����Ɉړ�
        if (Input::IsKey(DIK_A)) {
            dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90)));
        }

        // �E���Ɉړ�
        if (Input::IsKey(DIK_D)) {
            dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90)));
        }

        // ���K������
        dir = XMVector3Normalize(dir);
    }

    // ���x��ݒ�
    float speed = 0.1f;

    // �ړ�
    myObject->Move(dir, speed);
}

json InputMove::Save()
{
    json data;
    data["type"] = COMP_INPUTMOVE;
    // ���̃f�[�^���ۑ�����ꍇ�͂����ɒǉ�
    return data;
}
