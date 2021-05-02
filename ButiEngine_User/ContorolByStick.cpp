#include "stdafx_u.h"
#include "ContorolByStick.h"
#include"InputManager.h"

void ButiEngine::ContorolByStick::OnUpdate()
{
	if (cameraResetFrame < 10)
	{
		cameraResetFrame++;
		return;
	}

	Control();

	//カメラリセット
	if (InputManager::OnTriggerMobiusRotateResetKey())
	{
		cameraResetFrame = 0;
		rotation = Vector3(0, 0, 0);
		auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform(gameObject.lock()->transform->Clone());
			anim->GetTargetTransform()->SetLocalRotation(rotation);
			anim->SetSpeed(0.05f);
			anim->SetEaseType(Easing::EasingType::EaseOutElastic);
		}
	}

}

void ButiEngine::ContorolByStick::OnSet()
{
}

void ButiEngine::ContorolByStick::Start()
{
	cameraResetFrame = 0;
	rotationLimit = 60.0f;
	rotationSpeed = 2.0f;
	returnSpeed = 1.0f;
	initAxis = -gameObject.lock()->transform->GetUp();
	rotation = Vector3(0, 0, 0);
	currMousePos = Vector2(0, 0);
	preMousePos = Vector2(0, 0);
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

void ButiEngine::ContorolByStick::Control()
{
	auto GetRightStick = GameDevice::GetInput()->GetRightStick();
	auto transform = gameObject.lock()->transform;
	bool isChanged = false;

	preMousePos = currMousePos;
	currMousePos = GameDevice::GetInput()->GetMousePos();

	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::LeftClick))
	{
		isChanged = true;
		float moveRotation = (preMousePos.x - currMousePos.x) / 5.0f;
		rotation.y += moveRotation;

		if (rotation.y > rotationLimit)
		{
			rotation.y = rotationLimit;
		}
		else if (rotation.y < -rotationLimit)
		{
			rotation.y = -rotationLimit;
		}

		moveRotation = (preMousePos.y - currMousePos.y) / 5.0f;
		rotation.x += moveRotation;

		if (rotation.x > rotationLimit)
		{
			rotation.x = rotationLimit;
		}
		else if (rotation.x < -rotationLimit)
		{
			rotation.x = -rotationLimit;
		}

		if (isChanged)
		{
			transform->SetLocalRotation(rotation);
		}
		return;
	}

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

	if (isChanged)
	{
		transform->SetLocalRotation(rotation);
	}
}
