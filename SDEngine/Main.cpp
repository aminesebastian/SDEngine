#include <iostream>
#include <GLEW/glew.h>
#include "Display.h"
#include "Shader.h"
#include "StaticMesh.h"

#undef main
int main(int argc, char* argv[]) {
	Display display(1280, 720, "SD Engine", 8);
	Shader shader("./Res/BasicShader");

	Vertex verticies[] = { Vertex(glm::vec3(-0.5,-0.5,0)) ,
							Vertex(glm::vec3(0,0.5,0)) ,
							Vertex(glm::vec3(0.5,-0.5,0)) };

	StaticMesh mesh(verticies, sizeof(verticies) / sizeof(verticies[0]));

	while (!display.IsClosed()) {
		display.Clear(0, 0.75f, 1, 1);

		shader.Bind();
		mesh.Draw();
		display.Update();
	}

	return 0;
}