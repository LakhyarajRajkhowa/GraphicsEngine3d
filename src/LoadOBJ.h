#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace Lengine {

    struct VertexKey {
        int v, vt, vn;
        bool operator==(const VertexKey& other) const {
            return v == other.v && vt == other.vt && vn == other.vn;
        }
    };

    struct VertexKeyHasher {
        size_t operator()(const VertexKey& k) const {
            return (k.v * 73856093) ^ (k.vt * 19349663) ^ (k.vn * 83492791);
        }
    };

    // Set verbose = true for detailed per-line logging.
    inline void loadOBJ(const std::string& path, Mesh& mesh, bool verbose = false) {

        // --- Temporary raw data ---
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;

        // --- For building current submesh ---
        std::vector<Vertex> buildVerts;
        std::vector<unsigned int> buildIndices;

        std::unordered_map<VertexKey, unsigned int, VertexKeyHasher> vertexCache;

        // --- Helpers ---
        auto pushCurrentSubMeshIfNotEmpty = [&]() {
            if (!buildVerts.empty() && !buildIndices.empty()) {
                mesh.subMeshes.emplace_back(buildVerts, buildIndices);
                if (verbose) {
                    std::cout << "[OBJ] Pushed submesh (verts=" << buildVerts.size()
                        << ", indices=" << buildIndices.size() << ")\n";
                }
            }
            else {
                if (verbose && (!buildVerts.empty() || !buildIndices.empty())) {
                    std::cout << "[OBJ] WARNING: not pushing partially filled submesh: verts="
                        << buildVerts.size() << " indices=" << buildIndices.size() << "\n";
                }
            }
            buildVerts.clear();
            buildIndices.clear();
            vertexCache.clear();
            };

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open OBJ: " << path << "\n";
            return;
        }
        else {
            std::cout << "File loaded: " << path << "\n";
        }

        std::string line;
        size_t lineNo = 0;
        while (std::getline(file, line)) {
            ++lineNo;
            if (verbose) {
                std::cout << "[OBJ] Line " << lineNo << ": " << line << "\n";
            }

            // Trim leading spaces (simple)
            size_t firstNon = line.find_first_not_of(" \t\r\n");
            if (firstNon == std::string::npos) continue; // empty line
            std::string trimmed = line.substr(firstNon);

            std::istringstream iss(trimmed);
            std::string type;
            if (!(iss >> type)) continue; // nothing to read

            // ------------------------------
            // Vertex Positions
            // ------------------------------
            if (type == "v") {
                glm::vec3 p;
                if (!(iss >> p.x >> p.y >> p.z)) {
                    std::cerr << "[OBJ][Error] Line " << lineNo << " malformed v: '" << line << "'\n";
                    continue;
                }
                positions.push_back(p);
                if (verbose) std::cout << "[OBJ] v[" << positions.size() << "] = (" << p.x << "," << p.y << "," << p.z << ")\n";
            }

            // ------------------------------
            // Texture Coordinates
            // ------------------------------
            else if (type == "vt") {
                glm::vec2 uv;
                if (!(iss >> uv.x >> uv.y)) {
                    std::cerr << "[OBJ][Error] Line " << lineNo << " malformed vt: '" << line << "'\n";
                    continue;
                }
                texcoords.push_back(uv);
                if (verbose) std::cout << "[OBJ] vt[" << texcoords.size() << "] = (" << uv.x << "," << uv.y << ")\n";
            }

            // ------------------------------
            // Normals
            // ------------------------------
            else if (type == "vn") {
                glm::vec3 n;
                if (!(iss >> n.x >> n.y >> n.z)) {
                    std::cerr << "[OBJ][Error] Line " << lineNo << " malformed vn: '" << line << "'\n";
                    continue;
                }
                normals.push_back(n);
                if (verbose) std::cout << "[OBJ] vn[" << normals.size() << "] = (" << n.x << "," << n.y << "," << n.z << ")\n";
            }

            // ------------------------------
            // New Material → New Submesh
            // ------------------------------
            else if (type == "usemtl") {
                std::string matname;
                std::getline(iss, matname);
                // trim leading spaces
                size_t t = matname.find_first_not_of(" \t");
                if (t != std::string::npos) matname = matname.substr(t);
                if (verbose) std::cout << "[OBJ] usemtl '" << matname << "' (line " << lineNo << ") -> starting new submesh\n";
                pushCurrentSubMeshIfNotEmpty();
            }

            // ------------------------------
            // New Group → New Submesh
            // ------------------------------
            else if (type == "g") {
                std::string gname;
                std::getline(iss, gname);
                size_t t = gname.find_first_not_of(" \t");
                if (t != std::string::npos) gname = gname.substr(t);
                if (verbose) std::cout << "[OBJ] group '" << gname << "' (line " << lineNo << ") -> starting new submesh\n";
                pushCurrentSubMeshIfNotEmpty();
            }

            // ------------------------------
            // Faces
            // ------------------------------
            else if (type == "f") {
                std::string fv;
                std::vector<unsigned int> faceIndices;

                size_t faceVertexCount = 0;
                while (iss >> fv) {
                    ++faceVertexCount;

                    int v = 0, vt = 0, vn = 0;
                    int scanned = 0;

                    // reset parsed values to 0 before sscanf
                    v = vt = vn = 0;

                    // try all formats and capture how many matched
                    if ((scanned = sscanf(fv.c_str(), "%d/%d/%d", &v, &vt, &vn)) == 3) {
                        // v/vt/vn
                    }
                    else if ((scanned = sscanf(fv.c_str(), "%d//%d", &v, &vn)) == 2) {
                        // v//vn
                        vt = 0;
                    }
                    else if ((scanned = sscanf(fv.c_str(), "%d/%d", &v, &vt)) == 2) {
                        // v/vt
                        vn = 0;
                    }
                    else if ((scanned = sscanf(fv.c_str(), "%d", &v)) == 1) {
                        // v
                        vt = vn = 0;
                    }
                    else {
                        std::cerr << "[OBJ][Error] Line " << lineNo << " unsupported face token '" << fv << "'\n";
                        continue;
                    }

                    if (verbose) {
                        std::cout << "[OBJ]   face token '" << fv << "' -> scanned=" << scanned
                            << " v=" << v << " vt=" << vt << " vn=" << vn << "\n";
                    }

                    // Handle negative indices (relative indexing)
                    auto resolveIndex = [&](int idx, size_t containerSize) -> int {
                        if (idx > 0) return idx;
                        if (idx < 0) return static_cast<int>(containerSize) + idx + 1; // -1 -> last element
                        return 0;
                        };

                    int rv = resolveIndex(v, positions.size());
                    int rvt = resolveIndex(vt, texcoords.size());
                    int rvn = resolveIndex(vn, normals.size());

                    // Validate resolved indices
                    if (rv <= 0 || rv > static_cast<int>(positions.size())) {
                        std::cerr << "[OBJ][Error] Line " << lineNo << " vertex index out of range: v=" << v
                            << " resolved=" << rv << " positions.size=" << positions.size() << "  (token='" << fv << "')\n";
                        return;
                    }
                    if (rvt < 0 || rvt > static_cast<int>(texcoords.size())) {
                        std::cerr << "[OBJ][Error] Line " << lineNo << " texcoord index out of range: vt=" << vt
                            << " resolved=" << rvt << " texcoords.size=" << texcoords.size() << "  (token='" << fv << "')\n";
                        rvt = 0; // treat as missing
                        return;
                    }
                    if (normals.empty()) {
                        // No normals provided in file → ignore vn completely
                        rvn = 0;
                    }
                    else if (rvn <= 0 || rvn > (int)normals.size()) {
                        std::cerr << "[OBJ][Warning] Line " << lineNo
                            << ": incorrect normal index (" << rvn
                            << ") out of " << normals.size()
                            << " using default normal.\n";
                        rvn = 0; // fallback normal
                    }

                    VertexKey key{ rv, rvt, rvn };

                    // Check if vertex exists in cache
                    auto it = vertexCache.find(key);
                    if (it != vertexCache.end()) {
                        faceIndices.push_back(it->second);
                        continue;
                    }

                    // Build new vertex (safe accesses)
                    Vertex vert{};
                    vert.position = positions[rv - 1];
                    vert.texCoord = (rvt > 0) ? texcoords[rvt - 1] : glm::vec2(0.0f, 0.0f);
                    vert.normal = (rvn > 0) ? normals[rvn - 1] : glm::vec3(0.0f, 1.0f, 0.0f);
                    vert.tangent = glm::vec3(0.0f);
                    vert.bitangent = glm::vec3(0.0f);

                    buildVerts.push_back(vert);
                    unsigned int idx = static_cast<unsigned int>(buildVerts.size() - 1);
                    vertexCache[key] = idx;
                    faceIndices.push_back(idx);
                }

                if (faceIndices.size() < 3) {
                    std::cerr << "[OBJ][Error] Line " << lineNo << " face has fewer than 3 vertices (found "
                        << faceIndices.size() << "): '" << line << "'\n";
                    continue;
                }

                // Triangulate if needed (fan method)
                for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                    buildIndices.push_back(faceIndices[0]);
                    buildIndices.push_back(faceIndices[i]);
                    buildIndices.push_back(faceIndices[i + 1]);
                }
                if (verbose) {
                    std::cout << "[OBJ]   created " << (faceIndices.size() - 2) << " triangle(s) from face on line " << lineNo << "\n";
                }
            }

            // other tokens are ignored
        }

        // Final submesh (if any)
        pushCurrentSubMeshIfNotEmpty();

        file.close();

        if (mesh.subMeshes.empty()) {
            std::cerr << "[OBJ] Warning: no submeshes were created for file: " << path << "\n";
        }
        else {
            std::cout << "[OBJ] loadOBJ: created " << mesh.subMeshes.size() << " submesh(es)\n";
        }
    }

} // namespace Lengine
