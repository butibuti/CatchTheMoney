#include "stdafx_u.h"
#include "ClearBand.h"

void ButiEngine::ClearBand::OnUpdate()
{ 

}

void ButiEngine::ClearBand::OnSet()
{
}

void ButiEngine::ClearBand::Start()
{
	auto transform = gameObject.lock()->transform;
	transform->SetLocalPosition(Vector3(-14.5f,0,-979.0f));
	transform->SetLocalScale(Vector3(72.0f, 5.5f, 1.0f));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearBand::Clone()
{
	return ObjectFactory::Create<ClearBand>();
}