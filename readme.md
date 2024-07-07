Task completed by: Pavlov D.A. Phone: 89198350407
https://github.com/DonDimon1/PackageComparison

# **Description**
---------------------------------------------------------------------------------------------------------------------
The PackageComparison project is designed to compare packages between different branches of the ALT Linux distribution and generate a report in JSON format. The source code of the project consists of the following files:  
- libPackageComparison.hpp which declares the main structures and function prototypes for the program.
- libPackageComparison.cpp which implements functions for working with the site's API, processing responses, calculating, and creating a JSON report.
- main.cpp – the utility for user interaction and initiating all processes.

In compiled form, the program consists of the executable file  `PackageComparison` and the library `libPackageComparisonLib.so`.

# **Installation**
---------------------------------------------------------------------------------------------------------------------
## Prerequisites
Before installing PackageComparison, ensure that you have the following dependencies installed:

- C++ compiler supporting `C++11` standard
- `libcurl` (header files and development libraries)
- `nlohmann_json` (header files and development libraries)

For the `p10` branch, these dependencies can be downloaded here:
сurl: https://packages.altlinux.org/ru/p10/srpms/curl/rpms/
nlohmann-json: https://packages.altlinux.org/ru/p10/srpms/nlohmann-json/rpms/

# **Build Instructions**
---------------------------------------------------------------------------------------------------------------------
First, clone all files from the remote repository using git:
```
git clone https://github.com/DonDimon1/PackageComparison
```
###  Automatic Compilation and Program Launch
---------------------------------------------------------------------------------------------------------------------
Open a terminal in the folder where you downloaded the files from the remote repository and set permissions to run the build_and_run.sh script using the command:
```
chmod +x build_and_run.sh
```
Execute the script from the terminal using:
```
./build_and_run.sh
```
After that, the program source files will be compiled, then the program will run, and after its successful execution, a json file with the results of comparison of two branches selected by the user will be automatically opened. After that, the build folder will be created in the project folder, where the compiled program and the comparison result will be located. Also at the path /build/libPackageComparison/ there will be an extensible library libPackageComparisonLib.so. You can rerun the program using this script.
###  Automatic build with CMake
---------------------------------------------------------------------------------------------------------------------
If you are not satisfied with the first option, you can build and run the program using CMake commands in the terminal. To do this, open the terminal in the folder where you downloaded the files from the remote repository and run the following commands:
``` cmake
mkdir build
cd build
cmake ..
make 
```
After that, a build folder will be created in the project folder, where the compiled program will be located. To run the program, run the PackageComparison executable file, which is located in the build folder, using this command:
``` 
./PackageComparison
``` 
###  Manual Compilation
---------------------------------------------------------------------------------------------------------------------
If automatic compilation fails, you can build the library and executable manually. First, compile the libPackageComparison library. In the project directory where the source files were downloaded from the remote repository, open a terminal. Then compile the library using the general template:
``` 
g++ -std=c++11 -shared -fPIC -o name_new_library -I/path/to/headers -L/path/to/libs -lname_necessary_library
``` 
For example, in my case it looks like this:
``` 
g++ -std=c++11 -shared -fPIC -o libPackageComparison.so ./libPackageComparison/libPackageComparison.cpp -lcurl -I/usr/include/nlohmann/
``` 
Explanation of the compiler flags:
-shared: tells the compiler to create a shared library.
-fPIC: generates position-independent code, which is important for shared libraries.
-o libPackageComparison.so: specifies the name of the output shared library (.so extension is used for shared libraries in Linux).
path/to/libPackageComparison.cpp: the source file being compiled into the library.
-lcurl: ells the linker to add libcurl to the build process.
-I/usr/include/nlohmann/ tells the compiler where to find the nlohmann header files.

The directory should now contain the libPackageComparison.so library. Next, compile the main.cpp utility. Generally:
``` 
g++ -o my_program_name main.cpp -I/path/to/headers -L/path/to/libs -lname_necessary_library
``` 
For example, in my case it looks like this:
``` 
g++ -o PackageComparison main.cpp -I./libPackageComparison -L./  -lPackageComparison
``` 
-lPackageComparison tells the compiler to use the libPackageComparison.so library.
-L./ specifies the path where the libPackageComparison.so library is located.

After compiling main.cpp, the executable file of the program named PackageComparison will appear. Run the program with the following command:
``` 
./PackageComparison
``` 
After the program finishes, a JSON file with the results of the program's execution will be created in the project directory.

Thank you for your attention!
