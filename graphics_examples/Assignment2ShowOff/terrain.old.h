#pragma once

#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

#include "terrain.h"

class TerrainOld
{
public:

	~TerrainOld() 
	{
		if (terrain) delete(terrain);
	}

	Terrain* terrain;
	void initTerrain()
	{
		terrain = new Terrain(3.0f, 1.0f,6.0f);
		terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
		terrain->setColour(glm::vec3(0,1,1));
		terrain->createObject();
	}

	void drawTerrain(int drawmode)
	{
		terrain->drawObject(drawmode);
	}
};
