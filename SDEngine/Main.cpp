#include <iostream>
#include <GLEW/glew.h>
#include "Display.h"
#include "Shader.h"
#include "Texture2D.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Camera.h"
#include <AntTweakBar.h>
#include "GBuffer.h"
#include "Material.h"

#undef main
using namespace glm;

float deltaTime = 0.0f;
float worldTime = 0.0f;
std::vector<Entity*> entityList;
Display display(1680, 1050, "SD Engine", 8);
GBuffer S_Buffer;

void statUnit() {
	system("cls");
	std::cout << "Delta Time: " << deltaTime * 1000 << "ms" << std::endl;
	std::cout << "Frame Rate: " << 1.0f / deltaTime << "fps" << std::endl;
}
void DSLightPass() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	S_Buffer.BindForReading();

	GLint HalfWidth = (GLint)(display.GetDimensions().x / 2.0f);
	GLint HalfHeight = (GLint)(display.GetDimensions().y / 2.0f);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, 0, HalfHeight, HalfWidth, display.GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL); 
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_ALBEDO);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, HalfWidth, HalfHeight, display.GetDimensions().x, display.GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, HalfWidth, 0, display.GetDimensions().x, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}
int main(int argc, char* argv[]) {
	Shader shader("./Res/DefferedShader");
	Texture2D texture("./res/T_BookBaseColor.tga");

	vec3 ambientColor(1.0, 1.0, 0.9);
	vec3 directionalLight(0.0, 0.0, 1.0);
	float ambientIntensity = 0.1f;
	float directionalIntensity = 15.0f;

	Camera camera(vec3(0, 0, -3), 70, display.GetAspectRatio(), 0.01f, 1000.0f);


	Transform transform1;
	transform1.GetScale().x = 0.04f;
	transform1.GetScale().y = 0.04f;
	transform1.GetScale().z = 0.04f;
	transform1.GetRotation().y = 3.2;

	StaticMesh monkeyHead(transform1, "./res/Book.obj");
	entityList.push_back(&monkeyHead);

	long last = 0;
	int currentCount = 0;
	int debugCount = 5;
	int lastMouseX = 0;
	int lastMouseY = 0;

	S_Buffer.Init(display.GetDimensions().x, display.GetDimensions().y);

	while (!display.IsClosed()) {	
		shader.Bind();
		S_Buffer.BindForWriting();
		display.Clear(0.1, 0.1, 0.1, 1);

		for (int i = 0; i < entityList.size(); i++) {	
			texture.Bind(0);
			shader.Update(entityList[i]->GetTransform(), camera, directionalLight, ambientColor, ambientIntensity, ambientColor, directionalIntensity);
			entityList[i]->Draw();
		}

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				display.CloseDisplay();
			}
			if (e.type == SDL_KEYDOWN) {
				shader.RecompileShader();
			}
			if (e.type == SDL_MOUSEMOTION) {
				if (e.motion.state & SDL_BUTTON_LMASK) {
					entityList[0]->GetTransform().GetRotation().y += (float)(e.motion.x - lastMouseX) / 250.0f;
					entityList[0]->GetTransform().GetRotation().x += (float)(e.motion.y - lastMouseY) / 250.0f;

					lastMouseX = e.motion.x;
					lastMouseY = e.motion.y;
				}else if (e.motion.state & SDL_BUTTON_MMASK) {
					entityList[0]->GetTransform().GetPosition().x -= (float)(e.motion.x - lastMouseX) / 250.0f;
					entityList[0]->GetTransform().GetPosition().y -= (float)(e.motion.y - lastMouseY) / 250.0f;
					lastMouseX = e.motion.x;
					lastMouseY = e.motion.y;
				}else{
					lastMouseX = e.motion.x;
					lastMouseY = e.motion.y;
				}
			}
			if (e.type == SDL_MOUSEWHEEL) {
				camera.GetCameraPosition().z = clamp(camera.GetCameraPosition().z + ((float)e.wheel.y/35.0f), -3.0f, -1.5f);
			}
		}
		DSLightPass();
		display.Update();

		long now = SDL_GetTicks();
		deltaTime = ((float)(now - last))/1000;
		last = now;

		if (currentCount >= debugCount) {
			//statUnit();
			currentCount = 0;
		}
		currentCount++;
		worldTime += deltaTime;
	}
	return 0;
}
void DSGeometryPass() {
	S_Buffer.BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

