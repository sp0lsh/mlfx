#pragma once

#include "platform.h"

#include <assert.h>
#include <string>

struct Mesh;

constexpr size_t kPositionLoc = 0;
constexpr char kPositionName[] = "aPos";
constexpr size_t kColorLoc = 1;
constexpr char kColorName[] = "aNormal";

struct FrameBuffer {  
  void bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, width, height);
  }

  GLuint id = 0;
  GLuint colorTarget = 0;
  GLuint rboTarget = 0;
  size_t width = 0;
  size_t height = 0;
};


bool saveFramebufferToTGA(char* filename, int w, int h);

void check_gl_error(const char* op);

GLuint loadTexture(const uint8_t* data, int width, int height);

GLuint loadProgram(const GLchar* f_vertSource_p, const GLchar* f_fragSource_p);

void createTextureR(GLuint& texID, int width, int height);

void destroyTexture(GLuint texID);

void updateTexture(GLuint& texID, int slotID, int width, int height, const uint8_t* data);

FrameBuffer createFrameBuffer(size_t width, size_t height);

void deleteFrameBuffer(FrameBuffer& fb);

void createVAO(Mesh& mesh,
               unsigned int* VAO,
               unsigned int* EBO,
               unsigned int* VBO,
               unsigned int* VBO2);


struct PingPongFrameBuffer {
 public:
  void Init(int width, int height) {
    mWidth = width;
    mHeight = height;

    for (unsigned int i = 0; i < 2; i++) {
      mPingpongFBO[i] = createFrameBuffer(width, height);
    }
  }

  void Deinit() {
    for (unsigned int i = 0; i < 2; i++) {
      deleteFrameBuffer(mPingpongFBO[i]);
    }
  }

  FrameBuffer& GetWriteFBO() { return mPingpongFBO[mUses % 2]; };
  FrameBuffer& GetReadFBO() { return mPingpongFBO[(mUses + 1) % 2]; };

  void Flip() { mUses++; };

 private:
  FrameBuffer mPingpongFBO[2];

  int mUses = 0;
  int mWidth = 0;
  int mHeight = 0;
};

class ShaderProgram {
 public:
  ShaderProgram() : mName(""), mProgramPtr(nullptr) {}

  void Init(GLuint* programPtr, const char* name) {
    mProgramPtr = programPtr;
    mName = name;
    printf("ShaderProgram: %s loaded: %d\n", mName.c_str(),
           programPtr != nullptr);
  }

  ~ShaderProgram() { glDeleteProgram(*mProgramPtr); }

  inline bool isValid() const {
    return mProgramPtr != nullptr && *mProgramPtr != 0;
  }

  inline unsigned int getID() const {
    assert(mProgramPtr != nullptr);
    return *mProgramPtr;
  }

  void bind() const { glUseProgram(*mProgramPtr); }

  void unbind() const { glUseProgram(0); }

  void setUniform(const std::string& name, float value) const {
    assert(mProgramPtr != nullptr);
    printf("Uniform location %s:%d\n", name.c_str(),
           glGetUniformLocation(*mProgramPtr, name.c_str()));
    glUniform1f(glGetUniformLocation(*mProgramPtr, name.c_str()), value);
  }

  void setUniform(const std::string& name, float value0, float value1) const {
    assert(mProgramPtr != nullptr);
    glUniform2f(glGetUniformLocation(*mProgramPtr, name.c_str()), value0,
                value1);
  }

  void setUniform(const std::string& name, int value0, int value1) const {
    assert(mProgramPtr != nullptr);
    glUniform2i(glGetUniformLocation(*mProgramPtr, name.c_str()), value0,
                value1);
  }

  void setUniform(const std::string& name, int value) const {
    assert(mProgramPtr != nullptr);
    // printf("Uniform location %s:%d\n",
    //     name.c_str(),
    //     glGetUniformLocation(*mProgramPtr, name.c_str()));
    glUniform1i(glGetUniformLocation(*mProgramPtr, name.c_str()), value);
  }

  void setUniform(const std::string& name, int slot, GLuint textureID) const {
    assert(mProgramPtr != nullptr);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // printf("Uniform location %s:%d\n",
    //     name.c_str(),
    //     glGetUniformLocation(*mProgramPtr, name.c_str()));
    glUniform1i(glGetUniformLocation(*mProgramPtr, name.c_str()), slot);
  }

 private:
  std::string mName;
  GLuint* mProgramPtr;
};


/*
class DummyVAO {
 public:
  DummyVAO() : mVAO(-1) {}

  ~DummyVAO() { glDeleteVertexArrays(1, &mVAO); }

  void init() { glGenVertexArrays(1, &mVAO); }

  inline void draw() const {
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
  }

 private:
  GLuint mVAO;
};
*/
