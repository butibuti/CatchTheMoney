#pragma once

class GameSettings
{
public:
	~GameSettings();

	static const int windowWidth = 3072;
	static const int windowHeight = 256;
	static const int blockSize = 32;
	static const int air = 0;
	static const int player = 1;
	static const int block = 2;
private:
	GameSettings();
};

