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
#include "Scene.h"
#include "RenderingEngine.h"
#include "EyeActor.h"
#include "AssetManager.h"
#include "EngineStatics.h"
#include "WorkerThread.h"

#undef main
using namespace glm;

Engine* S_Engine;

int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Init()) {		
		return 0;
	}
	init_logger("SD_EngineLog.txt");
	PrintToLog("Engine Launched!");

	Scene* scene = new Scene();
	S_Engine->LoadScene(scene);

	S_Engine->StartEngine();

	return 0;
}