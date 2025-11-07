#pragma once
#include <memory>

#include "../resources/LoadOBJ.h"

namespace Lengine {
	class Model {
	public:
		void loadModel(const std::string& name, const std::string& path, std::shared_ptr<Lengine::Mesh>& mesh);

	};
}