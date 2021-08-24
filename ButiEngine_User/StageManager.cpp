#include"stdafx_u.h"
#include"StageManager.h"
#include"PauseManager.h"
#include"InputManager.h"
#include"PanelManager.h"
#include"ScrollManager.h"
#include"Map.h"
#include"CameraController.h"
#include"StageSelect.h"
#include"Player.h"
#include"ShakeComponent.h"
#include"NumberComponent.h"
#include"SquareParticleEmitter.h"
#include"GameSettings.h"
#include"SceneChangeAnimation.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"ClearButton.h"
#include"TalkText.h"
#include"Daikokuten.h"
#include"ParentDaikokuten.h"
#include"Frog.h"
#include "TutorialSelect.h"
#include"Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include"RotateButtonUI.h"

//#define OUTPUT_STAGERENDERTARGET
#ifdef DEBUG

static int f = 0;
#endif // DEBUG

ButiEngine::GameMode ButiEngine::StageManager::mode;

void ButiEngine::StageManager::OnUpdate()
{
	if (GameSettings::isTitle) { return; }
	StorePlayer();
	StoreFrog();

	//チュートリアル用のUI追加
	AddTutorialUI();

	//クリア時の演出用オブジェクト追加
	AddClearAnimation();

	//ステージ４のチュートリアル
	TutorialMode();

	//カエル初回出現時のアニメーション
	FrogEatAnimation();

	OnGoal();

	//Editモード・Charaモードの切り替え
	if (InputManager::OnTriggerModeChangeKey())
	{
		ModeChange();
	}

	ChangeUIAlpha();


	if (mode == GameMode::Edit) {
		auto swing = shp_scrollManager->GetCurrentScrollSwing()*360;

		shp_particleEmitter->SetRotation(particleScrollOffset+swing);
	}

#ifdef OUTPUT_STAGERENDERTARGET 
#ifdef DEBUG

	if (f==3) {
		const int stageNum = StageSelect::GetStageNum();

		std::string filePath = "Texture/StageSelect/stage_";


		if (stageNum < 10) {
			filePath += "0";
		}

		filePath += std::to_string(stageNum) + ".png";
		TextureTag output = TextureTag(":/wideScreen/1280/144");
		auto texture = GetManager().lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(output).lock();
		if (texture && !GameSettings::isTitle) {
			texture->ToPNG(GlobalSettings::GetResourceDirectory() + filePath);
		}
		f = 0;
	}
	if (f > 0) {
		f++;
	}
#endif // DEBUG
#endif // OUTPUT_STAGERENDERTARGET
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	fadeCount = 0;
	isNext = false;
	isOnce = false;
	isAddTutorial = false;
	isTutorialSelect = false;

	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_scrollManager = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>();
	shp_cameraController = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraController>();

	//UI生成
	CreateUI();

	if (!GameSettings::isTitle)
	{
		wkp_daikokutenHead = GetManager().lock()->GetGameObject("Daikokuten");
		wkp_daikokutenHandLeft = GetManager().lock()->GetGameObject("DaikokutenHand_L");
		wkp_daikokutenHandRight = GetManager().lock()->GetGameObject("DaikokutenHand_R");

		wkp_daikokutenStay = GetManager().lock()->GetGameObject("Daikokuten_Stay");
		wkp_daikokutenAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear");
		wkp_daikokutenReaction = GetManager().lock()->GetGameObject("Daikokuten_Reaction");
		wkp_daikokutenRHand = GetManager().lock()->GetGameObject("Daikokuten_Hand_R");
		wkp_daikokutenLHand = GetManager().lock()->GetGameObject("Daikokuten_Hand_L");
		wkp_daikokutenRAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear_R");
		wkp_daikokutenLAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear_L");

		wkp_daikokutenAppear.lock()->transform->SetBaseTransform(wkp_daikokutenReaction.lock()->transform);
		wkp_daikokutenStay.lock()->transform->SetBaseTransform(wkp_daikokutenAppear.lock()->transform);
		wkp_daikokutenHead.lock()->transform->SetBaseTransform(wkp_daikokutenStay.lock()->transform);
		wkp_daikokutenRHand.lock()->transform->SetBaseTransform(wkp_daikokutenRAppear.lock()->transform);
		wkp_daikokutenLHand.lock()->transform->SetBaseTransform(wkp_daikokutenLAppear.lock()->transform);
		wkp_daikokutenHandRight.lock()->transform->SetBaseTransform(wkp_daikokutenRHand.lock()->transform);
		wkp_daikokutenHandLeft.lock()->transform->SetBaseTransform(wkp_daikokutenLHand.lock()->transform);


		wkp_stageNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(Vector3(-550, 350, 0), Vector3::Zero, Vector3(80, 80, 0)));
		auto numComponent = wkp_stageNumber.lock()->GetGameComponent<NumberComponent>();
		numComponent->SetNumber(StageSelect::GetStageNum() + 1);
	}

	//会話テキスト追加
	AddTalkText();

	GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.7f), Vector3::Zero, Vector3(2112, 1188, 1)));

	shp_map->PutTile();

	clearAnimationFrame = CLEAR_FRAME;

	mode = GameMode::Chara;
	shp_particleEmitter= GetManager().lock()->GetGameObject("SquareParticleEmitter").lock()->GetGameComponent<SquareParticleEmitter>();

	defaultModeUIScale = Vector3(180.0f);

	clearButtonAnimation = false;
	selectedNext = true;

	for (int i = 0; i < StageSelect::GetMaxStage(); i++)
	{
		std::string stageName = "Stage" + std::to_string(i);
		if (stageName == StageSelect::GetRemoveStageName())
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			sceneManager->RemoveScene(stageName);
		}
	}

	bgm = SoundTag("Sound/BGM.wav");
	se_clear = SoundTag("Sound/Clear.wav");
	se_enter = SoundTag("Sound/Enter.wav");
	se_select = SoundTag("Sound/Select-Click.wav");
	se_panelMode = SoundTag("Sound/Panel_Pick.wav");
	se_charaMode = SoundTag("Sound/Panel_Drop.wav");
	if (!GameSettings::isTitle)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, GameSettings::masterVolume);
	}

	const int stageNum = StageSelect::GetStageNum();
	nextSceneName = "Stage" + std::to_string(stageNum + 1);

	GetManager().lock()->GetApplication().lock() ->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
