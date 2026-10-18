///////////////////////////////////////////////////////////////////////////////
// MiniComputer.h
// ==============
// Class definition for the custom Mac mini object created for CS-330
// Computational Graphics and Visualization at Southern New Hampshire University.
//
// Author: Christopher Riegger
// Date: October 2026
//
// This header declares the MiniComputer class, which encapsulates all rendering
// logic for the Mac mini object. The class provides modular construction of the
// device using SceneManager utilities for transformations, materials, textures,
// and mesh rendering. This design follows best practices and instructor feedback
// to keep SceneManager clean, maintainable, and aligned with the Final Project
// rubric.
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Forward declaration to avoid circular dependency.
// SceneManager provides transformation, material, and mesh utilities.
class SceneManager;

/**
 * @class MiniComputer
 * @brief Encapsulates the Mac mini object built for the Final Project.
 *
 * This class keeps the Mac mini’s construction modular and separate from
 * SceneManager, following best practices and instructor feedback.
 * The Mac mini is composed of a single box mesh with a brushed metal texture
 * and realistic material properties.
 */
class MiniComputer
{
public:

    /**
     * @brief Initialize Mac mini resources.
     *
     * Reserved for future expansion such as custom materials or additional
     * components. All rendering logic is handled inside Render().
     */
    void Initialize();

    /**
     * @brief Render the Mac mini object into the scene.
     *
     * Uses SceneManager to apply transformations, materials, textures,
     * and mesh rendering. This method encapsulates all rendering logic
     * for the Mac mini, keeping SceneManager clean and supporting modular,
     * maintainable code.
     *
     * @param sceneManager Pointer to the active SceneManager instance.
     */
    void Render(SceneManager* sceneManager);
};
