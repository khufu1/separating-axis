#pragma once

#include "global.h"
#include "vec2.hpp"
#include <SDL3/SDL.h>

using namespace math;

namespace sat {

/*
   A
   ---------------- B
   |              |
   |              |
   |              |
   |              |
 D ----------------
                  C
  When rendering a rectangle set vertices in this order {{A}, {B} ,{C} ,{D}}
  so that the Element buffer work correctly
*/
enum class Shapes { Triangle, Rectangle };

// An Drawing default shape is a recangle

class Drawing {
public:
  Drawing(Shapes shape, SDL_Texture *texture = nullptr) noexcept;
  ~Drawing() = default;

  inline static std::unordered_map<std::string, SDL_Texture *> Textures;
  static void loadTextures(SDL_Renderer *renderer, std::string directory) noexcept;
  static void destroyTextures() noexcept;
  static SDL_Texture *getTexturePtr(std::string name) noexcept;
  static void setBlendMode(SDL_Texture *t, SDL_BlendMode m) noexcept;
  static void setScaleMode(SDL_Texture *t, SDL_ScaleMode m) noexcept;

  void setTexture(SDL_Texture *texture) noexcept;
  void setVertices(std::vector<vec2f> v) noexcept;

  [[nodiscard]] SDL_Texture *texture() const noexcept;
  [[nodiscard]] double angle() const noexcept;
  [[nodiscard]] const vec2f center() const noexcept;
  [[nodiscard]] const std::vector<vec2f> &vertices() const noexcept;

  void translate(vec2f v) noexcept;
  void rotate(double a, vec2f center) noexcept;

  static inline std::vector<Drawing *> Drawings;

  static FORCE_INLINE_ void renderScene(SDL_Renderer *r) noexcept
  {
    for (auto &d : Drawings)
      d->blit(r);
  }

protected:
  void blit(SDL_Renderer *renderer) noexcept;
  void setStaticVertexData() noexcept;
  void setDynamicVertexData() noexcept;

private:
  template <Shapes T>
  void blit(SDL_Renderer *renderer) noexcept;
  template <Shapes T>
  void setStaticVertexData() noexcept;
  template <Shapes T>
  void setDynamicVertexData() noexcept;

protected:
  Shapes Shape;
  SDL_Texture *TexturePtr;
  double Angle{0.0f};
  std::vector<vec2f> Vertices;

private:
  static inline std::vector<SDL_Vertex> TextureVertices;
  static inline std::vector<int> TextureVertexIndices;
};

#include "Drawing.inl"

inline void operator<<(std::ostream &s, std::vector<vec2f> v)
{
  for (auto &p : v)
    s << p;
}

} // namespace sat
