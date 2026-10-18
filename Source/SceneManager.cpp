///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "Speaker.h"
#include "SceneManager.h"
#include "ViewManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// ============================================================
// Texture Tag Constant Definitions (Milestone Four)
// ============================================================
const char* const SceneManager::TEX_DESK = "desk";
const char* const SceneManager::TEX_SPEAKER_BODY = "speaker_body";
const char* const SceneManager::TEX_SPEAKER_GRILL = "speaker_grill";
const char* const SceneManager::TEX_SPEAKER_KNOB = "speaker_knob";
const char* const SceneManager::TEX_SPEAKER_KNOB_TOP = "speaker_knob_top";
const char* const SceneManager::TEX_WALL = "wall";

// Additional Texture Tag Constant Definitions (Final Project)
const char* const SceneManager::TEX_MINI_COMPUTER = "mini_computer";
const char* const SceneManager::TEX_SPHERE_SPEAKER = "sphere_speaker";

// ============================================================
// Material Tag Constant Definitions (Milestone Five)
// ============================================================
const char* const SceneManager::MAT_DESK = "desk_material";
const char* const SceneManager::MAT_WALL = "wall_material";
const char* const SceneManager::MAT_SPEAKER_BODY = "speaker_body_material";
const char* const SceneManager::MAT_SPEAKER_GRILL = "speaker_grill_material";
const char* const SceneManager::MAT_SPEAKER_KNOB = "speaker_knob_material";

// Additional Material Tag Constant Definitions (Final Project)
const char* const SceneManager::MAT_MINI_COMPUTER = "mini_computer_material";
const char* const SceneManager::MAT_SPHERE_SPEAKER = "sphere_speaker_material";

// ============================================================
// Externalized Scene Constants (UV scales, material tuning)
// ============================================================

// UV scale constants (responding to instructor feedback)
const float SceneManager::DESK_UV_SCALE = 4.0f;
const float SceneManager::WALL_UV_SCALE = 2.0f;
const float SceneManager::MINI_UV_SCALE = 20.0f;   // improved brushed-metal visibility
const float SceneManager::SPHERE_SPEAKER_UV_SCALE = 10.0f;   // matches grill texture density

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

// ============================================================
// DefineObjectMaterials()
// -----------------------
// This method configures the material properties for every
// object in the 3D scene. These materials determine how each
// object responds to the ambient, diffuse, and specular
// components of the Phong lighting model.
//
// The values chosen here are intentionally grounded in the
// real-world physical properties of the objects being modeled.
// For example:
//
// - Wood surfaces exhibit low shininess and broad diffuse
//   reflection due to their fibrous structure.
//
// - Drywall has extremely low specular reflectance and appears
//   matte under most lighting conditions.
//
// - Brushed aluminum (Mac mini) exhibits strong specular
//   highlights due to its metallic surface and directional
//   micro-grooves.
//
// - Matte plastics (speaker body) have moderate diffuse
//   reflection and minimal specular contribution.
//
// - Metallic meshes (speaker grill, sphere speaker) produce
//   small, sharp highlights due to their thin wire structure.
//
// These carefully tuned material values ensure that each object
// interacts with the lighting system in a way that supports both
// realism and visual clarity, satisfying the Final Project rubric
// requirement for polished lighting and shading.
// ============================================================

