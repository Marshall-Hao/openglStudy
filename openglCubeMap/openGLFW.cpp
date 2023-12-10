#include "Base.h"
#include "Camera.h"
#include "Shader.h"
#include "ffImage.h"

uint VAO_cube = 0;
uint VAO_sky = 0;

ffImage* _pImage = NULL;

Shader _shader;
Shader _shaderSky;

// 光照贴图
uint _textureBox = 0;
uint _textureSky = 0;

Camera _camera;

glm::mat4 _projMatrix(1.0f);
int _width = 800;
int _height = 600;

void rend()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  _camera.update();
  _projMatrix = glm::perspective(glm::radians(45.0f),
                                 (float)_width / (float)_height, 0.1f, 100.0f);
  glm::mat4 _modelMatrix(1.0f);
  _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

  // 渲染box
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureBox);

  _shader.start();
  _shader.setMatrix("_modelMatrix", _modelMatrix);
  _shader.setMatrix("_viewMatrix", _camera.getMatrix());
  _shader.setMatrix("_projMatrix", _projMatrix);

  // 绘制方盒
  glBindVertexArray(VAO_cube);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  _shader.end();

  // 渲染天空盒
  glDepthFunc(GL_LEQUAL);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _textureSky);
  _shaderSky.start();
  glm::mat4 _viewMatrix(glm::mat3(_camera.getMatrix()));
  _shaderSky.setMatrix("_viewMatrix", _viewMatrix);
  _shaderSky.setMatrix("_projMatrix", _projMatrix);
  glBindVertexArray(VAO_sky);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  _shaderSky.end();
  glDepthFunc(GL_LESS);
}
uint createPlane()
{
  uint _VAO = 0;
  uint _VBO = 0;
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  float planeVertices[] = {
      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
      0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
      0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));

  return _VAO;
}

uint createWindow()
{
  uint _VAO = 0;
  uint _VBO = 0;
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  float transparentVertices[] = {
      0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -0.5f, 0.0f,
      0.0f, 1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

      0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 1.0f, -0.5f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.5f,  0.0f, 1.0f, 0.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices),
               transparentVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));

  return _VAO;
}

uint createModel()
{
  uint _VAO = 0;
  uint _VBO = 0;

  float cubeVertices[] = {
      // Back face
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom-left
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // top-right
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // bottom-left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // top-left
      // Front face
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // top-right
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // top-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
      // Left face
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-right
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-right
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-right
                                        // Right face
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // top-left
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // top-right
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // top-left
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,    // bottom-left
      // Bottom face
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   // top-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-right
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // top-right
      // Top face
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // bottom-right
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f    // bottom-left
  };

  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(sizeof(float) * 3));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return _VAO;
}

uint createTexture(const char* _fileName)
{
  _pImage = ffImage::readFromFile(_fileName);
  uint _texture = 0;
  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(),
               _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               _pImage->getData());

  return _texture;
}

uint createSkyBoxTex()
{
  uint _tid = 0;
  glGenTextures(1, &_tid);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _tid);

  std::vector<std::string> _facePath = {
      "res/skybox/right.jpg",  "res/skybox/left.jpg",  "res/skybox/top.jpg",
      "res/skybox/bottom.jpg", "res/skybox/front.jpg", "res/skybox/back.jpg"};

  for (int i = 0; i < 6; i++)
  {
    ffImage* _pImage = ffImage::readFromFile(_facePath[i].c_str());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                 _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, _pImage->getData());
    if (_pImage)
    {
      std::cout << "load image " << _facePath[i] << " success" << std::endl;
    }
    delete _pImage;
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return _tid;
}

uint creatSkyBoxVAO()
{
  uint _VAO = 0;
  uint _VBO = 0;

  // clang-format off
  float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
  // clang-format on

  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return _VAO;
}

void initShader()
{
  _shader.initShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
  _shaderSky.initShader("shader/skyShaderv.glsl", "shader/skyShaderf.glsl");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    _camera.move(CAMERA_MOVE::MOVE_FRONT);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    _camera.move(CAMERA_MOVE::MOVE_BACK);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    _camera.move(CAMERA_MOVE::MOVE_LEFT);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    _camera.move(CAMERA_MOVE::MOVE_RIGHT);
  }
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, _width, _height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  _camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));
  _camera.setSpeed(0.001f);

  VAO_cube = createModel();
  VAO_sky = creatSkyBoxVAO();
  _textureBox = createTexture("res/box.png");
  _textureSky = createSkyBoxTex();
  initShader();

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    rend();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}