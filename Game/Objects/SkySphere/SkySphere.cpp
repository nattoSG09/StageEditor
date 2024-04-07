#include "SkySphere.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/DirectX/Direct3D.h"

SkySphere::SkySphere(GameObject* parent)
	:GameObject(parent,"SkySphere"),modelHandle_(-1)
{
}

void SkySphere::Initialize()
{
	modelHandle_ = Model::Load("Model/SkySphere/SkySphere.fbx");
	assert(modelHandle_ >= 0);
}

void SkySphere::Update()
{
}

void SkySphere::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_SKY);
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
	Direct3D::SetShader(Direct3D::SHADER_3D);

}

void SkySphere::Release()
{
}
