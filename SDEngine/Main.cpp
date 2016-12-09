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
#include <random>
#include "Grid.h"

#undef main
using namespace glm;

float deltaTime = 0.0f;
float worldTime = 0.0f;
int frameRate = 0.016f;
std::vector<Entity*> entityList;
std::vector<Light> lightList;
Display display(1280, 720, "SD Engine", 8);
GBuffer S_Buffer;
DefferedCompositor S_DefferedCompositor;
Shader shader("./Res/GeometryPassShader");
int lastMouseX = 0;
int lastMouseY = 0;
Camera* camera;
GLuint quadVAO = 0;
GLuint quadVBO;
World S_World;

float movementSpeed = 0.5f;
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
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(display.GetDimensions().x, display.GetDimensions().y);

	TwBar *infoBar;
	infoBar = TwNewBar("StatUnit");
	TwDefine(" StatUnit refresh=0.1 ");
	TwDefine(" StatUnit alpha=10 ");
	TwAddVarRO(infoBar, "Frame Time", TW_TYPE_FLOAT, &deltaTime, "");
	TwAddVarRO(infoBar, "Frame Rate", TW_TYPE_INT32, &frameRate, "");

	Texture2D albdeo("./res/Eye/T_EyeBaseColor.png");
	Texture2D roughness("./res/Eye/T_EyeRoughness.png", "tex_RMAO");
	Texture2D normal("./res/Eye/T_EyeNormal.png", "tex_Normal");

	Transform cameraTransform(vec3(0, 15, 5));
	cameraTransform.SetRotation(50, -180, 0);
	camera = &Camera(S_World, cameraTransform, 70, display.GetAspectRatio(), 0.01f, 1000.0f);

	Transform transform;
	transform.SetUniformScale(3.0f);
	transform.SetRotation(-60, 0, 0);
	transform.GetPosition().y = 10;
	transform.GetPosition().x = -6;
	StaticMesh eye(S_World, transform, "./res/Eye/Eye.fbx");
	eye.RegisterTexture(&roughness);
	eye.RegisterTexture(&albdeo);
	eye.RegisterTexture(&normal);
	entityList.push_back(&eye);

	Transform transform2;
	transform2.SetUniformScale(3.0f);
	transform2.SetRotation(-60, 0, 0);
	transform2.GetPosition().y = 10;
	transform2.GetPosition().x = 6;
	StaticMesh eye2(S_World, transform2, "./res/Eye/Eye.fbx");
	eye2.RegisterTexture(&roughness);
	eye2.RegisterTexture(&albdeo);
	eye2.RegisterTexture(&normal);
	entityList.push_back(&eye2);

	//Texture2D headTex("./res/Head/lambertian.jpg");
	//Transform transform;
	//transform.SetUniformScale(10.0f);
	////transform.SetRotation(90, 0, 0);
	//StaticMesh head(S_World, transform, "./res/Head/Head.fbx");
	//head.RegisterTexture(&headTex);
	//entityList.push_back(&head);

	Grid grid(40, 2);

	long last = 0;
	int currentCount = 0;
	int debugCount = 5;


	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			float r = (float)(rand()) / (float)(RAND_MAX);
			float g = (float)(rand()) / (float)(RAND_MAX);
			float b = (float)(rand()) / (float)(RAND_MAX);
			Transform tempTransform;
			tempTransform.GetPosition().x = (float)j*3 - 12.0f;
			tempTransform.GetPosition().y = ((float)(rand()) / (float)(RAND_MAX)) + 3;
			tempTransform.GetPosition().z = (float)i*3 - 13.5f;
			vec3 tempColor = vec3(r, g, b);
			float atten = ((float)(rand()) / (float)(RAND_MAX))*5;
			Light tempLight = Light(S_World, tempTransform, 20, tempColor, atten);
			lightList.push_back(tempLight);
		}
	}

