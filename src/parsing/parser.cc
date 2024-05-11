#include "parser.h"

#include <iostream>
#include <sstream>
#include <string>

auto VertexObject::parse_file(std::istream& input) -> void
{
    std::string word{};
    while (!input.eof()) {
        input >> word;
        if (word == "v") {
            std::array<double, 3> v{};
            input >> v[0];
            input >> v[1];
            input >> v[2];
            vertex_positions.push_back(v);
        } else if (word == "vn") {
            std::array<double, 3> v{};
            input >> v[0];
            input >> v[1];
            input >> v[2];
            vertex_normals.push_back(v);
        } else if (word == "vt") {
            std::array<double, 2> v{};
            input >> v[0];
            input >> v[1];
            vertex_uvs.push_back(v);
        } else if (word == "f") {
            std::string line;
            std::getline(input, line);
            std::stringstream liness(line);
            std::vector<VertexAttributes> face;
            while (!liness.eof()) {
                size_t vertex_index{};
                std::optional<size_t> uv_opt{};
                std::optional<size_t> normal_opt{};
                liness >> word;
                std::stringstream ss(word);
                ss >> vertex_index;
                if (!ss.eof()) {
                    char c{};
                    size_t idx;
                    ss >> c;
                    ss >> idx;
                    uv_opt = idx;
                }
                if (!ss.eof()) {
                    char c{};
                    size_t idx;
                    ss >> c;
                    ss >> idx;
                    normal_opt = idx;
                }
                face.push_back(
                    VertexAttributes(vertex_index, normal_opt, uv_opt)
                );
            }
            faces.push_back(face);
        } else {
            std::string line;
            std::getline(input, line);
        }
    }
}

auto VertexObject::extract_triangles() const -> std::vector<Triangle>
{
    std::vector<Triangle> triangles;
    for (auto va : faces) {
        // this is a quad so we need two triangles
        // so far I am parsing obj files generated by blender
        // and I have not scene raw triangles yet.
        if (va.size() == 4) {
            const bool has_normal =
                va[0].has_normal_idx() && va[1].has_normal_idx() &&
                va[2].has_normal_idx() && va[3].has_normal_idx();
            [[maybe_unused]] const bool has_uv =
                va[0].has_uv_idx() && va[1].has_uv_idx() &&
                va[2].has_uv_idx() && va[3].has_uv_idx();
            if (has_normal && has_uv) {
                triangles.push_back(Triangle{
                    &vertex_positions[va[0].get_position_idx()],
                    &vertex_positions[va[1].get_position_idx()],
                    &vertex_positions[va[2].get_position_idx()],
                    &vertex_normals[va[0].get_normal_idx()],
                    &vertex_normals[va[1].get_normal_idx()],
                    &vertex_normals[va[2].get_normal_idx()],
                    &vertex_uvs[va[0].get_uv_idx()],
                    &vertex_uvs[va[1].get_uv_idx()],
                    &vertex_uvs[va[2].get_uv_idx()]
                });
                triangles.push_back(Triangle{
                    &vertex_positions[va[2].get_position_idx()],
                    &vertex_positions[va[3].get_position_idx()],
                    &vertex_positions[va[0].get_position_idx()],
                    &vertex_normals[va[2].get_normal_idx()],
                    &vertex_normals[va[3].get_normal_idx()],
                    &vertex_normals[va[0].get_normal_idx()],
                    &vertex_uvs[va[2].get_uv_idx()],
                    &vertex_uvs[va[3].get_uv_idx()],
                    &vertex_uvs[va[0].get_uv_idx()],
                });
            }
        } else if (va.size() == 3) {
            triangles.push_back(Triangle{
                &vertex_positions[va[0].get_position_idx()],
                &vertex_positions[va[1].get_position_idx()],
                &vertex_positions[va[2].get_position_idx()],
            });
        } else {
            std::cerr << "something is wrong with parsing\n";
        }
    }
    return triangles;
}

VertexObject::VertexObject(std::istream& input) { parse_file(input); }
