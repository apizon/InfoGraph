#include "stdafx.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <KeyframedMeshRenderable.hpp>

#include "Car.hpp"
#include "World.hpp"

#include <lighting/Light.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/PointLightRenderable.hpp>
#include <lighting/SpotLightRenderable.hpp>

void initialize_scene(Viewer& viewer);

int main()
{
	srand(time(NULL));

	Viewer viewer(1280, 720);
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
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
	viewer.addShaderProgram(flatShader);

	ShaderProgramPtr lightShader = std::make_shared<ShaderProgram>("shaders/phongVertex.glsl", "shaders/phongFragment.glsl");
	viewer.addShaderProgram(lightShader);

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	glm::vec3 d_direction = glm::normalize(glm::vec3(-1.0, -1.0, -1.0));
	glm::vec3 d_ambient(0.0, 0.0, 0.0), d_diffuse(0.5, 0.5, 0.5), d_specular(0.3, 0.3, 0.3);
	DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
	viewer.setDirectionalLight(directionalLight);

	glm::vec3 p_ambient = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 p_diffuse = glm::vec3(1.0, 0.9, 0.8);
	glm::vec3 p_specular = glm::vec3(0.5, 0.45, 0.40);
	float p_constant = 1;
	float p_linear = 5e-3;
	float p_quadratic = 1e-4;
	PointLightPtr pointLight0 = std::make_shared<PointLight>(glm::vec3(0, 0, 5.0), p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
	viewer.addPointLight(pointLight0);

	p_diffuse = glm::vec3(1.0, 0.0, 0.0);
	p_specular = glm::vec3(0.5, 0.0, 0.0);
	p_quadratic = 1e-3;
	PointLightPtr pointLight1 = std::make_shared<PointLight>(glm::vec3(WORLD_SIZE / 3, WORLD_SIZE / 3, 10.0), p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
	viewer.addPointLight(pointLight1);

	p_diffuse = glm::vec3(0.0, 0.0, 1.0);
	p_specular = glm::vec3(0.0, 0.0, 0.5);
	p_quadratic = 1e-3;
	PointLightPtr pointLight2 = std::make_shared<PointLight>(glm::vec3(-WORLD_SIZE / 3, WORLD_SIZE / 3, 10.0), p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
	viewer.addPointLight(pointLight2);

	p_diffuse = glm::vec3(1.0, 0.0, 1.0);
	p_specular = glm::vec3(0.5, 0.0, 0.5);
	p_quadratic = 1e-3;
	PointLightPtr pointLight3 = std::make_shared<PointLight>(glm::vec3(-WORLD_SIZE / 3, -WORLD_SIZE / 3, 10.0), p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
	viewer.addPointLight(pointLight3);

	p_diffuse = glm::vec3(1.0, 0.8, 0.0);
	p_specular = glm::vec3(0.5, 0.4, 0.0);
	p_quadratic = 1e-3;
	PointLightPtr pointLight4 = std::make_shared<PointLight>(glm::vec3(WORLD_SIZE / 3, -WORLD_SIZE / 3, 10.0), p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
	viewer.addPointLight(pointLight4);

	WorldPtr world = std::make_shared<World>(lightShader);
	world->init();
	viewer.addRenderable(world);

	viewer.startAnimation();
}
