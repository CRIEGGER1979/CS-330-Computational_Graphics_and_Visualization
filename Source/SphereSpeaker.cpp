///////////////////////////////////////////////////////////////////////////////
// SphereSpeaker.cpp
// =================
// Rendering logic for the spherical speaker object created for CS-330
// Computational Graphics and Visualization at Southern New Hampshire University.
//
// Author: Christopher Riegger
// Date: October 2026
//
// This file implements the SphereSpeaker class, which encapsulates all rendering
// logic for the spherical speaker object. The speaker uses the standard sphere
// mesh and applies a grill texture and custom material to simulate a mesh-like
// acoustic surface. All transformations, textures, and materials are applied
// here to maintain modularity and keep SceneManager clean and maintainable.
///////////////////////////////////////////////////////////////////////////////

#include "SphereSpeaker.h"
#include "SceneManager.h"

void SphereSpeaker::Initialize()
{
    // Reserved for future expansion.
    // The spherical speaker uses the standard sphere mesh.
}

void SphereSpeaker::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
        return;

    // ============================================================
    // Transformations
    // ------------------------------------------------------------
    // The spherical speaker is modeled after a HomePod mini-style
    // device. The scale remains uniform to preserve the sphere’s
    // proportions, while the placement is carefully tuned to match
    // real-world behavior and ensure proper interaction with the
    // lighting system.
    //
    // The updated placement aligns the sphere with the newly
    // repositioned Mac mini. This ensures:
    // - The sphere remains centered on the mini
    // - The lower curvature sinks slightly into the mini to simulate
    //   the real device’s flat base
    // - The blue LED shelf light produces subtle rim lighting
    // - No floating or clipping occurs
    // ============================================================
    glm::vec3 scaleXYZ = glm::vec3(1.0f, 1.0f, 1.0f);   // restored correct size

    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    // ------------------------------------------------------------
    // Vertical Placement Logic (based on MiniComputer scale)
    // ------------------------------------------------------------
    // MiniComputer height = 0.4 → half-height = 0.2
    // MiniComputer center Y = 0.25
    // MiniComputer top surface = 0.25 + 0.2 = 0.45
    //
    // Sphere radius = 0.5 (scale 1.0f)
    // Perfect mathematical placement:
    //     0.45 + 0.5 = 0.95
    //
    // To simulate the real HomePod mini’s flat base, we lift the
    // sphere slightly so the lower curvature disappears into the
    // Mac mini. Final refined placement: 1.09f
    // ------------------------------------------------------------
    glm::vec3 positionXYZ = glm::vec3(-6.0f, 1.09f, 2.6f);   // restored correct height

    sceneManager->SetTransformations(
        scaleXYZ,
        rotX,
        rotY,
        rotZ,
        positionXYZ);

    // ============================================================
    // Texture
    // ------------------------------------------------------------
    // The spherical speaker uses the same grill texture as the
    // rectangular speaker body. To ensure the mesh-like pattern
    // remains visible across the curved surface, we increase the
    // UV tiling using the externalized SPHERE_SPEAKER_UV_SCALE
    // constant.
    //
    // This:
    // - Increases texture density
    // - Prevents stretching across the sphere’s poles
    // - Ensures consistent acoustic-mesh appearance
    // - Removes magic numbers and improves maintainability
    // ============================================================
    sceneManager->SetShaderTexture(SceneManager::TEX_SPHERE_SPEAKER);
    sceneManager->SetTextureUVScale(
        SceneManager::SPHERE_SPEAKER_UV_SCALE,
        SceneManager::SPHERE_SPEAKER_UV_SCALE);

    // ============================================================
    // Material
    // ------------------------------------------------------------
    // The spherical speaker uses a mesh-like material defined in
    // SceneManager. This material includes:
    // - Moderate ambient strength for soft base illumination
    // - Balanced diffuse values for realistic shading
    // - Metallic specular highlights to simulate woven mesh fibers
    //
    // These values ensure the speaker responds realistically to the
    // multi-light Phong lighting system.
    // ============================================================
    sceneManager->SetShaderMaterial(SceneManager::MAT_SPHERE_SPEAKER);

    // ============================================================
    // Draw Mesh
    // ------------------------------------------------------------
    // The spherical speaker is represented using the standard sphere
    // mesh provided by SceneManager.
    // ============================================================
    sceneManager->GetMeshes()->DrawSphereMesh();
}
