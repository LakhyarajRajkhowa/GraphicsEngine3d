#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include "UUID.h"


namespace Lengine {

    struct MetaFile {
        UUID uuid;
        std::string type;
        std::string source;
        std::unordered_map<std::string, std::string> importSettings;
    };

    class MetaFileSystem {
    public:

        static bool HasMeta(const std::string& assetPath) {
            std::ifstream f(assetPath + ".meta");
            return f.good();
        }

        static MetaFile Load(const std::string& assetPath) {
            MetaFile meta;

            std::ifstream file(assetPath + ".meta");
            if (!file.good()) {
                // Should never happen unless meta missing
                meta.uuid = UUID();
                return meta;
            }

            std::string line;
            while (std::getline(file, line)) {

                if (line.find("uuid:") != std::string::npos) {
                    meta.uuid = UUID(std::stoull(line.substr(line.find(":") + 1)));
                }
                else if (line.find("type:") != std::string::npos) {
                    meta.type = line.substr(line.find(":") + 1);
                }
                else if (line.find("source:") != std::string::npos) {
                    meta.source = line.substr(line.find(":") + 1);
                }
            }

            return meta;
        }

        static void Save(const std::string& assetPath, const MetaFile& meta) {
            std::ofstream file(assetPath + ".meta");

            file << "uuid: " << meta.uuid.value() << "\n";
            file << "type: " << meta.type << "\n";
            file << "source: " << meta.source << "\n";

            for (auto& [key, val] : meta.importSettings) {
                file << key << ": " << val << "\n";
            }
        }

    };

}
