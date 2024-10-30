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

class Drawing {
public:
  Drawing(Shapes shape = Shapes::Triangle, SDL_Texture *texture = nullptr) noexcept;
  ~Drawing() = default;

  static void loadTextures(SDL_Renderer *renderer, std::string directory) noexcept;
  static void destroyTextures() noexcept;
  [[nodiscard]] static SDL_Texture *getTexturePtr(std::string name) noexcept;
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

  static FORCE_INLINE_ void renderScene(SDL_Renderer *r) noexcept
  {
    for (auto &d : Drawings)
      d->blit(r);
  }

protected:
  void blit(SDL_Renderer *renderer) noexcept;
  void setDynamicVertexData() noexcept;

private:
  static constexpr bool setStaticVertexData() noexcept;

protected:
  Shapes Shape;
  SDL_Texture *TexturePtr;
  double Angle{0.0f};
  std::vector<vec2f> Vertices;

private:
  static inline std::unordered_map<std::string, SDL_Texture *> Textures;
  static inline std::vector<Drawing *> Drawings;
  static inline std::array<SDL_Vertex, 4> TextureVertices;
  static inline std::array<int, 6> TextureVertexIndices;
};

} // namespace sat
