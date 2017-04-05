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

	Texture2D albdeo("res/Eye/T_EyeBaseColor.png");
	Texture2D roughness("res/Eye/T_EyeRMAO.png", "tex_RMAO");
	Texture2D normal("res/Eye/T_EyeNormal.png", "tex_Normal");

	Transform transform;
	//StaticMesh plane(transform, "./res/TestPlane.fbx");
	//S_Engine->GetWorld()->RegisterEntity(&plane);

	transform.SetUniformScale(3.0f);
	transform.SetRotation(-60, 0, 0);
	transform.GetPosition().y = 10;
	//transform.GetPosition().x = -6;
	StaticMesh eye(transform, "./res/Eye/Eye.fbx");
	eye.RegisterTexture(&roughness);
	eye.RegisterTexture(&albdeo);
	eye.RegisterTexture(&normal);
	S_Engine->GetWorld()->RegisterEntity(&eye);

	Grid grid(40, 2);
	S_Engine->GetWorld()->RegisterEntity(&grid);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			float r = (float)(rand()) / (float)(RAND_MAX);
			float g = (float)(rand()) / (float)(RAND_MAX);
			float b = (float)(rand()) / (float)(RAND_MAX);
			Transform tempTransform;
			tempTransform.GetPosition().x = (float)j*5 - 12.5f;
			tempTransform.GetPosition().y = ((float)(rand()) / (float)(RAND_MAX)) + 3;
			tempTransform.GetPosition().z = (float)i*5 - 12.5f;
			vec3 tempColor = vec3(r, g, b);
			float atten = ((float)(rand()) / (float)(RAND_MAX))*50;
			Light* tempLight = new Light(tempTransform, 100, tempColor, atten);
			tempLight->ToggleDebug(true);
			S_Engine->GetWorld()->RegisterLight(tempLight);
		}
	}
	S_Engine->StartEngine();
	return 0;
}