#ifdef DEBUG
	f = 1;
#endif // DEBUG

}

void ButiEngine::StageManager::ShowGUI()
{

}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::OnGoal()
{
	//ゴール時
	if (clearAnimationFrame == 30)
	{
		//クリア時の選択肢出現
		shp_buttonNext->Appear();
		if (StageSelect::GetStageNum() != StageSelect::GetMaxStage())
		{
			shp_buttonSelect->Appear();
		}
	}
	else if (!clearButtonAnimation && clearAnimationFrame < 0)
	{
		//決定時
		clearButtonAnimation = true;
		shp_buttonNext->OnSelected();
	}
	ClearButtonUpdate();
	if (isNext)
	{
		fadeCount++;
	}
	if (fadeCount == 1)
	{
		//フェードアウト用オブジェクト追加
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1134, -0.7f), Vector3::Zero, Vector3(2112, 1188, 1)));
	}
	//シーンの切り替え
	if (fadeCount > 30)
	{
		shp_map->DestoryBlock();
		
		int nextStageNum = StageSelect::GetStageNum() + 1;
		if (nextStageNum > StageSelect::GetMaxStage())
		{
			//最終ステージクリア時、クリアシーンへ
			StageSelect::SetRemoveStageName("none");
			std::string sceneName = "ClearScene";
			ChangeScene(sceneName);
		}
		else
		{
			//次のステージへ(ステージ名)
			std::string sceneName = "Stage" + std::to_string(StageSelect::GetStageNum());
			if (nextSceneName == "StageSelect")
			{
				//次のステージ以外へ戻るときはシーン名を"none"
				sceneName = "none";
			}
			StageSelect::SetRemoveStageName(sceneName);
			ChangeScene(nextSceneName);
		}

		StageSelect::SetStageNum(nextStageNum);
	}
