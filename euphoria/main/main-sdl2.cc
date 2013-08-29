// Euphoria is awesome: 幸福感是真棒

#include "SDL.h"
#include <string>
#include <vector>
#include <cassert>
#include <memory>

void Error(const std::string& title, const std::string& text) {
    const int result = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), text.c_str(), NULL);
    if( result < 0 ) {
        fprintf(stderr, "ERROR: %s\n%s\n", title, text);
    }
}

class Sdl {
public:
    Sdl() : initialized_(false) {
        assert(this);
        const int result = SDL_Init(SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if( result >= 0) {
            initialized_ = true;
        }
        else {
            error_ = SDL_GetError();
        }
    }
    bool initialized() const {
        assert(this);
        return initialized_;
    }
    const std::string& error() const {
        assert(this);
        return error_;
    }
    ~Sdl() {
        assert(this);
        if( initialized_ ) {
            SDL_Quit();
        }
    }

private:
    bool initialized_;
    std::string error_;
};

class SDLCore {
public:
    bool initialized() const {
        return initialized_;
    }
    const std::string& error() const {
        return error_;
    }

    bool HasFailed() const {
        return initialized() == false;
    }

protected:
    SDLCore() : initialized_(false) {
        assert(this);
    }

    bool HandleStatus(int code) {
        assert(this);
        if( code < 0 ) {
            ReportFail();
            return false;
        }
        else {
            return true;
        }
    }

    void ReportFail() {
        assert(this);
        initialized_ = false;
        error_ = SDL_GetError();
    }

    void ReportOk() {
        assert(this);
        initialized_ = true;
    }

private:
    bool initialized_;
    std::string error_;
};

class VideoMode {
public:
    explicit VideoMode(const SDL_DisplayMode& mode)
        : width_(mode.w)
        , height_(mode.h)
        , refreshRate_(mode.refresh_rate)
        , pixelFormat_(SDL_GetPixelFormatName(mode.format)) {
            assert(this);
    }

    int width() const {
        assert(this);
        return width_;
    }

    int height() const {
        assert(this);
        return height_;
    }

    int refreshRate() const {
        assert(this);
        return refreshRate_;
    }

    const std::string& pixelFormat() const {
        assert(this);
        return pixelFormat_;
    }

private:
    int width_;
    int height_;
    int refreshRate_;
    std::string pixelFormat_;
};

class DisplayInfo : public SDLCore {
public:
    explicit DisplayInfo(int id) {
        assert(this);
        SDL_Rect bounds;
        if( HandleStatus(SDL_GetDisplayBounds(id, &bounds)) == false ) return;
        width_ = bounds.w;
        height_ = bounds.h;
        x_ = bounds.x;
        y_ = bounds.y;

        const char* name = SDL_GetDisplayName(id);
        if( name != 0 ) {
            name_ = name;
        }

        const int nummodes = SDL_GetNumDisplayModes(id);
        if( nummodes <= 0  ) {
            ReportFail();
            return;
        }
        for(int modeid=0; modeid<nummodes; ++modeid) {
            SDL_DisplayMode mode;
            if( HandleStatus(SDL_GetDisplayMode(id, modeid, &mode)) == false ) {
                return;
            }
            modes_.push_back(VideoMode(mode));
        }
        ReportOk();
    }

    int width() const {
        assert(this);
        return width_;
    }

    int height() const {
        assert(this);
        return height_;
    }

    int x() const {
        assert(this);
        return x_;
    }

    int y() const {
        assert(this);
        return y_;
    }

    const std::string& name() {
        assert(this);
        return name_;
    }

    const std::vector<VideoMode>& modes() const {
        assert(this);
        return modes_;
    }

private:
    int width_;
    int height_;
    int x_;
    int y_;
    std::string name_;
    std::vector<VideoMode> modes_;
};

class VideoDisplays : public SDLCore {
public:
    VideoDisplays() {
        assert(this);
        const int numberofdisplays = SDL_GetNumVideoDisplays();
        if( numberofdisplays <= 0 ) {
            ReportFail();
            return;
        }
        for(int displayid = 0; displayid<numberofdisplays; ++displayid) {
            displays_.push_back(DisplayInfo(displayid));
        }
        ReportOk();
    }

    const std::vector<DisplayInfo>& displays() const {
        assert(this);
        return displays_;
    }

    int PrimaryScreen() {
        assert(this);
        return 0;
    }
private:
    std::vector<DisplayInfo> displays_;
};

class Window : SDLCore {
public:
    Window(const std::string title, int x, int y, int width, int height, bool main)
        : window_(SDL_CreateWindow( title.c_str(), x, y, width, height,
        main ? (SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS) : SDL_WINDOW_BORDERLESS
        )) {
        assert(this);
        if( window_ == NULL ) {
            ReportFail();
        }
        ReportOk();
    }

    ~Window() {
        assert(this);
        if( window_ ) {
            SDL_DestroyWindow(window_);
            window_ = 0;
        }
    }

