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
#include "DefferedCompositor.h"
#include "Material.h"

#undef main
using namespace glm;

float deltaTime = 0.0f;
float worldTime = 0.0f;
std::vector<Entity*> entityList;
Display display(1280, 720, "SD Engine", 8);
GBuffer S_Buffer;
DefferedCompositor S_DefferedCompositor;
Shader shader("./Res/DefferedShader");
int lastMouseX = 0;
int lastMouseY = 0;
Camera camera(vec3(0, 0, 3), vec3(0, 0, -1), 70, display.GetAspectRatio(), 0.01f, 1000.0f);
GLuint quadVAO = 0;
GLuint quadVBO;

void StatUnit();
void DebugGBuffer();
//void LightPass();
void Movement();
void RenderQuad();

int main(int argc, char* argv[]) {
	Texture2D albdeo("./res/T_BookBaseColor.tga");
	Texture2D roughness("./res/T_BookRMAO.tga", "tex_RMAO");
	Texture2D normal("./res/T_BookNormal.tga", "tex_Normal");

	vec3 ambientColor(1.0, 1.0, 0.9);
	vec3 directionalLight(0.0, 0.0, 1.0);
	float ambientIntensity = 0.1f;
	float directionalIntensity = 15.0f;

	Transform transform1;
	transform1.GetScale().x = 0.04f;
	transform1.GetScale().y = 0.04f;
	transform1.GetScale().z = 0.04f;
	transform1.GetRotation().y = 6.2;

	SDL_GL_SetSwapInterval(-1);
	StaticMesh monkeyHead(transform1, "./res/Book.fbx");
	monkeyHead.RegisterTexture(&roughness);
	monkeyHead.RegisterTexture(&albdeo);
	monkeyHead.RegisterTexture(&normal);

	entityList.push_back(&monkeyHead);
	
	long last = 0;
	int currentCount = 0;
	int debugCount = 5;

	S_Buffer.Init(display.GetDimensions().x, display.GetDimensions().y);

	while (!display.IsClosed()) {	
		S_Buffer.BindForWriting();
		display.Clear(0.4, 0.1, 0.1, 1);

		glEnable(GL_DEPTH_TEST);
		for (int i = 0; i < entityList.size(); i++) {	
			shader.Update(entityList[i]->GetTransform(), camera);
			entityList[i]->Draw(shader);
		}
		glDisable(GL_DEPTH_TEST);
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		display.Clear(0.1, 0.1, 0.5, 1.0);
		S_DefferedCompositor.Composite(&S_Buffer);

		//DebugGBuffer();

		display.Update();

		long now = SDL_GetTicks();
		deltaTime = ((float)(now - last))/1000;
		last = now;

		if (currentCount >= debugCount) {
			//StatUnit();
			currentCount = 0;
		}
		currentCount++;
		worldTime += deltaTime;
		Movement();
	}
	return 0;
}


void StatUnit() {
	system("cls");
	std::cout << "Delta Time: " << deltaTime * 1000 << "ms" << std::endl;
	std::cout << "Frame Rate: " << 1.0f / deltaTime << "fps" << std::endl;
}
void DebugGBuffer() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	S_Buffer.BindForReading();

	GLint ThirdWidth = (GLint)(display.GetDimensions().x / 2.0f);
	GLint ThirdHeight = (GLint)(display.GetDimensions().y / 2.0f);

	GLint FirstThirdWidth = (GLint)(display.GetDimensions().x / 2.0f);
	GLint FirstThirdHeight = (GLint)(display.GetDimensions().y / 2.0f);
	GLint SecondThirdWidth = (GLint)(display.GetDimensions().x / 2.0f);
	GLint SecondThirdHeight = (GLint)(display.GetDimensions().y / 2.0f);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_ALBEDO);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, 0, ThirdHeight, ThirdWidth, display.GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, 0, 0, ThirdWidth, ThirdHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, ThirdWidth, ThirdHeight, display.GetDimensions().x, display.GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_AO);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, ThirdWidth, 0, display.GetDimensions().x, ThirdHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}
void Movement() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			display.CloseDisplay();
		}
		if (e.type == SDL_KEYDOWN) {
			shader.RecompileShader();
			S_DefferedCompositor.S_LightingShader.RecompileShader();
		}
		if (e.type == SDL_MOUSEMOTION) {
			if (e.motion.state & SDL_BUTTON_LMASK) {
				entityList[0]->GetTransform().GetRotation().y += (float)(e.motion.x - lastMouseX) / 250.0f;
				entityList[0]->GetTransform().GetRotation().x += (float)(e.motion.y - lastMouseY) / 250.0f;

				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
			else if (e.motion.state & SDL_BUTTON_MMASK) {
				entityList[0]->GetTransform().GetPosition().x += (float)(e.motion.x - lastMouseX) / 250.0f;
				entityList[0]->GetTransform().GetPosition().y -= (float)(e.motion.y - lastMouseY) / 250.0f;
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
			else {
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			camera.GetCameraPosition().z = clamp(camera.GetCameraPosition().z - ((float)e.wheel.y / 35.0f), 1.5f, 3.0f);
		}
	}
}
void RenderQuad() {
	if (quadVAO == 0) {
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
