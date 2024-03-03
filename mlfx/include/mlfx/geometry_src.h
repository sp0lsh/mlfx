#pragma once

#include <cstdint>

// clang-format off
static float cubeVertices[] = {
	-0.5,  0.5,  0.5, // Front Top Left
	 0.5,  0.5,  0.5, // Front Top Right		
	 0.5, -0.5,  0.5, // Front Bottom Righ
	-0.5, -0.5,  0.5, // Front Bottom Left
	
    -0.5,  0.5, -0.5, // Back Top Left
	 0.5,  0.5, -0.5, // Back Top Right
	 0.5, -0.5, -0.5, // Back Bottom Right
	-0.5, -0.5, -0.5, // Back Bottom Left	
};

static float colour[] = {
    -0.5,  0.5,  0.5,	// Red	- 0
     0.5,  0.5,  0.5,	// Green	- 1
     0.5, -0.5,  0.5,	// Blue	- 2
    -0.5, -0.5,  0.5,	// Cyan	- 3
    -0.5,  0.5, -0.5,	// Pink	- 4
     0.5,  0.5, -0.5,	// Yellow- 5
     0.5, -0.5, -0.5,	// White - 6
    -0.5, -0.5, -0.5,	// Gray  - 7
};

static unsigned short indices[] = {
    0, 3, 2,  //Front
	2, 1, 0,
	1, 5, 6,	//Right
	6, 2, 1,
	5, 4, 7,	//Left
	7, 6, 5,
	4, 7, 3,	//Back
	3, 0, 4,
	1, 5, 4,	//Top
	1, 0, 4,
	3, 2, 6,	//Bottom
	6, 7, 3,
};
// clang-format on