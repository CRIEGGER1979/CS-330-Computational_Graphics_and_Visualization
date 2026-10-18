///////////////////////////////////////////////////////////////////////////////
// Speaker.cpp
// ============
// Custom 3D speaker object created for CS-330 Computational Graphics
// and Visualization at Southern New Hampshire University.
//
// Author: Christopher Riegger
// Date: October 2026
//
// This file implements the Speaker class, which encapsulates all rendering
// logic for the multi-component speaker object. The speaker consists of:
//   - A matte-plastic main housing (box mesh)
//   - A metallic front grill (box mesh)
//   - A multi-pass textured knob (cylinder mesh)
//
// All transformations, textures, and materials are applied here to maintain
// modularity and keep SceneManager clean and maintainable.
///////////////////////////////////////////////////////////////////////////////

#include "Speaker.h"
#include "SceneManager.h"

void Speaker::Initialize()
{
    // No initialization required.
    // All geometry, material, and texture logic is handled inside Render()
    // to keep object-specific rendering modular and self-contained.
}

void Speaker::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
        return;

    glm::vec3 scaleXYZ;
    float rotX, rotY, rotZ;
    glm::vec3 positionXYZ;

    // ============================================================
    // Speaker Body — Main Housing (Box Mesh)
    // ------------------------------------------------------------
    // The speaker body uses a matte plastic material that produces
    // soft diffuse shading and avoids harsh reflections. The height
    // is slightly increased (0.6f) to improve visual balance with
    // surrounding objects such as the Mac mini and sphere speaker.
    //
    // The body texture provides subtle surface detail for realism.
    // ============================================================
    scaleXYZ = glm::vec3(5.0f, 0.6f, 1.0f);   // Height increased from 0.5f → 0.6f
    rotX = 0.0f;
    rotY = 0.0f;
    rotZ = 0.0f;

    // Centered at y = 0.5 on the desk surface.
    positionXYZ = glm::vec3(0.0f, 0.5f, 3.0f);

    sceneManager->SetTransformations(scaleXYZ, rotX, rotY, rotZ, positionXYZ);
    sceneManager->SetShaderTexture(SceneManager::TEX_SPEAKER_BODY);
    sceneManager->SetShaderMaterial(SceneManager::MAT_SPEAKER_BODY);
    sceneManager->SetTextureUVScale(1.0f, 1.0f);
    sceneManager->GetMeshes()->DrawBoxMesh();

    // ============================================================
    // Speaker Grill — Front Face (Box Mesh)
    // ------------------------------------------------------------
    // The grill uses a metallic mesh material with higher specular
    // values to simulate reflective wire patterns. UV tiling is
    // increased to sharpen the grill texture and prevent stretching.
    //
    // The grill height matches the updated speaker body height.
    // ============================================================
    scaleXYZ = glm::vec3(5.0f, 0.6f, 0.01f);  // Thin front face
    rotX = rotY = rotZ = 0.0f;

    // Slightly forward to sit flush on the speaker body.
    positionXYZ = glm::vec3(0.0f, 0.5f, 3.51f);

    sceneManager->SetTransformations(scaleXYZ, rotX, rotY, rotZ, positionXYZ);
    sceneManager->SetShaderTexture(SceneManager::TEX_SPEAKER_GRILL);
    sceneManager->SetShaderMaterial(SceneManager::MAT_SPEAKER_GRILL);
    sceneManager->SetTextureUVScale(4.0f, 4.0f);
    sceneManager->GetMeshes()->DrawBoxMesh();

    // ============================================================
    // Speaker Knob — Multi-Pass Texturing (Cylinder Mesh)
    // ------------------------------------------------------------
    // The knob uses two rendering passes:
    //
    // PASS 1 — Base cylindrical surface:
    //   - Uses the knob texture
    //   - Uses the knob material
    //   - UV tiling increases density for visible brushed detail
    //
    // PASS 2 — Top surface overlay:
    //   - Uses a separate top texture
    //   - Uses the same material
    //   - Different UV scaling to create natural separation
    //
    // The knob is slightly tilted (8 degrees) to improve visibility
    // of the top surface under overhead lighting.
    //
    // Placement is adjusted upward to sit flush on the newly
    // increased speaker body height (top = 0.8f).
    // ============================================================
    scaleXYZ = glm::vec3(0.35f, 0.15f, 0.35f);
    rotX = 8.0f;   // slight tilt for visibility
    rotY = 0.0f;
    rotZ = 0.0f;

    // Positioned just below the top surface to compensate for tilt.
    positionXYZ = glm::vec3(0.0f, 0.79f, 3.0f);

    // PASS 1 — Base knob texture
    sceneManager->SetTransformations(scaleXYZ, rotX, rotY, rotZ, positionXYZ);
    sceneManager->SetShaderTexture(SceneManager::TEX_SPEAKER_KNOB);
    sceneManager->SetShaderMaterial(SceneManager::MAT_SPEAKER_KNOB);
    sceneManager->SetTextureUVScale(1.4f, 1.4f);
    sceneManager->GetMeshes()->DrawCylinderMesh();

    // PASS 2 — Top texture overlay
    sceneManager->SetTransformations(scaleXYZ, rotX, rotY, rotZ, positionXYZ);
    sceneManager->SetShaderTexture(SceneManager::TEX_SPEAKER_KNOB_TOP);
    sceneManager->SetShaderMaterial(SceneManager::MAT_SPEAKER_KNOB);
    sceneManager->SetTextureUVScale(3.0f, 1.0f);
    sceneManager->GetMeshes()->DrawCylinderMesh();
}
