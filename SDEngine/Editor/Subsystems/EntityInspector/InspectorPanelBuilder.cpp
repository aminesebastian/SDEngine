#include "InspectorPanelBuilder.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Pictorum/Widgets/CheckboxWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Reflection/Utilities/ReflectionHelpers.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/Transform.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelGenerator.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelManager.h"
#include "Editor/UserInterface/Inspector/TypeInspectors/FloatInspectorWidget.h"
#include "Editor/UserInterface/Inspector/TypeInspectors/VectorInspectorWidget.h"

InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionHandle& Target) : TargetWrapper(Target) {
	OwningContainer          = Owner;
	OwningBuilder            = nullptr;
	DefaultCategory          = nullptr;
	CurrentCategoryContainer = nullptr;
}
InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionHandle& Target, InspectorPanelBuilder* OwningBuilder) : TargetWrapper(Target), OwningBuilder(OwningBuilder) {
	OwningContainer          = Owner;
	DefaultCategory          = nullptr;
	CurrentCategoryContainer = nullptr;
}
InspectorPanelBuilder::~InspectorPanelBuilder() {
	for (InspectorPanelBuilder* build : ChildBuilders) {
		delete build;
	}
	ChildBuilders.Clear();
}
void InspectorPanelBuilder::AddControlForProperty(const PropertyHandle& Property) {
	// Skip if null.
	if (&Property == nullptr) {
		return;
	}
	// Skip if duplicate.
	if (AddedProperties.Contains(Property.GetRawProperty())) {
		return;
	}

	// Update the current category.
	CurrentCategoryContainer = GetCategoryWidgetContainer(Property.GetMetadata().GetCategory());

	// Add the separator.
	if (CurrentCategoryContainer->GetChildCount() > 0) {
		AssignNewToChildLocal(CurrentCategoryContainer, SeparatorWidget, sep, "Separator");
		sep->SetSize(0.0f, 5.0f);
	}

	// Raise the appropriate method for the property (if it is a simple data GetType(), raise the simple data GetType() method, otherwise raise the property builder.
	if (Property.GetType() == TypeResolver<Vector4D>::Get()) {
		AddControlForVector4DProperty(Property);
	} else if (Property.GetType() == TypeResolver<Vector3D>::Get()) {
		AddControlForVector3DProperty(Property);
	} else if (Property.GetType() == TypeResolver<Vector2D>::Get()) {
		AddControlForVector2DProperty(Property);
	} else if (Property.GetType() == TypeResolver<bool>::Get()) {
		AddControlForBoolProperty(Property);
	} else if (Property.GetType() == TypeResolver<float>::Get()) {
		AddControlForFloatProperty(Property);
	} else {
		IInspectorPanelGenerator* generator = InspectorPanelManager::Get()->GetGenerator(Property.GetType());
		ReflectionHandle newWrapper(Property.GetValue<void>(), Property.GetType());
		InspectorPanelBuilder* propertyBuilder = new InspectorPanelBuilder(CurrentCategoryContainer, newWrapper, IsNested() ? OwningBuilder : this); // Ensure the top level parent remains.
		generator->GenerateInspector(*propertyBuilder);
		ChildBuilders.Add(propertyBuilder);
	}

	// Add property as registered.
	AddedProperties.Add(Property.GetRawProperty());
}
void InspectorPanelBuilder::AddControlForVector4DProperty(const PropertyHandle& Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, vectorEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	labels.Add("W");
	vectorEditor->SetLabels(labels);
	vectorEditor->SetTarget(Property);
	vectorEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
	vectorEditor->OnPropertyUpdatedDelegate.Add<InspectorPanelBuilder, & InspectorPanelBuilder::PropertyUpdatedCallback>(this);
}
void InspectorPanelBuilder::AddControlForVector3DProperty(const PropertyHandle& Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, vectorEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	vectorEditor->SetLabels(labels);
	vectorEditor->SetTarget(Property);
	vectorEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
	vectorEditor->OnPropertyUpdatedDelegate.Add<InspectorPanelBuilder, & InspectorPanelBuilder::PropertyUpdatedCallback>(this);
}
void InspectorPanelBuilder::AddControlForVector2DProperty(const PropertyHandle& Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, vectorEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	vectorEditor->SetLabels(labels);
	vectorEditor->SetTarget(Property);
	vectorEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
	vectorEditor->OnPropertyUpdatedDelegate.Add<InspectorPanelBuilder, & InspectorPanelBuilder::PropertyUpdatedCallback>(this);
}
void InspectorPanelBuilder::AddControlForBoolProperty(const PropertyHandle& Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, CheckboxWidget, checkbox, "CheckboxWidget");
	checkbox->Bind(TargetWrapper.GetPropertyHandle(Property.GetPropertyName()).GetValue<bool>());
}
void InspectorPanelBuilder::AddControlForFloatProperty(const PropertyHandle& Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, FloatInspectorWidget, floatInspector, "FloatWidget");
	floatInspector->SetTarget(Property);
	floatInspector->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
	floatInspector->OnPropertyUpdatedDelegate.Add<InspectorPanelBuilder, & InspectorPanelBuilder::PropertyUpdatedCallback>(this);
}
PictorumVerticalBox* InspectorPanelBuilder::GetCategoryWidgetContainer(const TString& Category) {
	if (Category == "Default") {
		if (!DefaultCategory) {
			// If this is a nested class, simply allocate a vertical box and add default properties to that.
			// If this is a top level class, make a 'Default' collapsible category.
			if (IsNested()) {
				AssignNewToChild(OwningContainer, PictorumVerticalBox, DefaultCategory, "DefaultCategory");
				Categories.emplace(Category, DefaultCategory);
			} else {
				AssignNewToChildLocal(OwningContainer, CollapsingCategoryWidget, defaultCat, "DefaultCategory");
				defaultCat->SetCategoryLabel(Category);
				Categories.emplace(Category, defaultCat->GetContainer());
				DefaultCategory = defaultCat->GetContainer();
			}
		}
		return DefaultCategory;
	}
	if (Categories.count(Category) == 0) {
		AssignNewToChildLocal(OwningContainer, CollapsingCategoryWidget, currentCat, Category);
		currentCat->SetCategoryLabel(Category);
		Categories.emplace(Category, currentCat->GetContainer());
	}
	return Categories.at(Category);
}
PictorumHorizontalBox* InspectorPanelBuilder::GetPropertyContainer(const PropertyHandle& Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "HBox" + Property.GetInspectorName());
	AssignNewToChildLocal(hBox, TextWidget, label, "Label" + Property.GetInspectorName());
	label->SetText(Property.GetInspectorName());
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	if (IsNested()) {
		OwningBuilder->TotalPropertiesList.emplace(Property.GetRawProperty(), hBox);
	} else {
		TotalPropertiesList.emplace(Property.GetRawProperty(), hBox);
	}

	return hBox;
}
const ITypeDescriptor* InspectorPanelBuilder::GetTypeDescriptor() const {
	return TargetWrapper.GetType();
}
const ReflectionHandle& InspectorPanelBuilder::GetTarget() const {
	return TargetWrapper;
}
const bool InspectorPanelBuilder::IsNested() const {
	return OwningBuilder != nullptr;
}
void InspectorPanelBuilder::PropertyUpdatedCallback(const FProperty* Property) {
	if (IsNested()) {
		if (OwningBuilder->TargetWrapper.IsObject()) {
			const EngineObject* obj = OwningBuilder->TargetWrapper.Get<EngineObject>();
			RemoveConst(obj)->OnPropertyUpdated(Property);
		}
	} else {
		if (TargetWrapper.IsObject()) {
			const EngineObject* obj = TargetWrapper.Get<EngineObject>();
			RemoveConst(obj)->OnPropertyUpdated(Property);
		}
	}
}