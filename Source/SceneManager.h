///////////////////////////////////////////////////////////////////////////////
// shadermanager.h
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//  Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"
#include "Speaker.h"
#include "MiniComputer.h"
#include "SphereSpeaker.h"

#include <string>
#include <vector>

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
    // ============================================================
    // Texture Tag Constants (Milestone Four: Texturing Objects)
    // Used to avoid hard‑coded strings and improve maintainability.
    // ============================================================
    static const char* const TEX_DESK;
    static const char* const TEX_SPEAKER_BODY;
    static const char* const TEX_SPEAKER_GRILL;
    static const char* const TEX_SPEAKER_KNOB;
    static const char* const TEX_SPEAKER_KNOB_TOP;
    static const char* const TEX_WALL;

    // Additional Texture Tag Constants (Final Project)
    static const char* const TEX_MINI_COMPUTER;
    static const char* const TEX_SPHERE_SPEAKER;

    // ============================================================
    // Material Tag Constants (Milestone Five: Lighting)
    // ============================================================
    static const char* const MAT_DESK;
    static const char* const MAT_WALL;
    static const char* const MAT_SPEAKER_BODY;
    static const char* const MAT_SPEAKER_GRILL;
    static const char* const MAT_SPEAKER_KNOB;

    // Additional Material Tag Constants (Final Project)
    static const char* const MAT_MINI_COMPUTER;
    static const char* const MAT_SPHERE_SPEAKER;

    // ============================================================
    // Global Scene Constants (Lighting, UV, etc.)
    // ============================================================
    static const int   NUM_LIGHTS = 4;

    // UV scale constants (externalized for maintainability)
    static const float DESK_UV_SCALE;
    static const float WALL_UV_SCALE;
    static const float MINI_UV_SCALE;
    static const float SPHERE_SPEAKER_UV_SCALE;

    // constructor / destructor
    SceneManager(ShaderManager* pShaderManager);
    ~SceneManager();

    struct TEXTURE_INFO
    {
        std::string tag;
        uint32_t    ID;
    };

    struct OBJECT_MATERIAL
    {
        float       ambientStrength;
        glm::vec3   ambientColor;
        glm::vec3   diffuseColor;
        glm::vec3   specularColor;
        float       shininess;
        std::string tag;
    };

private:
    // pointer to shader manager object
    ShaderManager* m_pShaderManager;
    // pointer to basic shapes object
    ShapeMeshes* m_basicMeshes;

    // total number of loaded textures
    int            m_loadedTextures;
    // loaded textures info
    TEXTURE_INFO   m_textureIDs[16];

    // defined object materials
    std::vector<OBJECT_MATERIAL> m_objectMaterials;

    // Scene objects
    Speaker       m_speaker;
    MiniComputer  m_miniComputer;
    SphereSpeaker m_sphereSpeaker;

    // ============================================================
    // Light configuration storage (for loop-based setup)
    // ============================================================
    glm::vec3 m_lightPositions[NUM_LIGHTS];
    glm::vec3 m_lightAmbient[NUM_LIGHTS];
    glm::vec3 m_lightDiffuse[NUM_LIGHTS];
    glm::vec3 m_lightSpecular[NUM_LIGHTS];
    float     m_lightFocalStrength[NUM_LIGHTS];
    float     m_lightSpecularIntensity[NUM_LIGHTS];

    // internal helpers
    bool CreateGLTexture(const char* filename, std::string tag);
    void BindGLTextures();
    void DestroyGLTextures();
    int  FindTextureID(std::string tag);
    int  FindTextureSlot(std::string tag);
    bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);

public:
    // access to basic mesh drawing functions
    ShapeMeshes* GetMeshes() { return m_basicMeshes; }

    // transform, color, texture, material setters
    void SetTransformations(
        glm::vec3 scaleXYZ,
        float     XrotationDegrees,
        float     YrotationDegrees,
        float     ZrotationDegrees,
        glm::vec3 positionXYZ);

    void SetShaderColor(
        float redColorValue,
        float greenColorValue,
        float blueColorValue,
        float alphaValue);

    void SetShaderTexture(std::string textureTag);
    void SetTextureUVScale(float u, float v);
    void SetShaderMaterial(std::string materialTag);

    // scene lifecycle
    void PrepareScene();
    void RenderScene();

    // lighting & materials
    void DefineObjectMaterials();
    void SetupSceneLights();
};
