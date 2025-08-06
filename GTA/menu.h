#pragma once
#include "stdafx.h"
#include "Graphics.h"

class Menu {
public:
    using MenuHandler = void(*)(Menu&);

    int menuStartTime = 0;
    int menuCloseEndTime;
    // Funções principais
    void Render();
    void ProcessInput();
    bool IsOpen() const;
    void SetInputDelay(int milliseconds);
    bool IsInputReady() const;

    // Funções para adicionar elementos ao menu
    Menu& submenu(const std::string& text, MenuHandler handler = nullptr);
    Menu& option(const std::string& text, void(*callback)() = nullptr);
    Menu& toggle(const std::string& text, bool& value, void(*callback)(bool) = nullptr);
    Menu& slider(const std::string& text, int& value, int min, int max, int step, int delay_ms = 50, void(*callback)(int) = nullptr);
    Menu& slider(const std::string& text, float& value, float min, float max, float step, int delay_ms = 50, void(*callback)(float) = nullptr);

    // Estado do menu
    bool is_open = false;
    int current_option = 1;
    int total_options = 0;
    int drawn_options = 0;
    int current_tab = 1;
    int drawn_tabs = 0;
    int total_tab_count = 0;
    int option_change_time = 0;

    // Configuração de submenus
    static constexpr int kMaxSubmenuLevels = 10;
    int submenu_level = -1;
    MenuHandler submenu_handlers[kMaxSubmenuLevels] = {};
    int saved_options[kMaxSubmenuLevels] = {};
    std::string submenu_names[kMaxSubmenuLevels];
    std::string submenu_name{};
    MenuHandler menuEnter{};

    struct
    {
        float positionX = 880;
        float positionY = 200.0f;
        float width = 350.0f;
        float height = 38.0f;
        float menuHeight = 150.0f;

        vec4_t HeaderColor = { 0, 0, 0, 0.90f };
        vec4_t backgroundColor = { 0, 0, 0, 0.75f };
        vec4_t scrollbar = { 0.52f, 0.15f, 0, 1 };

    }Scorpion;

private:
    float CalculateScrollPosition(float current_drawn_option);

    int input_delay_tick = 0;

    Rectangle submenu_bounds;



};

extern Menu* g_Menu;