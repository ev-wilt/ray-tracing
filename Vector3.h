//
// Created by Evan on 6/16/2019.
//

#ifndef RAYTRACING_VECTOR3_H
#define RAYTRACING_VECTOR3_H

#include <math.h>
#include <iosfwd>

class Vector3 {
public:
    Vector3() {}
    Vector3(float val0, float val1, float val2) { vals[0] = val0; vals[1] = val1; vals[2] = val2; }
    inline float x() const { return vals[0]; }
    inline float y() const { return vals[1]; }
    inline float z() const { return vals[2]; }
    inline float r() const { return vals[0]; }
    inline float g() const { return vals[1]; }
    inline float b() const { return vals[2]; }

    inline const Vector3& operator+() const { return *this; }
    inline Vector3 operator-() const { return  Vector3(-vals[0], -vals[1], -vals[2]); }
    inline float operator[](int i) const { return vals[i]; }
    inline float& operator[](int i) { return vals[i]; }

    inline Vector3& operator+=(const Vector3 &vec);
    inline Vector3& operator-=(const Vector3 &vec);
    inline Vector3& operator*=(const Vector3 &vec);
    inline Vector3& operator/=(const Vector3 &vec);
    inline Vector3& operator*=(const float multiplier);
    inline Vector3& operator/=(const float divisor);

    inline float length() const {
        return sqrt(pow(vals[0], 2) * pow(vals[1], 2) * pow(vals[2], 2));
    }
    inline float squared_length() const {
        return pow(vals[0], 2) * pow(vals[1], 2) * pow(vals[2], 2);
    }
    inline void make_unit_vector() {
        float mag = 1.0 / sqrt(pow(vals[0], 2) * pow(vals[1], 2) * pow(vals[2], 2));
        vals[0] *= mag; vals[1] *= mag; vals[2] *= mag;
    }

    float vals[3];


};

inline std::istream& operator>>(std::istream &stream, Vector3 &vec) {
    stream >> vec.vals[0] >> vec.vals[1] >> vec.vals[2];
    return stream;
}

inline std::ostream & operator>>(std::ostream &stream, Vector3 &vec) {
    stream << vec.vals[0] << ", " << vec.vals[1] << ", " << vec.vals[2];
    return stream;
}

inline Vector3 operator+(const Vector3 &vec1, const Vector3 &vec2) {
    return Vector3(vec1.vals[0] + vec2.vals[0], vec1.vals[1] + vec2.vals[1], vec1.vals[2] + vec2.vals[2]);
}

inline Vector3 operator-(const Vector3 &vec1, const Vector3 &vec2) {
    return Vector3(vec1.vals[0] - vec2.vals[0], vec1.vals[1] - vec2.vals[1], vec1.vals[2] - vec2.vals[2]);
}

inline Vector3 operator*(const Vector3 &vec1, const Vector3 &vec2) {
    return Vector3(vec1.vals[0] * vec2.vals[0], vec1.vals[1] * vec2.vals[1], vec1.vals[2] * vec2.vals[2]);
}

inline Vector3 operator/(const Vector3 &vec1, const Vector3 &vec2) {
    return Vector3(vec1.vals[0] / vec2.vals[0], vec1.vals[1] / vec2.vals[1], vec1.vals[2] / vec2.vals[2]);
}

inline Vector3 operator*(float scale, const Vector3 &vec) {
    return Vector3(scale * vec.vals[0], scale * vec.vals[1], scale * vec.vals[2]);
}

inline Vector3 operator/(const Vector3 &vec, float scale) {
    return Vector3(vec.vals[0] / scale, vec.vals[1] / scale, vec.vals[2] / scale);
}

inline Vector3 operator*(const Vector3 &vec, float scale) {
    return Vector3(scale * vec.vals[0], scale * vec.vals[1], scale * vec.vals[2]);
}

inline float dot_product(const Vector3 &vec1, const Vector3 &vec2) {
    return vec1.vals[0] * vec2.vals[0] + vec1.vals[1] * vec2.vals[1] + vec1.vals[2] * vec2.vals[2];
}

inline Vector3 cross_product(const Vector3 &vec1, const Vector3 &vec2) {
    return Vector3( (vec1.vals[1] * vec2.vals[2] - vec1.vals[2] * vec2.vals[1]),
                    (-(vec1.vals[0] * vec2.vals[2] - vec1.vals[2] * vec2.vals[0])),
                    (vec1.vals[0] * vec2.vals[1] - vec1.vals[1] * vec2.vals[0]));
}

inline Vector3& Vector3::operator+=(const Vector3 &vec) {
    vals[0] += vec.vals[0];
    vals[1] += vec.vals[1];
    vals[2] += vec.vals[2];
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3 &vec) {
    vals[0] -= vec.vals[0];
    vals[1] -= vec.vals[1];
    vals[2] -= vec.vals[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const Vector3 &vec) {
    vals[0] *= vec.vals[0];
    vals[1] *= vec.vals[1];
    vals[2] *= vec.vals[2];
    return *this;
}

inline Vector3& Vector3::operator/=(const Vector3 &vec) {
    vals[0] /= vec.vals[0];
    vals[1] /= vec.vals[1];
    vals[2] /= vec.vals[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const float multiplier) {
    vals[0] *= multiplier;
    vals[1] *= multiplier;
    vals[2] *= multiplier;
    return *this;
}

inline Vector3& Vector3::operator/=(const float multiplier) {
    float x = 1.0 / multiplier;
    vals[0] *= x;
    vals[1] *= x;
    vals[2] *= x;
    return *this;
}

inline Vector3 unit_vector(Vector3 vec) {
    return vec / vec.length();
}

#endif //RAYTRACING_VECTOR3_H