#ifdef _DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::C) && GameDevice::GetInput()->CheckKey(Keys::LeftCtrl))
	{
		StageSelect::SetRemoveStageName("none");
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
	}
#endif
}

void ButiEngine::StageManager::ChangeScene(const std::string& arg_sceneName)
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(arg_sceneName);
	sceneManager->LoadScene(arg_sceneName);
	sceneManager->ChangeScene(arg_sceneName);
	GameSettings::isTutorialInit = false;
}

void ButiEngine::StageManager::ModeChange()
{
	if (shp_cameraController->IsAnimation()) { return; }
	if (shp_panelManager->IsAnimation()) { return; }
	if (shp_pauseManager->IsPause()) { return; }
	if (StageSelect::GetStageNum() == 0) { return; }
	if (wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->IsLock()) { return; }
	auto player = wkp_player.lock()->GetGameComponent<Player>();
	if (player->IsClear() || player->IsFreeze() || player->IsTutorial()) { return; }
	if (wkp_frog.lock() && wkp_frog.lock()->GetGameComponent<Frog>()->IsAnimation()) { return; }

	shp_scrollManager->ResetScroll();
		
	if (mode == GameMode::Chara)
	{
		SetEditMode();
	}
	else
	{
		SetCharaMode();
	}
}

void ButiEngine::StageManager::CreateUI()
{
	if (GameSettings::isTitle) return;
	int stageNum = StageSelect::GetStageNum();

	if (stageNum == 0)
	{
	}
	else
	{
		//キャラ・エディットモードUI関連の追加
		wkp_x = GetManager().lock()->AddObjectFromCereal("X");
		wkp_switchDaikokuten = GetManager().lock()->AddObjectFromCereal("SwitchDaikokuten");
		wkp_switchNezumi = GetManager().lock()->AddObjectFromCereal("SwitchNezumi");
		GetManager().lock()->AddObjectFromCereal("SwitchArrow");
		if (stageNum >= 3)
		{
			wkp_RotateButtonUI = GetManager().lock()->AddObjectFromCereal("RotateButtonUI");
		}

		shp_XMesh = wkp_x.lock()->GetGameComponent<MeshDrawComponent>();
		shp_switchDaikokutenMesh = wkp_switchDaikokuten.lock()->GetGameComponent<MeshDrawComponent>();
		shp_switchNezumiMesh = wkp_switchNezumi.lock()->GetGameComponent<MeshDrawComponent>();

		wkp_switchNezumi.lock()->transform->SetLocalScale(216.0f);
		shp_switchDaikokutenMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = 0.5f;
	}

	wkp_buttonNext = GetManager().lock()->AddObjectFromCereal("Button_Next");
	wkp_buttonSelect = GetManager().lock()->AddObjectFromCereal("Button_Select");
	shp_buttonNext = wkp_buttonNext.lock()->GetGameComponent<ClearButton>();
	shp_buttonSelect = wkp_buttonSelect.lock()->GetGameComponent<ClearButton>();

	wkp_grab = GetManager().lock()->AddObjectFromCereal("BTextWindow");
}

void ButiEngine::StageManager::ChangeUIAlpha()
{
	return;
	auto closestPanel = wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock();
	if (!closestPanel) { return; }
	bool isLock = closestPanel->GetGameComponent<Panel>()->IsLock();

	float alpha = 1.0f;
	if (isLock)
	{
		alpha = 0.5f;
	}
	if (shp_XMesh && shp_switchDaikokutenMesh && shp_switchNezumiMesh)
	{
		shp_XMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = alpha;
		//shp_EditMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = alpha;
		//shp_CharaMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = alpha;
	}
}

