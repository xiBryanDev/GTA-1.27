#pragma once
#include "stdafx.h"

class Rectangle {
public:
    float x, y, width, height;

    Rectangle(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f)
        : x(x), y(y), width(width), height(height) {}

    void SetPosition(float newX, float newY) noexcept { x = newX; y = newY; }
    void SetSize(float newWidth, float newHeight) noexcept { width = newWidth; height = newHeight; }
    void Set(float newX, float newY, float newWidth, float newHeight) noexcept {
        x = newX; y = newY; width = newWidth; height = newHeight;
    }
};

namespace UI {


    enum class AnchorPoint {
        LEFT = 0x00,  // 0000 (horizontal: à esquerda)
        CENTER = 0x01,  // 0001 (horizontal ou vertical: centralizado)
        RIGHT = 0x02,  // 0010 (horizontal: à direita)
        TOP = 0x00,  // 0000 (vertical: no topo)
        BOTTOM = 0x08,  // 1000 (vertical: na base)

        // Presets para combinações comuns
        TopLeft = LEFT | TOP,      // 0000 | 0000 = 0x00
        TopCenter = CENTER | TOP,    // 0001 | 0000 = 0x01
        TopRight = RIGHT | TOP,     // 0010 | 0000 = 0x02

        CenterLeft = LEFT | CENTER,   // 0000 | 0100 = 0x04
        Center = CENTER | CENTER, // 0001 | 0100 = 0x05
        CenterRight = RIGHT | CENTER,  // 0010 | 0100 = 0x06

        BottomLeft = LEFT | BOTTOM,   // 0000 | 1000 = 0x08
        BottomCenter = CENTER | BOTTOM, // 0001 | 1000 = 0x09
        BottomRight = RIGHT | BOTTOM   // 0010 | 1000 = 0x0A

    };

    enum class TextAlign {
        Left,
        Center,
        Right
    };


    class Renderer {
    public:
        static void DrawRectangle(const Rectangle& rect, const vec4_t& color, AnchorPoint anchor = AnchorPoint::TopLeft);
        static void DrawBorderedRectangle(const Rectangle& rect, float borderSize, const vec4_t& borderColor, const vec4_t& backgroundColor, AnchorPoint anchor = AnchorPoint::TopLeft, bool hasBackground = false);
        static float DrawText(const std::string& text, int font, const Rectangle& pos, vec2_t scale, const vec4_t& color, TextAlign align = TextAlign::Left, bool computeTextWidth = false);
        static void DrawSprite(std::string streamedTexture, std::string textureName, const Rectangle& rect, float rotation, const vec4_t& color, AnchorPoint anchor = AnchorPoint::TopLeft);

    private:
        static void AdjustPositionForAnchor(float& x, float& y, float width, float height, AnchorPoint anchor);

    };


}// namespace Graphics