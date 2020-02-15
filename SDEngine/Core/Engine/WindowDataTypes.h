#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include <GLM/glm.hpp>

struct FDisplayState {
	FDisplayState(const glm::vec2& Resolution, const glm::vec2& DotsPerInch, const float& DiagonalDotsPerInch, const int32& RefreshRate) : Resolution(Resolution), DotsPerInch(DotsPerInch), DiagonalDotsPerInch(DiagonalDotsPerInch), RefreshRate(RefreshRate) {}

	const glm::vec2& GetResolution() const {
		return Resolution;
	}
	const glm::vec2& GetDotsPerInch() const {
		return DotsPerInch;
	}
	const float& GetDiagonalDotsPerInch() const {
		return DiagonalDotsPerInch;
	}
private:
	const glm::vec2 Resolution;
	const glm::vec2 DotsPerInch;
	const float DiagonalDotsPerInch;
	const int32 RefreshRate;
};