#include "World.h"



World::World() {
}
World::~World() {
}

void World::DrawWorld() {
	for (int i = 0; i < S_EntityList.size(); i++) {
		S_EntityList[i]->Draw();
	}
}