#include "modelUtils.hpp"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>

using namespace rapidjson;

namespace appNamespace{

    int ModelUtils::readGLTF_FromFile(const std::string& filepath)
    {
        // Open the file
        std::ifstream file(filepath);

        // Read the entire file into a string
        std::string json((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        // Create a Document object 
        // to hold the JSON data
        Document modelInfo;

        // Parse the JSON data
        modelInfo.Parse(json.c_str());

        // Check for parse errors
        if (modelInfo.HasParseError()) {
            std::cerr << "Error parsing JSON: "
                << modelInfo.GetParseError() << " \n ";
            return -1;
        }
        modelInfo["asset"]
    }
}
