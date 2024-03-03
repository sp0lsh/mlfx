
#include "gl_utils.h"
#include "geometry_src.h"
#include "utils.h"

namespace {

void printProgramLog(GLuint f_programId) {
  DBG_LOGI("printProgramLog\n");
  if (glIsProgram(f_programId)) {
    int logLen = 0;
    glGetProgramiv(f_programId, GL_INFO_LOG_LENGTH, &logLen);

    char* infoLog_a = new char[logLen];
    int infoLogLen = 0;
    glGetProgramInfoLog(f_programId, logLen, &infoLogLen, infoLog_a);

    // std::cout << infoLog_a << std::endl;
    DBG_LOGE("printProgramLog %s\n", infoLog_a);
    delete[] infoLog_a;
  }
}

void printShaderLog(GLuint f_shaderId) {
  DBG_LOGI("printShaderLog\n");
  if (glIsShader(f_shaderId)) {
    int logLen = 0;
    glGetShaderiv(f_shaderId, GL_INFO_LOG_LENGTH, &logLen);

    char* infoLog_a = new char[logLen];
    int infoLogLen = 0;
    glGetShaderInfoLog(f_shaderId, logLen, &infoLogLen, infoLog_a);

    // std::cout << infoLog_a << std::endl;
    DBG_LOGE("printProgramLog %s\n", infoLog_a);
    delete[] infoLog_a;
  }
}

GLuint loadShader(const GLchar* f_source_p, GLenum f_type) {
  DBG_LOGI("loadShader\n");
  GLuint shaderId = glCreateShader(f_type);
  glShaderSource(shaderId, 1, &f_source_p, nullptr);
  glCompileShader(shaderId);

  GLint compileStatus = GL_FALSE;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus) {
    printShaderLog(shaderId);
    glDeleteShader(shaderId);
    shaderId = 0;
  }

  return shaderId;
}

}  // namespace

void check_gl_error(const char* op) {
  for (GLint error = glGetError(); error; error = glGetError()) {
    DBG_LOGI("after %s() glError (0x%x)\n", op, error);
  }
}

GLuint loadProgram(const GLchar* f_vertSource_p, const GLchar* f_fragSource_p) {
  DBG_LOGI("loadProgram\n");
  GLuint vertShader = loadShader(f_vertSource_p, GL_VERTEX_SHADER);
  GLuint fragShader = loadShader(f_fragSource_p, GL_FRAGMENT_SHADER);

  if (!glIsShader(vertShader) || !glIsShader(fragShader)) {
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return 0;
  }

  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertShader);
  glAttachShader(programId, fragShader);

  glLinkProgram(programId);
  GLint linkStatus = GL_FALSE;
  glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

  if (!linkStatus) {
    printProgramLog(programId);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    glDeleteProgram(programId);
    return 0;
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  return programId;
}

GLuint loadTexture(const uint8_t* data, int width, int height) {
  GLuint texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT);  // set texture wrapping to GL_REPEAT (default
                               // wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  return texture1;
}

void createTextureR(GLuint& texID, int width, int height) {
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
               GL_UNSIGNED_BYTE, nullptr);
}

void updateTexture(GLuint& texID,
                   int slotID,
                   int width,
                   int height,
                   const uint8_t* data) {
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void destroyTexture(GLuint texID) {
  if (texID) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texID);
  }
}

bool saveFramebufferToTGA(char* filename, int w, int h) {
  // This prevents the images getting padded
  // when the width multiplied by 3 is not a multiple of 4
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  int nSize = w * h * 3;
  // First let's create our buffer, 3 channels per Pixel
  char* dataBuffer = (char*)malloc(nSize * sizeof(char));

  if (!dataBuffer)
    return false;

  // Let's fetch them from the backbuffer
  glReadBuffer(GL_FRONT);
  glReadPixels((GLint)0, (GLint)0, (GLint)w, (GLint)h, GL_RGB, GL_UNSIGNED_BYTE,
               dataBuffer);

  // GL_BGR does not compile
  for (int rgb = 0; rgb < nSize; rgb += 3)
    std::swap(dataBuffer[rgb], dataBuffer[rgb + 2]);

  // Now the file creation
  FILE* filePtr = fopen(filename, "wb");
  if (!filePtr)
    return false;

  unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned char header[6] = {
      static_cast<unsigned char>(w % 256), static_cast<unsigned char>(w / 256),
      static_cast<unsigned char>(h % 256), static_cast<unsigned char>(h / 256),
      static_cast<unsigned char>(24),      static_cast<unsigned char>(0)};

  // We write the headers
  fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
  fwrite(header, sizeof(unsigned char), 6, filePtr);

  // And finally our image data
  fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
  fclose(filePtr);

  free(dataBuffer);

  return true;
}

FrameBuffer createFrameBuffer(size_t width, size_t height) {
  FrameBuffer fb;  // pass ref

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  // create a color attachment texture
  unsigned int textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         textureColorbuffer, 0);
  // create a renderbuffer object for depth and stencil attachment (we won't be
  // sampling these)
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                        height);  // use a single renderbuffer object for both a
                                  // depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);  // now actually attach it

  // now that we actually created the framebuffer and added all attachments we
  // want to check if it is actually complete now
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
    fb.width = width;
    fb.height = height;
    fb.id = framebuffer;
    fb.colorTarget = textureColorbuffer;
    fb.rboTarget = rbo;

  } else {
    DBG_LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fb;
}

void deleteFrameBuffer(FrameBuffer& fb) {
  if (fb.id) {
    glDeleteFramebuffers(1, &fb.id);
  }

  if (fb.colorTarget) {
    glDeleteTextures(1, &fb.colorTarget);
  }

  if (fb.rboTarget) {
    glDeleteTextures(1, &fb.rboTarget);
  }

  std::memset(&fb, 0, sizeof(FrameBuffer));
}

void createVAO(Mesh& mesh,
               unsigned int* VAO,
               unsigned int* EBO,
               unsigned int* VBO,
               unsigned int* VBO2) {
  // per each geometry
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, VBO2);
  glGenBuffers(1, EBO);
  // bind the Vertex Array Object first, then bind and set vertex
  // buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colour), colour, GL_STATIC_DRAW);

  glVertexAttribPointer(kPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void*)0);
  glEnableVertexAttribArray(kPositionLoc);

  glVertexAttribPointer(kColorLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void*)0);
  glEnableVertexAttribArray(kColorLoc);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // note that this is allowed, the call to glVertexAttribPointer
  // registered VBO as the vertex attribute's bound vertex buffer object
  // so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound
  // element buffer object IS stored in the VAO; keep the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't
  // accidentally modify this VAO, but this rarely happens. Modifying
  // other VAOs requires a call to glBindVertexArray anyways so we
  // generally don't unbind VAOs (nor VBOs) when it's not directly
  // necessary.
  glBindVertexArray(0);
}