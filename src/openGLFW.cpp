#include <memory>

#include "Base.h"
#include "Camera.h"
#include "Shader.h"
#include "ffImage.h"

// VA0: vertex array object
uint VAO_cube = 0;
uint VAO_sun = 0;

// light
glm::vec3 light_pos(1.0f);
glm::vec3 light_color(1.0f);

// texture
uint _textureBox = 0;
uint _textureSpec = 0;

// Image
std::unique_ptr<ffImage> _pImage = nullptr;

// shader program
Shader _shaderCube;
Shader _shaderSun;
// parallel light
Shader _shaderDir;
// point light
Shader _shaderPoint;
// spot light
Shader _shaderSpot;

Camera _camera;

glm::mat4 _projectionMatrix(1.0f);

int _width = 800;
int _height = 600;

void render()
{
  // render
  // clear the buffer with a color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // clear the last frame buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // depth test for current frame
  glEnable(GL_DEPTH_TEST);

  // translation set
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // light set
  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

  // matrix for camera
  _camera.update();

  _projectionMatrix = glm::perspective(
      glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 100.f);

  // bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureBox);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _textureSpec);

  // alaways use shader program first, then it will be used to render,and the
  // unifrom will know where it should be

  glm::mat4 _modelMatrix(1.0f);
  _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

  // Render the cube
  _shaderSpot.start();
  _shaderSpot.setVec3("_viewPos", _camera.getPosition());
  // light properties

  _shaderSpot.setVec3("myLight.m_ambient", light_color * glm::vec3(0.1f));
  _shaderSpot.setVec3("myLight.m_diffuse", light_color * glm::vec3(0.7f));
  _shaderSpot.setVec3("myLight.m_specular", light_color * glm::vec3(0.5f));
  // spot light sames as camera position and direction
  _shaderSpot.setVec3("myLight.m_position", _camera.getPosition());
  _shaderSpot.setVec3("myLight.m_direction", _camera.getFront());

  _shaderSpot.setFloat("myLight.m_cutOff", glm::cos(glm::radians(12.5f)));
  _shaderSpot.setFloat("myLight.m_outerCutOff", glm::cos(glm::radians(20.5f)));

  _shaderSpot.setFloat("myLight.m_constant", 1.0f);
  _shaderSpot.setFloat("myLight.m_linear", 0.09f);
  _shaderSpot.setFloat("myLight.m_quadratic", 0.032f);
  // material properties
  // set the sampler2D to the correct texture unit 1 GL_TEXTURE1
  _shaderSpot.setInt("myMaterial.m_specular", 1);
  _shaderSpot.setFloat("myMaterial.m_shininess", 32.0f);
  // transform
  _shaderSpot.setMatrix("_viewMatrix", _camera.getViewMatrix());
  _shaderSpot.setMatrix("_projectionMatrix", _projectionMatrix);

  for (int i = 0; i < 10; i++)
  {
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, cubePositions[i]);
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(20.0f * i),
                               glm::vec3(0.0f, 1.0f, 0.0f));
    _shaderSpot.setMatrix("_modelMatrix", _modelMatrix);

    glBindVertexArray(VAO_cube);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  _shaderSpot.end();

  // Render the sun
  _shaderSun.start();
  _shaderSun.setMatrix("_viewMatrix", _camera.getViewMatrix());
  _shaderSun.setMatrix("_projectionMatrix", _projectionMatrix);
  for (int i = 0; i < 4; i++)
  {
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, pointLightPositions[i]);
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(0.2f));
    _shaderSun.setMatrix("_modelMatrix", _modelMatrix);
    glBindVertexArray(VAO_sun);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  _shaderSun.end();
}

uint createModel()
{
  uint _vao = 0;
  uint _vbo = 0;
  // clang-format off
 float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,           0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,           0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // enable the vertex attribute for color for layout 1, stride is 6 * sizeof
  // float , and color start at 3 * sizeof float

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(sizeof(float) * 3));

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(sizeof(float) * 5));
  // enable the vertex attribute
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // unbind the buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return _vao;
}

uint createTexture(const char* _fileName)
{
  _pImage = ffImage::readFromFile(_fileName);
  uint _texture = 0;
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

  return _texture;
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
  glViewport(200, 150, 1600, 1200);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // hide the cursor and capture it
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  // init camera
  _camera.lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -1.0f),
                 glm::vec3(0, 1, 0));
  _camera.setSpeed(0.08f);
  _camera.setSensitive(0.1f);
  VAO_cube = createModel();
  VAO_sun = createModel();
  light_pos = glm::vec3(3.0f, 1.0f, -1.0f);
  light_color = glm::vec3(1.0f, 1.0f, 1.0f);

  _textureBox = createTexture("textures/box.png");
  _textureSpec = createTexture("textures/specular.png");
  initShader(&_shaderCube, "shaders/vertexShader.glsl",
             "shaders/fragmentShader.glsl");
  initShader(&_shaderSun, "shaders/sunVertex.glsl", "shaders/sunFragment.glsl");
  initShader(&_shaderDir, "shaders/dirVertex.glsl", "shaders/dirFragment.glsl");
  initShader(&_shaderPoint, "shaders/pointVertex.glsl",
             "shaders/pointFragment.glsl");
  initShader(&_shaderSpot, "shaders/spotVertex.glsl",
             "shaders/spotFragment.glsl");
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
