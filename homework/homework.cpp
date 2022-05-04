/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#include "Level5.h"
#include "MyApp.h"

int _main_(int _argc, char** _argv)
{
	//Level5 app("e-star-homework-Level5", "", "");
	MyApp app("e-star-homework", "", "");
	return entry::runApp(&app, _argc, _argv);
}

