/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

int _main_(int _argc, char** _argv)
{
	//Level1 app("e-star-homework", "", "");
	//Level2 app("e-star-homework", "", "");
	Level3 app("e-star-homework", "", "");
	return entry::runApp(&app, _argc, _argv);
}

