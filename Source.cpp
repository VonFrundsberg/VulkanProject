#include "Application.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include "glTF_Loader.hpp"

int main() {
	GLTF_Loader::GLTF_Loader("glTF/cube.gltf");

	/*appNamespace::Application app;
	system("shaders\\compileFromSrc.bat");
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}