#include "stdafx_u.h"
#include "CorrectToInt.h"

void ButiEngine::CorrectToInt::OnUpdate()
{
}

void ButiEngine::CorrectToInt::OnSet()
{
}

void ButiEngine::CorrectToInt::Start()
{
}

void ButiEngine::CorrectToInt::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::CorrectToInt::Clone()
{
	return ObjectFactory::Create<CorrectToInt>();
}
