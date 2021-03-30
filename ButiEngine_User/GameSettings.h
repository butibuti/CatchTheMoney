#pragma once
class GameSettings
{
public:
	~GameSettings();

	static const int windowWidth = 1920;
	static const int windowHeight = 1080;
	static const int gridSize = 60;
private:
	GameSettings();
};

