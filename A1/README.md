## Assignment - 1
> ## 2-D Game in OpenGL

***Main Problem Statement***: Making a clone of Jetpack Joyride using OpenGL. Major components include the moving character, traps, background, and coins.

***Assignment PDF***: [CG_Assignment_1.pdf](./CG_Assignment_1.pdf)

***Coding Language***: C++ with OOPs

***Platform***: This assignment was done on Windows using Visual Studio 2022.

***Instructions***:
Firstly download this whole folder, and follow the below steps.

For running this assignment, you'll need to have Visual Studio installed on your system. (I used VS 2022, but you may consider higher versions of it)
Then you'll need the required libraries of OpenGL to run the code.

The libraries include:
- GLAD
- GLFW
- GLM
- freetype
- Irrklang (Here, included with the project itself)
- Standard libraries of C++

You may use [vcpkg](https://github.com/microsoft/vcpkg) (atleast this is what I used in windows), which would make your life pretty easy when it comes to install the above libraries/packages. Below are the steps to install it:

Install using (Assuming you have vscode already):
- `git clone https://github.com/microsoft/vcpkg.git` (in `C:\src\vcpkg`)
- `.\vcpkg\bootstrap-vcpkg.bat`
- `.\vcpkg\vcpkg integrate install`
- `.\vcpkg\vcpkg install glad GLM glfw3 freetype --triplet=x64-windows`

After you have all the above packages and libraries, you can use the `run` functionality of VSCode.

You must press space to accelerate the character up. You have three lives, so prevent colliding with the obstacles. There are total of 3 levels, and you must complete them to win the game. Collect more coins to increase your score.

(You may need to change the directory of the shader and other files to your local directory)
