#pragma once

class GameSettings
{
public:
	~GameSettings();

	static const int windowWidth = 1280;
	static const int windowHeight = 128;
	static const int panelCount = 8;
	static const int panelWidth = windowWidth / panelCount;
	static const int panelHeight = windowHeight;
	static const int blockSize = 16;
	static const int air = 0;
	static const int player = 1;
	static const int block = 2;
private:
	GameSettings();
};

