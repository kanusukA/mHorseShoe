import sys
import os
import shutil

arguments = sys.argv[1:]

# Check if ogre has imgui files

ogre_src_imgui = arguments[0] + "/Components/Overlay/include"
ogre_build_path = arguments[0] + "/../ogre-build"

ogre_src_imgui_path = os.path.join(ogre_src_imgui + "/imgui.h")
ogre_src_imconfig_path = os.path.join(ogre_src_imgui + "/imconfig.h")
ogre_src_imgui_stdlib_path = os.path.join(ogre_src_imgui + "/imgui_stdlib.h")
ogre_src_imgui_stdlib_cpp_path = os.path.join(ogre_src_imgui + "/imgui_stdlib.cpp")

#Find Ogre Build in which imgui is installed
folders = []
for dirpath, dirnames, filenames in os.walk(ogre_build_path):
    print(f"Subdirectories: {dirnames}")
    folders = dirnames
    break


imgui_build_folder = ""
for folder_names in folders:
    if folder_names.find("imgui") != -1:
        print("found imgui folder : " + folder_names)
        imgui_build_folder = folder_names
        break

ogre_build_imgui_path = ogre_build_path + "/" + imgui_build_folder

print("Ogre path - " + ogre_src_imgui_path)

if os.path.exists(ogre_src_imgui_path):
    print("imgui.h already exists")
else:
    shutil.copy(ogre_build_imgui_path + "/imgui.h",ogre_src_imgui)

if os.path.exists(ogre_src_imconfig_path):
    print("imconfig.h already exists")
else:
    shutil.copy(ogre_build_imgui_path + "/imconfig.h",ogre_src_imgui)


if os.path.exists(ogre_src_imgui_stdlib_path):
    print("imgui_stdlib.h already exists")
else:
    shutil.copy(ogre_build_imgui_path + "/misc/cpp/imgui_stdlib.h",ogre_src_imgui)

if os.path.exists(ogre_src_imgui_stdlib_cpp_path):
    print("imgui_stdlib.cpp already exists")
else:
     shutil.copy(ogre_build_imgui_path + "/misc/cpp/imgui_stdlib.cpp",ogre_src_imgui)


#Adding dlls and ogre related files
sln_dir = arguments[1] + "/Release"

#DLL FOLDERS
ogre_dll_path = ogre_build_path + "/bin/release"

if os.path.exists(sln_dir ):
    pass
else:
    os.mkdir(sln_dir )

files = []
for dirpath, dirnames, filenames in os.walk(ogre_dll_path):
    print(f"files: {filenames}")
    files = filenames
    break

for file in files:
    if file.find(".dll") > -1:
        if os.path.exists(sln_dir + "/" + file):
            print(file + " already exsists in binary")
        else:
            print("coping - " + file)
            shutil.copy(ogre_dll_path + "/" + file, sln_dir + "/" + file)

    