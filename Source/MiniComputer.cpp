///////////////////////////////////////////////////////////////////////////////
// MiniComputer.cpp
// =================
// Rendering logic for the custom Mac mini object created for CS-330
// Computational Graphics and Visualization at Southern New Hampshire University.
//
// Author: Christopher Riegger
// Date: October 2026
//
// This file implements the MiniComputer class, which encapsulates all rendering
// logic for the Mac mini object. The Mac mini is represented using a single box
// mesh textured with brushed metal and shaded using realistic material values.
// All transformations, textures, and materials are applied here to maintain
// modularity and keep SceneManager clean and maintainable.
///////////////////////////////////////////////////////////////////////////////

#include "MiniComputer.h"
#include "SceneManager.h"

void MiniComputer::Initialize()
{
    // Reserved for future expansion.
    // No geometry setup required because the Mac mini uses the standard box mesh.
}

void MiniComputer::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
        return;

    // ============================================================
    // Transformations
    // ------------------------------------------------------------
    // The Mac mini is a low-profile square device. The updated
    // placement increases spacing between the speaker and the mini,
    // creating a more natural desk layout. A subtle counter‑clockwise
    // rotation breaks symmetry and enhances realism.
    //
    // This staggered arrangement:
    // - Improves depth and visual interest
    // - Allows the blue LED shelf light to produce subtle rim lighting
    // - Prevents objects from appearing unnaturally aligned
    // ============================================================
    glm::vec3 scaleXYZ = glm::vec3(2.4f, 0.4f, 2.4f);
    float rotX = 0.0f;
    float rotY = 6.0f;    // corrected rotation direction
    float rotZ = 0.0f;

    // Increased spacing from the speaker (shifted left)
    glm::vec3 positionXYZ = glm::vec3(-6.0f, 0.25f, 2.6f);

    sceneManager->SetTransformations(
        scaleXYZ,
        rotX,
        rotY,
        rotZ,
        positionXYZ);

    // ============================================================
    // Texture
    // ------------------------------------------------------------
    // The brushed-metal texture is applied using the tag defined in
    // SceneManager. Because the standard box mesh compresses UVs on
    // several faces, the texture can appear washed out unless the UV
    // scale is increased.
    //
    // The externalized constant MINI_UV_SCALE ensures:
    // - Consistent brushed-metal visibility
    // - Maintainability (no magic numbers)
    // - Alignment with instructor feedback regarding constants
    // ============================================================
    sceneManager->SetShaderTexture(SceneManager::TEX_MINI_COMPUTER);
    sceneManager->SetTextureUVScale(
        SceneManager::MINI_UV_SCALE,
        SceneManager::MINI_UV_SCALE);

    // ============================================================
    // Material
    // ------------------------------------------------------------
    // The Mac mini uses a brushed-metal material defined in
    // SceneManager. This material includes:
    // - Moderate ambient strength for subtle base illumination
    // - Bright diffuse values to capture room lighting
    // - Strong specular highlights to simulate anodized aluminum
    // - High shininess for crisp reflections
    //
    // These values ensure the Mac mini responds realistically to the
    // multi-light Phong lighting system.
    // ============================================================
    sceneManager->SetShaderMaterial(SceneManager::MAT_MINI_COMPUTER);

    // ============================================================
    // Draw Mesh
    // ------------------------------------------------------------
    // The Mac mini is represented using the standard box mesh.
    // ============================================================
    sceneManager->GetMeshes()->DrawBoxMesh();
}