    SDL_Window* window() {
        assert(this);
        return window_;
    }

    void SetupDisplayModeToDesktop() {
        assert(this);
        assert(window_);
        int result = SDL_SetWindowDisplayMode(window_, NULL);
        if ( result < 0 ) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to set displaymode: %d", SDL_GetWindowID(window_));
        }
    }

    bool SetFullscreen(bool truefullscreen) {
        if( truefullscreen ) {
            const int fullscreen = SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Setting window fullscreen: %d - %d", SDL_GetWindowID(window_), fullscreen);
            if( fullscreen < 0 ) {
                const int fullscreendesktop = SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Setting window fake fullscreen: %d - %d", SDL_GetWindowID(window_), fullscreendesktop);
                return fullscreendesktop >= 0;
            }
            else {
                return true;
            }
        }
        else {
            const int fullscreendesktop = SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Setting window fake fullscreen: %d - %d", SDL_GetWindowID(window_), fullscreendesktop);
            return fullscreendesktop >= 0;
        }
    }

private:
    SDL_Window* window_;
};

class BlackRenderer : SDLCore {
public:
    explicit BlackRenderer(Window* window)
        : renderer_( SDL_CreateRenderer(window->window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ) {
            assert(this);
            if (renderer_ == NULL) {
                ReportFail();
            }
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            ReportOk();
    }

    void Render() {
        assert(this);
        SDL_RenderClear(renderer_);
        SDL_RenderPresent(renderer_);
    }

    ~BlackRenderer() {
        assert(this);
        SDL_DestroyRenderer(renderer_);
    }

private:
    SDL_Renderer* renderer_;
};

class Settings {
public:
    Settings()
        : blackout_(false)
        , width_(800)
        , height_(600)
        , fullscreen_(false)
    {
        assert(this);
    }

    const bool blackout() const {
        assert(this);
        return blackout_;
    }

    const int width() const {
        assert(this);
        return width_;
    }

    const int height() const {
        assert(this);
        return height_;
    }

    const bool fullscreen() const {
        assert(this);
        return fullscreen_;
    }

private:
    bool blackout_;
    int width_;
    int height_;
    bool fullscreen_;
};

class Timer {
public:
    Timer() : start_(0) {
        Reset();
    }

    void Reset() {
        start_ = SDL_GetPerformanceCounter();
    }

    template<typename F>
    F GetElapsedSeconds() const {
        const Uint64 now = SDL_GetPerformanceCounter();
        return static_cast<F>(now - start_) / SDL_GetPerformanceFrequency();
    }

private:
    Uint64 start_;
};

int main(int argc, char *argv[]) {
    Settings settings;

    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    Sdl sdl;
    if( sdl.initialized() == false ){
        Error("FATAL", sdl.error());
        return 0;
    }
    SDL_DisableScreenSaver();
    VideoDisplays displays;
    if( displays.HasFailed() ) {
        Error("Display failed", displays.error());
        return 0;
    }

    std::shared_ptr<Window> primaryscreen;
    std::vector<std::shared_ptr<Window> > windows;
    std::vector<std::shared_ptr<BlackRenderer> > blacks;

    for(size_t i=0; i<displays.displays().size(); ++i) {
        const bool isprimaryscrreen = i == displays.PrimaryScreen();
        const bool createscreen = isprimaryscrreen || settings.blackout();

        if( createscreen ) {
            DisplayInfo di = displays.displays()[i];
            const int width = isprimaryscrreen==false || settings.width() == 0 ? di.width() : settings.width();
            const int height = isprimaryscrreen==false ||settings.height() == 0 ? di.height() : settings.height();
            const int x = di.x() + (di.width() - width) / 2;
            const int y = di.y() + (di.height() - height) / 2;
            std::shared_ptr<Window> screen( new Window(isprimaryscrreen ? "Game" : "Black", x, y, width, height, isprimaryscrreen) );
            windows.push_back(screen);

            screen->SetupDisplayModeToDesktop();

            if( isprimaryscrreen) {
                primaryscreen = screen;
            }
            else {
                std::shared_ptr<BlackRenderer> br( new BlackRenderer(screen.get()) );
                blacks.push_back(br);
            }
        }
    }

    if( settings.fullscreen()){
        /*for(size_t i=0; i<windows.size(); ++i) {
            std::shared_ptr<Window> screen = windows[i];
            if( screen != primaryscreen ){
                screen->SetFullscreen(false);
            }
        }*/
        primaryscreen->SetFullscreen(true);
    }

    Timer timer;
    bool running = true;

    while(running) {
        const float delta = timer.GetElapsedSeconds<float>();
        timer.Reset();

        for(size_t i=0; i<blacks.size(); ++i) {
            blacks[i]->Render();
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if( event.type == SDL_QUIT) {
                running = false;
            }
            if( event.type == SDL_KEYDOWN ) {
                if( event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
    }

    return 0;
}
