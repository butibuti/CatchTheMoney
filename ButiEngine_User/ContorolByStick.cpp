#include "stdafx_u.h"
#include "ContorolByStick.h"

void ButiEngine::ContorolByStick::OnUpdate()
{
	auto GetRightStick = GameDevice::GetInput()->GetRightStick();
	auto transform = gameObject.lock()->transform;
	bool isChanged = false;

	if (GetRightStick.x != 0)
	{
		isChanged = true;
		float moveRotation = -GetRightStick.x * rotationSpeed;
		rotation.y += moveRotation;

		if (rotation.y > rotationLimit)
		{
			rotation.y = rotationLimit;
		}
		else if (rotation.y < -rotationLimit)
		{
			rotation.y = -rotationLimit;
		}

	}
	if (GetRightStick.y != 0)
	{
		isChanged = true;

		float moveRotation = GetRightStick.y * rotationSpeed;
		rotation.x += moveRotation;

		if (rotation.x > rotationLimit)
		{
			rotation.x = rotationLimit;
		}
		else if (rotation.x < -rotationLimit)
		{
			rotation.x = -rotationLimit;
		}

	}

	//カメラリセット
	if (GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_STICK_RIGHT))
	{
		isChanged = true;
		rotation = Vector3(0, 0, 0);
	}

	if (isChanged)
	{
		transform->SetLocalRotation(rotation);
	}

}

void ButiEngine::ContorolByStick::OnSet()
{
}

void ButiEngine::ContorolByStick::Start()
{
	rotationLimit = 60.0f;
	rotationSpeed = 1.0f;
	returnSpeed = 1.0f;
	initAxis = -gameObject.lock()->transform->GetUp();
	rotation = Vector3(0, 0, 0);
}

void ButiEngine::ContorolByStick::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::ContorolByStick::OnShowUI()
{
	GUI::DragFloat("rotationSpeed", rotationSpeed, 0.01f, 0.0f, 10.0f);
	GUI::DragFloat("retunSpeed", returnSpeed, 0.01f, 0.0f, 10.0f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ContorolByStick::Clone()
{
	return ObjectFactory::Create<ContorolByStick>();
}
