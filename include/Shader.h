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
};