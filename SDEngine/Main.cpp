#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Utilities/Logger.h"
#include "Entities/StaticMesh.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/Factories/StaticMeshAssetFactory.h"
#include "Core/Assets/Factories/MaterialAssetFactory.h"
#include "Core/Assets/Factories/Texture2DAssetFactory.h"

#undef main
using namespace glm;

Engine* S_Engine;

int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Initialize()) {		
		return 0;
	}
	AssetManager* manager = Engine::GetInstance()->GetAssetManager();
	manager->RegisterNewFactory("StaticMesh", new StaticMeshAssetFactory());
	manager->RegisterNewFactory("Material", new MaterialAssetFactory());
	manager->RegisterNewFactory("Texture2D", new Texture2DAssetFactory());

	//Texture2D* pointLightTex = new Texture2D("./Res/Textures/Editor/Sprites/PointLightSprite.png");
	//ByteBuffer* buffer = new ByteBuffer();
	//pointLightTex->SerializeToBuffer(*buffer);
	//buffer->WriteToCompressedFile("./Res/Assets/Textures/PointLightSprite.sasset");
	//delete buffer;
	//delete pointLightTex;


	SD_ENGINE_INFO("Engine Launched!");

	Scene* scene = new Scene();
	S_Engine->LoadScene(scene);

	S_Engine->StartEngine();

	return 0;
}