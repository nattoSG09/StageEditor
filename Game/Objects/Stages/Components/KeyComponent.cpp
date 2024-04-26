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
    // ëºÇÃÉfÅ[É^Ç‡ï€ë∂Ç∑ÇÈèÍçáÇÕÇ±Ç±Ç…í«â¡
    return data;
}
