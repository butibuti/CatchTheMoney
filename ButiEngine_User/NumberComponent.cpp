#include "stdafx_u.h"
#include "NumberComponent.h"

void ButiEngine::NumberComponent::OnUpdate()
{

}

void ButiEngine::NumberComponent::OnSet()
{
}

void ButiEngine::NumberComponent::Start()
{

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::NumberComponent::Clone()
{
	return ObjectFactory::Create<NumberComponent>();
}

unsigned int ButiEngine::NumberComponent::GetDigit(unsigned int arg_number)
{
	unsigned int digit = 0;
	if (arg_number == 0) digit = 1;
	while (arg_number != 0) {
		arg_number /= 10;
		digit++;
	}
	return digit;
}
