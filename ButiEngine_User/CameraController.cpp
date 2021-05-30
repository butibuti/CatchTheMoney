#include "stdafx_u.h"
#include "CameraController.h"
#include"GameSettings.h"
#include"ScrollManager.h"

void ButiEngine::CameraController::OnUpdate()
{
    if (gameObject.lock()->GetGameComponent<TransformAnimation>())
    {
        animation = true;
    }
    else
    {
        animation = false;
        if (isTitleZoomIn) {
            isTitleZoomIn = false;

            auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
            anim->SetTargetTransform(gameObject.lock()->transform->Clone());
            anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, initCameraZ));

            anim->SetSpeed(1.0f / titleZoomOutFrame);
            anim->SetEaseType(Easing::EasingType::EaseOutQuart);
        }
    }
}

void ButiEngine::CameraController::OnSet()
{
}

void ButiEngine::CameraController::Start()
{
    zoomInFrame = 20.0f;
    zoomOutFrame = 10.0f;
    titleZoomInFrame = 2.0f;
    titleZoomOutFrame = 75.0f-titleZoomInFrame;
    moveLength = 100.0f;
    initCameraZ = -1000.0f;
    animation = false;
    isTitleZoomIn = false;
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

void ButiEngine::CameraController::FrogZoomIn()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(200, 100, -500));
        anim->SetSpeed(1.0f / 40.0f);
        anim->SetEaseType(Easing::EasingType::EaseOutQuart);
    }
}

void ButiEngine::CameraController::FrogZoomOut()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(0, 0, initCameraZ));
        anim->SetSpeed(1.0f / 40.0f);
        anim->SetEaseType(Easing::EasingType::EaseOutQuart);
    }
}

void ButiEngine::CameraController::TitleZoomOut()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        isTitleZoomIn = true;
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->TranslateZ(50);
        anim->GetTargetTransform()->TranslateY(-20);
        anim->SetSpeed(1.0f / titleZoomInFrame);
        anim->SetEaseType(Easing::EasingType::Liner);
    }
}

void ButiEngine::CameraController::MobiusZoomIn(const Vector3& position,const float arg_frame)
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    Vector3 pos = position;
    if (!anim)
    {
        auto scrollManager = GetManager().lock()->GetGameObject("Screen").lock();
        auto scroll = scrollManager->GetGameComponent<ScrollManager>();
        auto currentScroll= scroll->GetCurrentScroll()* GameSettings::windowWidth;

        auto yRatio = ((pos.y + GameSettings::windowHeight * 0.5f) / (float)GameSettings::windowHeight)*2.0f;
        auto xRatio =- (( pos.x- currentScroll)) / (float)GameSettings::windowWidth;
        yRatio -= 1.0f;
        auto mobiusPosposition = MathHelper::GetMobiusPoint(MathHelper::ToRadian(xRatio*360), yRatio) * 125.0f;

        Vector3 position;

        auto normal= MathHelper::GetMobiusNormal(MathHelper::ToRadian(xRatio * 360), yRatio) * -400 * scrollManager->transform->GetWorldRotation();
        if (normal.z > 0) {
            normal *= -1;
        }
        position = ((mobiusPosposition)*  scrollManager ->transform->GetMatrix())+normal;



        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(Vector3(position.x, position.y,position.z));
        anim->GetTargetTransform()->SetLookAtRotation(mobiusPosposition* scrollManager->transform->GetMatrix());
        anim->GetTargetTransform()->TranslateY(-100);
        anim->SetSpeed(1.0f / arg_frame);
        anim->SetEaseType(Easing::EasingType::EaseOutQuart);

        auto daikokutenHead = GetManager().lock()->GetGameObject("Daikokuten").lock();
        auto daikokutenHand_L = GetManager().lock()->GetGameObject("DaikokutenHand_L").lock();
        auto daikokutenHand_R = GetManager().lock()->GetGameObject("DaikokutenHand_R").lock();
        daikokutenHead->transform->SetLocalPostionX(10000000000000);
        daikokutenHand_L->transform->SetLocalPostionX(10000000000000);
        daikokutenHand_R->transform->SetLocalPostionX(10000000000000);
    }
}
