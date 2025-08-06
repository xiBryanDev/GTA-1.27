#include "menu.h"
#include "pad.h"
#include "native.h"
#include "notification.h"

Menu* g_Menu;

#define VA_BUFFER_SIZE 0x1000
static char va_buffer[VA_BUFFER_SIZE];
const char* va(const char* fmt, ...)
{
    memset(va_buffer, 0, 0x1000);

    va_list ap;
    va_start(ap, fmt);
    vsprintf(va_buffer, fmt, ap);
    va_end(ap);

    return va_buffer;
}


void Menu::SetInputDelay(int milliseconds) {
    input_delay_tick = Sys_Milliseconds() + milliseconds;
}

bool Menu::IsInputReady() const {
    return Sys_Milliseconds() >= input_delay_tick;
}

bool Menu::IsOpen() const {
    return is_open || !IsInputReady();
}

float CalculateLerpFraction(int start_time, int duration) {
    if (duration < 1) return 1.0f;
    int delta_time = Sys_Milliseconds() - start_time;
    if (delta_time >= duration) return 1.0f;
    if (delta_time < 1) return 0.0f;
    return static_cast<float>(delta_time) / static_cast<float>(duration);
}

// Processa as entradas do usuário
void Menu::ProcessInput() {
    if (!IsInputReady()) return;

    if (!is_open) {
        if (IsButtonDown(BUTTON_L1) && IsButtonDown(BUTTON_DPAD_LEFT)) {
            is_open = true;
            menuStartTime = Sys_Milliseconds();
            SetInputDelay(100);
        }
        return;
    }

    if (IsButtonDown(BUTTON_CIRCLE)) {
        if (submenu_level == -1) {
            is_open = false;
            menuCloseEndTime = Sys_Milliseconds() + 500;

            SetInputDelay(100);
            return;
        }
        current_option = saved_options[submenu_level];
        submenu_level--;
        SetInputDelay(200);
        return;
    }

    if (IsButtonDown(BUTTON_DPAD_UP)) {
        current_option = (current_option <= 1) ? total_options : current_option - 1;
        option_change_time = Sys_Milliseconds();
        SetInputDelay(150);
    }
    else if (IsButtonDown(BUTTON_DPAD_DOWN)) {
        current_option = (current_option >= total_options) ? 1 : current_option + 1;
        option_change_time = Sys_Milliseconds();
        SetInputDelay(150);
    }
    else if (IsButtonDown(BUTTON_L1) && submenu_level == -1) {
        current_tab = (current_tab <= 1) ? total_tab_count : current_tab - 1;
        current_option = 1;
        SetInputDelay(150);
    }
    else if (IsButtonDown(BUTTON_R1) && submenu_level == -1) {
        current_tab = (current_tab >= total_tab_count) ? 1 : current_tab + 1;
        current_option = 1;
        SetInputDelay(150);
    }
}
bool test;
int gay;

float test1;
float test2;

