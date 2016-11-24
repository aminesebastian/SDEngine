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
#include "Light.h"

#undef main
using namespace glm;

float deltaTime = 0.0f;
float worldTime = 0.0f;
std::vector<Entity*> entityList;
std::vector<Light*> lightList;
Display display(1280, 720, "SD Engine", 8);
GBuffer S_Buffer;
DefferedCompositor S_DefferedCompositor;
Shader shader("./Res/GeometryPassShader");
int lastMouseX = 0;
int lastMouseY = 0;
Camera* camera;
GLuint quadVAO = 0;
GLuint quadVBO;

float movementSpeed = 0.05f;
float lookSpeed = 200.0f;

struct FKeyInfo {
	bool bWDown;
	bool bADown;
	bool bSDown;
	bool bDDown;
	bool bQDown;
	bool bEDown;
};
FKeyInfo keyInfo;

void StatUnit();
void DebugGBuffer();
void LightPass();
void Movement();
void RenderQuad();

int main(int argc, char* argv[]) {
	Texture2D albdeo("./res/T_BookBaseColor.tga");
	Texture2D roughness("./res/T_BookRMAO.tga", "tex_RMAO");
	Texture2D normal("./res/T_BookNormal.tga", "tex_Normal");

	Transform cameraTransform(vec3(0, 0, 3));
	cameraTransform.SetRotation(0, -180, 0);
	camera = &Camera(cameraTransform, 70, display.GetAspectRatio(), 0.01f, 1000.0f);

	Transform lightTransform;
	lightTransform.GetPosition().y = 2.0f;
	lightTransform.SetUniformScale(0.02f);
	Light tempLight(lightTransform, 5);
	lightList.push_back(&tempLight);

	Transform transform;
	transform.SetUniformScale(0.02f);
	transform.GetRotation().y = 6.2;

	StaticMesh book1(transform, "./res/Book.fbx");
	book1.RegisterTexture(&roughness);
	book1.RegisterTexture(&albdeo);
	book1.RegisterTexture(&normal);
	entityList.push_back(&book1);
	
	transform.GetPosition().y = 1;
	StaticMesh book2(transform, "./res/Book.fbx");
	book2.RegisterTexture(&roughness);
	book2.RegisterTexture(&albdeo);
	book2.RegisterTexture(&normal);
	entityList.push_back(&book2);

	StaticMesh arrow(lightTransform, "./res/Arrow.fbx");
	entityList.push_back(&arrow);

	long last = 0;
	int currentCount = 0;
	int debugCount = 5;


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	S_Buffer.Init(display.GetDimensions().x, display.GetDimensions().y);
	while (!display.IsClosed()) {	

		S_Buffer.BindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		glEnable(GL_DEPTH_TEST);
		for (int i = 0; i < entityList.size(); i++) {	
			shader.Update(entityList[i]->GetTransform(), *camera);
			entityList[i]->Draw(shader);
			entityList[i]->GetTransform().SetRotation(worldTime * 10, 0, 0);
		}
		//tempLight.GetTransform().SetRotation(worldTime * 10, 0, 0);
		//arrow.GetTransform().GetRotation() = tempLight.GetTransform().GetRotation();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		S_DefferedCompositor.Composite(&S_Buffer, lightList, camera);
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

	S_Buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_RMAO);
	glBlitFramebuffer(0, 0, display.GetDimensions().x, display.GetDimensions().y, ThirdWidth, 0, display.GetDimensions().x, ThirdHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}
void Movement() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			display.CloseDisplay();
		}
		if (e.type == SDL_MOUSEMOTION) {
			if (e.motion.state & SDL_BUTTON_RMASK) {
				camera->AddOrbit((float)(e.motion.y - lastMouseY) / lookSpeed, -(float)(e.motion.x - lastMouseX) / lookSpeed);
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}else if (e.motion.state & SDL_BUTTON_MMASK) {
				camera->GetCameraTransform().GetPosition().x -= (float)(e.motion.x - lastMouseX) / 250.0f;
				camera->GetCameraTransform().GetPosition().y += (float)(e.motion.y - lastMouseY) / 250.0f;
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}else {
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			movementSpeed = clamp(movementSpeed + ((float)e.wheel.y / 100.0f), 0.01f, 0.5f);
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN]) {
			shader.RecompileShader();
			S_DefferedCompositor.GetLightingShader().RecompileShader();
		}
		keyInfo.bWDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W];
		keyInfo.bSDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S];
		keyInfo.bDDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A];
		keyInfo.bADown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D];
		keyInfo.bQDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q];
		keyInfo.bEDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E];
	}
	if (keyInfo.bWDown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() + camera->GetCameraTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bSDown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() - camera->GetCameraTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bADown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() - camera->GetCameraTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bDDown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() + camera->GetCameraTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bQDown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() - camera->GetCameraTransform().GetUpVector() * movementSpeed;
	}
	if (keyInfo.bEDown) {
		camera->GetCameraTransform().GetPosition() = camera->GetCameraTransform().GetPosition() + camera->GetCameraTransform().GetUpVector() * movementSpeed;
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
		 //Setup plane VAO
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
