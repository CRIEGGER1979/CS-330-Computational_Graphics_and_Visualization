///////////////////////////////////////////////////////////////////////////////
// Speaker.h
// =========
// Class definition for the custom long speaker object created for CS-330
// Computational Graphics and Visualization at Southern New Hampshire University.
//
// Author: Christopher Riegger
// Date: October 2026
//
// This header declares the Speaker class, which encapsulates all rendering
// logic for the long speaker object. The class provides modular construction
// of the speaker using SceneManager utilities for transformations, materials,
// textures, and mesh rendering. This design follows best practices and
// instructor feedback to keep SceneManager clean, maintainable, and aligned
// with the Final Project rubric.
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Forward declaration to avoid circular dependency.
// SceneManager provides transformation, material, and mesh utilities.
class SceneManager;

/**
 * @class Speaker
 * @brief Encapsulates the long speaker object built for Milestone Two.
 *
 * This class keeps the speaker's construction modular and separate from
 * SceneManager, following best practices and instructor feedback.
 * The speaker is composed of multiple basic 3D shapes (prism and cylinder)
 * and rendered using SceneManager's transformation and shader utilities.
 */
class Speaker
{
public:

    /**
     * @brief Initialize speaker resources.
     *
     * Currently unused, but reserved for future expansion such as
     * custom materials, textures, or additional components.
     */
    void Initialize();

    /**
     * @brief Render the speaker object into the scene.
     *
     * Uses SceneManager to apply transformations, materials, textures,
     * and mesh rendering for each component of the speaker. This method
     * encapsulates all rendering logic for the speaker, keeping SceneManager
     * clean and supporting modular, maintainable code.
     *
     * @param sceneManager Pointer to the active SceneManager instance.
     */
    void Render(SceneManager* sceneManager);
};
