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
    // 方向を設定
    XMVECTOR dir{}; {
        // 視線ベクトルを取得
        XMVECTOR sightline = XMVector3Normalize(XMVectorSetY(Camera::GetSightline(),0));

        // 前方に移動
        if (Input::IsKey(DIK_W)) {
            dir += sightline;
        }
        // 後方に移動
        if (Input::IsKey(DIK_S)) {
            dir += -sightline;
        }

        // 左方に移動
        if (Input::IsKey(DIK_A)) {
            dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90)));
        }

        // 右方に移動
        if (Input::IsKey(DIK_D)) {
            dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90)));
        }

        // 正規化して
        dir = XMVector3Normalize(dir);
    }

    // 速度を設定
    float speed = 0.1f;

    // 移動
    myObject->Move(dir, speed);
}

json InputMove::Save()
{
    json data;
    data["type"] = COMP_INPUTMOVE;
    // 他のデータも保存する場合はここに追加
    return data;
}
