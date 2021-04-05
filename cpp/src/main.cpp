#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include <fmt/core.h>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat4x4 = std::array<vec4, 4>;

inline mat4x4 mat4x4_identity() {
  mat4x4 M;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      M[i][j] = i == j ? 1. : 0.;
  return M;
}

inline mat4x4 mat4x4_mul(mat4x4 const& a, mat4x4 const& b) {
  mat4x4 temp;
  for (int c = 0; c < 4; ++c)
    for (int r = 0; r < 4; ++r) {
      temp[c][r] = 0;
      for (int k = 0; k < 4; ++k)
        temp[c][r] += a[k][r] * b[c][k];
    }
  return temp;
}

inline mat4x4 mat4x4_rotate_Z(mat4x4 const& M, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);
  mat4x4 R = {{
      {{c, s, 0, 0}},   //
      {{-s, c, 0, 0}},  //
      {{0, 0, 1, 0}},   //
      {{0, 0, 0, 1}}    //
  }};
  return mat4x4_mul(M, R);
}

inline mat4x4 mat4x4_ortho(float l, float r, float b, float t, float n, float f) {
  mat4x4 M{};
  M[0][0] = 2 / (r - l);
  M[1][1] = 2 / (t - b);
  M[2][2] = -2 / (f - n);
  M[3][0] = -(r + l) / (r - l);
  M[3][1] = -(t + b) / (t - b);
  M[3][2] = -(f + n) / (f - n);
  M[3][3] = 1;
  return M;
}

struct Vertex {
  vec2 pos;
  vec3 col;
};

static const std::array<Vertex, 3> vertices = {{
    {{-0.6, -0.4}, {1., 0., 0.}},  //
    {{0.6, -0.4}, {0., 1., 0.}},   //
    {{0., 0.6}, {0., 0., 1.}}      //
}};

static const char* const vertex_shader_text
    = "#version 330\n"
      "uniform mat4 MVP;\n"
      "in vec3 vCol;\n"
      "in vec2 vPos;\n"
      "out vec3 color;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
      "    color = vCol;\n"
      "}\n";

static const char* const fragment_shader_text
    = "#version 330\n"
      "in vec3 color;\n"
      "out vec4 fragment;\n"
      "void main()\n"
      "{\n"
      "    fragment = vec4(color, 1.0);\n"
      "}\n";

static void error_callback(int error, const char* description) {
  fmt::print(stderr, "Error[{}]: {}\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  // NOTE: OpenGL error checks have been omitted for brevity

  GLuint vertex_buffer{};
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
  glCompileShader(vertex_shader);

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
  glCompileShader(fragment_shader);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  GLuint vertex_array{};
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

  while (!glfwWindowShouldClose(window)) {
    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m = mat4x4_identity();
    m = mat4x4_rotate_Z(m, (float)glfwGetTime());
    mat4x4 p = mat4x4_ortho(-ratio, ratio, -1., 1., 1., -1.);
    mat4x4 mvp = mat4x4_mul(p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}