#include "vector.h"
#include <fastmath.h>

namespace math {

    // Implementações de vec2_t
    vec2_t::vec2_t(float x, float y) : x(x), y(y) {}

    float vec2_t::dot(const vec2_t& other) const {
        return x * other.x + y * other.y;
    }

    float vec2_t::length() const {
        return sqrtf(x * x + y * y);
    }

    vec2_t vec2_t::normalize() const {
        float len = length();
        return (len != 0.0f) ? (*this / len) : *this;
    }

    vec2_t vec2_t::operator+(const vec2_t& other) const {
        return vec2_t(x + other.x, y + other.y);
    }

    vec2_t vec2_t::operator-(const vec2_t& other) const {
        return vec2_t(x - other.x, y - other.y);
    }

    vec2_t vec2_t::operator*(float scalar) const {
        return vec2_t(x * scalar, y * scalar);
    }

    vec2_t vec2_t::operator/(float scalar) const {
        return vec2_t(x / scalar, y / scalar);
    }

    float& vec2_t::operator[](int index) {
        return data[index];
    }

    const float& vec2_t::operator[](int index) const {
        return data[index];
    }

    // Implementações de vec3_t
    vec3_t::vec3_t(float x, float y, float z) : x(x), y(y), z(z) {}

    float vec3_t::dot(const vec3_t& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3_t vec3_t::cross(const vec3_t& other) const {
        return vec3_t(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float vec3_t::length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    vec3_t vec3_t::normalize() const {
        float len = length();
        return (len != 0.0f) ? (*this / len) : *this;
    }

    vec3_t vec3_t::operator+(const vec3_t& other) const {
        return vec3_t(x + other.x, y + other.y, z + other.z);
    }

    vec3_t vec3_t::operator-(const vec3_t& other) const {
        return vec3_t(x - other.x, y - other.y, z - other.z);
    }

    vec3_t vec3_t::operator*(float scalar) const {
        return vec3_t(x * scalar, y * scalar, z * scalar);
    }

    vec3_t vec3_t::operator/(float scalar) const {
        return vec3_t(x / scalar, y / scalar, z / scalar);
    }

    float& vec3_t::operator[](int index) {
        return data[index];
    }

    const float& vec3_t::operator[](int index) const {
        return data[index];
    }
    void vec3_t::operator-=(const vec3_t vec)
    {
        this->x -= vec.x;
        this->y -= vec.y;
        this->z -= vec.z;
    }

    void vec3_t::operator+=(const vec3_t vec)
    {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
    }

    void vec3_t::operator*=(const vec3_t vec)
    {
        this->x *= vec.x;
        this->y *= vec.y;
        this->z *= vec.z;
    }

    void vec3_t::operator*=(const float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
    }

    bool vec3_t::operator==(const vec3_t vec)
    {
        return (this->x == vec.x && this->y == vec.y && this->z == vec.z);
    }

    bool vec3_t::operator==(const float value)
    {
        return (this->x == value && this->y == value && this->z == value);
    }

    bool vec3_t::operator!=(const vec3_t vec)
    {
        return !operator==(vec);
    }

    bool vec3_t::operator!=(const float value)
    {
        return !operator==(value);
    }

    float vec3_t::distance(vec3_t vec)
    {
        float _x = this->x - vec.x;
        float _y = this->y - vec.y;
        float _z = this->z - vec.z;
        return f_sqrtf((_x * _x) + (_y * _y) + (_z * _z));
    }

    // Implementações de vec4_t
    vec4_t::vec4_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    vec4_t::vec4_t(const vec3_t& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

    float vec4_t::dot(const vec4_t& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float vec4_t::length() const {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    vec4_t vec4_t::normalize() const {
        float len = length();
        return (len != 0.0f) ? (*this / len) : *this;
    }

    vec3_t vec4_t::xyz() const {
        return vec3_t(x, y, z);
    }

    vec2_t vec4_t::xy() const {
        return vec2_t(x, y);
    }

    vec4_t vec4_t::operator+(const vec4_t& other) const {
        return vec4_t(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4_t vec4_t::operator-(const vec4_t& other) const {
        return vec4_t(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vec4_t vec4_t::operator*(float scalar) const {
        return vec4_t(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    vec4_t vec4_t::operator/(float scalar) const {
        return vec4_t(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    float& vec4_t::operator[](int index) {
        return data[index];
    }

    const float& vec4_t::operator[](int index) const {
        return data[index];
    }

} // namespace math


float dot_product(math::vec3_t lhs, math::vec3_t rhs)
{
    return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
}

math::vec3_t angles_forward(const math::vec3_t& Origin, const math::vec3_t& Angles, float diff) {
    const float degToRad = M_PI / 180.0f;

    // Converter ângulos de graus para radianos
    float yaw = Angles.y * degToRad;
    float pitch = Angles.x * degToRad;
    float roll = Angles.z * degToRad;

    // Calcular seno e cosseno dos ângulos
    float sy = sinf(yaw);
    float cy = cosf(yaw);
    float sp = sinf(pitch);
    float cp = cosf(pitch);
    float sr = sinf(roll);
    float cr = cosf(roll);

    // Calcular a direção para frente usando os ângulos
    float forwardX = (cp * cy * diff) + Origin.x;
    float forwardY = (cp * sy * diff) + Origin.y;
    float forwardZ = (-sp * diff) + Origin.z;

    return  math::vec3_t(forwardX, forwardY, forwardZ);
}

#define PI (3.1415926535897931)

double atan22(double x, int n)
{
    double a = 0.0;// 1st term
    double sum = 0.0;

    // special cases
    if (x == 1.0) return PI / 4.0;
    if (x == -1.0) return -PI / 4.0;

    if (n > 0)
    {
        if ((x < -1.0) || (x > 1.0))
        {
            // constant term
            if (x > 1.0)
                sum = PI / 2.0;
            else
                sum = -PI / 2.0;
            // initial value of a
            a = -1.0 / x;
            for (int j = 1; j <= n; j++)
            {
                sum += a;
                a *= -1.0 * (2.0 * j - 1) / ((2.0 * j + 1) * x * x);// next term from last
            }
        }
        else// -1 < x < 1
        {
            // constant term
            sum = 0.0;
            // initial value of a
            a = x;
            for (int j = 1; j <= n; j++)
            {
                sum += a;
                a *= -1.0 * (2.0 * j - 1) * x * x / (2.0 * j + 1);// next term from last
            }
        }
        //r_err = a;// max. error = 1st term not taken for alternating series
    }

    return sum;
}


float degrees_to_radians(float a) { return a * (static_cast<float>(M_PI) / 180.0f); }


void angle_vectors(math::vec3_t angles, math::vec3_t* forward, math::vec3_t* right, math::vec3_t* up)
{
    std::float_t angle, sin_pitch, sin_yaw, sin_roll, cos_pitch, cos_yaw, cos_roll;

    angle = degrees_to_radians(angles.x);
    sin_pitch = std::sinf(angle);
    cos_pitch = std::cosf(angle);

    angle = degrees_to_radians(angles.y);
    sin_yaw = std::sinf(angle);
    cos_yaw = std::cosf(angle);

    angle = degrees_to_radians(angles.z);
    sin_roll = std::sinf(angle);
    cos_roll = std::cosf(angle);

    if (forward)
    {
        forward->x = cos_pitch * cos_yaw;
        forward->y = cos_pitch * sin_yaw;
        forward->z = -sin_pitch;
    }

    if (right)
    {
        right->x = (-1.0f * sin_roll * sin_pitch * cos_yaw + -1.0f * cos_roll * -sin_yaw);
        right->y = (-1.0f * sin_roll * sin_pitch * sin_yaw + -1.0f * cos_roll * cos_yaw);
        right->z = -1.0f * sin_roll * cos_pitch;
    }

    if (up)
    {
        up->x = (cos_roll * sin_pitch * cos_yaw + -sin_roll * -sin_yaw);
        up->y = (cos_roll * sin_pitch * sin_yaw + -sin_roll * cos_yaw);
        up->z = cos_roll * cos_pitch;
    }
}



float dot_product(math::vec3_t vec)
{
    return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float vector_length(math::vec3_t vec)
{
    return __fsqrts(dot_product(vec));
}

float vector_normalize(math::vec3_t* direction)
{
    auto length = vector_length(*direction);
    auto ret_val = length;

    if (-length >= 0.0f)
        length = 1.0f;

    direction->x *= 1.0f / length;
    direction->y *= 1.0f / length;
    direction->z *= 1.0f / length;

    return ret_val;
}

float calculate_distance(math::vec3_t end, math::vec3_t start)
{
    auto dir = end - start;

    return vector_length(dir);
}