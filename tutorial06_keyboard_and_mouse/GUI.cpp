#include "GUI.h"
#include <AntTweakBar.h>

void GUI::init(const char * title)
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(1024, 768);
	this->mBar = TwNewBar(title);
}

void GUI::drawBar()
{
	TwDraw();
}

void GUI::addVar(TwType type, void * var)
{
	TwAddVarRW(this->mBar, "CAMVOIT", type, var, "label='CAMERA' opened=true ");
}

TwBar * GUI::getBar()
{
	return this->mBar;
}
