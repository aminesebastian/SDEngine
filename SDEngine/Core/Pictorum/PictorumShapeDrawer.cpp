#include "PictorumShapeDrawer.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

PictorumShapeDrawer::PictorumShapeDrawer(const TString& Name) : EngineObject(Name) {
	QuadVertexBuffer = new GPUVertexBufferArray();

	SArray<Vector3D> verticies;
	verticies.Emplace(0.0f, 1.0f, 0.0f);
	verticies.Emplace(0.0f, 0.0f, 0.0f);
	verticies.Emplace(1.0f, 1.0f, 0.0f);
	verticies.Emplace(1.0f, 0.0f, 0.0f);

	SArray<Vector2D> texCoords;
	texCoords.Emplace(0.0f, 1.0f);
	texCoords.Emplace(0.0f, 0.0f);
	texCoords.Emplace(1.0f, 1.0f);
	texCoords.Emplace(1.0f, 0.0f);

	QuadVertexBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	QuadVertexBuffer->AddBuffer("TexCoord", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	QuadVertexBuffer->SetBufferData(0, verticies, verticies.Count());
	QuadVertexBuffer->SetBufferData(1, texCoords, texCoords.Count());
	QuadVertexBuffer->Update();
}
PictorumShapeDrawer::~PictorumShapeDrawer() {

}

void PictorumShapeDrawer::DrawQuad(const FRenderGeometry& Geometry, const FBoxDrawInstruction& Instruction) const {
	Shader* shader = EngineStatics::GetUISolidShader();
	shader->Bind();

	Vector2D ndcLocation = ((Instruction.Location / Geometry.GetRenderResolution()) * 2.0f) - 1.0f;
	Vector2D relativeSize = Instruction.Size / Geometry.GetRenderResolution();

	shader->SetShaderVector2("RENDER_TARGET_RESOLUTION", Geometry.GetRenderResolution());
	shader->SetShaderVector2("LOCATION", Instruction.Location);
	shader->SetShaderVector2("CENTER_LOCATION", Instruction.Location + (Instruction.Size / 2.0f));
	shader->SetShaderVector2("NDC_LOCATION", ndcLocation);
	shader->SetShaderVector2("RELATIVE_SIZE", relativeSize);
	shader->SetShaderVector2("SIZE", Instruction.Size);
	shader->SetShaderVector4("COLOR", Instruction.BackgroundColor);
	shader->SetShaderFloat("EDGE_SOFTNESS", Instruction.EdgeSoftness);
	shader->SetShaderVector4("BORDER_RADIUS", Instruction.BorderRadius.GetRadii());
	shader->SetShaderFloat("BORDER_THICKNESS", Instruction.BorderThickness);
	shader->SetShaderFloat("BORDER_SOFTNESS", Instruction.BorderSoftness);
	
	glDisable(GL_DEPTH_TEST);
	QuadVertexBuffer->DrawTriangleStrip(0, 4);
	glEnable(GL_DEPTH_TEST);
}
void PictorumShapeDrawer::DrawImage(const FRenderGeometry& Geometry, const FImageDrawInstruction& Instruction) const {
	Shader* shader = EngineStatics::GetUIImageShader();
	shader->Bind();

	Vector2D ndcLocation = ((Instruction.Location / Geometry.GetRenderResolution()) * 2.0f) - 1.0f;
	Vector2D relativeSize = Instruction.Size / Geometry.GetRenderResolution();

	shader->SetShaderVector2("NDC_LOCATION", ndcLocation);
	shader->SetShaderVector2("RELATIVE_SIZE", relativeSize);
	shader->SetShaderVector4("TINT", Instruction.Tint);

	if (Instruction.Texture) {
		Instruction.Texture->Bind("IMAGE", shader, 0);
	}

	glDisable(GL_DEPTH_TEST);
	QuadVertexBuffer->DrawTriangleStrip(0, 4);
	glEnable(GL_DEPTH_TEST);
}