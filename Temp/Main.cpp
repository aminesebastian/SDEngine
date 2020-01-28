#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/Factories/StaticMeshAssetFactory.h"
#include "Core/Assets/Factories/MaterialAssetFactory.h"
#include "Utilities/Logger.h"
#include "Entities/StaticMesh.h"
#include "Rendering/Material.h"
#include "Entities/Actors/StaticMeshActor.h"
#include "Entities/Light.h"
#include "Rendering/Shader.h"

#undef main
using namespace glm;

Engine* S_Engine;

int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Initialize()) {
		return 0;
	}

	SD_ENGINE_INFO("Engine Launched!");

	//AssetManager* manager = Engine::GetInstance()->GetAssetManager();
	//manager->RegisterNewFactory("StaticMesh", new StaticMeshAssetFactory());
	//manager->RegisterNewFactory("Material", new MaterialAssetFactory());
	//StaticMesh* test = manager->GetAsset<StaticMesh>("./Res/Assets/Head.sasset");

	StaticMesh* test = new StaticMesh("head", "./Res/Head/Head.fbx");
	Material* mat = new Material(EngineStatics::GetDefaultGeometryPassShader());
	Texture2D* albedo = new Texture2D("./Res/Head/T_Head_Level2_BaseColor.tga");
	Texture2D* rmao = new Texture2D("./Res/Head/T_Head_Level2_Normal.tga");
	Texture2D* normal = new Texture2D("./Res/Head/T_Head_Level2_RMAO.tga");
	mat->SetTextureParameter("albedo", albedo);
	mat->SetTextureParameter("RMAO", rmao);
	mat->SetTextureParameter("normal", normal);

	test->GetSubMeshes()[0]->SubMeshMaterial = mat;
	//Scene* scene = new Scene();
	//S_Engine->LoadScene(scene);

	StaticMeshActor* smActor = new StaticMeshActor("Head");
	smActor->SetStaticMesh(test);
	S_Engine->GetWorld()->RegisterActor(smActor);

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

	S_Engine->StartEngine();

	return 0;
}