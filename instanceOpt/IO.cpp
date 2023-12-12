#include "IO.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Base.h"
#include "ffImage.h"
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

    _shader.setFloat("myMaterial." + _typeName + _numStr, i);
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

  glBufferData(GL_ARRAY_BUFFER, m_vertexVec.size() * sizeof(ffVertex),
               &m_vertexVec[0], GL_STATIC_DRAW);

  glGenBuffers(1, &_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexVec.size() * sizeof(uint),
               &m_indexVec[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
  glEnableVertexAttribArray(1);
  // offsetof: get the offset of the member in the struct
  // void*: means the offset is a pointer
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex),
                        (void*)offsetof(ffVertex, m_normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex),
                        (void*)offsetof(ffVertex, m_texCoord));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void ffModel::loadModel(std::string _path)
{
  // read file via ASSIMP
  Assimp::Importer _importer;
  const aiScene* _scene =
      _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !_scene->mRootNode)
  {
    std::cout << "model read fail!" << std::endl;
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

  // material
  if (_mesh->mMaterialIndex >= 0)
  {
    // _mesh->mMaterialIndex is the index of the material in the scene
    // every mesh has a material index
    aiMaterial* _mat = _scene->mMaterials[_mesh->mMaterialIndex];
    std::vector<ffTexture> _diffuseVec =
        loadMaterialTexture(_mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE_STR);
    // the reason why we use insert is that we want to append the vector
    _texVec.insert(_texVec.end(), _diffuseVec.begin(), _diffuseVec.end());

    std::vector<ffTexture> _specularVec =
        loadMaterialTexture(_mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR_STR);
    _texVec.insert(_texVec.end(), _specularVec.begin(), _specularVec.end());
  }

  return ffMesh(_vertexVec, _indexVec, _texVec);
}

std::vector<ffTexture> ffModel::loadMaterialTexture(aiMaterial* _mat,
                                                    aiTextureType _type,
                                                    std::string _typeName)
{
  std::vector<ffTexture> _texVec;

  // get the number of textures
  for (uint i = 0; i < _mat->GetTextureCount(_type); i++)
  {
    ffTexture _tex;
    // _mat->GetTexture will get the path of the texture
    //_path: the path  relative to the model file, its a relative path and value
    // preserved
    aiString _path;

    _mat->GetTexture(_type, i, &_path);

    _tex.m_id =
        ffTextureManager::getInstance()->createTexture(_path.C_Str(), m_dir);
    _tex.m_path = _path.C_Str();
    _tex.m_type = _typeName;

    _texVec.push_back(_tex);
  }

  return _texVec;
}

// draw the model with every
void ffModel::draw(Shader& _shader)
{
  for (uint i = 0; i < m_meshVec.size(); i++)
  {
    m_meshVec[i].draw(_shader);
  }
}

// set the ffTextureManager as a single instance ,point to nullptr for now
SINGLE_INSTANCE_SET(ffTextureManager)

uint ffTextureManager::createTexture(std::string _path)
{
  // iterate the map to find if the texture has been created
  std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
  if (_it != m_texMap.end())
  {
    return _it->second;
  }

  ffImage* _pImage = ffImage::readFromFile(_path.c_str());
  uint _texID = 0;
  glGenTextures(1, &_texID);
  glBindTexture(GL_TEXTURE_2D, _texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(),
               _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               _pImage->getData());

  delete _pImage;
  m_texMap[_path] = _texID;
  return _texID;
}

uint ffTextureManager::createTexture(std::string _path, std::string _dir)
{
  return createTexture(_dir + '/' + _path);
}
}  // namespace FF