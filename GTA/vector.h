#pragma once

//#include "stdafx.h"


namespace math {

    // Classe vec2_t
    class vec2_t {
    public:
        union {
            struct { float x, y; };
            struct { float u, v; };
            float data[2];
        };

        vec2_t(float x = 0.0f, float y = 0.0f);
        float dot(const vec2_t& other) const;
        float length() const;
        vec2_t normalize() const;

        vec2_t operator+(const vec2_t& other) const;
        vec2_t operator-(const vec2_t& other) const;
        vec2_t operator*(float scalar) const;
        vec2_t operator/(float scalar) const;

        float& operator[](int index);
        const float& operator[](int index) const;

        bool operator==(const vec2_t& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Classe vec3_t
    class vec3_t {
    public:
        union {
            struct { float x, y, z; };
            struct { float r, g, b; };
            float data[3];
        };

        vec3_t(float x = 0.0f, float y = 0.0f, float z = 0.0f);
        float dot(const vec3_t& other) const;
        vec3_t cross(const vec3_t& other) const;
        float length() const;
        vec3_t normalize() const;

        vec3_t operator+(const vec3_t& other) const;
        vec3_t operator-(const vec3_t& other) const;
        vec3_t operator*(float scalar) const;
        vec3_t operator/(float scalar) const;

        void operator-=(const vec3_t vec);
        void operator+=(const vec3_t vec);
        void operator*=(const vec3_t vec);
        void operator*=(const float scalar);

        bool operator==(const vec3_t vec);
        bool operator==(const float value);
        bool operator!=(const vec3_t vec);
        bool operator!=(const float value);

        float& operator[](int index);
        const float& operator[](int index) const;
        float distance(vec3_t vec);
        // Adicionado operador ==
        bool operator==(const vec3_t& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
    // Classe vec4_t
    class vec4_t {
    public:
        union {
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
            float data[4];
        };

        vec4_t(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
        vec4_t(const vec3_t& xyz, float w = 1.0f);
        float dot(const vec4_t& other) const;
        float length() const;
        vec4_t normalize() const;

        vec3_t xyz() const;
        vec2_t xy() const;

        vec4_t operator+(const vec4_t& other) const;
        vec4_t operator-(const vec4_t& other) const;
        vec4_t operator*(float scalar) const;
        vec4_t operator/(float scalar) const;

        float& operator[](int index);
        const float& operator[](int index) const;
    };

}

float dot_product(math::vec3_t lhs, math::vec3_t rhs);
math::vec3_t angles_forward(const math::vec3_t& Origin, const math::vec3_t& Angles, float diff);
double atan22(double x, int n);
void angle_vectors(math::vec3_t angles, math::vec3_t* forward, math::vec3_t* right, math::vec3_t* up);
float vector_normalize(math::vec3_t* direction);
float calculate_distance(math::vec3_t end, math::vec3_t start);
float degrees_to_radians(float a);