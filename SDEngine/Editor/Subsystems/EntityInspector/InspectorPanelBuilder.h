#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Core/Reflection/Reflection.h"

class PictorumWidget;

class InspectorPanelBuilder {
public:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, EngineObject* CustomizationTarget);

	template<typename T>
	const T* GetTarget() const {
		return Cast<T>(Target);
	}
	void AddControlForProperty(const FProperty& Property);

protected:
	virtual void AddControlForFloatProperty(const FProperty& Property);
	virtual void AddControlForVector2DProperty(const FProperty& Property);
	virtual void AddControlForVector3DProperty(const FProperty& Property);
	virtual void AddControlForVector4DProperty(const FProperty& Property);
	virtual void AddControlForBoolProperty(const FProperty& Property);
	virtual void AddControlForTransformProperty(const FProperty& Property);
private:
	PictorumVerticalBox* Parent;
	EngineObject* Target;
	int32 AddedPropertyCount;
};
