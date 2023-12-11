#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Base.h"
#include "Shader.h"
namespace FF
{
// struct will make the memory align, and stay together
struct ffVertex
{
  glm::vec3 m_pos;
  glm::vec3 m_normal;
  glm::vec2 m_texCoord;
  ffVertex()
  {
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
    m_texCoord = glm::vec2(0.0f, 0.0f);
  }
};

#define TEXTURE_DIFFUSE_STR "texture_diffuse"
#define TEXTURE_SPECULAR_STR "texture_specular"

struct ffTexture
{
  uint m_id;
  std::string m_type;
  std::string m_path;
};

class ffMesh
{
 public:
  ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec,
         std::vector<ffTexture> _texVec);

  void draw(Shader& _shader);

 private:
  std::vector<ffVertex> m_vertexVec;
  std::vector<uint> m_indexVec;
  std::vector<ffTexture> m_texVec;

  uint m_VAO;
  uint m_VBO;
  void setupMesh();
};

class ffModel
{
 public:
  ffModel(std::string _path) { loadModel(_path); }
  void draw(Shader& _shader);

 private:
  std::vector<ffMesh> m_meshVec;
  std::string m_dir;

  void loadModel(std::string _path);
  // processNode will be called recursively, aiNode is a treem aiScene is a tree
  void processNode(aiNode* _node, const aiScene* _scene);
  ffMesh processMesh(aiMesh* _mesh, const aiScene* _scene);
  // loadMaterialTexture will be called recursively
  std::vector<ffTexture> loadMaterialTexture(aiMaterial* _mat,
                                             aiTextureType _type,
                                             std::string _typeName);
};

class ffTextureManager
{
 public:
  void SINGLE_OVER() {}
  uint createTexture(std::string _path);
  uint createTexture(std::string _path, std::string _dir);

 private:
  SINGLE_INSTANCE(ffTextureManager)
  ffTextureManager() {}

  std::map<std::string, uint> m_texMap;
};
}  // namespace FF