#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "Mesh.h"

namespace Lengine {

    inline void loadOBJ(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << path << "\n";
            return;
        }

        std::vector<glm::vec3> temp_positions;
        std::vector<glm::vec3> temp_normals;
        std::vector<glm::vec2> temp_texCoords;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                temp_positions.push_back(pos);
            }
            else if (type == "vt") {
                glm::vec2 uv;
                iss >> uv.x >> uv.y;
                temp_texCoords.push_back(uv);
            }
            else if (type == "vn") {
                glm::vec3 n;
                iss >> n.x >> n.y >> n.z;
                temp_normals.push_back(n);
            }
            else if (type == "f") {
                std::string v1, v2, v3;
                iss >> v1 >> v2 >> v3;
                std::string faceVertices[3] = { v1, v2, v3 };

                for (auto& fv : faceVertices) {
                    unsigned int vIdx = 0, vtIdx = 0, vnIdx = 0;

                    // Try all 4 possible formats
                    if (sscanf_s(fv.c_str(), "%u/%u/%u", &vIdx, &vtIdx, &vnIdx) == 3) {
                        // v/t/n
                    }
                    else if (sscanf_s(fv.c_str(), "%u//%u", &vIdx, &vnIdx) == 2) {
                        // v//n
                    }
                    else if (sscanf_s(fv.c_str(), "%u/%u", &vIdx, &vtIdx) == 2) {
                        // v/t
                    }
                    else if (sscanf_s(fv.c_str(), "%u", &vIdx) == 1) {
                        // v
                    }
                    else {
                        std::cerr << "Unknown face format: " << fv << "\n";
                        continue;
                    }

                    // Safety checks before indexing
                    if (vIdx == 0 || vIdx > temp_positions.size()) {
                        std::cerr << "Invalid vertex index: " << vIdx << "\n";
                        continue;
                    }

                    Vertex vertex;
                    vertex.position = temp_positions[vIdx - 1];

                    if (vtIdx > 0 && vtIdx <= temp_texCoords.size())
                        vertex.texCoord = temp_texCoords[vtIdx - 1];
                    else
                        vertex.texCoord = glm::vec2(0.0f, 0.0f);

                    if (vnIdx > 0 && vnIdx <= temp_normals.size())
                        vertex.normal = temp_normals[vnIdx - 1];
                    else
                        vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // fallback

                    vertices.push_back(vertex);
                    indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
                }
            }
        }

        file.close();

        std::cout << "Loaded OBJ: " << path << std::endl;
          
    }

    
}
