#include "Stage.h"

#include "StageObject.h"
#include "Components/TestMove.h"
#include "Components/InputMove.h"
#include "Components/Gravity.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/Json/JsonReader.h"
#include "../../../Engine/DirectX/Input.h"

namespace {
	// ��������̑S�Ă� "\\" �� "/" �ɒu������֐�
	void ReplaceBackslashes(string& str) {
		size_t found = str.find("\\");
		while (found != string::npos) {
			str.replace(found, 1, "/"); // "\\" �� "/" �ɒu������
			found = str.find("\\", found + 1); // ���� "\\" ����������
		}
	}

	// �J�����g�f�B���N�g������̑��΃p�X���擾����֐�
	std::string GetAssetsRelativePath(const std::string& absolutePath) {
		const std::string assetsFolder = "Assets\\";
		// "Assets\\" �̈ʒu������
		size_t assetsPos = absolutePath.find(assetsFolder);
		if (assetsPos != std::string::npos) {
			// "Assets\\" �̕������������A����ȍ~�̕������擾
			std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
			return relativePath;
		}
		else {
			// "Assets\\" ��������Ȃ��ꍇ�́A���̃t�@�C���p�X��Ԃ�
			return absolutePath;
		}
	}

}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),objects_{},selectedIndex_(-1)
{
}

void Stage::Initialize()
{
#ifndef _DEBUG
	this->Load("Data/TestStage.json");
#endif // !_DEBUG
}

void Stage::Update()
{
#ifdef _DEBUG
	// ���[���h�A�E�g���C�i�[��\��
	DrawWorldOutliner();

	// �ڍ׃E�B���h�E��\��
	DrawDatails();
	
	//debug�e�L�X�g�E�B���h�E��\��
	DrawDebugTexts();

#endif // _DEBUG
}

void Stage::Draw()
{
}

void Stage::Release()
{
}

void Stage::DrawWorldOutliner()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �E�B���h�E��\��
	ImGui::Begin("World Outliner", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// - ActionButtons ------------------------------------------ 
		ImGui::TextWrapped("StageObjects Edit");
		ImGui::SameLine();

		// �ǉ�����{�^����\��
		if (ImGui::Button("Add")) {
			// �ǉ�����I�u�W�F�N�g�̏�������ݒ�
			string name = "object" + std::to_string(objects_.size() + 1);

			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			string filePath{}; {
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �쐬�𒆒f
					ImGui::End();
					return;
				}
			}

			// �I�u�W�F�N�g���쐬
			StageObject* obj = CreateStageObject(name, filePath, this);

			// �I�u�W�F�N�g�̃��X�g�ɒǉ�
			this->Add(obj);
		}

		// �ۑ�����{�^����\��
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ۑ���̃t�@�C���p�X���擾
			string filePath{}; {
				// �u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
					char fileName[MAX_PATH] = "����.json";
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_OVERWRITEPROMPT;
					ofn.lpstrDefExt = "json";
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}
				
				// �t�@�C����ۑ�����_�C�A���O�̕\��
				if (GetSaveFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �ۑ��𒆒f
					ImGui::End();
					return;
				}
			}

			// �t�@�C���ɃX�e�[�W����ۑ�
			this->Save(filePath);
		}

		// �Ǎ�����{�^����\��
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǂݍ��ރt�@�C���̃p�X���擾
			string filePath{}; {
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("JSON�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �쐬�𒆒f
					ImGui::End();
					return;
				}
			}

			// �t�@�C����ǂݍ��݃X�e�[�W�𐶐�
			this->Load(filePath);
		}
		
		// �폜����{�^����\��
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			AllDelete();
		}

		// ��؂�ׁ̈A���C����\��
		ImGui::Separator();

		// - ObjectList ---------------------------------------------
		if (ImGui::TreeNodeEx("ObjectList", ImGuiTreeNodeFlags_DefaultOpen)) {
			// ���X�g��\��
			for (int i = 0; i < objects_.size(); ++i) 
				if (ImGui::Selectable(objects_[i]->GetObjectName().c_str(), selectedIndex_ == i)) {
					selectedIndex_ = i;
				}
			ImGui::TreePop();
		}

		// ----------------------------------------------------------
	}
	ImGui::End();
}

