#include "AssetManager.h"
#include "StaticMesh.h"
#include "EngineStatics.h"
#include "SAsset.h"
#include "Engine.h"
#include "Light.h"
#include "Scene.h"
#include "WorkerThread.h"

Scene::Scene() {

}
Scene::~Scene() {

}
bool Scene::LoadScene() {
	Engine* engine = Engine::GetInstance();

	//m_ActiveThreads.push_back(new WorkerThread([this]() { LoadHead(); }));
	//m_ActiveThreads.push_back(new WorkerThread([this]() { LoadHead(); }));
	//m_ActiveThreads.push_back(new WorkerThread([this]() { LoadGizmo(); }));
	//m_ActiveThreads.push_back(new WorkerThread([this]() { LoadLights(); }));

	//for (WorkerThread* thread : m_ActiveThreads) {
	//	thread->Wait();
	//}
	LoadPlane();
	LoadHead();
	LoadGizmo();
	LoadLights();
	//LoadSponza();

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

	PrintToLog("Plane Loaded!");
}
void Scene::LoadHead() {
	Transform headTransform;
	headTransform.GetLocation().z = 7;
	SAsset* headAsset = Engine::GetInstance()->GetAssetManager()->GetAsset("./Res/Assets/Head.sasset");
	StaticMesh* head = headAsset->GetAsStaticMesh("Head");
	head->SetTransform(headTransform);
	Engine::GetInstance()->GetWorld()->RegisterEntity(head);

	PrintToLog("Head Loaded!");
}
void Scene::LoadGizmo() {
	Transform gizmoTransform;
	SAsset* gizmoAsset = Engine::GetInstance()->GetAssetManager()->GetAsset("./Res/Assets/Gizmo.sasset");
	StaticMesh* gizmo = gizmoAsset->GetAsStaticMesh("Gizmo");
	gizmo->SetTransform(gizmoTransform);
	Engine::GetInstance()->GetWorld()->RegisterEntity(gizmo);

	PrintToLog("Gizmo Loaded!");
}
void Scene::LoadSponza() {
	Transform sponzaTransform;
	StaticMesh* sponza = new StaticMesh("Sponza", sponzaTransform, "./Res/Sponza.fbx");
	Engine::GetInstance()->GetWorld()->RegisterEntity(sponza);
	PrintToLog("Sponza Loaded!");
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
				Engine::GetInstance()->GetWorld()->RegisterLight(tempLight);
				count++;
			}
		}
	}

	Transform fillLightTransform;
	fillLightTransform.SetRotation(0, 100, 0);
	fillLightTransform.GetLocation().z = 30;
	Light* fillLight = new Light("Directional Light", fillLightTransform, DIRECTIONAL, 6, vec3(0.75, 0.9, 0.8));
	fillLight->SetCastsShadows(true);
	Engine::GetInstance()->GetWorld()->RegisterLight(fillLight);
}