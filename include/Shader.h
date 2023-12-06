#include "Base.h"

class Shader
{
 private:
  unsigned int m_shaderProgram;

 public:
  Shader() { m_shaderProgram = 0; }
  ~Shader() {}

  void initShader(const char* _vertexPath, const char* _fragPath);
  inline void start() { glUseProgram(m_shaderProgram); }
  inline void end() { glUseProgram(0); }
  void setMatrix(const std::string& _name, const glm::mat4& _matrix);
  void setVec3(const std::string& _name, const glm::vec3& _vec);
  void setFloat(const std::string& _name, float _value);
  void setInt(const std::string& _name, int _value);
};