# CS 330 – Computational Graphics and Visualization  
### Final Project: 3D Desk Scene (OpenGL)

This repository contains my final project for CS 330: Computational Graphics and Visualization. The project recreates a real-world desk scene in a 3D OpenGL environment using low‑polygon primitives, realistic materials, Phong lighting, and interactive camera controls. The scene is based on a 2D reference photo of my actual desk and demonstrates core competencies in 3D object creation, texturing, lighting, and navigation.

---

## 🎯 Project Overview

The goal of this project was to transform a 2D reference image into a fully interactive 3D scene. The final result includes:

- A desk surface and background wall  
- A rectangular Mac mini (box primitive)  
- A spherical speaker modeled after the Apple HomePod Mini (sphere primitive)  
- A long front speaker built from multiple primitives (box + cylinder)  
- Realistic textures for metal, plastic, wood, and speaker grille  
- A complete Phong lighting system with multiple light sources  
- Full keyboard and mouse navigation  
- Switchable perspective and orthographic projection modes  

The project was developed in C++ using OpenGL, GLM, and standard shader pipelines.

---

## 🧱 3D Objects & Modeling

- **Mac mini:** Scaled box mesh with brushed metal material  
- **HomePod Mini:** Sphere mesh with a flat‑bottom illusion created by embedding the lower geometry into the Mac mini  
- **Long speaker:** Box housing + cylinder knob (multi‑primitive construction)  
- **Desk surface:** Plane mesh  
- **Wall:** Plane mesh  

All objects use low‑polygon primitives and organized geometry under 1,000 triangles per model.

---

## 🎨 Textures & Materials

- Brushed metal texture for the Mac mini  
- Matte plastic texture for speaker bodies  
- Mesh grille texture for speaker fronts  
- Light wood texture for the desk (chosen for better contrast than the real black wood surface)

UV scaling constants were externalized to eliminate magic numbers and improve maintainability.

---

## 💡 Lighting System

The scene uses a polished Phong lighting setup with:

- Two overhead soft‑white lights  
- One front fill light  
- One blue LED point light (matching the shelf illumination in the reference photo)

All lights include ambient, diffuse, and specular components. Light configuration is modular and loop‑based for scalability.

---

## 🎮 Navigation & Camera Controls

**Keyboard Movement**
- `W` / `S` – Forward / Backward  
- `A` / `D` – Left / Right  
- `Q` / `E` – Down / Up  

**Mouse Controls**
- Move mouse – Adjust pitch and yaw  
- Scroll wheel – Adjust movement speed  

**Projection Modes**
- `P` – Perspective view  
- `O` – Orthographic view  

Camera orientation remains consistent when switching projections.

---

## 🧩 Code Architecture & Modularity

The project uses a clean, modular structure:

- **MiniComputer**, **Speaker**, and **SphereSpeaker** classes each implement their own `Render()` method  
- **SceneManager** handles:
  - Transformations  
  - Shader texture binding  
  - Material setup  
  - Light configuration  
  - Scene rendering flow  

This architecture supports readability, reusability, and future expansion.

---

## 📁 Folder Structure

CS-330-Computational_Graphics_and_Visualization/
├── Source/                     # C++ source files
├── Textures/                   # Texture assets
├── Debug/                      # Optional build output
├── Release/                    # Optional build output
│
├── 7-1_FinalProjectMilestones.sln
├── 7-1_FinalProjectMilestones.vcxproj
├── 7-1_FinalProjectMilestones.vcxproj.filters
├── 7-1_FinalProjectMilestones.vcxproj.user
│
└── README.md


---

## 🔧 How to Build & Run

1. Open the solution file:  
   `7-1_FinalProjectMilestones.sln`
2. Build the project in Visual Studio (Debug or Release).
3. Run the executable from Visual Studio or the `/Debug` or `/Release` folder.
4. Use keyboard and mouse controls to explore the scene.

---

## 📚 Technologies Used

- C++  
- OpenGL  
- GLSL shaders  
- GLM (math library)  
- Visual Studio  
- Royalty‑free textures (1024×1024+)  

---

## 🚀 Future Improvements

If expanded beyond the course requirements, potential enhancements include:

- Adding shadow mapping for more realistic lighting  
- Implementing normal mapping on speaker grille textures  
- Creating custom meshes for objects like the diffuser  
- Adding animation or dynamic lighting effects  
- Introducing UI elements for toggling lights or materials  
- Modularizing desk and wall planes into dedicated classes  

---

## ⚠ Known Issues

- The HomePod Mini uses a flat‑bottom illusion rather than a custom mesh  
- The diffuser object was removed due to UV distortion on cylinder caps  
- Debug and Release folders may contain machine‑specific build artifacts  
- Some textures may appear slightly stretched depending on UV scaling  

---

## 📝 Notes

This project was completed as part of the CS 330 curriculum at Southern New Hampshire University and demonstrates core competencies in:

- 3D object generation  
- Interactive graphics programming  
- Full OpenGL scene development  

---

## © Author

**Chris D. Riegger**  
CS 330 – Computational Graphics and Visualization  
Southern New Hampshire University    

