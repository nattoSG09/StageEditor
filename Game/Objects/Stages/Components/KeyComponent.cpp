#include "KeyComponent.h"

KeyComponent::KeyComponent(StageObject* myObj)
	:StageObjectComponent(myObj, "KeyComponent")
{
}

void KeyComponent::Initialize()
{
    
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
