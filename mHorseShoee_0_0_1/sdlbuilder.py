import sys
import os
import shutil


arguments = sys.argv[1:]
sdl_source_dir = arguments[0]
binary_dir = arguments[1] + "/Release"

sdl_dll_path = sdl_source_dir + "/../sdl3-build/Release"

files = []
for dirpath, dirnames, filenames in os.walk(sdl_dll_path):
    print(f"files: {filenames}")
    files = filenames
    break

for file in files:
    if file.find(".dll") > -1:
        if os.path.exists(binary_dir + "/" + file):
            print(file + " already exsists in binary")
        else:
            print("coping - " + file)
            shutil.copy(sdl_dll_path + "/" + file, binary_dir + "/" + file)

