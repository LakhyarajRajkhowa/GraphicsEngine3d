#pragma once

#include <string>

#include "../external/tinyfiledialogs.h"

namespace Lengine {
	inline std::string getObjFilePath(){
        const char* filters[] = { "*.obj" };

        const char* filePath = tinyfd_openFileDialog(
            "Select OBJ File",
            "",
            1,
            filters,
            NULL,
            0
        );

        if (filePath) {
            printf("Loaded file: %s\n", filePath);
            // Load your mesh here...
        }

        return filePath;
	}
}