/*	Transform tempLightTrasform;
	tempLightTrasform.GetPosition().y = 5;
	float atten = ((float)(rand()) / (float)(RAND_MAX))*5;
	Light tempLight = Light(S_World, tempLightTrasform, 100, vec3(1,1,1), atten);
	lightList.push_back(tempLight)*/;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	S_Buffer.Init(display.GetDimensions().x, display.GetDimensions().y);
	while (!display.IsClosed()) {	

		S_Buffer.BindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		glEnable(GL_DEPTH_TEST);
		for (int i = 0; i < entityList.size(); i++) {	
			shader.Update(entityList[i]->GetTransform(), *camera);
			if (entityList[i]->IsVisible()) {
				entityList[i]->Draw(shader);
			}
			if (entityList[i]->NeedsTick()) {
				entityList[i]->Tick(deltaTime);
			}
		}

		shader.Update(grid.GetTransform(), *camera);
		//grid.Draw(shader);
		for (int i = 0; i < lightList.size(); i++) {
			if (i % 3 == 0) {
				lightList[i].GetTransform().GetPosition().x = lightList[i].GetInitialTransform().GetPosition().x + sin(worldTime + (i/5.0f)) * 2;
				lightList[i].GetTransform().GetPosition().z = lightList[i].GetInitialTransform().GetPosition().z + cos(worldTime + (i / 5.0f)) * 2;
				lightList[i].SetLightIntensity(cos(worldTime + (i / 5.0f)) + 1.5f);
			}else if (i % 5 == 0) {
				lightList[i].GetTransform().GetPosition().x = lightList[i].GetInitialTransform().GetPosition().x + cos(worldTime + (i / 5.0f)) * 2;
				lightList[i].GetTransform().GetPosition().z = lightList[i].GetInitialTransform().GetPosition().z + sin(worldTime + (i / 5.0f)) * 2;
				lightList[i].SetLightIntensity(sin(worldTime + (i / 5.0f)) + 1.5f);
			}else {
				lightList[i].GetTransform().GetPosition().x = lightList[i].GetInitialTransform().GetPosition().x + cos(worldTime + (i / 10.0f)) * 2;
				lightList[i].GetTransform().GetPosition().z = lightList[i].GetInitialTransform().GetPosition().z + sin(worldTime + (i / 10.0f)) * 2;
				lightList[i].SetLightIntensity(sin(worldTime + (i / 10.0f)) + 2.0f);
			}
		}

		S_DefferedCompositor.CompositeLighting(&S_Buffer, lightList, camera);
		S_DefferedCompositor.CompositePostProcesing(&S_Buffer, camera);
		long now = SDL_GetTicks();
		deltaTime = ((float)(now - last)) / 1000;
		frameRate = 1.0f / deltaTime;
		last = now;

		if (currentCount >= debugCount) {
			//StatUnit();
			currentCount = 0;
		}
		currentCount++;
		worldTime += deltaTime;

		TwDraw();
		display.Update();

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
				SDL_ShowCursor(0);
				camera->AddOrbit((float)(e.motion.y - lastMouseY) / lookSpeed, -(float)(e.motion.x - lastMouseX) / lookSpeed);
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}else if (e.motion.state & SDL_BUTTON_MMASK) {
				SDL_ShowCursor(0);
				camera->GetTransform().GetPosition().x -= (float)(e.motion.x - lastMouseX) / 250.0f;
				camera->GetTransform().GetPosition().y += (float)(e.motion.y - lastMouseY) / 250.0f;
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}else {
				SDL_ShowCursor(1);
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			movementSpeed = clamp(movementSpeed + ((float)e.wheel.y / 100.0f), 0.01f, 2.5f);
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN]) {
			shader.RecompileShader();
			S_DefferedCompositor.GetLightingShader().RecompileShader();
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
			camera->SetTransform(camera->GetInitialTransform());
		}
		keyInfo.bWDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W];
		keyInfo.bSDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S];
		keyInfo.bDDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A];
		keyInfo.bADown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D];
		keyInfo.bQDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q];
		keyInfo.bEDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E];
	}
	if (keyInfo.bWDown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() + camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bSDown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() - camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bADown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() - camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bDDown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() + camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bQDown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() - camera->GetTransform().GetUpVector() * movementSpeed;
	}
	if (keyInfo.bEDown) {
		camera->GetTransform().GetPosition() = camera->GetTransform().GetPosition() + camera->GetTransform().GetUpVector() * movementSpeed;
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
