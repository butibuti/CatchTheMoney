#include "stdafx_u.h"
#include "ShakeComponent.h"

void ButiEngine::ShakeComponent::OnUpdate()
{
	if (!start)
	{
		return;
	}

	float x = ButiRandom::GetRandom(-amplitude, amplitude, 100);
	float y = ButiRandom::GetRandom(-amplitude, amplitude, 100);
	float z = 0;

	if (amplitude > 0)
	{
		amplitude -= subAmplitude;
	}
	else
	{
		ShakeStop();
	}

	Vector3 newRotate = defaultRotate + Vector3(x, y, z);

	gameObject.lock()->transform->SetLocalRotation(newRotate);
}

void ButiEngine::ShakeComponent::OnSet()
{
}

void ButiEngine::ShakeComponent::Start()
{
	start = false;
	amplitude = 0.0f;
	subAmplitude = 0.0f;
}

void ButiEngine::ShakeComponent::ShakeStart(float arg_amplitude)
{
	amplitude = arg_amplitude;
	subAmplitude = amplitude / 8.0f;
	start = true;
	defaultRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
}

void ButiEngine::ShakeComponent::ShakeStart(float arg_amplitude, int arg_frame)
{
	amplitude = arg_amplitude;
	subAmplitude = amplitude / (float)arg_frame;
	start = true;
	defaultRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
}

void ButiEngine::ShakeComponent::ShakeStop()
{
	gameObject.lock()->transform->SetLocalRotation(defaultRotate);
	amplitude = 0.0f;
	subAmplitude = 0.0f;
	start = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}
