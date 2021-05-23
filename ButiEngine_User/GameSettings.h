#pragma once

class GameSettings
{
public:
	~GameSettings();

	static const int windowWidth = 1280;
	static const int windowHeight = 144;
	static const int panelCount = 8;
	static const int panelWidth = windowWidth / panelCount;
	static const int panelHeight = windowHeight;
	static const int blockSize = 16;
	static const int air = 0;
	static const int player = 1;
	static const int block = 2;
	static const int goalUp = 3;
	static const int coreUp = 4;
	static const int coreDown = 5;
	static const int fill = 6;
	static const int goalDown = 7;
	static const int blockTop = 8;
	static const int blockBottom = 9;
	static const int blockReversible = 10;
	static const int lockPanel = 100;
	static const float panelZ;
	static const float playerZ;
	static const float blockZ;
	static const float goalZ;
	static const float coreZ;
	static const float gravity;
	static const float masterVolume;

	static bool isTitle;

private:
	GameSettings();
};

