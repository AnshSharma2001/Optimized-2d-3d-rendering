Simple C++ OpenGL wrapper optimized for 2d/3d NPR rendering

**Currently supporting:**

* Frame Buffer Object
* Shader Storage Buffer
* Vertex Buffer Object
* Vertex Array Object
* Texture (only GL_TEXTURE2D)
* Glsl Compute, Vertex, Tesselation Control, Tesselation Evaluation, Geometry, Fragment
* Window and context creation based on [GLFW](https://github.com/glfw/glfw)


## This is NOT:

* Well tested
* A rendering engine with various rendering backends (Consider using [magnum](https://github.com/mosra/magnum))
* A complete wrapper of OpenGL

## Example usage, Hello World, first triangle:

```C++
#include "prgl/FrameBufferObject.hxx"
#include "prgl/GlslProgram.hxx"
#include "prgl/GlslRenderingPipelineProgram.hxx"
#include "prgl/Texture2d.hxx"
#include "prgl/VertexArrayObject.hxx"
#include "prgl/VertexBufferObject.hxx"
#include "prgl/Window.hxx"

int32_t main(int32_t argc, char** args)
{
  // depth
  const std::array<int32_t, 4> rgbaBits = {16, 16, 16, 16};

  // create window and context
  auto gl = std::make_unique<prgl::Window>(1024, 768, "window", rgbaBits, 16, 8,
                                           4, true);

  // create vertex positions
  std::vector<prgl::vec3> positions = {
    {{-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {0.0F, 1.0F, 0.0F}}};
  auto vboPosition = prgl::VertexBufferObject::Create(
    prgl::VertexBufferObject::Usage::StaticDraw);
  vboPosition->createBuffer(positions);
  // create vertex colors
  std::vector<prgl::vec3> colors = {
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
  auto vboColors = prgl::VertexBufferObject::Create(
    prgl::VertexBufferObject::Usage::StaticDraw);
  vboColors->createBuffer(colors);

  // create Vertex Array Object
  auto vao = prgl::VertexArrayObject::Create();
  // add positions first (attrib 0)
  vao->addVertexBufferObject(0U, vboPosition);
  // add colors (attrib 1)
  vao->addVertexBufferObject(1U, vboColors);

  // create Frame Buffer Object and Texture target
  auto fbo = prgl::FrameBufferObject::Create();
  auto tex = prgl::Texture2d::Create(
    gl->getWidth(), gl->getHeight(), prgl::TextureFormatInternal::Rgb32F,
    prgl::TextureFormat::Rgb, prgl::DataType::Float);
  {

    std::vector<prgl::vec3> colorBuffer(gl->getWidth() * gl->getHeight());
    // fill the texture with a color
    prgl::vec3 color = {0.25f, 0.1f, 0.3f};
    for (auto& c : colorBuffer)
      {
        c = color;
      }
    tex->upload(colorBuffer.data());
    fbo->attachTexture(tex);
  }

  // create shader
  auto glsl = prgl::GlslRenderingPipelineProgram::Create();
  glsl->attachVertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec3 vertexColor;

    out vec3 vColor;

    void main()
    {
      vColor = vertexColor;

      gl_Position = vec4(vertexPosition, 1.0);
    }
  )");
  glsl->attachFragmentShader(R"(
    #version 330 core

    in vec3 vColor;
    out vec3 color;

    void main()
    {
      color = vColor;
    }
  )");

  gl->setRenderFunction([&tex, &gl, &vao, &glsl, &fbo]() {
    {
      // bind the fbo
      const auto fboBinder = prgl::Binder(fbo);
      {
        // bind the shader program
        const auto shaderBinder = prgl::Binder(glsl);
        {
          // bind the VertexArrayObject
          const auto vaoBinder = prgl::Binder(vao);

          // render the object
          vao->render(prgl::DrawMode::Triangles, 0U, 3U);
        }
      }
    }

    // render the texture into the main window
    tex->render(0.0F, 0.0F, gl->getWidth(), gl->getHeight());
  });

  gl->renderLoop(true);

  return EXIT_SUCCESS;
}

```
This is what you should see

![](doc/prgl_FirstTriangle.png)
