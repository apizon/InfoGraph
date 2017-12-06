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

//void initialize_scene(Viewer& viewer)
//{
//	//Default shader
//	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
//	viewer.addShaderProgram(flatShader);
//
//	//Define a shader that encode an illumination model
//	ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>("shaders/phongVertex.glsl", "shaders/phongFragment.glsl");
//	viewer.addShaderProgram(phongShader);
//
//	//Add a 3D frame to the viewer
//	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
//	viewer.addRenderable(frame);
//
//	//Define a transformation
//	glm::mat4 parentTransformation, localTransformation;
//
//	//Define a directional light for the whole scene
//	glm::vec3 d_direction = glm::normalize(glm::vec3(0.0, -1.0, -1.0));
//	glm::vec3 d_ambient(0.0, 0.0, 0.0), d_diffuse(0.3, 0.3, 0.1), d_specular(0.3, 0.3, 0.1);
//	//glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
//	DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
//	//Add a renderable to display the light and control it via mouse/key event
//	glm::vec3 lightPosition(0.0, 5.0, 8.0);
//	DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
//	localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
//	directionalLightRenderable->setLocalTransform(localTransformation);
//	viewer.setDirectionalLight(directionalLight);
//	viewer.addRenderable(directionalLightRenderable);
//
//	//Define a point light
//	glm::vec3 p_position(0.0, 0.0, 0.0), p_ambient(0.0, 0.0, 0.0), p_diffuse(0.0, 0.0, 0.0), p_specular(0.0, 0.0, 0.0);
//	float p_constant = 0.0, p_linear = 0.0, p_quadratic = 0.0;
//
//	p_position = glm::vec3(-8, 5.0, 5.0);
//	p_ambient = glm::vec3(0.0, 0.0, 0.0);
//	p_diffuse = glm::vec3(1.0, 0.0, 0.0);
//	p_specular = glm::vec3(1.0, 0.0, 0.0);
//	p_constant = 1.0;
//	p_linear = 5e-1;
//	p_quadratic = 0;
//	PointLightPtr pointLight1 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
//	PointLightRenderablePtr pointLightRenderable1 = std::make_shared<PointLightRenderable>(flatShader, pointLight1);
//	localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
//	pointLightRenderable1->setLocalTransform(localTransformation);
//	viewer.addPointLight(pointLight1);
//	viewer.addRenderable(pointLightRenderable1);
//
//	p_position = glm::vec3(8, 5.0, 5.0);
//	p_ambient = glm::vec3(0.0, 0.0, 0.0);
//	p_diffuse = glm::vec3(0.0, 0.0, 1.0);
//	p_specular = glm::vec3(0.0, 0.0, 1.0);
//	p_constant = 1.0;
//	p_linear = 5e-1;
//	p_quadratic = 0;
//	PointLightPtr pointLight2 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
//	PointLightRenderablePtr pointLightRenderable2 = std::make_shared<PointLightRenderable>(flatShader, pointLight2);
//	localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
//	pointLightRenderable2->setLocalTransform(localTransformation);
//	viewer.addPointLight(pointLight2);
//	viewer.addRenderable(pointLightRenderable2);
//
//	//Define a spot light
//	glm::vec3 s_position(0.0, 5.0, -8.0), s_spotDirection = glm::normalize(glm::vec3(0.0, -1.0, 1.0));
//	//glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
//	glm::vec3 s_ambient(0.0, 0.0, 0.0), s_diffuse(0.5, 0.5, 0.5), s_specular(0.5, 0.5, 0.5);
//	float s_constant = 1.0, s_linear = 0.0, s_quadratic = 0.0;
//	float s_innerCutOff = std::cos(glm::radians(20.0f)), s_outerCutOff = std::cos(glm::radians(40.0f));
//	SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
//		s_ambient, s_diffuse, s_specular,
//		s_constant, s_linear, s_quadratic,
//		s_innerCutOff, s_outerCutOff);
//	SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
//	localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
//	spotLightRenderable->setLocalTransform(localTransformation);
//	viewer.addSpotLight(spotLight);
//	viewer.addRenderable(spotLightRenderable);
//
//	//Define materials
//	glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
//	float mShininess = 0.0;
//	MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
//
//	//Lighted Mesh 1
//	MaterialPtr bronze = Material::Bronze();
//	LightedMeshRenderablePtr suzanne1 = std::make_shared<LightedMeshRenderable>(phongShader, "meshes/suzanne.obj");
//	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(-2.0, 0.0, 0.0));
//	localTransformation = glm::mat4(1.0);
//	suzanne1->setParentTransform(parentTransformation);
//	suzanne1->setLocalTransform(localTransformation);
//	suzanne1->setMaterial(bronze);
//	viewer.addRenderable(suzanne1);
//
//	//Lighted Mesh 2
//	MaterialPtr emerald = Material::Emerald();
//	LightedMeshRenderablePtr suzanne2 = std::make_shared<LightedMeshRenderable>(phongShader, "meshes/suzanne.obj");
//	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2.0, 0.0, 0.0));
//	localTransformation = glm::mat4(1.0);
//	suzanne2->setParentTransform(parentTransformation);
//	suzanne2->setLocalTransform(localTransformation);
//	suzanne2->setMaterial(emerald);
//	viewer.addRenderable(suzanne2);
//
//	viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(-5, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
//	viewer.startAnimation();
//	viewer.setAnimationLoop(true, 4.0);
//}