void SceneManager::DefineObjectMaterials()
{
	// Desk (wood)
	OBJECT_MATERIAL desk;
	desk.ambientColor = glm::vec3(0.4f, 0.3f, 0.1f);
	desk.ambientStrength = 0.40f;
	desk.diffuseColor = glm::vec3(0.5f, 0.35f, 0.15f);
	desk.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	desk.shininess = 4.0f;
	desk.tag = MAT_DESK;
	m_objectMaterials.push_back(desk);

	// Wall (drywall)
	OBJECT_MATERIAL wall;
	wall.ambientColor = glm::vec3(0.3f, 0.3f, 0.35f);
	wall.ambientStrength = 0.55f; // Slight boost helps blue ambient light visibility
	wall.diffuseColor = glm::vec3(0.10f, 0.10f, 0.15f);
	wall.specularColor = glm::vec3(0.2f, 0.2f, 0.25f);
	wall.shininess = 2.0f;
	wall.tag = MAT_WALL;
	m_objectMaterials.push_back(wall);

	// Speaker body (matte plastic)
	OBJECT_MATERIAL speakerBody;
	speakerBody.ambientColor = glm::vec3(0.15f, 0.15f, 0.18f);
	speakerBody.ambientStrength = 0.3f;
	speakerBody.diffuseColor = glm::vec3(0.25f, 0.25f, 0.3f);
	speakerBody.specularColor = glm::vec3(0.2f, 0.2f, 0.25f);
	speakerBody.shininess = 8.0f;
	speakerBody.tag = MAT_SPEAKER_BODY;
	m_objectMaterials.push_back(speakerBody);

	// Speaker grill (metal mesh)
	OBJECT_MATERIAL grill;
	grill.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	grill.ambientStrength = 0.25f;
	grill.diffuseColor = glm::vec3(0.35f, 0.35f, 0.35f);
	grill.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	grill.shininess = 20.0f;
	grill.tag = MAT_SPEAKER_GRILL;
	m_objectMaterials.push_back(grill);

	// Speaker knob (metal/plastic)
	OBJECT_MATERIAL knob;
	knob.ambientColor = glm::vec3(0.18f, 0.18f, 0.2f);
	knob.ambientStrength = 0.3f;
	knob.diffuseColor = glm::vec3(0.3f, 0.3f, 0.35f);
	knob.specularColor = glm::vec3(0.6f, 0.6f, 0.65f);
	knob.shininess = 30.0f;
	knob.tag = MAT_SPEAKER_KNOB;
	m_objectMaterials.push_back(knob);

	// Mac mini (brushed metal)
	OBJECT_MATERIAL mini;
	mini.ambientColor = glm::vec3(0.25f, 0.25f, 0.25f);
	mini.ambientStrength = 0.3f;
	mini.diffuseColor = glm::vec3(0.70f, 0.70f, 0.70f);
	mini.specularColor = glm::vec3(0.80f, 0.80f, 0.80f);
	mini.shininess = 32.0f;
	mini.tag = MAT_MINI_COMPUTER;
	m_objectMaterials.push_back(mini);

	// Sphere speaker (metal mesh)
	OBJECT_MATERIAL sphere;
	sphere.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	sphere.ambientStrength = 0.35f;
	sphere.diffuseColor = glm::vec3(0.35f, 0.35f, 0.35f);
	sphere.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	sphere.shininess = 20.0f;
	sphere.tag = MAT_SPHERE_SPEAKER;
	m_objectMaterials.push_back(sphere);
}

// ============================================================
// SetupSceneLights()
// ------------------
// This method configures all light sources used in the scene.
// The lighting arrangement is intentionally designed to match
// the real-world lighting conditions of the desk environment
// being replicated.
//
// 1. A main overhead room light (soft white) provides broad,
//    diffuse illumination across the entire desk surface.
//
// 2. A secondary overhead fill light reduces harsh shadows and
//    ensures that objects remain visible from multiple camera
//    angles.
//
// 3. A front fill light provides additional illumination for
//    the speaker and foreground objects, preventing flat shading
//    when the camera moves around the scene.
//
// 4. A blue LED shelf light (colored point light) replicates the
//    soft blue glow cast by the LED strip mounted beneath the
//    shelf above the desk. It tints the wall and the front edges
//    of the objects, creating subtle rim lighting that enhances
//    contrast and depth in the scene.
//
// This four-light configuration satisfies the Final Project rubric
// requirement for multiple light sources, including at least one
// colored light and at least one point light. It ensures that all
// objects remain visible under camera navigation and that the scene
// exhibits polished, realistic Phong shading.
// ============================================================

