#include "IO.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

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

void ffModel::loadModel(const char* _path)
{
  // read file via ASSIMP
  Assimp::Importer _importer;
  const aiScene* _scene =
      _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !_scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << _importer.GetErrorString() << std::endl;
    return;
  }

  m_dir = _path.substr(0, _path.find_last_of('/'));
  processNode(_scene->mRootNode, _scene);
}
// processNode will be called recursively, aiNode is a treem aiScene is a tree
void ffModel::processNode(aiNode* _node, const aiScene* _scene)
{
  for (uint i = 0; i < _node->mNumMeshes; i++)
  {
    aiMesh* _mesh = _scene->mMeshes[_node->mMeshes[i]];
    m_meshVec.push_back(processMesh(_mesh, _scene));
  }

  for (uint i = 0; i < _node->mNumChildren; i++)
  {
    processNode(_node->mChildren[i], _scene);
  }
}
ffMesh ffModel::processMesh(aiMesh* _mesh, const aiScene* _scene)
{
  std::vector<ffVertex> _vertexVec;
  std::vector<uint> _indexVec;
  std::vector<ffTexture> _texVec;

  for (uint i = 0; i < _mesh->mNumVertices; i++)
  {
    ffVertex _vertex;

    // position
    glm::vec3 _pos;
    _pos.x = _mesh->mVertices[i].x;
    _pos.y = _mesh->mVertices[i].y;
    _pos.z = _mesh->mVertices[i].z;
    _vertex.m_pos = _pos;

    // normal
    glm::vec3 _normal;
    _normal.x = _mesh->mNormals[i].x;
    _normal.y = _mesh->mNormals[i].y;
    _normal.z = _mesh->mNormals[i].z;
    _vertex.m_normal = _normal;

    // UV
    // mesh can have up to 8 different texture coordinates, 0 is the basic uv
    if (_mesh->mTextureCoords[0])
    {
      glm::vec3 _texCoord;
      _texCoord.x = _mesh->mTextureCoords[0][i].x;
      _texCoord.y = _mesh->mTextureCoords[0][i].y;
      _vertex.m_texCoord = _texCoord;
    }

    _vertexVec.push_back(_vertex);
  }
  // index
  // mNumFaces: the number of faces
  for (uint i = 0; i < _mesh->mNumFaces; i++)
  {
    // mFaces: the array of faces
    aiFace _face = _mesh->mFaces[i];
    // mNumIndices: the number of indices of the face
    for (uint j = 0; j < _face.mNumIndices; j++)
    {
      _indexVec.push_back(_face.mIndices[j]);
    }
  }
}
}  // namespace FF