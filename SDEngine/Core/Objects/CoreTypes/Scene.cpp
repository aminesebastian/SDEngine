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
bool Scene::LoadScene() {
	Engine* engine = Engine::GetInstance();

	LoadPlane();
	LoadHead();
	LoadLights();

	return true;
}
bool Scene::SaveScene() {
	return true;
}
void Scene::LoadPlane() {
	//CurrentTransform planeTransform;
	//Material* planeMat = EngineStatics::GetDefaultMaterial();
	//StaticMesh* plane = new StaticMesh("Plane", planeTransform, planeMat, "./res/Plane.fbx");
	//Engine::GetInstance()->GetWorld()->RegisterActor(plane);

	//SD_ENGINE_INFO("Plane Loaded!");
}
void Scene::LoadHead() {
	Transform headTransform;
	headTransform.GetLocation().z = 7;
	HeadActor* head = new HeadActor("TestHead");
	head->SetTransform(headTransform);
	Engine::GetInstance()->GetWorld()->RegisterActor(head);

	SD_ENGINE_INFO("Head Loaded!");
}
void Scene::LoadLights() {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				float r = (float)(rand()) / (float)(RAND_MAX);
				float g = (float)(rand()) / (float)(RAND_MAX);
				float b = (float)(rand()) / (float)(RAND_MAX);
				Transform tempTransform;
				tempTransform.GetLocation().x = (float)j * 5.5f - 5.5f;
				tempTransform.GetLocation().y = (float)i * 5.5f - 5.5f;
				tempTransform.GetLocation().z = 9 + (float)k * 6.5f;
				vec3 tempColor = vec3(r, g, b);
				float atten = (((float)(rand()) / (float)(RAND_MAX)) + 1) * 20;
				Light* tempLight = new Light("Light " + std::to_string(count), tempTransform, POINT, 25, tempColor, atten);
				Engine::GetInstance()->GetWorld()->RegisterActor(tempLight);
				count++;
			}
		}
	}

	Transform fillLightTransform;
	fillLightTransform.SetRotation(0, glm::radians(100.0f), 0);
	fillLightTransform.GetLocation().z = 30;
	Light* fillLight = new Light("Directional Light", fillLightTransform, DIRECTIONAL, 6, vec3(0.75, 0.9, 0.8));
	fillLight->SetCastsShadows(true);
	Engine::GetInstance()->GetWorld()->RegisterActor(fillLight);
}