#include "Grid.h"



void Grid::Draw(Shader& shader) {
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
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