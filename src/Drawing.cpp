#include "Drawing.hpp"

#include "global.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace sat {

Drawing::Drawing(Shapes shape, SDL_Texture *texture) noexcept
    : Shape(shape), TexturePtr(texture)
{
  [[maybe_unused]] static bool _ = Drawing::setStaticVertexData();
  if (shape == Shapes::Triangle)
    Vertices.resize(3);
  else
    Vertices.resize(4);
  Drawings.push_back(this);
}

void Drawing::loadTextures(SDL_Renderer *renderer, std::string directory) noexcept
{
  std::pair<std::string, SDL_Texture *> dNode;

  for (const auto &dirEntry :
       std::filesystem::recursive_directory_iterator(directory)) {
    if (dirEntry.is_regular_file() &&
        dirEntry.path().filename().extension() == ".png") [[likely]] {
      dNode.first = dirEntry.path().filename().string();
      dNode.second =
          IMG_LoadTexture(renderer, dirEntry.path().relative_path().c_str());

      setBlendMode(dNode.second, SDL_BLENDMODE_BLEND);
      setScaleMode(dNode.second, SDL_SCALEMODE_NEAREST);

      Textures.insert(dNode);

      if (dNode.second == nullptr) [[unlikely]]
        print("Couldn't load texture : {}\n", dNode.first);
      else
        print("loaded texture : {}\n", dNode.first);
    }
  }
}

void Drawing::destroyTextures() noexcept
{
  for (auto &t : Textures)
    SDL_DestroyTexture(t.second);
}

void Drawing::setTexture(SDL_Texture *texture) noexcept
{
  TexturePtr = texture;
}

void Drawing::setVertices(std::vector<vec2f> v) noexcept
{
  if (v.size() == Vertices.size())
    Vertices = v;
  else
    print("Wrong vertices num \n");

  if (Angle != 0.0f) {
    rotate(Angle, center());
  }
}

void Drawing::setBlendMode(SDL_Texture *t, SDL_BlendMode m) noexcept
{
  LOG_IF(SDL_SetTextureBlendMode(t, m) != true, "Couldnt set texture blend mode.");
}

void Drawing::setScaleMode(SDL_Texture *t, SDL_ScaleMode m) noexcept
{
  LOG_IF(SDL_SetTextureScaleMode(t, m) != true, "Couldnt set texture scale mode.");
}

SDL_Texture *Drawing::texture() const noexcept
{
  return TexturePtr;
}

double Drawing::angle() const noexcept
{
  return Angle;
}

const std::vector<vec2f> &Drawing::vertices() const noexcept
{
  return Vertices;
}

const vec2f Drawing::center() const noexcept
{
  float sumX{0.0f};
  float sumY{0.0f};
  for (size_t i = 0; i < Vertices.size(); i++) {
    sumX += Vertices[i].x;
    sumY += Vertices[i].y;
  }
  return {sumX / Vertices.size(), sumY / Vertices.size()};
}

SDL_Texture *Drawing::getTexturePtr(std::string name) noexcept
try {
  return Textures.at(name + ".png");
}
catch (std::out_of_range &e) {
  print("Wrong Texture Name : {} \n", name);
  return nullptr;
}

void Drawing::translate(vec2f v) noexcept
{
  for (size_t i = 0; i < Vertices.size(); i++) {
    Vertices[i].translate(v);
  }
}

void Drawing::rotate(double a, vec2f center) noexcept
{
  Angle = fmod(Angle + a, 360.0f);
  for (size_t i = 0; i < Vertices.size(); i++) {
    Vertices[i].rotate(a, center);
  }
}

constexpr bool Drawing::setStaticVertexData() noexcept
{
  // A
  TextureVertices[0].color.r = 1.0f;
  TextureVertices[0].color.g = 1.0f;
  TextureVertices[0].color.b = 1.0f;
  TextureVertices[0].color.a = 1.0f;
  TextureVertices[0].tex_coord.x = 0.0f;
  TextureVertices[0].tex_coord.y = 0.0f;
  // B
  TextureVertices[1].color.r = 1.0f;
  TextureVertices[1].color.g = 1.0f;
  TextureVertices[1].color.b = 1.0f;
  TextureVertices[1].color.a = 1.0f;
  TextureVertices[1].tex_coord.x = 1.0f;
  TextureVertices[1].tex_coord.y = 0.0f;
  // C
  TextureVertices[2].color.r = 1.0f;
  TextureVertices[2].color.g = 1.0f;
  TextureVertices[2].color.b = 1.0f;
  TextureVertices[2].color.a = 1.0f;
  TextureVertices[2].tex_coord.x = 1.0f;
  TextureVertices[2].tex_coord.y = 1.0f;
  // D
  TextureVertices[3].color.r = 1.0f;
  TextureVertices[3].color.g = 1.0f;
  TextureVertices[3].color.b = 1.0f;
  TextureVertices[3].color.a = 1.0f;
  TextureVertices[3].tex_coord.x = 0.0f;
  TextureVertices[3].tex_coord.y = 1.0f;
  // similar to opengl's EBO
  TextureVertexIndices = {0, 1, 2,  // first triangle
                          0, 3, 2}; // second triangle}
  return true;
}

void Drawing::setDynamicVertexData() noexcept
{
  if (Shape == Shapes::Rectangle) {
    TextureVertices[0].position = {Vertices[0].x, Vertices[0].y};
    TextureVertices[1].position = {Vertices[1].x, Vertices[1].y};
    TextureVertices[2].position = {Vertices[2].x, Vertices[2].y};
    TextureVertices[3].position = {Vertices[3].x, Vertices[3].y};
  }
  else {
    TextureVertices[0].position = {Vertices[0].x, Vertices[0].y};
    TextureVertices[1].position = {Vertices[1].x, Vertices[1].y};
    TextureVertices[2].position = {Vertices[2].x, Vertices[2].y};
  }
}

void Drawing::blit(SDL_Renderer *renderer) noexcept
{
  setDynamicVertexData();
  if (Shape == Shapes::Rectangle) {
    SDL_RenderGeometry(renderer, TexturePtr, TextureVertices.data(), 4,
                       TextureVertexIndices.data(), 6);
  }
  else {
    SDL_RenderGeometry(renderer, TexturePtr, TextureVertices.data(), 3,
                       TextureVertexIndices.data(), 3);
  }
}

} // namespace sat
