#include "Quake2Module.h"
#include <cassert>
#include <iostream>
#include "ImageUtils.h"
#include "Camera.h"
#include "Light.h"

Quake2Module::Quake2Module() {
}
Quake2Module::~Quake2Module() {
}


void Quake2Module::GenerateLightMapLights(World* world, const std::string& LightMap, int YMin, int YMax) {
	int width, height, numComponents;
	unsigned char* imageData = stbi_load((LightMap).c_str(), &width, &height, &numComponents, 4);

	assert(imageData != NULL && numComponents == 4);

	glm::vec3 MinWorldBounds;
	glm::vec3 MaxWorldBounds;

	world->GetWorldBounds(MinWorldBounds, MaxWorldBounds);
	float worldToMapScale = (float)width / MinWorldBounds.x;
	float alphaMap = 1.0f;
	int alphaMin = 0;
	int alphaMax = 256;
	glm::vec4 prevColor;

	int spaceCount;
	for (int i = 0; i < width*height; i++) {
		glm::vec4 tempColor((int)imageData[3*i], (int)imageData[3 * i + 1], (int)imageData[3 * i + 2], (int)imageData[3 * i + 3]);

		if (tempColor != prevColor) {
			float XPos = i % width;
			float YPos = i / width;
			float ZPos = (YMin+YMax)/2.0f;
			Light tempLight(*world, Transform(glm::vec3(XPos, YPos, ZPos)), tempColor.w, glm::vec3(tempColor.x, tempColor.y, tempColor.z), worldToMapScale*spaceCount);

			if (tempColor.x < alphaMin) {
				alphaMin = tempColor.x;
			}
			if (tempColor.x > alphaMax) {
				alphaMax = tempColor.x;
			}
			prevColor = tempColor;
			spaceCount = 0;
		}else{
			spaceCount++;
		}
	}
	alphaMap = alphaMax / (alphaMin + 1);
}

void Quake2Module::DrawParticles(int num_particles, const particle_t particles[], const unsigned colortable[768], Camera* camera) {
	const particle_t	*p;
	int					i;
	glm::vec3			up, right;
	float				scale;
	unsigned char		color[4]; //From Byte

	for (p = particles, i = 0; i < num_particles; i++, p++) {
		// hack a scale up to keep particles from disapearing
		scale = (p->origin[0] - camera->GetTransform().GetPosition().x) * camera->GetTransform().GetForwardVector().x +
			(p->origin[1] - camera->GetTransform().GetPosition().y) * camera->GetTransform().GetForwardVector().y +
			(p->origin[2] - camera->GetTransform().GetPosition().z) * camera->GetTransform().GetForwardVector().z;

		if (scale < 20) {
			scale = 1;
		}else{
			scale = 1 + scale * 0.004;
		}

		*(int *)color = colortable[p->color];
		color[3] = p->alpha * 255;

		glColor4ubv(color);

		glTexCoord2f(0.0625, 0.0625);
		glVertex3fv(&p->origin[0]);

		glTexCoord2f(1.0625, 0.0625);
		glVertex3f(p->origin[0] + up[0] * scale,
			p->origin[1] + up[1] * scale,
			p->origin[2] + up[2] * scale);

		glTexCoord2f(0.0625, 1.0625);
		glVertex3f(p->origin[0] + right[0] * scale,
			p->origin[1] + right[1] * scale,
			p->origin[2] + right[2] * scale);
	}
}