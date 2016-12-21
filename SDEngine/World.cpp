#include "World.h"
#include "Light.h"


World::World() {
}
World::~World() {
}
void World::Tick(float DeltaTime) {
	//Optimize by Octree traversal to see if lights need to be updated. Exchanges performance for memory usage - worth.

	for (int i = 0; i < S_LightMapLights.size(); i++) {
		/*Do the Octree check and see if it passes.
		if(S_LightMapLights[i]->CachedOctreeValues != CurrentOctreeValues) {
			glm::vec3 tempTracedColor;
			glm::vec3 mostImportantImpactNormal;
			float averageRoughnessOfImpactSurface;
			bool tempIsEmptyInInfluence;

			S_LightMapLights[i]->ConeTraceAgainstEnvironment(tempTracedColor, mostImportantImpactNormal, averageRoughnessOfImpactSurface, tempIsEmptyInInfluence);

			if(!tempIsEmptyInInfluence) {
				S_LightMapLights[i]->MarkForSleep();
			}else{
				S_LightMapLights[i]->SetColor(tempTracedColor);
				S_LightMapLights[i]->SetRotation(MakeRotationFromXAxis(mostImportantImpactNormal));
				S_LightMapLights[i]->SetIntensity(averageRoughnessOfImpactSurface*arbitryScaleValue);
			}
			

		}*/
	}
}
void World::GetWorldBounds(glm::vec3& MinBound, glm::vec3& MaxBound) {
	//Optimize by storing entites in octree
	MinBound.x = S_EntityList[0]->GetTransform().GetPosition().x;
	MinBound.y = S_EntityList[0]->GetTransform().GetPosition().y;
	MinBound.z = S_EntityList[0]->GetTransform().GetPosition().z;
	
	MaxBound.x = 0; 
	MaxBound.y = 0;
	MaxBound.z = 0;
	

	for (int i = 0; i < S_EntityList.size(); i++) {
		if (S_EntityList[i]->GetTransform().GetPosition().x < MinBound.x) {
			MinBound.x = S_EntityList[i]->GetTransform().GetPosition().x;
		}
		if (S_EntityList[i]->GetTransform().GetPosition().y < MinBound.y) {
			MinBound.y = S_EntityList[i]->GetTransform().GetPosition().y;
		}
		if (S_EntityList[i]->GetTransform().GetPosition().z < MinBound.z) {
			MinBound.z = S_EntityList[i]->GetTransform().GetPosition().z;
		}

		if (S_EntityList[i]->GetTransform().GetPosition().x > MaxBound.x) {
			MaxBound.x = S_EntityList[i]->GetTransform().GetPosition().x;
		}
		if (S_EntityList[i]->GetTransform().GetPosition().y > MaxBound.y) {
			MaxBound.y = S_EntityList[i]->GetTransform().GetPosition().y;
		}
		if (S_EntityList[i]->GetTransform().GetPosition().z > MaxBound.z) {
			MaxBound.z = S_EntityList[i]->GetTransform().GetPosition().z;
		}
	}
}
void World::DrawWorld() {

}