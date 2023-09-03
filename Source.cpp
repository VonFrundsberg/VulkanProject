#include "Application.hpp"
#include <chrono>
#include <thread>
#include <iostream>


#include "rapidjson/document.h"
#include <fstream>

using namespace rapidjson;

int main() {


	appNamespace::Application app;
	//system("shaders\\compileFromSrc.bat");
	/*try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}