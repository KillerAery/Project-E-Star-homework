/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"

int _main_(int _argc, char** _argv)
{
	//Level1 app("e-star-homework-Level1", "", "");
	//Level2 app("e-star-homework-Level2", "", "");
	//Level3 app("e-star-homework-Level3", "", "");
	//Level4 app("e-star-homework-Level4", "", "");
	Level5 app("e-star-homework-Level5", "", "");
	return entry::runApp(&app, _argc, _argv);
}

