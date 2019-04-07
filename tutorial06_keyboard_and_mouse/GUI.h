#pragma once
#include <AntTweakBar.h>

class GUI {
public:
	static GUI& getInstance() {
		static GUI instance;
		return instance;
	}

	void init(const char * title);
	void drawBar();
	void addVar(TwType type, void * var);
	TwBar * getBar();

private:
	GUI() = default;
	~GUI() = default;
	GUI(const GUI&) = delete;
	GUI& operator=(const GUI&) = delete;

	TwBar * mBar;
};