void ButiEngine::StageManager::ClearButtonUpdate()
{
	if (!clearButtonAnimation) { return; }

	if (StageSelect::GetStageNum() != StageSelect::GetMaxStage() && !isNext)
	{
		if (InputManager::OnTriggerRightKey())
		{
			if (!selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = true;
			shp_buttonNext->OnSelected();
			shp_buttonSelect->OnEndSelect();
			nextSceneName = "Stage" + std::to_string(StageSelect::GetStageNum() + 1);
		}
		else if (InputManager::OnTriggerLeftKey())
		{
			if (selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = false;
			shp_buttonNext->OnEndSelect();
			shp_buttonSelect->OnSelected();
			nextSceneName = "StageSelect";
		}
	}

	if (InputManager::OnTriggerDecisionKey() && !isNext)
	{
		isNext = true;
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (selectedNext)
		{
			wkp_buttonNext.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
		else
		{
			wkp_buttonSelect.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
	}
}

void ButiEngine::StageManager::CommonTextObject()
{
	//会話時に必要なUIのテンプレート
	wkp_textWindow = GetManager().lock()->AddObjectFromCereal("TextWindow", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.12f), Vector3::Zero, Vector3(1920, 640, 1)));
	GetManager().lock()->AddObjectFromCereal("Abutton", ObjectFactory::Create<Transform>(Vector3(790, -380, -0.16f), Vector3::Zero, Vector3(180, 180, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipYbutton", ObjectFactory::Create<Transform>(Vector3(675, -200, -0.16f), Vector3::Zero, Vector3(80, 80, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipText", ObjectFactory::Create<Transform>(Vector3(780, -200, -0.16f), Vector3::Zero, Vector3(160, 80, 1)));
}

//カエル初回出現時アニメーション
void ButiEngine::StageManager::FrogEatAnimation()
{
	if (StageSelect::GetStageNum() != TalkStageNum::FROG_TALK || frogEatAnimationCount <= 0) return;

	frogEatAnimationCount--;
	const int ZOOM_IN_START = 230;
	const int ZOOM_OUT_START = ZOOM_IN_START - 160;
	if (frogEatAnimationCount == ZOOM_IN_START)
	{
		shp_cameraController->FrogZoomIn();
	}
	else if (frogEatAnimationCount == ZOOM_OUT_START)
	{
		shp_cameraController->FrogZoomOut();
	}

	if (frogEatAnimationCount == 1)
	{
		//カエル説明用テキスト追加
		GetManager().lock()->AddObjectFromCereal("FrogTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
}

void ButiEngine::StageManager::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::StageManager::StoreFrog()
{
	if (!wkp_frog.lock())
	{
		wkp_frog = GetManager().lock()->GetGameObject("Frog");
	}
}

void ButiEngine::StageManager::TutorialMode()
{
	if (PauseManager::IsPause())return;
	auto player = wkp_player.lock()->GetGameComponent<Player>();
	if (!player->IsTutorial())return;

	if (InputManager::OnTriggerUpKey() || InputManager::OnTriggerDownKey())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
		isTutorialSelect = !isTutorialSelect;
		wkp_tutorialNo.lock()->GetGameComponent<TutorialSelect>()->SetNowSelect(isTutorialSelect);
		wkp_tutorialYes.lock()->GetGameComponent<TutorialSelect>()->SetNowSelect(isTutorialSelect);
	}
	if (InputManager::OnTriggerDecisionKey())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (!isTutorialSelect)
		{
			//もう一回！
			GameSettings::isTutorialInit = true;
			shp_pauseManager->OnDecideReset();
		}
		else
		{
			//バッチリ！！
			wkp_player.lock()->GetGameComponent<Player>()->Clear();
			wkp_player.lock()->GetGameComponent<Player>()->ExitTutorial();
			GameSettings::isTutorialInit = false;
		}

		if (wkp_tutorialNo.lock()->GetGameComponent<TransformAnimation>())
		{
			wkp_tutorialNo.lock()->GetGameComponent<TransformAnimation>()->SetIsActive(false);
		}
		if (wkp_tutorialYes.lock()->GetGameComponent<TransformAnimation>())
		{
			wkp_tutorialYes.lock()->GetGameComponent<TransformAnimation>()->SetIsActive(false);
		}

		const float AWAY_POS = -3000;
		wkp_tutorialNo.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
		wkp_tutorialYes.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
		wkp_tutorialTextWindow.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
		wkp_reverseCheckText.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
	}
}

void ButiEngine::StageManager::AddTutorialUI()
{
	auto player = wkp_player.lock()->GetGameComponent<Player>();
	if (!player->IsTutorial())return;
	if (isAddTutorial)return;

	//チュートリアル時に必要なUIを追加する
	isAddTutorial = true;
	wkp_tutorialNo = GetManager().lock()->AddObjectFromCereal("Tutorial_No", ObjectFactory::Create<Transform>(Vector3(650, 40, 0), Vector3::Zero, Vector3(200, 76, 1)));
	wkp_tutorialYes = GetManager().lock()->AddObjectFromCereal("Tutorial_Yes", ObjectFactory::Create<Transform>(Vector3(650, -84, 0), Vector3::Zero, Vector3(200, 76, 1)));
	wkp_tutorialTextWindow = GetManager().lock()->AddObjectFromCereal("TutorialTextWindow", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.12f), Vector3::Zero, Vector3(1920, 640, 1)));
	wkp_reverseCheckText = GetManager().lock()->AddObjectFromCereal("ReverseCheckTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
}

void ButiEngine::StageManager::AddClearAnimation()
{
	auto player = wkp_player.lock()->GetGameComponent<Player>();
	if (player->IsTutorial())return;
	if (!player->IsClear())return;

	//ゴール時にオブジェクトを追加するタイミングを少し遅らせる
	const int GOAL_LATE_FRAME = 20;

	//クリア時演出に必要なオブジェクトの追加
	const int ADD_FLASH_FRAME = CLEAR_FRAME - GOAL_LATE_FRAME;
	const int ADD_BAND_FRAME = ADD_FLASH_FRAME - 10;
	const int ADD_TEXT_FRAME = ADD_BAND_FRAME - 20;
	if (clearAnimationFrame == ADD_FLASH_FRAME)
	{
		Vector3 zoomInPosition = wkp_player.lock()->transform->GetWorldPosition();

		auto frog = player->GetHoldSita().lock();
		if (!frog)
		{
			GetManager().lock()->AddObjectFromCereal("ClearFlash", ObjectFactory::Create<Transform>(Vector3(0.0f, 0.0f, 1000.0f)));
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_clear, GameSettings::masterVolume);
		}

		auto cameraController = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraController>();
		cameraController->MobiusZoomIn(zoomInPosition, 30);
	}
	else if (clearAnimationFrame == ADD_BAND_FRAME)
	{
		GetManager().lock()->AddObjectFromCereal("ClearBand");
	}
	else if (clearAnimationFrame == ADD_TEXT_FRAME)
	{
		GetManager().lock()->AddObjectFromCereal("ClearText");
	}

	clearAnimationFrame--;
}

void ButiEngine::StageManager::AddTalkText()
{
	if (GameSettings::isTitle)
	{
		TalkText::Delete();
		return;
	}

	auto stageNum = StageSelect::GetStageNum();
	if (stageNum == TalkStageNum::FIRST_TALK)
	{
		//最初の会話用テキスト追加
		GetManager().lock()->AddObjectFromCereal("FirstTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (stageNum == TalkStageNum::PANEL_TALK)
	{
		//パネル移動説明用テキスト追加
		GetManager().lock()->AddObjectFromCereal("PanelTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (stageNum == TalkStageNum::REVERSE_TALK)
	{
		//反転説明用テキスト追加
		if (GameSettings::isTutorialInit)
		{
			GetManager().lock()->AddObjectFromCereal("ReverseSecondTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		}
		else
		{
			GetManager().lock()->AddObjectFromCereal("ReverseTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		}
		CommonTextObject();
	}
	else if (stageNum == TalkStageNum::REVERSE_RE_TALK)
	{
		//反転再警告用テキスト追加
		GetManager().lock()->AddObjectFromCereal("ReverseRemindTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (stageNum == TalkStageNum::GRAVITY_TALK)
	{
		//重力コア説明用テキスト追加
		GetManager().lock()->AddObjectFromCereal("GravityTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (stageNum == TalkStageNum::FROG_TALK)
	{
		//カエル説明時のみ専用処理
		frogEatAnimationCount = 240;
		TalkText::Revive();
	}
	else if (stageNum == TalkStageNum::LAST_TALK)
	{
		//最後の会話用テキスト追加
		GetManager().lock()->AddObjectFromCereal("LastTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else
	{
		//テキスト出さないステージはDelete
		TalkText::Delete();
	}
}

void ButiEngine::StageManager::SetEditMode()
{
	//キャラモードならエディットモードへ
	mode = GameMode::Edit;

	wkp_switchDaikokuten.lock()->transform->SetLocalScale(defaultModeUIScale * 1.2f);
	wkp_switchNezumi.lock()->transform->SetLocalScale(defaultModeUIScale);
	shp_switchNezumiMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = 0.5f;
	shp_switchDaikokutenMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = 1.0f;

	//表示
	if (wkp_RotateButtonUI.lock())
	{
		wkp_RotateButtonUI.lock()->GetGameComponent<RotateButtonUI>()->Appear();
	}

	shp_cameraController->ZoomOut();
	shp_panelManager->ResetMoveNum();
	shp_panelManager->ResetMoveHistories();

	shp_particleEmitter->SetIsEdit(true);

	auto nowPosX = wkp_player.lock()->transform->GetWorldPosition();
	particleScrollOffset = nowPosX.x - shp_panelManager->GetClosestPanel(nowPosX.x).lock()->transform->GetWorldPosition().x;

	particleScrollOffset = 360 * (particleScrollOffset / (float)GameSettings::windowWidth);

	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelMode, GameSettings::masterVolume * 3.0f);

	//オヤブンを出現させる
	wkp_daikokutenHead.lock()->GetGameComponent<Daikokuten>()->NormalScale();
	wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
	wkp_daikokutenRAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
	wkp_daikokutenLAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
}

void ButiEngine::StageManager::SetCharaMode()
{
	//エディットモードならキャラモードへ
	mode = GameMode::Chara;
	shp_cameraController->ZoomIn();

	shp_particleEmitter->SetIsEdit(false);
	//少し大きくする
	wkp_switchNezumi.lock()->transform->SetLocalScale(defaultModeUIScale * 1.2f);
	wkp_switchDaikokuten.lock()->transform->SetLocalScale(defaultModeUIScale);
	shp_switchDaikokutenMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = 0.5f;
	shp_switchNezumiMesh->GetCBuffer<ObjectInformation>("ObjectInformation")->Get().lightDir.w = 1.0f;

	//非表示
	if (wkp_RotateButtonUI.lock())
	{
		wkp_RotateButtonUI.lock()->GetGameComponent<RotateButtonUI>()->Disappear();
	}

	shp_cameraController->ZoomIn();

	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_charaMode, GameSettings::masterVolume * 3.0f);

	//オヤブンを引っ込める
	wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	wkp_daikokutenRAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	wkp_daikokutenLAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
}
