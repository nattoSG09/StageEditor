#include "TestMove.h"
#include "../StageObject.h"

TestMove::TestMove(StageObject* myObj)
	:StageObjectComponent(myObj,"TestMove")
{
}

void TestMove::Initialize()
{
}

void TestMove::Update()
{
	angle++;
	myObject->SetRotateY(angle);
}

json TestMove::Save()
{
	json data;
	data["type"] = COMP_TESTMOVE;
	// ‘¼‚Ìƒf[ƒ^‚à•Û‘¶‚·‚éê‡‚Í‚±‚±‚É’Ç‰Á
	return data;
}
