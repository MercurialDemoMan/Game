#include "Object.hpp"

void Object::setupVertexAttributes(const Engine3D::Shader& shader)
{
    m_mesh.bindVertexPositionWithShader(shader, "in_pos");
    m_mesh.bindVertexNormalWithShader(shader, "in_nor");
    m_mesh.bindVertexUVWithShader(shader, "in_uv");
}