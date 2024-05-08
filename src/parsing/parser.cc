#include "parser.h"
#include <iostream>
#include <sstream>
#include <string>

auto basic_obj_parse(std::istream& input) -> obj_t
{
    std::vector<std::array<double, 3>> vertexes;
    std::vector<std::array<double, 3>> normals;
    std::vector<std::array<double, 2>> uvs;
    std::vector<std::vector<VertexAttributes>> faces;
    std::string word{};
    while (!input.eof()) {
        input >> word;
        if (word == "v") {
            std::array<double, 3> v{};
            input >> v[0];
            input >> v[1];
            input >> v[2];
            vertexes.push_back(v);
        } else if (word == "vn") {
            std::array<double, 3> v{};
            input >> v[0];
            input >> v[1];
            input >> v[2];
            normals.push_back(v);
        } else if (word == "vt") {
            std::array<double, 2> v{};
            input >> v[0];
            input >> v[1];
            uvs.push_back(v);
        } else if (word == "f") {
            std::string line;
            std::getline(input, line);
            std::stringstream liness(line);
            std::vector<VertexAttributes> face;
            while (!liness.eof()) {
                size_t vertex_index{};
                std::optional<size_t> normal_index_opt{};
                std::optional<size_t> uv_opt{};
                liness >> word;
                std::stringstream ss(word);
                ss >> vertex_index;
                if (!ss.eof()) {
                    char c{};
                    size_t idx;
                    ss >> c;
                    ss >> idx;
                    normal_index_opt = idx;
                }
                if (!ss.eof()) {
                    char c{};
                    size_t idx;
                    ss >> c;
                    ss >> idx;
                    uv_opt = idx;
                }
                face.push_back(
                    VertexAttributes(vertex_index, normal_index_opt, uv_opt)
                );
            }
            faces.push_back(face);
        } else {
            std::string line;
            std::getline(input, line);
        }
    }

    return {vertexes, normals, uvs, faces};
}
