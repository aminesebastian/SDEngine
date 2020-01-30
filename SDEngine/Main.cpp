#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Utilities/Logger.h"
#include "Entities/StaticMesh.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/Factories/StaticMeshAssetFactory.h"
#include "Core/Assets/Factories/MaterialAssetFactory.h"
#include "Core/Assets/Factories/Texture2DAssetFactory.h"
#include "Core/Assets/AssetMetadata.h"

#undef main
using namespace glm;

Engine* S_Engine;

void serializeTexture(TString TextureName,TString TexturePath, TString AssetPath, AssetManager* Manager);
void serializeStaticMesh(TString MeshName, TString MeshPath, TString AssetPath, AssetManager* Manager);

int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Initialize()) {		
		return 0;
	}
	AssetManager* manager = Engine::GetInstance()->GetAssetManager();
	manager->RegisterNewFactory("StaticMesh", new StaticMeshAssetFactory());
	manager->RegisterNewFactory("Material", new MaterialAssetFactory());
	manager->RegisterNewFactory("Texture2D", new Texture2DAssetFactory());

	//serializeTexture("light_texture", "./Res/Textures/Editor/Sprites/PointLightSprite.png", "./Res/Assets/Textures/PointLightSprite.sasset", manager);
	//serializeStaticMesh("head", "./Res/Head/Head.fbx", "./Res/Assets/Head.sasset", manager);

	SD_ENGINE_INFO("Engine Launched!");

	Scene* scene = new Scene();
	S_Engine->LoadScene(scene);

	S_Engine->StartEngine();

	return 0;
}

void serializeTexture(TString TextureName, TString TexturePath, TString AssetPath, AssetManager* Manager) {
	ByteBuffer buffer;
	SerializationStream stream(buffer);

	Texture2D tex(TextureName, TexturePath);
	AssetMetadata metadata("Texture2D", 1, stream);
	tex.SerializeToBuffer(stream);
	buffer.WriteToCompressedFile(AssetPath);
}
void serializeStaticMesh(TString MeshName, TString MeshPath, TString AssetPath, AssetManager* Manager) {
	ByteBuffer buffer;
	SerializationStream stream(buffer);

	StaticMesh tex(MeshName, MeshPath);
	AssetMetadata metadata("StaticMesh", 1, stream);
	tex.SerializeToBuffer(stream);
	buffer.WriteToCompressedFile(AssetPath);
}