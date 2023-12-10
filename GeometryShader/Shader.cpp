#include "Shader.h"
void Shader::initShader(const char* _vertexPath, const char* _fragPath,
                        const char* _geoPath)
{
  // shader�ı���
  unsigned int _vertexID = 0, _fragID = 0, _geoID = 0;
  _vertexID = compileShader(_vertexPath, GL_VERTEX_SHADER);
  _fragID = compileShader(_fragPath, GL_FRAGMENT_SHADER);
  _geoID = compileShader(_geoPath, GL_GEOMETRY_SHADER);

  char _infoLog[512];
  int _successFlag = 0;

  // ����
  m_shaderProgram = glCreateProgram();
  if (_vertexID != -1)
  {
    glAttachShader(m_shaderProgram, _vertexID);
  }
  if (_fragID != -1)
  {
    glAttachShader(m_shaderProgram, _fragID);
  }
  if (_geoID != -1)
  {
    glAttachShader(m_shaderProgram, _geoID);
  }
  glLinkProgram(m_shaderProgram);

  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
  if (!_successFlag)
  {
    glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
    std::string errStr(_infoLog);
    std::cout << _infoLog << std::endl;
  }
  glDeleteShader(_vertexID);
  glDeleteShader(_fragID);
  glDeleteShader(_geoID);
}

uint Shader::compileShader(const char* _shaderPath, GLint _shaderType)
{
  if (_shaderPath == "" || _shaderPath == NULL)
  {
    return -1;
  }

  std::string _shaderCode("");

  std::ifstream _shaderFile;

  _shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    _shaderFile.open(_shaderPath);

    std::stringstream _shaderStream;
    _shaderStream << _shaderFile.rdbuf();

    _shaderCode = _shaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::string errStr = "read shader fail";
    std::cout << errStr << std::endl;
  }
  const char* _shaderStr = _shaderCode.c_str();

  // shader�ı�������
  unsigned int _shaderID = 0;
  char _infoLog[512];
  int _successFlag = 0;

  // ����
  _shaderID = glCreateShader(_shaderType);
  glShaderSource(_shaderID, 1, &_shaderStr, NULL);
  glCompileShader(_shaderID);

  glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &_successFlag);
  if (!_successFlag)
  {
    glGetShaderInfoLog(_shaderID, 512, NULL, _infoLog);
    std::string errStr(_infoLog);
    std::cout << _infoLog << std::endl;
  }

  return _shaderID;
}

void Shader::setMatrix(const std::string& _name, glm::mat4 _matrix) const
{
  glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(_matrix));
}

void Shader::setVec3(const std::string& _name, glm::vec3 _vec3) const
{
  glUniform3fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1,
               glm::value_ptr(_vec3));
}

void Shader::setFloat(const std::string& _name, float _f) const
{
  glUniform1f(glGetUniformLocation(m_shaderProgram, _name.c_str()), _f);
}

void Shader::setInt(const std::string& _name, int _i) const
{
  glUniform1i(glGetUniformLocation(m_shaderProgram, _name.c_str()), _i);
}