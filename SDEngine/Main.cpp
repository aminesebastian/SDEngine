#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Utilities/Logger.h"

#undef main
using namespace glm;

Engine* S_Engine;

int main(int argc, char* argv[]) {
	S_Engine = Engine::GetInstance();
	if (!S_Engine->Initialize()) {		
		return 0;
	}

	SD_ENGINE_INFO("Engine Launched!");

	Scene* scene = new Scene();
	S_Engine->LoadScene(scene);

	S_Engine->StartEngine();

	return 0;
}