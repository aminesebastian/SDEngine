#include "Core/Objects/CoreTypes/StaticMesh.h"
#include "Core/Objects/Entities/Actors/HeadActor.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Engine.h"
#include "Core/Objects/Entities/Light.h"
#include "Scene.h"
#include "Core/Utilities/Logger.h"

Scene::Scene() {

}
Scene::~Scene() {

}
bool Scene::LoadScene(World* WorldIn) {
	LoadPlane(WorldIn);
	LoadHead(WorldIn);
	LoadLights(WorldIn);

	return true;
}
bool Scene::UnloadScene(World* WorldIn) {
	return true;
}
bool Scene::SaveScene() {
	return true;
}
void Scene::LoadPlane(World* WorldIn) {
	//CurrentTransform planeTransform;
	//Material* planeMat = EngineStatics::GetDefaultMaterial();
	//StaticMesh* plane = new StaticMesh("Plane", planeTransform, planeMat, "./res/Plane.fbx");
	//Engine::GetInstance()->GetWorld()->RegisterActor(plane);

	//SD_ENGINE_INFO("Plane Loaded!");
}
void Scene::LoadHead(World* WorldIn) {
	FTransform headTransform;
	headTransform.SetLocation(vec3(0.0f, 0.0f, 7.0f));
	HeadActor* head = new HeadActor("TestHead");
	head->SetTransform(headTransform);
	WorldIn->RegisterActor(head);

	SD_ENGINE_INFO("Head Loaded!");
}
void Scene::LoadLights(World* WorldIn) {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				float r = (float)(rand()) / (float)(RAND_MAX);
				float g = (float)(rand()) / (float)(RAND_MAX);
				float b = (float)(rand()) / (float)(RAND_MAX);
				FTransform tempTransform;
				Vector3D location = Vector3D((float)j * 5.5f - 5.5f, (float)i * 5.5f - 5.5f, 9 + (float)k * 6.5f);
				tempTransform.SetLocation(location);
				FColor tempColor = FColor(r, g, b);
				float atten = (((float)(rand()) / (float)(RAND_MAX)) + 1) * 20;
				Light* tempLight = new Light("Light " + std::to_string(count), tempTransform, POINT, 25, tempColor, atten);
				WorldIn->RegisterActor(tempLight);
				count++;
			}
		}
	}

	FTransform fillLightTransform;
	fillLightTransform.SetRotation(0, glm::radians(100.0f), 0);
	fillLightTransform.SetLocation(Vector3D(0.0f, 0.0f, 30.0f));
	Light* fillLight = new Light("Directional Light", fillLightTransform, DIRECTIONAL, 6, FColor(0.75f, 0.9f, 0.8f));
	fillLight->SetCastShadows(true);
	WorldIn->RegisterActor(fillLight);
}