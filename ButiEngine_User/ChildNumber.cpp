#include "stdafx_u.h"
#include "ChildNumber.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::ChildNumber::OnUpdate()
{
	if (number == previousNumber) return;
	previousNumber = number;

	shp_spriteAnimation->SetHorizontalAnim(number);

}

void ButiEngine::ChildNumber::OnSet()
{
}

void ButiEngine::ChildNumber::Start()
{
	previousNumber = 0;
	number = 0;

	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ChildNumber::Clone()
{
	return ObjectFactory::Create<ChildNumber>();
}

void ButiEngine::ChildNumber::SetNumber(int arg_number)
{
	number = arg_number;
}
