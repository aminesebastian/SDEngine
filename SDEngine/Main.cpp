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

#undef main
using namespace glm;

Engine* S_Engine;

/*
Physics
Collision
AI/Pathfinding
Resource Management
Content Creation

How to implement? (IMO)
What are concerns you have about it?


*/

int main(int argc, char* argv[]) {
	S_Engine = new Engine();
	if (!S_Engine->Init()) {		
		return 0;
	}
	init_logger("SD_EngineLog.txt");
	PrintToLog("Engine Launched!");

	Texture2D torusAlbedo("res/T_TorusBaseColor.tga");
	Texture2D torusRoughness("res/T_TorusRMAO.tga", "tex_RMAO");
	Texture2D torusNormal("res/T_TorusNormal.tga", "tex_Normal");

	Transform torusTransform;
	torusTransform.SetUniformScale(5.0f);
	StaticMesh torus(torusTransform, "./res/Torus.fbx");
	torus.RegisterTexture(&torusAlbedo);
	torus.RegisterTexture(&torusRoughness);
	torus.RegisterTexture(&torusNormal);
	S_Engine->GetWorld()->RegisterEntity(&torus);


	Transform transform;
	Texture2D albdeo("res/Eye/T_EyeBaseColor.png");
	Texture2D roughness("res/Eye/T_EyeRMAO.png", "tex_RMAO");
	Texture2D normal("res/Eye/T_EyeNormal.png", "tex_Normal");

	transform.SetUniformScale(3.0f);
	transform.SetRotation(-60, 0, 0);
	transform.GetPosition().y = 10;
	//transform.GetPosition().x = -6;
	StaticMesh eye(transform, "./res/Eye/Eye.fbx");
	//eye.RegisterTexture(&roughness);
	eye.RegisterTexture(&albdeo);
	eye.RegisterTexture(&normal);
	eye.SetCustomRoughness(0);
	S_Engine->GetWorld()->RegisterEntity(&eye);


	Grid grid(40, 2);
	S_Engine->GetWorld()->RegisterEntity(&grid);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			float r = (float)(rand()) / (float)(RAND_MAX);
			float g = (float)(rand()) / (float)(RAND_MAX);
			float b = (float)(rand()) / (float)(RAND_MAX);
			Transform tempTransform;
			tempTransform.GetPosition().x = (float)j*10- 50.0f;
			//tempTransform.GetPosition().y = ((float)(rand()) / (float)(RAND_MAX))*2 + 5;
			tempTransform.GetPosition().z = (float)i*10 - 50.0f;
			vec3 tempColor = vec3(r, g, b);
			float atten = ((float)(rand()) / (float)(RAND_MAX))*50;
			Light* tempLight = new Light(tempTransform, 20, tempColor, atten);
			tempLight->ToggleDebug(true);
			S_Engine->GetWorld()->RegisterLight(tempLight);
		}
	}
	//Transform tempTransform;
	//vec3 tempColor = vec3(1.0, 0.8, 0.1);
	//Light* tempLight = new Light(tempTransform, 100, tempColor, 100);
	//S_Engine->GetWorld()->RegisterLight(tempLight);
	S_Engine->StartEngine();
	return 0;
}