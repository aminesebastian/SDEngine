#include <iostream>
#include <GLEW/glew.h>
#include "Display.h"
#include "Shader.h"
#include "Texture2D.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Light.h"
#include <random>
#include "Grid.h"
#include "Engine.h"
#include "RenderingEngine.h"
#include "EyeActor.h"
#include "AssetManager.h"
#include "EngineStatics.h"

#undef main
using namespace glm;

/*
Physics
Collision
AI/Pathfinding
Resource Management
Content Creation

How to implement? (IMO)
What are concerns you have about it?


*/
Engine* S_Engine;
int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Init()) {		
		return 0;
	}
	init_logger("SD_EngineLog.txt");
	PrintToLog("Engine Launched!");

	//Texture2D* torusAlbedo = new Texture2D("res/T_TorusBaseColor.tga");
	//Texture2D* torusRoughness = new Texture2D("res/T_TorusRMAO.tga");
	//Texture2D* torusNormal = new Texture2D("res/T_TorusNormal.tga");

	//Material* torusMaterial = new Material("res/Shaders/TorusShader");
	//Transform torusTransform;
	//torusTransform.GetPosition().y += 10;
	//torusTransform.GetPosition().x += 7;
	//torusTransform.SetUniformScale(5.0f);
	//StaticMesh* torus = new StaticMesh(torusTransform, "./res/Torus.fbx");
	//torus->SetMaterial(EngineStatics::GetDefaultMaterial());
	//torusMaterial->SetTextureParameter("albedo", torusAlbedo);
	//torusMaterial->SetTextureParameter("RMAO", torusRoughness);
	//torusMaterial->SetTextureParameter("normal", torusNormal);

	//S_Engine->GetWorld()->RegisterEntity(torus);

	//Transform skySphereTransform;
	//skySphereTransform.SetUniformScale(100.0f);
	//Texture2D* skySphereTexture = new Texture2D("res/ForestEnvironment.jpg");

	//Material* skyMaterial = new Material("./Res/Shaders/SkySphereShader");
	//skyMaterial->SetTextureParameter("Albedo", skySphereTexture);
	//skyMaterial->SetShaderModel(EShaderModel::UNLIT);
	//StaticMesh* sky = new StaticMesh(skySphereTransform, skyMaterial, "./res/SkySphere.fbx");
	//S_Engine->GetWorld()->RegisterEntity(sky);


	//Transform innerEyetransform;
	//innerEyetransform.SetUniformScale(3.0f);
	//innerEyetransform.SetRotation(-60, 0, 0);
	//innerEyetransform.GetPosition().y = 10;

	//SAsset* eyeAsset = S_Engine->GetAssetManager()->GetAsset("./Res/Assets/Eye.sasset");
	//StaticMesh* eyeMesh = eyeAsset->GetAsStaticMesh();
	//eyeMesh->SetTransform(innerEyetransform);
	//S_Engine->GetWorld()->RegisterEntity(eyeMesh);

	Transform testCubeTransform;
	testCubeTransform.GetPosition().y = 10;
	testCubeTransform.SetUniformScale(3.0f);
	StaticMesh* testCubes = new StaticMesh(testCubeTransform, "./res/TestCubes.fbx");
	testCubes->SetMaterial(EngineStatics::GetDefaultMaterial());
	S_Engine->GetWorld()->RegisterEntity(testCubes);

	Transform headTransform;
	headTransform.GetPosition().y = 10;
	headTransform.SetUniformScale(3.0f);
	SAsset* headAsset = S_Engine->GetAssetManager()->GetAsset("./Res/Assets/Head.sasset");
	StaticMesh* head = headAsset->GetAsStaticMesh();
	head->SetTransform(headTransform);
	S_Engine->GetWorld()->RegisterEntity(head);

	Transform headTransform2;
	headTransform2.GetPosition().y = 10;
	headTransform2.GetPosition().x = 1;
	headTransform2.SetUniformScale(3.0f);
	StaticMesh* head2 = headAsset->GetAsStaticMesh();
	head2->SetTransform(headTransform2);
	S_Engine->GetWorld()->RegisterEntity(head2);

	//Transform outerEyeTransform;
	//outerEyeTransform.SetUniformScale(3.0f);
	//outerEyeTransform.SetRotation(-60, 0, 0);
	//outerEyeTransform.GetPosition().y = 10;

	//Texture2D* outerRoughness = new Texture2D("res/Eye/T_EyeClearRoughness.tga");
	//Texture2D* outerNormal = new Texture2D("res/Eye/T_EyeClearNormal.tga");

	//Material* outerEyeMaterial = new Material("./Res/Shaders/TranslucentShader");
	//outerEyeMaterial->SetShaderModel(EShaderModel::TRANSLUCENT);
	//outerEyeMaterial->SetVec3Parameter("Albedo", vec3(1.0, 1.0, 1.0));
	//outerEyeMaterial->SetScalarParameter("Opacity", 0.2f);
	//StaticMesh* outerEye = new StaticMesh(outerEyeTransform, outerEyeMaterial, "./res/Eye/EyeClear.fbx");
	//S_Engine->GetWorld()->RegisterEntity(outerEye);

	Grid grid(40, 2);
	S_Engine->GetWorld()->RegisterEntity(&grid);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				float r = (float)(rand()) / (float)(RAND_MAX);
				float g = (float)(rand()) / (float)(RAND_MAX);
				float b = (float)(rand()) / (float)(RAND_MAX);
				Transform tempTransform;
				tempTransform.GetPosition().x = (float)j * 2 - 2.0f;
				tempTransform.GetPosition().y = 10+(float)k * 3.0;
				tempTransform.GetPosition().z = (float)i * 2 - 2.0f;
				vec3 tempColor = vec3(r, g, b);
				float atten = (((float)(rand()) / (float)(RAND_MAX))+1)*2;
				tempTransform.SetUniformScale(0.25f);
				Light* tempLight = new Light(tempTransform, POINT, 45, tempColor, atten);
				tempLight->ToggleDebug(false);
				S_Engine->GetWorld()->RegisterLight(tempLight);
			}
		}
	}

	Transform fillLightTransform;
	fillLightTransform.SetRotation(180, 0, 0);
	fillLightTransform.GetPosition().y = 50;
	Light* fillLight = new Light(fillLightTransform, DIRECTIONAL, 800, vec3(0.75, 0.9, 0.8));
	fillLight->ToggleDebug(false);
	S_Engine->GetWorld()->RegisterLight(fillLight);

	//TwAddVarRW(S_Engine->GetInfoBar(), "Light Color", TW_TYPE_COLOR3F, &fillLight->GetLightInfo().Color, " opened=true help='Main Light Color' ");
	//Transform bounceLightTransform;
	//bounceLightTransform.GetPosition().y = -20;
	//Light* bounceLight = new Light(bounceLightTransform, DIRECTIONAL, 500, vec3(0.75, 0.9, 0.8));
	//bounceLight->ToggleDebug(true);
	//S_Engine->GetWorld()->RegisterLight(bounceLight);

	S_Engine->StartEngine();
	return 0;
}