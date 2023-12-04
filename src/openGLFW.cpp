#include "Base.h"
#include "Shader.h"
// VBO: vertex buffer object
unsigned int VBO = 0;
// VA0: vertex array object
unsigned int VAO = 0;
// shader program
Shader _shader;

void render()
{
  // alaways use shader program first, then it will be used to render,and the
  // unifrom will know where it should be
  _shader.start();
  // time
  // pass the uniform value to the shader

  // draw our first triangle
  // 1. bind vertex array object
  glBindVertexArray(VAO);

  // 3. draw the triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
  // 4. unbind vertex array object
  _shader.end();
}

void initModel()
{
  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 1.0f , 0.0f, 0.0f,// left
      0.5f,  -0.5f, 0.0f, 0.0f , 1.0f, 0.0f,// right
      0.0f,  0.5f,  0.0f, 0.0f , 0.0f, 1.0f // top
  };
  // clang-format on

  // create a vertex array object
  glGenVertexArrays(1, &VAO);
  // bind the vertex array object
  // VAO include the VBO and the vertex attribute
  glBindVertexArray(VAO);

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  // enable the vertex attribute for color for layout 1, stride is 6 * sizeof
  // float , and color start at 3 * sizeof float
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(sizeof(float) * 3));

  // enable the vertex attribute
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // unbind the buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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
