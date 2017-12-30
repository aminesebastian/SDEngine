#include "FAABB.h"
#include "Shader.h"
#include "EngineStatics.h"
#include "Camera.h"

FAABB::FAABB(SArray<vec3> Verticies) {
	vec3 min = vec3(0, 0, 0);
	vec3 max = vec3(0, 0, 0);

	for (int i = 0; i<Verticies.size(); i++) {
		if (Verticies[i].x < min.x) {
			min.x = Verticies[i].x;
		}
		if (Verticies[i].y < min.y) {
			min.y = Verticies[i].y;
		}
		if (Verticies[i].z < min.z) {
			min.z = Verticies[i].z;
		}
		if (Verticies[i].x > max.x) {
			max.x = Verticies[i].x;
		}
		if (Verticies[i].y > max.y) {
			max.y = Verticies[i].y;
		}
		if (Verticies[i].z > max.z) {
			max.z = Verticies[i].z;
		}
	}

	S_AABBMin = min;
	S_AABBMax = max;
}
FAABB::FAABB(vec3 Min, vec3 Max) : S_AABBMin(Min), S_AABBMax(Max) {

}
FAABB::~FAABB() {}

vec3 FAABB::GetMinExtent() {
	return S_AABBMin;
}
vec3 FAABB::GetMaxExtent() {
	return S_AABBMax;
}

bool FAABB::IntersectWithRay(vec3 RayOrigin, vec3 RayDirection, Transform OwnerTransform, float& IntersectionDistance, vec3& CollisionPoint) {
	float tMin = 0.0f;
	float tMax = 100000.0f;
	mat4 ModelMatrix = OwnerTransform.GetModelMatrix();
	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - RayOrigin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(RayDirection, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + S_AABBMin.x) / f; // Intersection with the "left" plane
			float t2 = (e + S_AABBMax.x) / f; // Intersection with the "right" plane
											  // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											  // We want t1 to represent the nearest intersection, 
											  // so if it's not the case, invert t1 and t2
			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + S_AABBMin.x > 0.0f || -e + S_AABBMax.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(RayDirection, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + S_AABBMin.y) / f;
			float t2 = (e + S_AABBMax.y) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + S_AABBMin.y > 0.0f || -e + S_AABBMax.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(RayDirection, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + S_AABBMin.z) / f;
			float t2 = (e + S_AABBMax.z) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + S_AABBMin.z > 0.0f || -e + S_AABBMax.z < 0.0f)
				return false;
		}
	}

	IntersectionDistance = tMin;
	CollisionPoint = RayOrigin + (RayDirection * IntersectionDistance);
	return true;
}
void FAABB::DrawDebug(vec3 Color, Camera* Camera, Transform OwnerTransform) {
	EngineStatics::GetLineShader()->Bind();
	EngineStatics::GetLineShader()->SetShaderVector3("COLOR", Color);
	EngineStatics::GetLineShader()->Update(OwnerTransform, Camera);
	glBegin(GL_LINES);

	//Bottom
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);

	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);

	//Top
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	//Sides
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);

	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	glEnd();
}