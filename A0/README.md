## Assignment - 0
> ## Basics of OpenGL

***Main Problem Statement***: Generating a n-sided polygon/prism and rendering it using OpenGL in C++.

***Assignment PDF***: [CG_Assignment_0.pdf](./CG_Assignment_0.pdf)

***Coding Language***: C++

***Platform***: This assignment was done on Windows using Visual Studio 2022.

***Instructions***:
Firstly download this whole folder, and follow the below steps.

For running this assignment, you'll need to have Visual Studio installed on your system. (I used VS 2022, but you may consider higher versions of it)
Then you'll need the required libraries of OpenGL to run the code.

The libraries include:
- GLAD
- GLFW
- GLM
- Standard libraries of C++

You may use [vcpkg](https://github.com/microsoft/vcpkg) (atleast this is what I used in windows), which would make your life pretty easy when it comes to install the above libraries/packages. Below are the steps to install it:

Install using (Assuming you have vscode already):
- `git clone https://github.com/microsoft/vcpkg.git` (in `C:\src\vcpkg`)
- `.\vcpkg\bootstrap-vcpkg.bat`
- `.\vcpkg\vcpkg integrate install`
- `.\vcpkg\vcpkg install glad GLM glfw3 freetype --triplet=x64-windows`

After you have all the above packages and libraries, you can use the `run` functionality of VSCode, and then enter the number of sides.

Other features include camera and object movement using `U,I,O,J,K,L` keys and `W,A,S,D` keys respectively. Also, toggling on between prism and polygon can be done using `T` key.