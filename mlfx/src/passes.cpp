
#include "passes.h"
#include "component.h"
#include "geometry_src.h"
#include "gl_utils.h"

void PassClear::init() {}

void PassClear::run() {
  glClearColor(0.0F, 0.1F, 0.0F, 1.F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PassFullscreen::init(FrameBuffer in_fb,
                          GLuint in_program,
                          Uniforms in_uniforms) {
  fb = in_fb;  // TODO: copy of fb (4 ints)
  program = in_program;
  uniforms = in_uniforms;

  for (auto& res : uniforms) {
    res.location = glGetUniformLocation(program, res.name.c_str());
  }
}

void PassFullscreen::run() {
  fb.bind();

  glDisable(GL_DEPTH_TEST);

  glUseProgram(program);

  size_t active_texture_slots = 0;
  for (auto& res : uniforms) {
    if (res.type == ResType::kTexture) {
      glActiveTexture(GL_TEXTURE0 + active_texture_slots);
      glUniform1i(res.location, active_texture_slots);
      glBindTexture(GL_TEXTURE_2D, res.textureID);
      ++active_texture_slots;

    } else if (res.type == ResType::kMatrix4f) {
      glUniformMatrix4fv(res.location, 1, GL_FALSE, res.floatPtr);

    } else if (res.type == ResType::kFloat) {
      glUniform1f(res.location, res.floatValue);
    }
  }

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glEnable(GL_DEPTH_TEST);
}

void PassGeometry::init(FrameBuffer in_fb, GLuint in_program) {
  DBG_LOGI("PassGeometry::init()\n");
  if (is_init)
    return;

  fb = in_fb;
  program = in_program;

  // loop over uniforms
  mMVPMatrixLoc = glGetUniformLocation(program, "mvp");
  mMMatrixLoc = glGetUniformLocation(program, "mv");

  is_init = true;
}

void PassGeometry::run(const entt::registry& reg) {
  if (!is_init)
    return;

  // Find active camera
  CameraComponent cmpCamActive;
  TransformComponent cmpCamXformActive;
  auto viewCam = reg.view<CameraComponent, TransformComponent>();
  for (auto [entity, cmpCam, cmpXform] : viewCam.each()) {
    if (cmpCam.is_active) {
      cmpCamActive = cmpCam;
      cmpCamXformActive = cmpXform;
      break;
    }
  }

  float ratio = (float)fb.width / (float)fb.height;  
  Matrix4 ndcFromCam = Matrix4::perspective(
      cmpCamActive.fovY, ratio, cmpCamActive.near, cmpCamActive.far);

  Matrix4 camFromWorld = Matrix4::translate(cmpCamXformActive.position) *
                         Matrix4::scale(cmpCamXformActive.scale) *
                         Matrix4::rollPitchYaw(cmpCamXformActive.euler.x(),
                                               cmpCamXformActive.euler.y(),
                                               cmpCamXformActive.euler.z());

  glDisable(GL_CULL_FACE);

  fb.bind();

  // bind program
  glUseProgram(program);
  // bind program with geometry
  glBindAttribLocation(program, kPositionLoc, kPositionName);
  glBindAttribLocation(program, kColorLoc, kColorName);

  // Loop over renderables meshes
  auto viewMesh =
      reg.view<GLMeshComponent, MeshComponent, TransformComponent>();
  for (auto [entity, cmpMeshGL, cmpMesh, cmpXform] : viewMesh.each()) {
    // transform
    Matrix4 worldFromModel =
        Matrix4::translate(cmpXform.position) * Matrix4::scale(cmpXform.scale) *
        Matrix4::rollPitchYaw(cmpXform.euler.x(), cmpXform.euler.y(),
                              cmpXform.euler.z());

    // per each geometry
    Matrix4 ndcFromModel = ndcFromCam * camFromWorld * worldFromModel;

    // set program uniforms
    glUniformMatrix4fv(mMMatrixLoc, 1, GL_FALSE, worldFromModel.data);
    glUniformMatrix4fv(mMVPMatrixLoc, 1, GL_FALSE, ndcFromModel.data);

    // bind geometry and draw
    glBindVertexArray(cmpMeshGL.VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDrawElements(GL_TRIANGLES, cmpMesh.mesh.indices.size(), GL_UNSIGNED_SHORT,
                   0);
    glBindVertexArray(0);
  }

  glCullFace(GL_BACK);
}
