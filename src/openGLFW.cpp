#include <memory>

#include "Base.h"
#include "Camera.h"
#include "Shader.h"
#include "ffImage.h"

// VA0: vertex array object
uint VAO_cube = 0;
uint VAO_sun = 0;
// texture Id
uint _texture = 0;
// Image
std::unique_ptr<ffImage> _pImage = nullptr;

// shader program
Shader _shaderCube;
Shader _shaderSun;

Camera _camera;

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

  // matrix for camera
  _camera.update();

  _projectionMatrix = glm::perspective(
      glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 100.f);

  glBindTexture(GL_TEXTURE_2D, _texture);
  // alaways use shader program first, then it will be used to render,and the
  // unifrom will know where it should be

  glm::mat4 _modelMatrix(1.0f);
  _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

  // Render the cube
  _shaderCube.start();
  _shaderCube.setMatrix("_modelMatrix", _modelMatrix);
  _shaderCube.setMatrix("_viewMatrix", _camera.getViewMatrix());
  _shaderCube.setMatrix("_projectionMatrix", _projectionMatrix);
  glBindVertexArray(VAO_cube);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  _shaderCube.end();

  // Render the sun
  _shaderSun.start();
  _modelMatrix = glm::translate(_modelMatrix, glm::vec3(3.0f, 0.0f, -3.0f));

  _shaderSun.setMatrix("_modelMatrix", _modelMatrix);
  _shaderSun.setMatrix("_viewMatrix", _camera.getViewMatrix());
  _shaderSun.setMatrix("_projectionMatrix", _projectionMatrix);
  glBindVertexArray(VAO_sun);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  _shaderSun.end();
}

uint createModel()
{
  uint _vao = 0;
  uint _vbo = 0;
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
  glGenVertexArrays(1, &_vao);
  // bind the vertex array object
  // VAO include the VBO and the vertex attribute
  glBindVertexArray(_vao);

  // EBo: element buffer object

  // create a vertex buffer object , 1 is how many VBOs we want to generate
  // VBO is a buffer in the GPU's memory
  glGenBuffers(1, &_vbo);
  // bind the buffer to the GL_ARRAY_BUFFER target
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
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

  return _vao;
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

void initShader(Shader* _shader, const char* _vertexPath, const char* _fragPath)
{
  _shader->initShader(_vertexPath, _fragPath);
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

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    _camera.move(CAMERA_MOVE::MOVE_FORWARD);

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    _camera.move(CAMERA_MOVE::MOVE_BACKWARD);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    _camera.move(CAMERA_MOVE::MOVE_LEFT);

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    _camera.move(CAMERA_MOVE::MOVE_RIGHT);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  _camera.onMouseMove(xpos, ypos);
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

  // hide the cursor and capture it
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  // init camera
  _camera.lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -1.0f),
                 glm::vec3(0, 1, 0));
  _camera.setSpeed(0.08f);
  _camera.setSensitive(0.1f);
  VAO_cube = createModel();
  VAO_sun = createModel();
  initTexture();
  initShader(&_shaderCube, "shaders/vertexShader.glsl",
             "shaders/fragmentShader.glsl");
  initShader(&_shaderSun, "shaders/sunVertex.glsl", "shaders/sunFragment.glsl");
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
