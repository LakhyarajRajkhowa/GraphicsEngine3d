#pragma once

#include <vector>
#include <string>

namespace Lengine {

	class IOManager
	{
	private:

	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};
}

