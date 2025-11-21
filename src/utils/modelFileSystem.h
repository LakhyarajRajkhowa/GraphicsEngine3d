#pragma once
#include "../utils/UUID.h"
#include "../utils/metaFileSystem.h"



namespace Lengine {

    struct ModelSubmeshInfo {
        std::string name;
        UUID materialUUID;
    };

    struct ModelFile {
        UUID uuid;                
        std::string type;         
        UUID sourceMeshId;        
        std::vector<ModelSubmeshInfo> submeshes;
    };


    class ModelFileSystem {
    private:
        std::string modelFolderPath;
    public:
        ModelFileSystem()
        {
            EngineSettings settings;
            modelFolderPath = settings.gameFolderPath + "/internals/model/";
        }
        
        static bool Exists(const std::string& filePath) {
            std::ifstream f(filePath + ".model");
            return f.good();
        }

        static ModelFile Load(const std::string& filePath) {
            ModelFile model;
            std::ifstream file(filePath + ".model");

            if (!file.good()) {
                model.uuid = UUID::Null;
                return model;
            }

            std::string line;
            while (std::getline(file, line)) {

                // Remove whitespace at ends
                line = StripQuotes(line);

                if (line.rfind("uuid:", 0) == 0) {
                    model.uuid = UUID(std::stoull(line.substr(5)));
                }
                else if (line.rfind("type:", 0) == 0) {
                    model.type = line.substr(5);
                }
                else if (line.rfind("sourceMeshId:", 0) == 0) {
                    model.sourceMeshId = UUID(std::stoull(line.substr(13)));
                }
                else if (line.rfind("submesh:", 0) == 0) {
                    // Format:  submesh: <name> <materialId>
                    std::string rest = line.substr(8);

                    size_t space = rest.find(' ');
                    if (space != std::string::npos) {
                        ModelSubmeshInfo info;
                        info.name = rest.substr(0, space);
                        info.materialUUID = UUID(std::stoull(rest.substr(space + 1)));
                        model.submeshes.push_back(info);
                    }
                }
            }

            return model;
        }

        static void Save(const std::string& filePath, const ModelFile& model) {
            std::ofstream file(filePath + ".model");

            file << "uuid: " << model.uuid.value() << "\n";
            file << "type: " << model.type << "\n";
            file << "sourceMeshId: " << model.sourceMeshId.value() << "\n\n";

            for (auto& sm : model.submeshes) {
                file << "submesh: " << sm.name
                    << " " << sm.materialUUID.value() << "\n";
            }
        }

    };

}
