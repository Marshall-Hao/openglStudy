#include "IO.h"

namespace FF
{
ffMesh::ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec,
               std::vector<ffTexture> _texVec)
{
  m_vertexVec = _vertexVec;
  m_indexVec = _indexVec;
  m_texVec = _texVec;
  setupMesh();
}

void ffMesh::draw(Shader& _shader)
{
  uint _diffuseN = 1;
  uint _specularN = 1;

  for (uint i = 0; i < m_texVec.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string _typeName = m_texVec[i].m_type;
    std::string _numStr = "";
    if (_typeName == TEXTURE_DIFFUSE_STR)
    {
      // after the assignment, _numStr + 1
      _numStr = std::to_string(_diffuseN++);
    }
    if (_typeName == TEXTURE_SPECULAR_STR)
    {
      _numStr = std::to_string(_specularN++);
    }

    _shader.setFloat("myMaterial." + _typename + _numStr, i);
    glBindTexture(GL_TEXTURE_2D, m_texVec[i].m_id);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_indexVec.size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void ffMesh::setupMesh()
{
  // vertex array object
  uint _VBO = 0;
  // element buffer object
  uint _EBO = 0;
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);
  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  glbufferData(GL_ARRAY_BUFFER, m_vertexVec.size() * sizeof(ffVertex),
               &m_vertexVec[0], GL_STATIC_DRAW);

  glGenBuffers(1, &_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexVec.size() * sizeof(uint),
               &m_indexVec[0], GL_STATIC_DRAW);

  glenableVertexAttribArray(0);
  glvertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
  glenableVertexAttribArray(1);
  // offsetof: get the offset of the member in the struct
  // void*: means the offset is a pointer
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex),
                        (void*)offsetof(ffVertex, m_normal));
  glenableVertexAttribArray(2);
  glvertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex),
                        (void*)offsetof(ffVertex, m_texCoord));

  glBindbuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
}  // namespace FF