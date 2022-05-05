/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#include "MyApp.h"

int _main_(int _argc, char** _argv)
{
	MyApp app("e-star-homework", "", "");
	return entry::runApp(&app, _argc, _argv);
}