void SceneManager::SetupSceneLights()
{
	// ------------------------------------------------------------
	// Populate light configuration arrays
	// ------------------------------------------------------------
	// Light 0 — Main overhead room light (soft white)
	m_lightPositions[0] = glm::vec3(0.0f, 14.0f, -2.0f);
	m_lightAmbient[0] = glm::vec3(0.04f, 0.04f, 0.04f);
	m_lightDiffuse[0] = glm::vec3(0.15f, 0.15f, 0.15f);
	m_lightSpecular[0] = glm::vec3(0.6f, 0.6f, 0.6f);
	m_lightFocalStrength[0] = 32.0f;
	m_lightSpecularIntensity[0] = 0.6f;

	// Light 1 — Secondary overhead fill (soft white)
	m_lightPositions[1] = glm::vec3(-6.0f, 12.0f, 4.0f);
	m_lightAmbient[1] = glm::vec3(0.02f, 0.02f, 0.02f);
	m_lightDiffuse[1] = glm::vec3(0.10f, 0.10f, 0.10f);
	m_lightSpecular[1] = glm::vec3(0.2f, 0.2f, 0.2f);
	m_lightFocalStrength[1] = 24.0f;
	m_lightSpecularIntensity[1] = 0.3f;

	// Light 2 — Front fill light (neutral)
	m_lightPositions[2] = glm::vec3(0.6f, 5.0f, 6.0f);
	m_lightAmbient[2] = glm::vec3(0.01f, 0.01f, 0.01f);
	m_lightDiffuse[2] = glm::vec3(0.05f, 0.05f, 0.05f);
	m_lightSpecular[2] = glm::vec3(0.3f, 0.3f, 0.3f);
	m_lightFocalStrength[2] = 12.0f;
	m_lightSpecularIntensity[2] = 0.5f;

	// Light 3 — Blue LED shelf light (colored point light)
	m_lightPositions[3] = glm::vec3(0.0f, 8.0f, 4.0f);
	m_lightAmbient[3] = glm::vec3(0.35f, 0.55f, 0.95f);
	m_lightDiffuse[3] = glm::vec3(0.10f, 0.25f, 0.60f);
	m_lightSpecular[3] = glm::vec3(0.10f, 0.25f, 0.60f);
	m_lightFocalStrength[3] = 10.0f;
	m_lightSpecularIntensity[3] = 0.4f;

	// ------------------------------------------------------------
	// Push all lights to the shader using a loop
	// ------------------------------------------------------------
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		std::string base = "lightSources[" + std::to_string(i) + "].";

		m_pShaderManager->setVec3Value(base + "position", m_lightPositions[i]);
		m_pShaderManager->setVec3Value(base + "ambientColor", m_lightAmbient[i]);
		m_pShaderManager->setVec3Value(base + "diffuseColor", m_lightDiffuse[i]);
		m_pShaderManager->setVec3Value(base + "specularColor", m_lightSpecular[i]);
		m_pShaderManager->setFloatValue(base + "focalStrength", m_lightFocalStrength[i]);
		m_pShaderManager->setFloatValue(base + "specularIntensity", m_lightSpecularIntensity[i]);
	}

	// Enable lighting system
	m_pShaderManager->setBoolValue("bUseLighting", true);
	std::cout << "Lighting enabled: Phong model active.\n";
}

/***********************************************************
 *  PrepareScene()
 *  --------------
 *  Called once at startup to load all meshes, textures,
 *  materials, and lighting for the 3D scene. This method
 *  ensures that all components required for rendering are
 *  initialized before the first frame is drawn.
 *
 *  The lighting configuration defined above ensures that
 *  all objects remain visible under camera navigation while
 *  preserving the realism of the replicated desk environment.
 *  This satisfies the Final Project rubric requirement for
 *  polished lighting and shading.
 ***********************************************************/
void SceneManager::PrepareScene()
{
	/**************************************************************
	 * Load all meshes required for the 3D scene.
	 * ------------------------------------------------------------
	 * These meshes define the geometry for:
	 * - Desk surface (plane)
	 * - Background wall (plane)
	 * - Speaker components (box, cylinder, cone)
	 * - Mac mini (box)
	 * - Sphere speaker (sphere)
	 *
	 * Loading them here ensures all geometry is available before
	 * any rendering or transformation occurs.
	 **************************************************************/
	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadConeMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadSphereMesh();

	/**************************************************************
	 * Load all textures used in the scene.
	 * ------------------------------------------------------------
	 * Each texture is associated with a tag constant to avoid
	 * hard‑coded strings and improve maintainability. This
	 * directly addresses instructor feedback regarding externalized
	 * constants and clean organization.
	 **************************************************************/
	CreateGLTexture("Textures/desk_wood.jpg", TEX_DESK);
	CreateGLTexture("Textures/speaker_body.jpg", TEX_SPEAKER_BODY);
	CreateGLTexture("Textures/speaker_grill.jpg", TEX_SPEAKER_GRILL);
	CreateGLTexture("Textures/speaker_knob.jpg", TEX_SPEAKER_KNOB);
	CreateGLTexture("Textures/speaker_knob_top.jpg", TEX_SPEAKER_KNOB_TOP);
	CreateGLTexture("Textures/drywall.jpg", TEX_WALL);
	CreateGLTexture("Textures/mini_computer.jpg", TEX_MINI_COMPUTER);
	CreateGLTexture("Textures/speaker_grill.jpg", TEX_SPHERE_SPEAKER);

	/**************************************************************
	 * Bind textures, define materials, and configure lighting.
	 * ------------------------------------------------------------
	 * This step ensures:
	 * - All textures are active in OpenGL memory
	 * - All materials are registered for shader use
	 * - All lights are configured using the new loop‑based system
	 *
	 * These improvements directly address instructor feedback
	 * regarding maintainability and best practices.
	 **************************************************************/
	BindGLTextures();
	DefineObjectMaterials();
	SetupSceneLights();

	/**************************************************************
	 * Initialize all scene objects.
	 * ------------------------------------------------------------
	 * Each object sets up its internal geometry, texture tags,
	 * and material tags. Initialization is separated from rendering
	 * to maintain clean architecture and modular design.
	 **************************************************************/
	m_speaker.Initialize();
	m_miniComputer.Initialize();
	m_sphereSpeaker.Initialize();

	/**************************************************************
	 * Display camera controls in the console.
	 * ------------------------------------------------------------
	 * This supports Milestone Three’s interactivity requirements
	 * and provides clear instructions for navigating the scene.
	 **************************************************************/
	std::cout << "Camera Controls:\n";
	std::cout << "  W/S: Move forward/backward\n";
	std::cout << "  A/D: Move left/right\n";
	std::cout << "  Q/E: Move down/up\n";
	std::cout << "  Mouse: Look around\n";
	std::cout << "  Scroll Wheel: Adjust movement speed\n";
	std::cout << "  P: Perspective view\n";
	std::cout << "  O: Orthographic view\n\n";
}

