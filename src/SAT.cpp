#include "Collider.hpp"
#include "global.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdlib>

using namespace sat;
using namespace math;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
bool windowShouldOpen{true};

Collider *r1Ptr, *r2Ptr;

static void handleEvents() noexcept;

int main()
{
  LOG_IF(SDL_Init(SDL_INIT_VIDEO) != 0, "COULDNT INIT SDL \n");
  LOG_IF(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG, "COULDNT INIT IMG \n");

  SDL_CreateWindowAndRenderer("SAT", 600, 600, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);
  SDL_SetRenderVSync(renderer, true);

  Collider::loadTextures(renderer, "assets/");
  Collider r1{Shapes::Triangle, Drawing::getTexturePtr("rectGreen")};
  r1.setVertices({{400, 400}, {600, 400}, {400, 600}});
  Collider r2{Shapes::Rectangle, Drawing::getTexturePtr("rectWhite")};
  r2.setVertices({{200, 200}, {400, 200}, {400, 400}, {200, 400}});
  LOG_IF(!r1.texture() || !r2.texture(), "Couldnt load textures \n");
  r1Ptr = &r1;
  r2Ptr = &r2;

  while (windowShouldOpen) {
    SDL_RenderClear(renderer);
    handleEvents();
    Drawing::renderScene(renderer);
    SDL_RenderPresent(renderer);
  }
  Collider::destroyTextures();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return EXIT_SUCCESS;
}

static void handleEvents() noexcept
{
  static bool left{false};
  static bool right{false};
  static bool up{false};
  static bool down{false};
  static bool lShift{false};
  static bool rShift{false};

  while (SDL_PollEvent(&event)) {
    SDL_Scancode key = event.key.scancode;
    switch (event.type) {
    case SDL_EVENT_QUIT:
      windowShouldOpen = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      if (key == SDL_SCANCODE_LSHIFT)
        lShift = true;
      if (key == SDL_SCANCODE_RSHIFT)
        rShift = true;
      if (key == SDL_SCANCODE_LEFT)
        left = true;
      if (key == SDL_SCANCODE_RIGHT)
        right = true;
      if (key == SDL_SCANCODE_UP)
        up = true;
      if (key == SDL_SCANCODE_DOWN)
        down = true;
      if (key == SDL_SCANCODE_ESCAPE)
        windowShouldOpen = false;
      break;
    case SDL_EVENT_KEY_UP:
      if (key == SDL_SCANCODE_LSHIFT)
        lShift = false;
      if (key == SDL_SCANCODE_RSHIFT)
        rShift = false;
      if (key == SDL_SCANCODE_LEFT)
        left = false;
      if (key == SDL_SCANCODE_RIGHT)
        right = false;
      if (key == SDL_SCANCODE_UP)
        up = false;
      if (key == SDL_SCANCODE_DOWN)
        down = false;
      break;
    default:
      break;
    }
  }

  if (lShift) {
    r1Ptr->rotate(3, r1Ptr->center());
  }
  if (rShift) {
    r2Ptr->rotate(3, r2Ptr->center());
  }
  if (left) {
    r1Ptr->translate({-1, 0});
  }
  if (right) {
    r1Ptr->translate({1, 0});
  }
  if (up) {
    r1Ptr->translate({0, -1});
  }
  if (down) {
    r1Ptr->translate({0, 1});
  }

  if (r1Ptr->collidingWith(*r2Ptr)) {
    r2Ptr->setTexture(Drawing::getTexturePtr("rectCollide"));
  }
  else {
    r2Ptr->setTexture(Drawing::getTexturePtr("rectWhite"));
  }
}
