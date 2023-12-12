#include "Base.h"
#include "Camera.h"
#include "IO.h"
#include "Shader.h"
#include "ffImage.h"

#define AMOUNT 1000

glm::mat4* _mMatrixArr = NULL;

uint VAO_cube = 0;

ffImage* _pImage = NULL;

Shader _shader;
FF::ffModel* _model = NULL;
// 光照贴图
uint _textureBox = 0;
uint _VBO_ins;

uint _texId;

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
  // glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);

  _shader.start();
  _shader.setMatrix("_modelMatrix", _modelMatrix);
  _shader.setMatrix("_viewMatrix", _camera.getMatrix());
  _shader.setMatrix("_projMatrix", _projMatrix);
  _shader.setFloat("myMaterial.m_shiness", 32.0f);
  _shader.setVec3("view_pos", _camera.getPosition());

  _shader.setVec3("myLight.m_pos", glm::vec3(0.0f, 1.0f, 1.0f));
  _shader.setVec3("myLight.m_ambient", glm::vec3(0.2f, 0.2f, 0.2f));
  _shader.setVec3("myLight.m_diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
  _shader.setVec3("myLight.m_specular", glm::vec3(1.0f, 1.0f, 1.0f));

  _shader.setFloat("myLight.c", 1.0f);
  _shader.setFloat("myLight.l", 0.09f);
  _shader.setFloat("myLight.q", 0.032f);

  for (uint i = 0; i < _model->m_meshVec.size(); i++)
  {
    glBindVertexArray(_model->m_meshVec[i].m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            _model->m_meshVec[i].m_vertexVec.size(),
                            GL_UNSIGNED_INT, 0, AMOUNT);
    glBindVertexArray(0);
  }

  _shader.end();
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

void initShader()
{
  _shader.initShader("shader/vPointShader.glsl", "shader/fPointShader.glsl");

  // _shader.initShader("shader/vertexShader.glsl",
  // "shader/fragmentShader.glsl");
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

// inistialize the instance array matrix transformation
void initInstanceArray()
{
  // 准备模型变换矩阵
  _mMatrixArr = new glm::mat4[AMOUNT];
  srand(glfwGetTime());

  float radius = 3.0;
  float offset = 2.5f;

  glm::mat4 model = glm::mat4(1.0f);
  for (unsigned int i = 0; i < AMOUNT; i++)
  {
    glm::mat4 model = glm::mat4(1.0f);

    float angle = (float)i / (float)AMOUNT * 360.0f;

    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;

    model = glm::translate(model, glm::vec3(x, y, z));

    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    float rotAngle = (rand() % 360);
    model =
        glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.4f, 0.6f, 0.8f));

    _mMatrixArr[i] = model;
  }

  // generate a VBO buffer, store the matrix array
  glGenBuffers(1, &_VBO_ins);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO_ins);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * AMOUNT, &_mMatrixArr[0],
               GL_STATIC_DRAW);
  for (uint i = 0; i < _model->m_meshVec.size(); i++)
  {
    glBindVertexArray(_model->m_meshVec[i].m_VAO);

    // since vertex attribute array can only store vec4, we need to use several
    // rows vec4 to store a mat4
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void*)(sizeof(glm::vec4)));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void*)(sizeof(glm::vec4) * 2));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void*)(sizeof(glm::vec4) * 3));

    // 1 means update the attribute per instance
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }
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
  _camera.setSpeed(0.01f);

  VAO_cube = createModel();
  _texId = createTexture("textures/box.png");
  _model = new FF::ffModel("res/bag/backpack.obj");
  initInstanceArray();
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