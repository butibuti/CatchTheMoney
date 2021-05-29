#include "stdafx_u.h"
#include "ContorolByStick.h"
#include"InputManager.h"
#include"PauseManager.h"
#include "GameSettings.h"

void ButiEngine::ContorolByStick::OnUpdate()
{
	if (shp_pauseManager->IsPause()) { return; }

	bool isCameraResetFrame = cameraResetFrame < 10;
	if (isCameraResetFrame || GameSettings::isTitle)
	{
		cameraResetFrame++;
		return;
	}

	//メビウスぐりぐり
	Control();

	//カメラリセット
	//Reset();

}

void ButiEngine::ContorolByStick::OnSet()
{
}

void ButiEngine::ContorolByStick::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	cameraResetFrame = 0;
	rotationLimit = 60.0f;
	rotationSpeed = 2.0f;
	returnSpeed = 1.0f;
	initAxis = -gameObject.lock()->transform->GetUp();
	initRotate = Vector3(-25, -25, 0);
	curRotation = initRotate;
	preRotation = initRotate;
	rotation = initRotate;
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

	//マウスでぐりぐり
	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::LeftClick))
	{
		isChanged = true;
		float moveRotation = (preMousePos.x - currMousePos.x) / 5.0f;
		curRotation.y += moveRotation;

		if (curRotation.y > rotationLimit)
		{
			curRotation.y = rotationLimit;
		}
		else if (curRotation.y < -rotationLimit)
		{
			curRotation.y = -rotationLimit;
		}

		moveRotation = (preMousePos.y - currMousePos.y) / 5.0f;
		curRotation.x += moveRotation;

		if (curRotation.x > rotationLimit)
		{
			curRotation.x = rotationLimit;
		}
		else if (curRotation.x < -rotationLimit)
		{
			curRotation.x = -rotationLimit;
		}

		preRotation = curRotation;

		transform->SetLocalRotation(preRotation);

		return;
	}

	//コントローラーでぐりぐり
	if ((GetRightStick.x >= deadZone || GetRightStick.x <= -deadZone) || 
		(GetRightStick.y >= deadZone || GetRightStick.y <= -deadZone))
	{
		isChanged = true;
		//float moveRotation = -GetRightStick.x * rotationSpeed;
		//rotation.y += moveRotation;

		curRotation.y = -GetRightStick.x * rotationLimit;

		if (curRotation.y > rotationLimit)
		{
			curRotation.y = rotationLimit;
		}
		else if (curRotation.y < -rotationLimit)
		{
			curRotation.y = -rotationLimit;
		}

	}
	if ((GetRightStick.x >= deadZone || GetRightStick.x <= -deadZone) ||
		(GetRightStick.y >= deadZone || GetRightStick.y <= -deadZone))
	{
		isChanged = true;

		//float moveRotation = GetRightStick.y * rotationSpeed;
		//rotation.x += moveRotation;

		curRotation.x = GetRightStick.y * rotationLimit;

		if (curRotation.x > rotationLimit)
		{
			curRotation.x = rotationLimit;
		}
		else if (curRotation.x < -rotationLimit)
		{
			curRotation.x = -rotationLimit;
		}
	}

	const float LERP_SCALE = 0.2f;
	preRotation.y = preRotation.y * (1.0f - LERP_SCALE) + curRotation.y * LERP_SCALE;
	preRotation.x = preRotation.x * (1.0f - LERP_SCALE) + curRotation.x * LERP_SCALE;

	if (!isChanged)
	{
		curRotation = initRotate;
	}
	transform->SetLocalRotation(preRotation);
}

void ButiEngine::ContorolByStick::Reset()
{
	if (InputManager::OnTriggerMobiusResetRotationKey())
	{
		cameraResetFrame = 0;
		rotation = initRotate;
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
