#include "Grid.h"
#include "Engine/Engine.h"


void Grid::Draw(Camera* Camera, bool bCallerProvidesShader) {
	S_GridMaterial->BindMaterial(this, Camera);
	glBegin(GL_LINES);
	for (int i = -S_HalfSize; i <= S_HalfSize; i++){
		if ((i+10)%S_StepInterval == 0 || i == S_HalfSize || i == -S_HalfSize) {
			glVertex3f((float)i, 0, (float)-S_HalfSize);
			glVertex3f((float)i, 0, (float)S_HalfSize);

			glVertex3f((float)-S_HalfSize, 0, (float)i);
			glVertex3f((float)S_HalfSize, 0, (float)i);
		}
	}
	glEnd();
}