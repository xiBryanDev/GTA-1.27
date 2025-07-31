#include "graphics.h"
#include "native.h"

namespace UI
{
    constexpr float SCREEN_WIDTH = 1280.0f;
    constexpr float SCREEN_HEIGHT = 1080.0f;

    void Renderer::AdjustPositionForAnchor(float& x, float& y, float width, float height, AnchorPoint anchor) {
        int anchorValue = static_cast<int>(anchor);
        // Horizontal
        if ((anchorValue & 0x03) == static_cast<int>(AnchorPoint::CENTER)) {
            x -= width * 0.5f;
        }
        else if ((anchorValue & 0x03) == static_cast<int>(AnchorPoint::RIGHT)) {
            x -= width;
        }
        // Vertical
        if ((anchorValue & 0x0C) == (static_cast<int>(AnchorPoint::CENTER) << 2)) {
            y -= height * 0.5f;
        }
        else if ((anchorValue & 0x0C) == static_cast<int>(AnchorPoint::BOTTOM)) {
            y -= height;
        }
    }

    void Renderer::DrawRectangle(const Rectangle& rect, const vec4_t& color, AnchorPoint anchor) {
        if (color.a <= 0.f) return;

        float x = rect.x, y = rect.y, width = rect.width, height = rect.height;
        if (anchor != AnchorPoint::TopLeft) AdjustPositionForAnchor(x, y, width, height, anchor);

        // Para GTA/FiveM: centro do retângulo em unidades relativas
        float relX = (x + width / 2.f) / SCREEN_WIDTH;
        float relY = (y + height / 2.f) / SCREEN_HEIGHT;
        float relWidth = width / SCREEN_WIDTH;
        float relHeight = height / SCREEN_HEIGHT;

        int r = static_cast<int>(color.r * 255.f);
        int g = static_cast<int>(color.g * 255.f);
        int b = static_cast<int>(color.b * 255.f);
        int a = static_cast<int>(color.a * 255.f);

        invoke<void*>(0xDD2BFC77, relX, relY, relWidth, relHeight, r, g, b, a);
    }

    void Renderer::DrawBorderedRectangle(const Rectangle& rect, float borderSize, const vec4_t& borderColor, const vec4_t& backgroundColor, AnchorPoint anchor, bool hasBackground) {
        float x = rect.x, y = rect.y, width = rect.width, height = rect.height;
        if (anchor != AnchorPoint::TopLeft) AdjustPositionForAnchor(x, y, width, height, anchor);

        // Bordas
        DrawRectangle(Rectangle(x, y, borderSize, height), borderColor); // Left
        DrawRectangle(Rectangle(x + width - borderSize, y, borderSize, height), borderColor); // Right
        DrawRectangle(Rectangle(x, y, width, borderSize), borderColor); // Top
        DrawRectangle(Rectangle(x, y + height - borderSize, width, borderSize), borderColor); // Bottom

        // Fundo
        if (hasBackground && backgroundColor.a > 0.f) {
            DrawRectangle(Rectangle(x + borderSize, y + borderSize, width - 2.f * borderSize, height - 2.f * borderSize), backgroundColor);
        }
    }

    // Função utilitária para calcular a largura do texto (opcional)
    float GetTextWidth(const char* text, int font, float scaleX, float scaleY) {
        UI::SET_TEXT_FONT(font);
        UI::SET_TEXT_SCALE(scaleX, scaleY);
        UI::_SET_TEXT_ENTRY("STRING");
        UI::_ADD_TEXT_COMPONENT_STRING(text);
        return UI::_END_TEXT_COMMAND_GET_WIDTH(true);
    }

    float Renderer::DrawText(const std::string& text, int font, const Rectangle& pos, vec2_t scale, const vec4_t& color, TextAlign align, bool computeTextWidth)
    {
        float x = pos.x;
        float y = pos.y;

        // Converte para relativo se precisar
        float relX = (x > 1.0f) ? x / SCREEN_WIDTH : x;
        float relY = (y > 1.0f) ? y / SCREEN_HEIGHT : y;

        int r = static_cast<int>(color.r * 255.f);
        int g = static_cast<int>(color.g * 255.f);
        int b = static_cast<int>(color.b * 255.f);
        int a = static_cast<int>(color.a * 255.f);

        float retWidth = computeTextWidth ? GetTextWidth(text.c_str(), font, scale.x, scale.y) : 0.0f;

        UI::SET_TEXT_FONT(font);
        UI::SET_TEXT_SCALE(scale.x, scale.y);
        UI::SET_TEXT_COLOUR(r, g, b, a);
        UI::SET_TEXT_WRAP(0.0f, 1.0f);
        UI::SET_TEXT_DROPSHADOW(5, 5, 5, 0, 80);
       UI::SET_TEXT_EDGE(2, 255, 255, 255, 150);


        switch (align) {
        case TextAlign::Center:
            UI::SET_TEXT_CENTRE(true);
            break;
        case TextAlign::Right:
            UI::SET_TEXT_RIGHT_JUSTIFY(true);
            UI::SET_TEXT_WRAP(0.0f, relX);
            break;
        case TextAlign::Left:
        default:
            break;
        }
        UI::SET_TEXT_OUTLINE();
        UI::_SET_TEXT_ENTRY("STRING");
        UI::_ADD_TEXT_COMPONENT_STRING(text.c_str());
        UI::_DRAW_TEXT(relX, relY);

        return retWidth;
    }

    void Renderer::DrawSprite(std::string streamedTexture, std::string textureName,const Rectangle& rect,float rotation,const vec4_t& color, AnchorPoint anchor)
    {
        float x = rect.x;
        float y = rect.y;
        float width = rect.width;
        float height = rect.height;

        if (anchor != AnchorPoint::TopLeft)
            AdjustPositionForAnchor(x, y, width, height, anchor);

        float relX = (x + width / 2.f) / SCREEN_WIDTH;
        float relY = (y + height / 2.f) / SCREEN_HEIGHT;
        float relWidth = width / SCREEN_WIDTH;
        float relHeight = height / SCREEN_HEIGHT;

        // Solicita o dicionário de textura, se necessário
        GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(streamedTexture.data(), false);
        if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(streamedTexture.data())) {
            GRAPHICS::DRAW_SPRITE(
                streamedTexture.data(),
                textureName.data(),
                relX, relY,
                relWidth, relHeight,
                rotation,
                static_cast<int>(color.r * 255.f),
                static_cast<int>(color.g * 255.f),
                static_cast<int>(color.b * 255.f),
                static_cast<int>(color.a * 255.f)
            );
        }
    }

}