#include "Collider.hpp"
#include "global.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3_image/SDL_image.h>

using namespace sat;
using namespace math;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
bool windowShouldOpen{true};

class Convex : public Collider {
public:
  using Collider::Collider;

  void display() noexcept override
  {
    blit(renderer);
  }
};

Convex *r1Ptr, *r2Ptr;

static void handleEvents() noexcept;

int main()
{
  LOG_IF(SDL_Init(SDL_INIT_VIDEO) != 0, "COULDNT INIT SDL \n");
  LOG_IF(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG, "COULDNT INIT IMG \n");

  SDL_CreateWindowAndRenderer("SAT", 600, 600, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);
  SDL_SetRenderVSync(renderer, true);

  Convex::loadTextures(renderer, "assets/");
  Convex r1{Shapes::Triangle, Drawing::getTexturePtr("rectGreen")};
  r1.setVertices({{400, 400}, {600, 400}, {400, 600}});
  Convex r2{Shapes::Rectangle, Drawing::getTexturePtr("rectWhite")};
  r2.setVertices({{200, 200}, {400, 200}, {400, 400}, {200, 400}});
  LOG_IF(!r1.texture() || !r2.texture(), "Couldnt load textures \n");
  r1Ptr = &r1;
  r2Ptr = &r2;

  while (windowShouldOpen) {
    SDL_RenderClear(renderer);
    handleEvents();
    Convex::renderScene();
    SDL_RenderPresent(renderer);
  }
  Convex::destroyTextures();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
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
    switch (event.type) {
    case SDL_EVENT_QUIT:
      windowShouldOpen = false;
      break;
    case SDL_EVENT_KEY_DOWN: // no need to perfectly handle events
      if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
        lShift = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
        rShift = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
        left = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        right = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_UP)
        up = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
        down = true;
      if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        windowShouldOpen = false;
      break;
    case SDL_EVENT_KEY_UP: // no need to perfectly handle events
      if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
        lShift = false;
      if (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
        rShift = false;
      if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
        left = false;
      if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        right = false;
      if (event.key.keysym.scancode == SDL_SCANCODE_UP)
        up = false;
      if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
        down = false;
      break;
    default:
      break;
    }
  }

  if (lShift) {
    r1Ptr->rotate(3, r1Ptr->center());
    // std::cout << r1Ptr->vertices();
  }
  if (rShift) {
    r2Ptr->rotate(3, r2Ptr->center());
    // std::cout << "2" << r2Ptr->vertices();
  }
  if (left) {
    // std::cout << r1Ptr->vertices();
    r1Ptr->translate({-1, 0});
  }
  if (right) {
    // std::cout << r1Ptr->vertices();
    r1Ptr->translate({1, 0});
  }
  if (up) {
    // std::cout << r1Ptr->vertices();
    r1Ptr->translate({0, -1});
  }
  if (down) {
    // std::cout << r1Ptr->vertices();
    r1Ptr->translate({0, 1});
  }

  if (r1Ptr->collidingWith(*r2Ptr)) {
    r2Ptr->setTexture(Drawing::getTexturePtr("rectCollide"));
    // std::cout << "1" << r1Ptr->angle() << std::endl;
    // std::cout << "2" << r2Ptr->angle() << std::endl;
    // std::cout << "1" << r1Ptr->vertices();
    // std::cout << "2" << r2Ptr->vertices();
  }
  else {
    r2Ptr->setTexture(Drawing::getTexturePtr("rectWhite"));
  }
}
