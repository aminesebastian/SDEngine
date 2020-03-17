#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Pictorum/Utilities/TextLine.h"
#include "Core/Utilities/Logger.h"

class TextGeometryCache {
public:
	TextGeometryCache(const DistanceFieldFont* Font, const float& Leading, const float& Tracking, const ETextAlignment& Alignment);
	~TextGeometryCache();

	const int32 GetLineCount() const;
	const SArray<TextLine*>& GetLines() const;
	const TextLine* GetLine(const int32& Index) const;
	const float GetLineYOffset(const int32& Index) const;
	const SArray<Vector2D>& GetVerticies() const;
	const SArray<Vector2D>& GetTextureCoordinates() const;
	const SArray<int32>& GetInidices() const;
	const Vector2D GetMinimumPosition() const;
	const Vector2D GetMaximumPosition() const;

	void SetAligment(const ETextAlignment& AlignmentIn);
	void SetTracking(const float& TrackingIn);
	void SetLeading(const float& LeadingIn);
	void AddLine(const TString& Text);
	void Finalize();
	void Flush();
private:
	const DistanceFieldFont* Font;
	ETextAlignment Alignment;
	float Leading;
	float Tracking;

	SArray<TextLine*> Lines;

	Vector2D MaxPosition;
	Vector2D MinPosition;

	SArray<Vector2D> Verticies;
	SArray<Vector2D> TexCoords;
	SArray<int32> Indices;

	float CurrentYPosition;
	int32 CurrentIndex;

	bool bFinialized;
};