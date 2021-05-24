#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL.h>

#ifdef _WIN32
# include <SDL2/SDL_main.h>
#endif

constexpr int WIDTH = 800, HEIGHT = 600;

int main(int, char**)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    auto win = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow(
                        "Окно нашего графического приложения",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        WIDTH, HEIGHT,
                        SDL_WINDOW_SHOWN),
            SDL_DestroyWindow);
    if (win == nullptr) {
        std::cerr << "При создании окна произошла ошибка: " <<
                SDL_GetError() << std::endl;
        return 1;
    }

    auto ren = std::shared_ptr<SDL_Renderer>(
            SDL_CreateRenderer(win.get(), -1,
                    SDL_RENDERER_ACCELERATED |
                    SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    if (ren == nullptr) {
        std::cerr << "При создании renderer-а произошла ошибка: " <<
                SDL_GetError() << std::endl;
        return 1;
    }


    SDL_Event event;
    while(1){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return 0;
        }
        SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 0);
        SDL_RenderClear(ren.get());

        SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);

        SDL_Rect body = {250, 200, 200, 200};
        SDL_RenderDrawRect(ren.get(), &body);
        SDL_RenderDrawLine(ren.get(), 250, 200, 350, 100);
        SDL_RenderDrawLine(ren.get(), 350, 100, 450, 200);
        SDL_Rect door = {300, 300, 50, 100};
        SDL_RenderDrawRect(ren.get(), &door);

        SDL_Rect chimney = {375, 95, 15, 50};
        SDL_RenderDrawRect(ren.get(), &chimney);

        SDL_Rect windowHouse = {375, 325, 50, 50};
        SDL_RenderDrawRect(ren.get(), &windowHouse);

        int x0=340, y0 = 150, radius = 20;
        int x = 0;
        int y = radius;
        int delta = 1 - 2 * radius;
        int error = 0;
        while(y >= 0) {
            SDL_RenderDrawPoint(ren.get(), x0 + x, y0 + y);
            SDL_RenderDrawPoint(ren.get(), x0 + x, y0 - y);
            SDL_RenderDrawPoint(ren.get(), x0 - x, y0 + y);
            SDL_RenderDrawPoint(ren.get(), x0 - x, y0 - y);

            error = 2 * (delta + y) - 1;
            if(delta < 0 && error <= 0) {
                ++x;
                delta += 2 * x + 1;
                continue;
            }
            error = 2 * (delta - x) - 1;
            if(delta > 0 && error > 0) {
                --y;
                delta += 1 - 2 * y;
                continue;
            }
            ++x;
            delta += 2 * (x - y);
                --y;
        }
            SDL_RenderPresent(ren.get());
        }

        return 0;
}
