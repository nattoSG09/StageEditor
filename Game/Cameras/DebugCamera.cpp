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
	// �J�����̏œ_�E�ʒu���i�[����ϐ���p��
	XMFLOAT3 camTarget{};
	XMFLOAT3 camPosition{};

	XMFLOAT3 center = transform_.position_;

    if (!Input::IsKey(DIK_LCONTROL)) {
        if (setupFlag == true) return;
    }
	// �J�����̕��s�ړ�����
	{
        float angle = angle_.y;
        // ������ݒ�
        XMVECTOR dir{}; {
            // �����x�N�g�����擾
            XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

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
        Move(dir, speed);
	}


    // ��]�̂��߂̏����擾
    {

        XMFLOAT3 mouseMove = Input::GetMouseMove();
        angle_.x += mouseMove.y * sensitivity_;
        angle_.y += mouseMove.x * sensitivity_;
        center_To_camTargetDistance_ += mouseMove.z * 0.01f;
        // ������]�̏���E������ݒ肵��]�𐧌�
        {
            const float upperlimit = -80.f;
            if (angle_.x < upperlimit)angle_.x -= mouseMove.y * sensitivity_;
            const float lowerlimit = 80.f;
            if (angle_.x > lowerlimit)angle_.x -= mouseMove.y * sensitivity_;
        }
    }
    // �����̉�]���s��
    {
        // ��]�s����쐬
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));
        // ���̍s������ɉ�]
        XMVECTOR center_To_camTarget = { 0,0,1,0 };
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);
        // ������������
        center_To_camTarget *= center_To_camTargetDistance_;
        // ���_����̈ʒu�����߂āA�J�����̏œ_��ݒ�
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);
        // center_To_camTarget�̋t�x�N�g����p��
        XMVECTOR center_To_camPosition = -center_To_camTarget;
        // ������Ɖ�]������
        float littleAngle = -30.f;
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(littleAngle)));
        // ���_����̈ʒu�����߂āA�J�����̈ʒu��ݒ�
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&camPosition, origin_To_camPosition);
    }
    // �����̉�]���s��
    {
        // ���S���ړ�
        XMVECTOR newCenter = (XMLoadFloat3(&camPosition) + XMLoadFloat3(&camTarget)) * 0.5f;
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);
        // �c��]�̎����쐬
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);
        //// ��]�s����쐬
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis, XMConvertToRadians(angle_.x));
        //�@�œ_��ݒ� 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&camTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);
        // �ʒu��ݒ�
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
