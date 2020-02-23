#include "Core/Assets/AssetManager.h"
#include "Core/Assets/AssetMetadata.h"
#include "Core/Assets/Factories/FontAssetFactory.h"
#include "Core/Assets/Factories/MaterialAssetFactory.h"
#include "Core/Assets/Factories/StaticMeshAssetFactory.h"
#include "Core/Assets/Factories/Texture2DAssetFactory.h"
#include "Core/Engine/Engine.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Objects/CoreTypes/Scene.h"
#include "Core/Objects/CoreTypes/StaticMesh.h"
#include "Core/Utilities/Logger.h"

#undef main
using namespace glm;

Engine* S_Engine;

void serializeTexture(TString TextureName,TString TexturePath, TString AssetPath, AssetManager* Manager);
void serializeStaticMesh(TString MeshName, TString MeshPath, TString AssetPath, AssetManager* Manager);
void serializeAllTextures(AssetManager* Manager);
void serializeFont(TString FontName, TString FontPath, TString AssetPath, AssetManager* Manager);

int main(int argc, char* argv[]) {
	S_Engine = Engine::Get();

	AssetManager* manager = Engine::Get()->GetAssetManager();
	manager->RegisterNewFactory("StaticMesh", new StaticMeshAssetFactory());
	manager->RegisterNewFactory("Material", new MaterialAssetFactory());
	manager->RegisterNewFactory("Texture2D", new Texture2DAssetFactory());
	manager->RegisterNewFactory("Font", new FontAssetFactory());

	//serializeFont("Arial", "./Res/Fonts/Arial", "./Res/Assets/Editor/Fonts/Arial.sasset", manager);
	//serializeStaticMesh("head", "./Res/Head/Head.fbx", "./Res/Assets/Head.sasset", manager);
	//serializeAllTextures(manager);

	if (!S_Engine->Initialize()) {		
		return 0;
	}

	SD_ENGINE_INFO("Engine Launched!");

	S_Engine->StartEngine();

	return 0;
}
void serializeAllTextures(AssetManager* Manager) {
	serializeTexture("light_texture", "./Res/Textures/Editor/Sprites/PointLightSprite.png", "./Res/Assets/Editor/Textures/PointLightSprite.sasset", Manager);
	serializeTexture("close_icon", "./Res/Textures/Editor/UI/CloseIcon.png", "./Res/Assets/Editor/Textures/UI/CloseIcon.sasset", Manager);
	serializeTexture("restore_icon", "./Res/Textures/Editor/UI/RestoreIcon.png", "./Res/Assets/Editor/Textures/UI/RestoreIcon.sasset", Manager);
	serializeTexture("minimize_icon", "./Res/Textures/Editor/UI/MinimizeIcon.png", "./Res/Assets/Editor/Textures/UI/MinimizeIcon.sasset", Manager);
	serializeTexture("maximize_icon", "./Res/Textures/Editor/UI/MaximizeIcon.png", "./Res/Assets/Editor/Textures/UI/MaximizeIcon.sasset", Manager);
}
void serializeTexture(TString TextureName, TString TexturePath, TString AssetPath, AssetManager* Manager) {
	ByteBuffer buffer;
	SerializationStream stream(buffer);

	Texture2D tex(TextureName);
	tex.ImportAsset(TexturePath);
	AssetMetadata metadata("Texture2D", 1, stream);
	tex.SerializeToBuffer(stream);
	buffer.WriteToCompressedFile(AssetPath);
}
void serializeStaticMesh(TString MeshName, TString MeshPath, TString AssetPath, AssetManager* Manager) {
	ByteBuffer buffer;
	SerializationStream stream(buffer);

	StaticMesh tex(MeshName);
	tex.ImportAsset(MeshPath);
	AssetMetadata metadata("StaticMesh", 1, stream);
	tex.SerializeToBuffer(stream);
	buffer.WriteToCompressedFile(AssetPath);
}
void serializeFont(TString FontName, TString FontPath, TString AssetPath, AssetManager* Manager) {
	ByteBuffer buffer;
	SerializationStream stream(buffer);

	DistanceFieldFont font(FontName);
	font.ImportAsset(FontPath);
	AssetMetadata metadata("Font", 1, stream);
	font.SerializeToBuffer(stream);
	buffer.WriteToCompressedFile(AssetPath);
}