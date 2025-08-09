import sys
import os
import shutil
import subprocess

arguments = sys.argv[1:]
physx_src_path = arguments[0] + "/physx"

gen_file = physx_src_path + "/generate_projects.bat"

try:
    process = subprocess.Popen(gen_file, 
                     creationflags=subprocess.CREATE_NEW_CONSOLE
                     )
    
    
except subprocess.CalledProcessError as e:
    print("Error in running generate_project.bat" + e)
    print("STDOUT:")
    print(e.stdout)
    print("STDERR:")
    print(e.stderr)

result_code = process.wait()
print("exit code ",result_code)
if result_code == 0:
    print("PhysX project generated")
else:
    print("unable to generate PhysX project")
    sys.exit(1)

## FIX - ONLY ACCESS vc17win64-cpu-only
## CAPTURE USER INPUT FROM PHYSX INSTALLER

#Check if msbuild is in path
msBuild_cmd_path = shutil.which("msbuild")

if(msBuild_cmd_path):
    print("msbuild Found")
else:
    print("msbuild was not found. Make sure it's installed and in system path")
    sys.exit(69)

sln_path = physx_src_path + "/compiler/vc17win64-cpu-only/PhysXSDK.sln"

process = subprocess.Popen(
    ['msbuild',sln_path,"/p:Configuration=Release","/p:RuntimeLibrary=MultiThreaded"],
    creationflags=subprocess.CREATE_NEW_CONSOLE
)
process.wait()

physx_dll_path = physx_src_path + "/bin/win.x86_64.vc143.mt/release"

files = []
for dirpath, dirnames, filenames in os.walk(physx_dll_path):
    print(f"files: {filenames}")
    files = filenames
    break

sln_dir = arguments[1] + "/Release"

for file in files:
    if file.find(".dll") > -1:
        if os.path.exists(sln_dir + "/" + file):
            print(file + " already exsists in binary")
        else:
            print("coping - " + file)
            shutil.copy(physx_dll_path + "/" + file, sln_dir + "/" + file)

