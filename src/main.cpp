#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Screen {
public:
    Screen(const char *title, int width, int height);
    ~Screen();

    void render();
    void handleEvent();
    void loop();

    bool isStarOnRect(SDL_Point &starPos, SDL_Rect &rect);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    int width;
    int height;

    bool quit=false;
    SDL_Event event;
    bool isDragging = false;
    int dragX = 0, dragY = 0;
};

Screen::Screen(const char *title, int width, int height)
    : width(width), height(height) {
    // 初始化SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (!window) {
        printf("窗口创建失败: %s\n", SDL_GetError());
        exit(-2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        exit(-2);
    }
}
Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::render() {
    const SDL_Rect barbox = {0, 0, width, 30};
    const SDL_Rect quitButton = {750, 5, 35, 20};

    // 清除屏幕
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // 在这里添加渲染的代码...
    SDL_SetRenderDrawColor(renderer, 125, 195, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->renderer, &barbox);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->renderer, &quitButton);

    // 刷新渲染器
    SDL_RenderPresent(renderer);
}


void Screen::handleEvent() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) this->quit=true;
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    
                    this->dragX = event.button.x;
                    this->dragY = event.button.y;
                    SDL_Point starPos ={dragX,dragY};
                    SDL_Rect box= {0, 0, width, 30};
                    SDL_Rect quitButton = {750, 5, 35, 20};
                    this->quit = isStarOnRect(starPos, quitButton);

                    this->isDragging = isStarOnRect(starPos, box);
                    //printf("%s\n",isStarOnRect(starPos, box) ? "is on rect" :"is NOT on rect");
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) this->isDragging = false;
                break;
            case SDL_MOUSEMOTION:
                if (this->isDragging) {
                    int x = event.motion.x;
                    int y = event.motion.y;
                    int dx = x - this->dragX;
                    int dy = y - this->dragY;
                    SDL_GetWindowPosition(window, &x, &y);
                    SDL_SetWindowPosition(window, x + dx, y + dy);
                }
                break;
        }
    }
}

void Screen::loop() {
    while (not quit) {
        this->render();
        this->handleEvent();
    }
}

bool Screen::isStarOnRect(SDL_Point &starPos, SDL_Rect &rect) {
    if (starPos.x >= rect.x && starPos.x <= rect.x + rect.w
        &&
        starPos.y >= rect.y && starPos.y <= rect.y + rect.h) {
            return true;
    }
    return false;
}

int main(int argc, char **argv) {
    Screen screen("My Window", 800, 600);
    screen.loop();
    return 0;
}