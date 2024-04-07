#include "DebugCamera.h"
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/GameObject/Camera.h"
#include "../../Engine/ImGui/imgui.h"

namespace {
    const float DEF_SENSITIVITY = 0.3f;
    const float SENSITIVITY_MAX = 1;
    const float SENSITIVITY_MIN = 0;
    const float DEF_DISTANCE = 7.f;
}

DebugCamera::DebugCamera(GameObject* parent)
	:GameObject(parent,"DebugCamera"), sensitivity_(DEF_SENSITIVITY), center_To_camTargetDistance_(DEF_DISTANCE)
{
}

void DebugCamera::Initialize()
{
}

void DebugCamera::Update()
{
    static bool setupFlag = false;
	// カメラの焦点・位置を格納する変数を用意
	XMFLOAT3 camTarget{};
	XMFLOAT3 camPosition{};

	XMFLOAT3 center = transform_.position_;

    if (!Input::IsKey(DIK_LCONTROL)) {
        if (setupFlag == true) return;
    }
	// カメラの平行移動処理
	{
        float angle = angle_.y;
        // 方向を設定
        XMVECTOR dir{}; {
            // 視線ベクトルを取得
            XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

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
        Move(dir, speed);
	}


    // 回転のための情報を取得
    {

        XMFLOAT3 mouseMove = Input::GetMouseMove();
        angle_.x += mouseMove.y * sensitivity_;
        angle_.y += mouseMove.x * sensitivity_;
        center_To_camTargetDistance_ += mouseMove.z * 0.01f;
        // ｘ軸回転の上限・下限を設定し回転を制限
        {
            const float upperlimit = -80.f;
            if (angle_.x < upperlimit)angle_.x -= mouseMove.y * sensitivity_;
            const float lowerlimit = 80.f;
            if (angle_.x > lowerlimit)angle_.x -= mouseMove.y * sensitivity_;
        }
    }
    // ｙ軸の回転を行う
    {
        // 回転行列を作成
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));
        // ↑の行列を元に回転
        XMVECTOR center_To_camTarget = { 0,0,1,0 };
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);
        // 長さを加える
        center_To_camTarget *= center_To_camTargetDistance_;
        // 原点からの位置を求めて、カメラの焦点を設定
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);
        // center_To_camTargetの逆ベクトルを用意
        XMVECTOR center_To_camPosition = -center_To_camTarget;
        // ちょっと回転させる
        float littleAngle = -30.f;
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(littleAngle)));
        // 原点からの位置を求めて、カメラの位置を設定
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&camPosition, origin_To_camPosition);
    }
    // ｘ軸の回転を行う
    {
        // 中心を移動
        XMVECTOR newCenter = (XMLoadFloat3(&camPosition) + XMLoadFloat3(&camTarget)) * 0.5f;
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);
        // 縦回転の軸を作成
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);
        //// 回転行列を作成
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis, XMConvertToRadians(angle_.x));
        //　焦点を設定 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&camTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);
        // 位置を設定
        XMVECTOR newCenter_To_camPosition = -newCenter_To_camTarget;
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + newCenter_To_camPosition;
        XMStoreFloat3(&camPosition, origin_To_camPosition);
    }

    Camera::SetTarget(center);
    Camera::SetPosition(camPosition);
    setupFlag = true;
}

void DebugCamera::Draw()
{
}

void DebugCamera::Release()
{
}

void DebugCamera::Move(XMVECTOR dir, float speed)
{
    XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + (dir * speed));
}
