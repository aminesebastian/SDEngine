#pragma once
#include "Entity.h"


class Grid : public Entity {
public:
	Grid(int GridSize = 100, int GridStep = 10) :
		S_HalfSize(GridSize / 2),
		S_StepInterval(GridStep)
	{}
	~Grid() {}

	virtual void Draw(Shader& shader) override;
private:
	int S_HalfSize;
	int S_StepInterval;
};

