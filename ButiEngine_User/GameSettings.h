#pragma once
class GameSettings
{
public:
	~GameSettings();

	static const int windowWidth = 3072;
	static const int windowHeight = 256;
	static const int gridSize = 32;
private:
	GameSettings();
};