// Renderiza o menu
void Menu::Render() {
    if (strcmp(SCRIPT::GET_THIS_SCRIPT_NAME(), "ingamehud") != 0)
        return;

    notificationSystem.Render();

    ProcessInput();

    bool isClosing = !is_open && (Sys_Milliseconds() < menuCloseEndTime);

    if (!is_open && !isClosing)
        return;

    total_tab_count = drawn_tabs;
    drawn_tabs = 0;
    total_options = drawn_options;
    drawn_options = 0;

    float positionX = Scorpion.positionX;
    float positionY = Scorpion.positionY;
    float width = Scorpion.width;
    float height = Scorpion.height;
    vec4_t accent = { 0.25f, 0.70f, 1.0f, 1.0f };
    vec4_t glossColor = { 1, 1, 1, 0.18f };

    // Animação de abertura/fechamento do menu
    float animationDuration = 0.5f; // Duração da animação (em segundos)
    float elapsedTime = isClosing
        ? (animationDuration - (menuCloseEndTime - Sys_Milliseconds()) / 1000.0f)
        : (Sys_Milliseconds() - menuStartTime) / 1000.0f;                     

    float animationProgress = (elapsedTime < animationDuration) ? (elapsedTime / animationDuration) : 1.0f;
    if (isClosing) {
        animationProgress = 1.0f - animationProgress; // Inverte o progresso da animação para fechamento
    }

    // Ajusta a largura e a altura do menu com base no progresso da animação
    float animatedWidth = width * animationProgress;    // Largura animada
    float animatedHeight = height * animationProgress;  // Altura animada
    float animatedX = positionX + (width - animatedWidth) / 2.0f; // Centraliza horizontalmente
    float animatedY = positionY + (height - animatedHeight) / 2.0f; // Centraliza verticalmente

    // Desenha o menu animado
    Rectangle mainMenu(animatedX, animatedY, animatedWidth, animatedHeight);
    UI::Renderer::DrawRectangle(mainMenu, Scorpion.HeaderColor, UI::AnchorPoint::TopLeft);

    // Apenas desenha o conteúdo do menu se ele estiver completamente aberto
    if (animationProgress >= 1.0f && !isClosing) {
        vec2_t textScale = { 0.0f, 0.40f };
        vec4_t textColor = { 1, 1, 1, 1 };
        float textHeight = 81 * textScale.y;
        float centerY = mainMenu.y + (mainMenu.height / 2.0f);
        float textY = centerY - (textHeight / 2.0f);

        // Título do menu
        UI::Renderer::DrawText(submenu_level == -1 ? "SCORPION V1.0" : submenu_name, 0, { mainMenu.x + 5, textY }, textScale, textColor, UI::TextAlign::Left);
        UI::Renderer::DrawText(va("%i/%i", current_option, total_options), 0, { mainMenu.x + mainMenu.width - 5, textY - 1 }, { 0, 0.45f }, textColor, UI::TextAlign::Right);

        mainMenu.y += mainMenu.height + 2;
        submenu_bounds = mainMenu;

        // Renderiza submenus e opções
        if (submenu_level == -1) {
            submenu("Self Options", [](Menu& menu)
                {
                    menu.option("Test", []()
                        {
                            notificationSystem.AddNotification("~r~Crash Detected (Sync)",
                            "Player: " + std::string("xiBryan") + "\n" +
                            "XUID: " + std::string("0000000000000000") + "\n" +
                            "IP: " + va("%i.%i.%i.%i", 0, 0, 0, 0),
                                5000.0f);
                        });
                });
            submenu("Network Options", nullptr);
            submenu("Vehicle Options", nullptr);
            submenu("Weapons Options", nullptr);
            submenu("World Options", nullptr);
            submenu("Teleportation", nullptr);
            submenu("Modder Protection", nullptr);
            submenu("Settings", [](Menu& menu)
                {
                    menu.submenu("Scorpion", [](Menu& menu)
                        {
                            menu.slider("X", menu.Scorpion.positionX, 0, 1280, 1, 10);
                            menu.slider("Y", menu.Scorpion.positionY, 0, 1080, 1, 10);
                            menu.slider("width", menu.Scorpion.width, 100.0f, 500.0f, 1, 10);
                            menu.slider("Header Height", menu.Scorpion.height, 0, 500.0f, 1, 100);
                        });
                });
        }
        else {
            submenu_handlers[submenu_level](*this);
        }
    }

    if (menuEnter) {
        submenu_level++;
        saved_options[submenu_level] = current_option;
        submenu_handlers[submenu_level] = menuEnter;
        submenu_names[submenu_level] = submenu_name;
        current_option = 1;
        menuEnter = nullptr;
    }
}

float Menu::CalculateScrollPosition(float current_drawn_option) {
    const int kMaxVisibleOptions = 20;
    if ((current_option <= kMaxVisibleOptions && current_drawn_option <= kMaxVisibleOptions) ||
        (current_drawn_option > (current_option - kMaxVisibleOptions) && current_drawn_option <= current_option)) {
        return (current_option <= kMaxVisibleOptions && current_drawn_option <= kMaxVisibleOptions)
            ? current_drawn_option
            : (current_drawn_option - (current_option - kMaxVisibleOptions));
    }
    return 0.0f;
}