void Stage::DrawDatails()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �E�B���h�E��\��
	ImGui::Begin("Datails", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// �I������Ă���I�u�W�F�N�g�̏ڍׂ�\��
		if (selectedIndex_ >= 0 && selectedIndex_ < objects_.size()) {

			// - name -------------------------------------------------------
			if (ImGui::TreeNode("name")) {
				ImGui::Text("Current name : %s", objects_[selectedIndex_]->objectName_.c_str());
				char buffer[256] = "";
				if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
					objects_[selectedIndex_]->objectName_ = buffer;
				ImGui::TreePop();
			}

			// - transform --------------------------------------------------
			if (ImGui::TreeNode("Transform")) {
				if (ImGui::TreeNode("position_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.position_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.position_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.position_.z);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("rotate_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.rotate_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.rotate_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.rotate_.z);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("scale_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.scale_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.scale_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.scale_.z);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			// - component --------------------------------------------------
			if (ImGui::TreeNode("Component")) {
				static bool showComponentWindow = false;

				if (ImGui::Button("AddComponent")) {
					showComponentWindow = true;
				}

				// �ǉ��ł���R���|�[�l���g���X�g�̃E�B���h�E
				if (showComponentWindow) {
					ImGui::Begin("Component List", &showComponentWindow);

					// �R���|�[�l���g���X�g��\�����A�I�����ꂽ�R���|�[�l���g����������
					for (int i = 0; i < COMP_MAX; i++) {
						if (ImGui::Selectable(ComponentTypeToString((ComponentType)i).c_str(), selectedComponentType_ == i)) {
							selectedComponentType_ = (ComponentType)i;

							switch (selectedComponentType_)
							{
							case COMP_TESTMOVE:objects_[selectedIndex_]->AddComponent(new TestMove(objects_[selectedIndex_])); break;
							case COMP_INPUTMOVE:objects_[selectedIndex_]->AddComponent(new InputMove(objects_[selectedIndex_])); break;
							case COMP_GRAVITY:objects_[selectedIndex_]->AddComponent(new Gravity(objects_[selectedIndex_]));break;
							case COMP_MAX:
								break;
							}

							showComponentWindow = false;
						}
					}

					ImGui::End();
				}


				if (ImGui::TreeNode("ComponentList")) {
					for (auto comp : objects_[selectedIndex_]->components_)
					{
						if (ImGui::TreeNode(comp->name_.c_str())) {

							comp->DrawDatails();

							if (ImGui::Button("delete")) {
								objects_[selectedIndex_]->DeleteComponent(comp);
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			// --------------------------------------------------------------
			ImGui::Separator();
			ImGui::Text("object delete"); ImGui::SameLine();
			if (ImGui::Button("delete"))Delete(objects_[selectedIndex_]);
		}
	}
	ImGui::End();
}

void Stage::DrawDebugTexts()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(0, Direct3D::screenHeight_ * 0.7f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.3f));
	
	// �E�B���h�E��\��
	ImGui::Begin("Debug", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

void Stage::Add(StageObject* obj)
{
	objects_.push_back(obj);
}

bool Stage::Save(string filePath)
{
	// �ۑ��f�[�^���쐬
	json saveObj; {
		for (auto obj : objects_) {
			saveObj[obj->objectName_]["position_"]["x"] = obj->transform_.position_.x;
			saveObj[obj->objectName_]["position_"]["y"] = obj->transform_.position_.y;
			saveObj[obj->objectName_]["position_"]["z"] = obj->transform_.position_.z;

			saveObj[obj->objectName_]["rotate_"]["x"] = obj->transform_.rotate_.x;
			saveObj[obj->objectName_]["rotate_"]["y"] = obj->transform_.rotate_.y;
			saveObj[obj->objectName_]["rotate_"]["z"] = obj->transform_.rotate_.z;

			saveObj[obj->objectName_]["scale_"]["x"] = obj->transform_.scale_.x;
			saveObj[obj->objectName_]["scale_"]["y"] = obj->transform_.scale_.y;
			saveObj[obj->objectName_]["scale_"]["z"] = obj->transform_.scale_.z;

			saveObj[obj->objectName_]["modelFilePath_"] = obj->modelFilePath_;

			// �R���|�[�l���g�̕ۑ�
			for (auto comp : obj->components_) {
				saveObj[obj->objectName_]["components"].push_back(comp->Save());
			}
		}
	}

	// �ۑ�
	return JsonReader::Save(filePath, saveObj);
}

bool Stage::Load(string filePath)
{
	// �i�[�p�I�u�W�F�N�g��p��
	json loadObj;

	// �t�@�C����ǂݍ���
	if (JsonReader::Load(filePath, loadObj) == false)return false;

	// �ۑ�����Ă��Ȃ����݂̃f�[�^���폜
	AllDelete();

	// ���[�h�����f�[�^����X�e�[�W�I�u�W�F�N�g�𕜌�����
	for (auto it = loadObj.begin(); it != loadObj.end(); ++it) {
		// �I�u�W�F�N�g���𕜌�
		string objectName = it.key();

		auto& data = it.value(); 
		// �t�@�C���p�X�𕜌�
		string modelFilePath{};
		if (data.contains("modelFilePath_"))modelFilePath = data["modelFilePath_"];

		// �ʒu�A��]�A�X�P�[���𕜌�
		Transform transform;
		if (data.contains("position_")) {
			transform.position_.x = data["position_"]["x"];
			transform.position_.y = data["position_"]["y"];
			transform.position_.z = data["position_"]["z"];
		}
		if (data.contains("rotate_")) {
			transform.rotate_.x = data["rotate_"]["x"];
			transform.rotate_.y = data["rotate_"]["y"];
			transform.rotate_.z = data["rotate_"]["z"];
		}
		if (data.contains("scale_")) {
			transform.scale_.x = data["scale_"]["x"];
			transform.scale_.y = data["scale_"]["y"];
			transform.scale_.z = data["scale_"]["z"];
		}


		// ���������f�[�^�����ɃI�u�W�F�N�g���쐬
		StageObject* obj = CreateStageObject(objectName, modelFilePath, this);
		obj->SetTransform(transform);

		// �R���|�\�l���g�̃��[�h
		for (auto compData : data["components"]) {
			ComponentType type = static_cast<ComponentType>(compData["type"]);
			StageObjectComponent* comp = nullptr;

			switch (type) {
			case COMP_TESTMOVE:
				comp = new TestMove(obj);
				break;
			case COMP_INPUTMOVE:
				comp = new InputMove(obj);
				break;
			case COMP_GRAVITY:
				comp = new Gravity(obj);

			default:
				// ���m�̃R���|�[�l���g�^�C�v
				break;
			}

			if (comp != nullptr) {
				obj->components_.push_back(comp);
			}
		}

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		this->Add(obj);
	}
	return this;
}

void Stage::Delete(StageObject* obj)
{
	// �I�u�W�F�N�g���폜����
	obj->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(objects_.begin(), objects_.end(), obj);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != objects_.end()) objects_.erase(it);
}

void Stage::AllDelete()
{
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}
