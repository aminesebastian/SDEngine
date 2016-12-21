#pragma once
#include "Entity.h"
#include <vector>

class Prefab : public Entity {

public:
	Prefab();
	~Prefab();

	virtual void Draw(Shader& shader) override;
	virtual void Tick(float DeltaTime) override;

	void AddComponent(Entity* Entity) {
		Children.push_back(Entity);
	}
	bool RemoveComponent(Entity* entity) {
		for(std::vector<Entity*>::iterator iter = Children.begin(); iter != Children.end(); ++iter) {
			if (*iter == entity) {
				Children.erase(iter);
				break;
			}
		}
	}
private:
	std::vector<Entity*> Children;
};

