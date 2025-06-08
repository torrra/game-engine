#include "ui/components/TriangleMeshComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/Engine.h>
#include <engine/resource/model/Model.h>
#include <engine/core/components/Transform.h>

#pragma endregion

editor::TriangleMeshComponent::TriangleMeshComponent(void)
{
    SetName("TriangleMesh");
    SetType(TRIANGLE_MESH);
}

editor::TriangleMeshComponent::~TriangleMeshComponent(void)
{
}

void editor::TriangleMeshComponent::SectionContent(void)
{
    if (engine::TriangleMesh* triangleMesh = GetData<engine::TriangleMesh>())
    {
        const char* text = "Collider type: TriangleMesh"; // text for the label
        math::Vector2f textSize = ui::GetTextSize(text); // gets text size (in pixels)
        ui::Text(text); // render text
        ui::SameLine(textSize.GetX() + 5.0f);

        engine::TriangleMeshStaticFactory::CreateStatic(engine::Engine::GetEngine()->GetGraph(),
            triangleMesh->GetOwner());
    }
}
