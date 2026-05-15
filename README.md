A 3D rendering engine, that focuses on shader creation and algorithms to render objects efficiently. This project uses OGRE (Object Oriented Graphics Rendering Engine) to set a ground for low level rendering operations, as Ogre does not have many key required components, such as a GUI System. <br>

The project adds runtime shader update system (RSUS), that can greatly imporve visual quality of the rendering engine. The project also adds ImGui to create reliable and lightweight GUI components to show RSUS parameters.

## RSUS - Runtime Shader Update System

<img width="481" height="434" alt="Screenshot (110)" src="https://github.com/user-attachments/assets/bbbe01b6-60f1-45e4-9c15-7692a52433bd" /> <br>

**RSUS** first finds all the material files that is defined outside the OGRE system, filepath is set using the ResourceHandler. Afterwards Shaders parameters are read and stored, these values are then compared with the running instance of OGRE as executing shaders on GPU provideson method for checking parameters. These values then can be editied and saved.

<img width="493" height="548" alt="Screenshot (111)" src="https://github.com/user-attachments/assets/8042943c-6039-4920-9510-3c193318a996" /> <br>

Using this feature, shader file can be updated/changed and RSUS will manage all variable fecthes and saved data integration.


# Examples

### SKYBOX
<img width="1920" height="1080" alt="Screenshot (87)" src="https://github.com/user-attachments/assets/6dd539d0-b347-44b5-919b-248fe16dacb5" />

Horseshoee provides skyboxs, which is a UV Sphere, on which shaders can be applied to simulate an environment.

### TERRAIN 

<img width="1920" height="1080" alt="Screenshot (88)" src="https://github.com/user-attachments/assets/64ffdc28-3ec0-456c-b9ea-d12424ef0a6a" />

Horseshoee provides terrain creation using heightmap images.

### PARALLAX

<img width="1920" height="1080" alt="Screenshot (104)" src="https://github.com/user-attachments/assets/2e632a04-e2aa-493e-86be-5e12271ef342" />

An custom implementation of parallax based on ray-marching parallax.

### BRDF
<img width="1351" height="787" alt="Screenshot (89)" src="https://github.com/user-attachments/assets/42e2d48a-181b-4b46-9b5d-2cefee3dcf11" />

An custom implementation of **BRDF** (Bidirectional Reflectance Distribution Function)




# Installation

## NOTE : THIS IS AN HOBBY PROJECT. RECOMMENDING MHORSESHOE FOR DEVELOPMENT IS LIKE RECOMMENDING CANCER.

The project uses cmake as its build system. 
Dependencies are installed in the build EXCEPT FOR FREEIMAGE, install that independently.

It is highly recommanded to set RelWithDebInfo Build type, Ogre has a hard time managing debug build.

Ogre is an very integrated part of mHorseshoee but it will be removed in far future due to its mind numbing build.

## Dependencies
<ul>
  <li> <a href="https://github.com/OGRECave/ogre">OGRE</a> </li>
  <li> <a href="https://github.com/ocornut/imgui">ImGui</a> </li>
  <li> <a href="https://github.com/libsdl-org/SDL">SDL</a> </li>
  <li> <a href="https://vulkan.lunarg.com/sdk/home">VULKAN SDK</a> </li>
  <li> <a href="https://freeimage.sourceforge.io/">FREEIMAGE</a> This is temporary dependency (Codec_STBI in OGRE is not working in this build) </li>
  <li> <s>PHYSX</s> </li>
</ul>
<br>
Imgui is optional as its installed along with OGRE.
SDL2 is also installed with OGRE but the project is built using SDL3, as such SDL3 is recommended. <br>

The program requires an **asset** folder (provided in the repo) <br>
During the first launch, in the Resource Tab enter the directories in load path. Note some load paths search recursively and some don't.
A restart is required for the load path directories to take effect.

