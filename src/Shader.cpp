#include "Shader.h"

void Shader::initShader(const char* _vertexPath, const char* _fragPath)
{
  std::string _vertexCode("");
  std::string _fragCode("");

  // read the shader code from the file
  std::ifstream _vertexFile;
  std::ifstream _fragFile;

  // ensure ifstream objects can throw exceptions
  _vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  _fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    // open  files
    _vertexFile.open(_vertexPath);
    _fragFile.open(_fragPath);
    // read file's buffer contents into streams, stringstream is a stream, can
    // be dynamic
    std::stringstream _vertexStream, _fragStream;
    // read file buffer content into stream
    _vertexStream << _vertexFile.rdbuf();
    _fragStream << _fragFile.rdbuf();
    // close file handlers
    _vertexCode = _vertexStream.str();
    _fragCode = _fragStream.str();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  // convert string to char*
  // c_str() convert string to char*
  const char* _vShaderStr = _vertexCode.c_str();
  const char* _fShaderStr = _fragCode.c_str();

  // shader program
  unsigned int _vertexID = 0, _fragID = 0;
  char _infoLog[512];
  int _successFlag;

  // create vertex shader
  // shader ID
  _vertexID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(_vertexID, 1, &_vShaderStr, NULL);
  glCompileShader(_vertexID);
  // check for shader compile errors
  glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);
  if (!_successFlag)
  {
    glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
    std::string errStr(_infoLog);
    std::cout << errStr << std::endl;
  }

  _fragID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(_fragID, 1, &_fShaderStr, NULL);
  glCompileShader(_fragID);

  glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
  if (!_successFlag)
  {
    glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
    std::string errStr(_infoLog);
    std::cout << errStr << std::endl;
  }

  // create shader program
  m_shaderProgram = glCreateProgram();
  // attach shader to the program
  glAttachShader(m_shaderProgram, _vertexID);
  glAttachShader(m_shaderProgram, _fragID);
  // link the program
  glLinkProgram(m_shaderProgram);

  // check for linking errors
  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
  if (!_successFlag)
  {
    glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
    std::string errStr(_infoLog);
    std::cout << errStr << std::endl;
  }
  // delete the shader, its all has been linked to the program
  glDeleteShader(_vertexID);
  glDeleteShader(_fragID);
}

void Shader::setMatrix(const std::string& _name, const glm::mat4& _matrix)
{
  // get the uniform location in the shader
  unsigned int _matrixLoc =
      glGetUniformLocation(m_shaderProgram, _name.c_str());
  // set the uniform value
  glUniformMatrix4fv(_matrixLoc, 1, GL_FALSE, glm::value_ptr(_matrix));
}