Menu& Menu::submenu(const std::string& text, MenuHandler handler) {
    drawn_options++;
    float scroll_pos = CalculateScrollPosition(drawn_options);
    if (scroll_pos <= 0.0f) return *this;

    vec2_t textScale = { 0.0f, 0.35f };
    vec4_t textColor = { 1, 1, 1, 1 };

    UI::Renderer::DrawRectangle({ submenu_bounds.x, submenu_bounds.y, submenu_bounds.width, 34}, drawn_options == current_option ? Scorpion.scrollbar : Scorpion.backgroundColor, UI::AnchorPoint::TopLeft);

    UI::Renderer::DrawText(text, 0, { submenu_bounds.x + 5, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Left);

    if (IsInputReady() && current_option == drawn_options && IsButtonDown(BUTTON_CROSS)) {
        if (handler) {
            menuEnter = handler;
            submenu_name = text;
        }
        SetInputDelay(200);
    }
    submenu_bounds.y += 36;
    return *this;
}

Menu& Menu::option(const std::string& text, void(*callback)()) {
	drawn_options++;
    float scroll_pos = CalculateScrollPosition(drawn_options);
    if (scroll_pos <= 0.0f) return *this;

    vec2_t textScale = { 0.0f, 0.35f };
    vec4_t textColor = { 1, 1, 1, 1 };

    UI::Renderer::DrawRectangle({ submenu_bounds.x, submenu_bounds.y, submenu_bounds.width, 34 }, drawn_options == current_option ? Scorpion.scrollbar : Scorpion.backgroundColor, UI::AnchorPoint::TopLeft);

    UI::Renderer::DrawText(text, 0, { submenu_bounds.x + 5, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Left);

	if (IsInputReady() && current_option == drawn_options && IsButtonDown(BUTTON_CROSS)) {
		if (callback) callback();
		SetInputDelay(200);
	}
	submenu_bounds.y += 36;
	return *this;
}   

Menu& Menu::toggle(const std::string& text, bool& value, void(*callback)(bool)) {
    drawn_options++;
    float scroll_pos = CalculateScrollPosition(drawn_options);
    if (scroll_pos <= 0.0f) return *this;


    vec2_t textScale = { 0.0f, 0.35f };
    vec4_t textColor = { 1, 1, 1, 1 };

    UI::Renderer::DrawRectangle({ submenu_bounds.x, submenu_bounds.y, submenu_bounds.width, 34 }, drawn_options == current_option ? Scorpion.scrollbar : Scorpion.backgroundColor, UI::AnchorPoint::TopLeft);

    UI::Renderer::DrawText(text, 0, { submenu_bounds.x + 5, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Left);
   
	UI::Renderer::DrawSprite("commonmenu", value ? "shop_box_tick" : "shop_box_blank", { submenu_bounds.x - 10 + submenu_bounds.width, submenu_bounds.y - 5 ,45, 45 }, 0, textColor, UI::AnchorPoint::RIGHT);


    if (IsInputReady() && current_option == drawn_options && IsButtonDown(BUTTON_CROSS)) {
        value ^= 1;
        if (callback) callback(value);
        SetInputDelay(200);
    }

    submenu_bounds.y += 36;
    return *this;
}

std::string to_string_precise(float value, int precision = 2) {
    std::string result = "";

    // Lida com negativo
    if (value < 0) {
        result += "-";
        value = -value;
    }

    // Arredonda para a precisão desejada
    float pow10 = std::pow(10.0f, precision);
    value = std::round(value * pow10) / pow10;

    // Parte inteira
    int int_part = static_cast<int>(value);
    float frac_part = value - static_cast<float>(int_part);

    // Converte inteiro para string manualmente
    if (int_part == 0) {
        result += "0";
    }
    else {
        char int_buffer[16];
        int int_index = 0;
        int temp = int_part;
        while (temp > 0) {
            int_buffer[int_index++] = '0' + (temp % 10);
            temp = temp / 10;
        }
        for (int i = int_index - 1; i >= 0; --i) {
            result += int_buffer[i];
        }
    }

    // Parte decimal
    if (precision > 0) {
        result += ".";
        frac_part *= pow10;
        int frac_int = static_cast<int>(frac_part + 0.5f); // arredonda

        char frac_buffer[16];
        int frac_index = 0;
        if (frac_int == 0) {
            for (int i = 0; i < precision; ++i) result += "0";
        }
        else {
            // Garante zeros à esquerda
            int temp = frac_int;
            while (frac_index < precision) {
                frac_buffer[frac_index++] = '0' + (temp % 10);
                temp = temp / 10;
            }
            // Escreve parte decimal na ordem correta
            for (int i = frac_index - 1; i >= 0; --i) {
                result += frac_buffer[i];
            }
        }

        // Remove zeros à direita na parte decimal
        int last = result.size() - 1;
        while (last > 0 && result[last] == '0') {
            --last;
        }
        if (result[last] == '.') --last; // remove ponto final se necessário
        result = result.substr(0, last + 1);
    }
    return result;
}

Menu& Menu::slider(const std::string& text, float& value, float min, float max, float step, int delay_ms, void(*callback)(float)) {
    drawn_options++;
    float scroll_pos = CalculateScrollPosition(drawn_options);
    if (scroll_pos <= 0.0f) return *this;

    vec2_t textScale = { 0.0f, 0.35f };
    vec4_t textColor = { 1, 1, 1, 1 };

    UI::Renderer::DrawRectangle({ submenu_bounds.x, submenu_bounds.y, submenu_bounds.width, 34 }, drawn_options == current_option ? Scorpion.scrollbar : Scorpion.backgroundColor, UI::AnchorPoint::TopLeft);

    UI::Renderer::DrawText(text, 0, { submenu_bounds.x + 5, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Left);


    UI::Renderer::DrawText(to_string_precise(value, 5), 0, { submenu_bounds.x + submenu_bounds.width - 32, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Center);



    if (IsInputReady() && current_option == drawn_options) {
        if (value < min) value = min;
        if (IsButtonDown(BUTTON_DPAD_LEFT)) {
            value = std::max(min, value - step);
            if (callback) callback(value);
            SetInputDelay(delay_ms);
        }
        else if (IsButtonDown(BUTTON_DPAD_RIGHT)) {
            value = std::min(max, value + step);
            if (callback) callback(value);
            SetInputDelay(delay_ms);
        }
    }

    submenu_bounds.y += 36;
    return *this;
}


Menu& Menu::slider(const std::string& text, int& value, int min, int max, int step, int delay_ms, void(*callback)(int)) {
    drawn_options++;
    float scroll_pos = CalculateScrollPosition(drawn_options);
    if (scroll_pos <= 0.0f) return *this;

    vec2_t textScale = { 0.0f, 0.35f };
    vec4_t textColor = { 1, 1, 1, 1 };

    UI::Renderer::DrawRectangle({ submenu_bounds.x, submenu_bounds.y, submenu_bounds.width, 34 }, drawn_options == current_option ? Scorpion.scrollbar : Scorpion.backgroundColor, UI::AnchorPoint::TopLeft);

    UI::Renderer::DrawText(text, 0, { submenu_bounds.x + 5, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Left);


    UI::Renderer::DrawText(va("%i", value), 0, { submenu_bounds.x + submenu_bounds.width - 32, submenu_bounds.y }, textScale, textColor, UI::TextAlign::Center);

    

    if (IsInputReady() && current_option == drawn_options) {
        if (value < min) value = min;
        if (IsButtonDown(BUTTON_DPAD_LEFT)) {
            value = std::max(min, value - step);
            if (callback) callback(value);
            SetInputDelay(delay_ms);
        }
        else if (IsButtonDown(BUTTON_DPAD_RIGHT)) {
            value = std::min(max, value + step);
            if (callback) callback(value);
            SetInputDelay(delay_ms);
        }
    }

    submenu_bounds.y += 36;
    return *this;
}
