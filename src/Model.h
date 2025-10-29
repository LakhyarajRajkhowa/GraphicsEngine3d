#pragma once

#include "Mesh.h"
#include <memory>
#include "objLoader.h"

namespace Lengine {
	class Model {
	public:
		void loadModel(const std::string& path, std::shared_ptr<Lengine::Mesh>& mesh);

	};
}