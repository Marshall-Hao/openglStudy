// clang-format off
#include <glad/glad.h>// GLAD should be included first
#include <GLFW/glfw3.h>
// clang-format on
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

unsigned int VBO = 0;
unsigned int shaderProgram = 0;

void render() {}

void initModel()
{
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,  // left
      0.5f,  -0.5f, 0.0f,  // right
      0.0f,  0.5f,  0.0f   // top
  };

  // create a vertex buffer object , 1 is how many VBOs we want to generate
  // VBO is a buffer in the GPU's memory
  glGenBuffers(1, &VBO);
  // bind the buffer to the GL_ARRAY_BUFFER target
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // copy the vertices data into the buffer's memory
  // GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // tell OpenGL how it should interpret the vertex data (per vertex attribute)
  // 0 is  the vertex attribute location ,layout (location = 0) in the vertex
  // (void*)0 for last parameter: the offset of where the position data begins,
  // this case is the first element of the array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // enable the vertex attribute
  glEnableVertexAttribArray(0);
  // unbind the buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initShader(const char* _vertexPath, const char* _fragPath)
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
}

// settings
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
int main()
{
  // glfw: initialize and configure
  glfwInit();
  // Set GLFW window hints depending on the operating system
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
  // macOS requires a forward-compatible core profile context
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
  // On other operating systems, use compatibility profile (or no profile)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

  // glfw window creation
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // render loop
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  initModel();
  initShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);
    // render
    // clear the buffer with a color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse,
    // double cached
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return EXIT_SUCCESS;
}
