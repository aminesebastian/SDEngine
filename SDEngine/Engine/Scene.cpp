#include "AssetManager.h"
#include "Entities/StaticMesh.h"
#include "Engine/EngineStatics.h"
#include "SAsset.h"
#include "Engine/Engine.h"
#include "Entities/Light.h"
#include "Scene.h"
#include "WorkerThread.h"
#include "Utilities/Logger.h"

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
	Transform planeTransform;
	Material* planeMat = EngineStatics::GetDefaultMaterial();
	StaticMesh* plane = new StaticMesh("Plane", planeTransform, planeMat, "./res/Plane.fbx");
	Engine::GetInstance()->GetWorld()->RegisterEntity(plane);

	SD_ENGINE_INFO("Plane Loaded!");
}
void Scene::LoadHead() {
	Transform headTransform;
	headTransform.GetLocation().z = 7;
	SAsset* headAsset = Engine::GetInstance()->GetAssetManager()->GetAsset("./Res/Assets/Head.sasset");
	StaticMesh* head = headAsset->GetAsStaticMesh("Head");
	head->SetTransform(headTransform);
	Engine::GetInstance()->GetWorld()->RegisterEntity(head);

	SD_ENGINE_INFO("Head Loaded!");
}
void Scene::LoadSponza() {
	Transform sponzaTransform;
	StaticMesh* sponza = new StaticMesh("Sponza", sponzaTransform, "./Res/Sponza.fbx");
	Engine::GetInstance()->GetWorld()->RegisterEntity(sponza);
	SD_ENGINE_INFO("Sponza Loaded!");
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
				tempTransform.SetUniformScale(0.25f);
				Light* tempLight = new Light("Light " + std::to_string(count), tempTransform, POINT, 25, tempColor, atten);
				Engine::GetInstance()->GetWorld()->RegisterEntity(tempLight);
				count++;
			}
		}
	}

	Transform fillLightTransform;
	fillLightTransform.SetRotation(0, glm::radians(100.0f), 0);
	fillLightTransform.GetLocation().z = 30;
	Light* fillLight = new Light("Directional Light", fillLightTransform, DIRECTIONAL, 6, vec3(0.75, 0.9, 0.8));
	fillLight->SetCastsShadows(true);
	Engine::GetInstance()->GetWorld()->RegisterEntity(fillLight);
}