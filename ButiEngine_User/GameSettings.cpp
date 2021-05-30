#include "stdafx_u.h"
#include "GameSettings.h"

const float GameSettings::skyZ = 1.5f;
const float GameSettings::treeZ = 1.0f;
const float GameSettings::gravityArrowZ = 0.9f;
const float GameSettings::panelZ = 0.0f;
const float GameSettings::blockZ = -0.1f;
const float GameSettings::goalZ = -0.2f;
const float GameSettings::coreZ = -0.3f;
const float GameSettings::frogZ = -4.4f;
const float GameSettings::playerZ = -4.5f;
const float GameSettings::frameZ = -1.0f;
const float GameSettings::predictionLineZ = -6.0f;

const float GameSettings::gravity = 0.4f;
const float GameSettings::masterVolume = 0.1f;

bool GameSettings::isTitle = true;
bool GameSettings::isStageSelect = false;
bool GameSettings::isTutorialInit = false;

GameSettings::~GameSettings()
{
}
