#include "stdafx_u.h"
#include "CameraController.h"

void ButiEngine::CameraController::OnUpdate()
{
    if (gameObject.lock()->GetGameComponent<TransformAnimation>())
    {
        animation = true;
    }
    else
    {
        animation = false;
    }
}

void ButiEngine::CameraController::OnSet()
{
}

void ButiEngine::CameraController::Start()
{
    zoomInFrame = 20.0f;
    zoomOutFrame = 10.0f;
    titleZoomOutFrame = 75.0f;
    moveLength = 100.0f;
    initCameraZ = -1000.0f;
    animation = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::CameraController::Clone()
{
    return ObjectFactory::Create<CameraController>();
}

void ButiEngine::CameraController::OnShowUI()
{
    GUI::SliderFloat("zoomInFrame", &zoomInFrame, 1.0f, 120.0f);
    GUI::SliderFloat("zoomOutFrame", &zoomOutFrame, 1.0f, 120.0f);
    GUI::SliderFloat("length", &moveLength, 50.0f, 300.0f);
}

void ButiEngine::CameraController::ZoomIn()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, initCameraZ));
        anim->SetSpeed(1.0f / zoomInFrame);
        anim->SetEaseType(Easing::EasingType::EaseOutQuart);
    }
}

void ButiEngine::CameraController::ZoomOut()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim) 
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, initCameraZ - moveLength));
        anim->SetSpeed(1.0f / zoomOutFrame);
        anim->SetEaseType(Easing::EasingType::EaseInQuad);
    }
}

void ButiEngine::CameraController::TitleZoomOut()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, initCameraZ));
        anim->SetSpeed(1.0f / titleZoomOutFrame);
        anim->SetEaseType(Easing::EasingType::EaseOutQuart);
    }
}
