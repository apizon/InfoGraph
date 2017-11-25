#include "stdafx.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <Light.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <KeyframedMeshRenderable.hpp>

#include "Car.hpp"
#include "World.hpp"


#define SHADER	lightShader

void initialize_scene(Viewer& viewer);

int main()
{
	srand(time(NULL));

	Viewer viewer(800, 600);
	initialize_scene(viewer);

	while (viewer.isRunning())
	{
		viewer.handleEvent();
		viewer.animate();
		viewer.draw();
		viewer.display();
	}

	return EXIT_SUCCESS;
}


void initialize_scene(Viewer& viewer) {
	ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>("shaders/defaultVertex.glsl", "shaders/defaultFragment.glsl");
	ShaderProgramPtr lightShader = std::make_shared<ShaderProgram>("shaders/lightVertex.glsl", "shaders/lightFragment.glsl");
	viewer.addShaderProgram(SHADER);

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(SHADER);
	viewer.addRenderable(frame);

	CarPtr car = std::make_shared<Car>(SHADER, 1000.0f, 300.0f, 10000.0f);
	car->init();
	viewer.addRenderable(car);

	WorldPtr world = std::make_shared<World>(SHADER);
	world->init();
	viewer.addRenderable(world);

	viewer.startAnimation();
}
