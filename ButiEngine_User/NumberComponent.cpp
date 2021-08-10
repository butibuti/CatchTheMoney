#include "stdafx_u.h"
#include "NumberComponent.h"
#include "ChildNumber.h"

void ButiEngine::NumberComponent::OnUpdate()
{
	CreateChildNum();
	SetChildNum();
}

void ButiEngine::NumberComponent::OnSet()
{
}

void ButiEngine::NumberComponent::Start()
{
	isOnce = false;
	digit = 1;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::NumberComponent::Clone()
{
	return ObjectFactory::Create<NumberComponent>();
}

void ButiEngine::NumberComponent::SetNumber(int arg_num)
{
	number = arg_num;
}

void ButiEngine::NumberComponent::SetDigit(int arg_digit)
{
	digit = arg_digit;
}

//“n‚³‚ê‚½’l‚ÌŒ…”‚ð•Ô‚·ŠÖ”
unsigned int ButiEngine::NumberComponent::GetDigit(unsigned int arg_number)
{
	unsigned int _digit = 0;
	if (arg_number == 0) _digit = 1;
	while (arg_number != 0) {
		arg_number /= 10;
		_digit++;
	}
	return _digit;
}

void ButiEngine::NumberComponent::CreateChildNum()
{
	if (isOnce) return;
	isOnce = true;

	auto parentTransform = gameObject.lock()->transform;

	digit = GetDigit(number);
	for (unsigned int i = 0; i < digit; i++)
	{
		childNumbers.push_back(GetManager().lock()->AddObjectFromCereal("ChildNumber", ObjectFactory::Create<Transform>(Vector3::Zero, Vector3::Zero, Vector3(1,1,1))));
	}

	auto end = childNumbers.end();
	for (auto itr = childNumbers.begin(); itr != end; ++itr)
	{
		(*itr).lock()->transform->SetBaseTransform(parentTransform);
	}
}

void ButiEngine::NumberComponent::SetChildNum()
{
	int calcDigit = digit - 1;
	int multiplier = pow(10, calcDigit);
	int calcNumber = number;

	auto position = gameObject.lock()->transform->GetLocalPosition();
	auto childCount = childNumbers.size();
	for (int i = 0; i < childCount; ++i)
	{
		int setNum = calcNumber / multiplier;
		childNumbers[i].lock()->GetGameComponent<ChildNumber>()->SetNumber(setNum);
		childNumbers[i].lock()->transform->SetLocalPosition(Vector3(i, 0, 0));
		calcNumber = calcNumber % multiplier;
		multiplier /= 10;
	}

	//auto end = childNumbers.end();
	//for (auto itr = childNumbers.begin(); itr != end; ++itr)
	//{
	//	(*itr)->GetGameComponent<ChildNumber>()->SetNumber(0);
	//}
}
