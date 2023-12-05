#include <memory>

#include "Base.h"
#include "Shader.h"
#include "ffImage.h"
// VBO: vertex buffer object
unsigned int VBO = 0;
// VA0: vertex array object
unsigned int VAO = 0;
// texture Id
unsigned int _texture = 0;
// Image
std::unique_ptr<ffImage> _pImage = nullptr;

// shader program
Shader _shader;

glm::mat4 _viewMatrix(1.0f);
glm::mat4 _projectionMatrix(1.0f);

int _width = 800;
int _height = 600;

void render()
{
  // render
  // clear the buffer with a color
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // clear the last frame buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // depth test for current frame
  glEnable(GL_DEPTH_TEST);

  // translation set
  glm::vec3 modelVecs[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  // matrix for camera
  _viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),  // camera position
                            glm::vec3(0.0f, 0.0f, 0.0f),  // look at
                            glm::vec3(0.0f, 1.0f, 0.0f)   // up
  );

  _projectionMatrix = glm::perspective(
      glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 100.f);

  glBindTexture(GL_TEXTURE_2D, _texture);
  // alaways use shader program first, then it will be used to render,and the
  // unifrom will know where it should be
  for (int i = 0; i < 10; i++)
  {
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, modelVecs[i]);
    _modelMatrix = glm::rotate(
        _modelMatrix, glm::radians((float)glfwGetTime() * (i + 1) * 10),
        glm::vec3(0.0f, 1.0f, 0.0f));
    // start the current program
    _shader.start();

    // set the texture uniform
    _shader.setMatrix("_modelMatrix", _modelMatrix);
    _shader.setMatrix("_viewMatrix", _viewMatrix);
    _shader.setMatrix("_projectionMatrix", _projectionMatrix);
    // time
    // pass the uniform value to the shader

    // draw our first triangle
    // 1. bind vertex array object
    glBindVertexArray(VAO);

    // 3. draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // end the program
    _shader.end();
  }
}

void initModel()
{
  // clang-format off
 float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

  // clang-format on
  // create a vertex array object
  glGenVertexArrays(1, &VAO);
  // bind the vertex array object
  // VAO include the VBO and the vertex attribute
  glBindVertexArray(VAO);

  // EBo: element buffer object

  // create a vertex buffer object , 1 is how many VBOs we want to generate
  // VBO is a buffer in the GPU's memory
  glGenBuffers(1, &VBO);
  // bind the buffer to the GL_ARRAY_BUFFER target
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // copy the vertices data into the buffer's memory
  // GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // tell OpenGL how it should interpret the vertex data (per vertex attribute)
  // 0 is  the vertex attribute location, corresponding to the layout (location
  // =0 or 1 or ...
  // (void*)0 for last parameter: the offset of where the
  // position data begins, this case is the first element of the array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  // enable the vertex attribute for color for layout 1, stride is 6 * sizeof
  // float , and color start at 3 * sizeof float

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(sizeof(float) * 3));
  // enable the vertex attribute
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // unbind the buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void initTexture()
{
  _pImage = ffImage::readFromFile("textures/wall.jpg");
  // create a texture
  glGenTextures(1, &_texture);
  // bind the texture
  glBindTexture(GL_TEXTURE_2D, _texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(),
               _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               _pImage->getData());
}

void initShader(const char* _vertexPath, const char* _fragPath)
{
  _shader.initShader(_vertexPath, _fragPath);
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
  GLFWwindow* window =
      glfwCreateWindow(_width, _height, "OpenGL Core", NULL, NULL);
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
  initTexture();
  initShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    render();
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse,
    // double cached
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return EXIT_SUCCESS;
}
