#ifndef PRGL_OPENGL_HEADER
#define PRGL_OPENGL_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

#include <array>
#include <memory>

#include "prgl/Types.hxx"

namespace prgl {
/**
 * @brief Shader, texture handles that are not yet initl
 */
constexpr uint32_t INVALID_HANDLE = 0U;

/**
 * @brief Check for OpenGL errors: checkGLError(__FILE__, __FUNCTION__,
 * __LINE__);
 *
 * @param file the file where the function gets called.
 * @param function the function where the function gets called.
 * @param line the line where the function gets called.
 */
void checkGLError(const char* file, const char* function, int line);

/**
 * @brief "Specifies the data type of the pixel data. The following symbolic
 * values are accepted:""
 *
 */
enum class DataType : uint32_t {
  UnsignedByte  = GL_UNSIGNED_BYTE,
  Byte          = GL_BYTE,
  UnsignedShort = GL_UNSIGNED_SHORT,
  Short         = GL_SHORT,
  UnsignedInt   = GL_UNSIGNED_INT,
  Int           = GL_INT,
  HalfFloat     = GL_HALF_FLOAT,
  Float         = GL_FLOAT,
  Double        = GL_DOUBLE
};

template <typename T>
class DataTypeTr {};

template <>
class DataTypeTr<float> {
 public:
  static constexpr DataType dataType = DataType::Float;
};

template <>
class DataTypeTr<double> {
 public:
  static constexpr DataType dataType = DataType::Double;
};

template <>
class DataTypeTr<int32_t> {
 public:
  static constexpr DataType dataType = DataType::Int;
};

template <>
class DataTypeTr<uint32_t> {
 public:
  static constexpr DataType dataType = DataType::UnsignedInt;
};

template <>
class DataTypeTr<int16_t> {
 public:
  static constexpr DataType dataType = DataType::Short;
};

template <>
class DataTypeTr<uint16_t> {
 public:
  static constexpr DataType dataType = DataType::UnsignedShort;
};

template <>
class DataTypeTr<int8_t> {
 public:
  static constexpr DataType dataType = DataType::Byte;
};

template <>
class DataTypeTr<uint8_t> {
 public:
  static constexpr DataType dataType = DataType::UnsignedByte;
};

/**
 * @brief Can be used to bind unbind an OpenGL resource. Similar to how a mutex
 * locker would work.
 *
 * @tparam T the class wrapped: needs to implement bind(bool). C++17/20 Concepts
 * would be better here?
 */
template <class T>
class Binder final {
  const std::shared_ptr<T>& mBindable;

 public:
  // Bind on construction
  Binder(const std::shared_ptr<T>& bindable);
  // Unbind on destruction
  ~Binder();

 private:
  // no need for default constructor, assignment operator and copy constructor
  Binder()              = delete;
  Binder(const Binder&) = delete;
  Binder& operator=(const Binder&) = delete;
};

template <class T>
Binder<T>::Binder(const std::shared_ptr<T>& bindable) : mBindable(bindable) {
  mBindable->bind(true);
}

template <class T>
Binder<T>::~Binder() {
  mBindable->bind(false);
}

}  //  namespace prgl

#endif  // !PRGL_OPENGL_HEADER
