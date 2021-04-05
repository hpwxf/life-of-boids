#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

//#include "linmath.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4x4[4];

#ifdef LINMATH_NO_INLINE
#define LINMATH_H_FUNC static
#else
#define LINMATH_H_FUNC static inline
#endif

LINMATH_H_FUNC void mat4x4_identity(mat4x4 M) {
  int i, j;
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      M[i][j] = i == j ? 1.f : 0.f;
}

LINMATH_H_FUNC void vec4_dup(vec4 r, vec4 const src) {
  int i;
  for (i = 0; i < 4; ++i)
    r[i] = src[i];
}

LINMATH_H_FUNC void mat4x4_dup(mat4x4 M, mat4x4 const N) {
  int i;
  for (i = 0; i < 4; ++i)
    vec4_dup(M[i], N[i]);
}

LINMATH_H_FUNC void mat4x4_mul(mat4x4 M, mat4x4 const a, mat4x4 const b) {
  mat4x4 temp;
  int k, r, c;
  for (c = 0; c < 4; ++c)
    for (r = 0; r < 4; ++r) {
      temp[c][r] = 0.f;
      for (k = 0; k < 4; ++k)
        temp[c][r] += a[k][r] * b[c][k];
    }
  mat4x4_dup(M, temp);
}

LINMATH_H_FUNC void mat4x4_rotate_Z(mat4x4 Q, mat4x4 const M, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);
  mat4x4 R = {{c, s, 0.f, 0.f}, {-s, c, 0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 1.f}};
  mat4x4_mul(Q, M, R);
}

LINMATH_H_FUNC void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t, float n, float f) {
  M[0][0] = 2.f / (r - l);
  M[0][1] = M[0][2] = M[0][3] = 0.f;

  M[1][1] = 2.f / (t - b);
  M[1][0] = M[1][2] = M[1][3] = 0.f;

  M[2][2] = -2.f / (f - n);
  M[2][0] = M[2][1] = M[2][3] = 0.f;

  M[3][0] = -(r + l) / (r - l);
  M[3][1] = -(t + b) / (t - b);
  M[3][2] = -(f + n) / (f - n);
  M[3][3] = 1.f;
}

typedef struct Vertex {
  vec2 pos;
  vec3 col;
} Vertex;

static const Vertex vertices[3]
    = {{{-0.6f, -0.4f}, {1.f, 0.f, 0.f}}, {{0.6f, -0.4f}, {0.f, 1.f, 0.f}}, {{0.f, 0.6f}, {0.f, 0.f, 1.f}}};

static const char* vertex_shader_text
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

static const char* fragment_shader_text
    = "#version 330\n"
      "in vec3 color;\n"
      "out vec4 fragment;\n"
      "void main()\n"
      "{\n"
      "    fragment = vec4(color, 1.0);\n"
      "}\n";

static void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  // NOTE: OpenGL error checks have been omitted for brevity

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

  while (!glfwWindowShouldClose(window)) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m, p, mvp;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, (float)glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

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

//! [code]
