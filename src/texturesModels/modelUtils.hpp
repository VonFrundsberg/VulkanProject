#pragma once
#include "../buffers.hpp"

namespace appNamespace {
	class ModelUtils {
	public:

		ModelUtils();
		~ModelUtils();
		ModelUtils(const ModelUtils&) = delete;
		ModelUtils& operator=(const ModelUtils&) = delete;
		static int readGLTF_FromFile(const std::string& filepath);
	};
}