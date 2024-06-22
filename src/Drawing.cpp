#include "Drawing.hpp"

#include "global.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace sat {

Drawing::Drawing(Shapes shape, SDL_Texture *texture) noexcept
    : Shape{shape}, TexturePtr{texture}
{
  switch (shape) {
  case Shapes::Rectangle:
    Vertices.resize(4);
    TextureVertices.resize(4);
    TextureVertexIndices.resize(6);
    break;
  case Shapes::Triangle:
    Vertices.resize(3);
    TextureVertices.resize(3);
    TextureVertexIndices.resize(3);
    break;
  }
  setStaticVertexData();
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
      // FIXME - fix wchar c_str on windows u can load each one individualy
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
  if (v.size() == Vertices.size()) {
    Vertices = v;
  }
  else {
    print("Wrong vertices num BOZO\n");
    exit(EXIT_FAILURE);
  }

  if (Angle != 0.0f) {
    rotate(Angle, center());
  }
}

void Drawing::setBlendMode(SDL_Texture *t, SDL_BlendMode m) noexcept
{
  LOG_IF(SDL_SetTextureBlendMode(t, m) < 0, "Couldnt set texture blend mode ");
}

void Drawing::setScaleMode(SDL_Texture *t, SDL_ScaleMode m) noexcept
{
  LOG_IF(SDL_SetTextureScaleMode(t, m) < 0, "Couldnt set texture scale mode ");
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
  print("Wrong Name : {} \n", name);
  print("{} \n", e.what());
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

void Drawing::blit(SDL_Renderer *renderer) noexcept
{
  switch (Shape) {
  case Shapes::Rectangle:
    blit<Shapes::Rectangle>(renderer);
    break;
  case Shapes::Triangle:
    blit<Shapes::Triangle>(renderer);
    break;
  }
}

void Drawing::setStaticVertexData() noexcept
{
  switch (Shape) {
  case Shapes::Rectangle:
    setStaticVertexData<Shapes::Rectangle>();
    break;
  case Shapes::Triangle:
    setStaticVertexData<Shapes::Triangle>();
    break;
  }
}

void Drawing::setDynamicVertexData() noexcept
{
  switch (Shape) {
  case Shapes::Rectangle:
    setDynamicVertexData<Shapes::Rectangle>();
    break;
  case Shapes::Triangle:
    setDynamicVertexData<Shapes::Triangle>();
    break;
  }
}

} // namespace sat
