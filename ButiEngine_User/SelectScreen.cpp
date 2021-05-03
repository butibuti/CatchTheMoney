#include "stdafx_u.h"
#include "SelectScreen.h"

void ButiEngine::SelectScreen::OnUpdate()
{
	if (!isAnimation) return;

	gameObject.lock()->transform->RollLocalRotationY_Degrees(rotationY);

	if (rotationY > 0)
	{
		rotationY -= subRotationY;
	}
}

void ButiEngine::SelectScreen::OnSet()
{
}

void ButiEngine::SelectScreen::Start()
{
	isAnimation = false;
	rotationY = 40.0f;
	subRotationY = rotationY / 60.0f;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectScreen::Clone()
{
	return ObjectFactory::Create<SelectScreen>();
}