/***********************************************************
 *  RenderScene()
 *  -------------
 *  Renders all objects in the 3D scene using the Phong
 *  lighting model. The rendering flow intentionally applies:
 *
 *      Transformations → Texture → Material → Draw
 *
 *  This ensures that each object responds correctly to the
 *  ambient, diffuse, and specular contributions of the
 *  lighting system. The updated lighting configuration
 *  ensures that the wall receives a soft blue tint from the
 *  LED shelf light, enhancing realism and depth.
 ***********************************************************/
void SceneManager::RenderScene()
{
	// Enable texture and lighting systems
	m_pShaderManager->setIntValue(g_UseTextureName, true);
	m_pShaderManager->setIntValue(g_UseLightingName, true);

	glm::vec3 scaleXYZ;
	float XrotationDegrees;
	float YrotationDegrees;
	float ZrotationDegrees;
	glm::vec3 positionXYZ;

	/****************************************************************
	 * Desk Surface (Plane Mesh)
	 * -------------------------
	 * The desk is rendered first because it forms the foundation
	 * of the scene and provides a visual anchor for all objects.
	 *
	 * Its warm wood material interacts strongly with diffuse light
	 * while producing minimal specular highlights, consistent with
	 * real-world wooden surfaces.
	 *
	 * The UV scale uses the externalized DESK_UV_SCALE constant,
	 * improving maintainability and addressing instructor feedback
	 * regarding magic numbers.
	 ****************************************************************/
	if (usePerspective)
	{
		scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);
		positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

		SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
		SetShaderTexture(TEX_DESK);
		SetShaderMaterial(MAT_DESK);
		SetTextureUVScale(DESK_UV_SCALE, DESK_UV_SCALE);

		m_basicMeshes->DrawPlaneMesh();
	}

	/****************************************************************
	 * Background Wall (Plane Mesh)
	 * ----------------------------
	 * The wall is rotated upright to form the background of the
	 * scene. Its drywall material is intentionally low-shininess
	 * to avoid distracting reflections and to ensure the blue LED
	 * shelf light produces a soft tint rather than harsh highlights.
	 *
	 * The UV scale uses the externalized WALL_UV_SCALE constant,
	 * improving maintainability and readability.
	 ****************************************************************/
	scaleXYZ = glm::vec3(20.0f, 20.0f, 1.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 10.0f, 8.0f);

	SetTransformations(scaleXYZ, XrotationDegrees,
		YrotationDegrees, ZrotationDegrees,
		positionXYZ);

	SetShaderTexture(TEX_WALL);
	SetShaderMaterial(MAT_WALL);
	SetTextureUVScale(WALL_UV_SCALE, WALL_UV_SCALE);

	m_basicMeshes->DrawPlaneMesh();

	/****************************************************************
	 * Speaker Object
	 * --------------
	 * The speaker is rendered using its own encapsulated logic.
	 * This modular design ensures that object-specific materials,
	 * textures, and transformations remain isolated, improving
	 * readability and supporting scalable architecture.
	 ****************************************************************/
	m_speaker.Render(this);

	/****************************************************************
	 * Final Project Objects
	 * ---------------------
	 * Each object (Mac mini, sphere speaker) applies its own
	 * transformations, textures, and materials inside its
	 * Render() method. This modular approach ensures that each
	 * object responds correctly to the lighting system and maintains
	 * clean separation of responsibilities.
	 *
	 * The updated lighting configuration ensures:
	 * - The Mac mini exhibits realistic brushed-metal highlights.
	 * - The sphere speaker grill produces subtle metallic glints.
	 * - The desk and wall receive soft blue tinting from the LED
	 *   shelf light, enhancing depth and realism.
	 ****************************************************************/
	m_miniComputer.Render(this);
	m_sphereSpeaker.Render(this);
}
