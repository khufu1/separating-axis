template <>
inline void Drawing::setStaticVertexData<Shapes::Rectangle>() noexcept
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
  TextureVertices[2].tex_coord.y = 1.0f;
  TextureVertices[2].tex_coord.x = 1.0f;
  // D
  TextureVertices[3].color.r = 1.0f;
  TextureVertices[3].color.g = 1.0f;
  TextureVertices[3].color.b = 1.0f;
  TextureVertices[3].color.a = 1.0f;
  TextureVertices[3].tex_coord.x = 0.0f;
  TextureVertices[3].tex_coord.y = 1.0f;
  // similar to opengl's EBO
  TextureVertexIndices = {0, 1, 2,  // first triangle
                          0, 3, 2}; // second triangle
}

template <>
inline void Drawing::setStaticVertexData<Shapes::Triangle>() noexcept
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
  TextureVertices[2].tex_coord.y = 1.0f;
  TextureVertices[2].tex_coord.x = 1.0f;
  // similar to opengl's EBO
  TextureVertexIndices = {0, 1, 2};
}

template <>
inline void Drawing::setDynamicVertexData<Shapes::Rectangle>() noexcept
{
  TextureVertices[0].position = {Vertices[0].x, Vertices[0].y};
  TextureVertices[1].position = {Vertices[1].x, Vertices[1].y};
  TextureVertices[2].position = {Vertices[2].x, Vertices[2].y};
  TextureVertices[3].position = {Vertices[3].x, Vertices[3].y};
}

template <>
inline void Drawing::setDynamicVertexData<Shapes::Triangle>() noexcept
{
  TextureVertices[0].position = {Vertices[0].x, Vertices[0].y};
  TextureVertices[1].position = {Vertices[1].x, Vertices[1].y};
  TextureVertices[2].position = {Vertices[2].x, Vertices[2].y};
}

template <>
inline void Drawing::blit<Shapes::Rectangle>(SDL_Renderer *renderer) noexcept
{
  setDynamicVertexData<Shapes::Rectangle>();
  SDL_RenderGeometry(renderer, TexturePtr, TextureVertices.data(), 4,
                     TextureVertexIndices.data(), 6);
}

template <>
inline void Drawing::blit<Shapes::Triangle>(SDL_Renderer *renderer) noexcept
{
  setDynamicVertexData<Shapes::Triangle>();
  SDL_RenderGeometry(renderer, TexturePtr, TextureVertices.data(), 3,
                     TextureVertexIndices.data(), 3);
}
