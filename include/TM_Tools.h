/**
 * @file TM_Tools.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */

#ifndef _TM_TOOLS_H
#define _TM_TOOLS_H

#include <variant>
#include <json.hpp>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>

/*
 * Types definitions and tools to use them inside TailorMade
 * These functions have to be update for each new type.
 */
#include <cctype>

// Types and operators
/// Vector2 structure and operators.
typedef struct Vector2 { float x; float y; 

    /// Addition
    Vector2 operator+(const Vector2 v2) const {
        return { x + v2.x, y + v2.y };
    }

    /// Soustraction
    Vector2 operator-(const Vector2 v2) const {
        return { x - v2.x, y - v2.y };
    }

    /// Dot product
    float operator*(const Vector2 v2) const {
        return x * v2.x + y * v2.y;
    }

    /// Scalar product
    Vector2 operator*=(const float s) const {
        return { x * s, y * s };
    }

    /// Scalar division
    Vector2 operator/=(const float s) const {
        return { x / s, y / s };
    }

    /// Norm
    float operator!() const {
        return sqrt(x * x + y * y);
    }

    /// Normalization
    Vector2 operator~() const {
        float norm = !(*this);
        return { x / norm, y / norm };
    }

    /// Angle
    float operator%(const Vector2 v2) const {
        float norm1 = !(*this);
        float norm2 = !v2;
        return acos(((*this) * v2) / (norm1 * norm2));
    }

    /// Projection
    Vector2 operator>>(const Vector2 v2) const {
        float norm2 = !v2;
        norm2 *= norm2; // Squared the norm
        float dotProd = (*this) * v2;
        return (*this) *= (dotProd / norm2);
    }
} Vector2;

/// Vector3 structure and operators.
typedef struct Vector3 { float x; float y; float z; 

    /// Addition
    Vector3 operator+(const Vector3 v2) const {
        return { x + v2.x, y + v2.y, z + v2.z };
    }

    /// Soustraction
    Vector3 operator-(const Vector3 v2) const {
        return { x - v2.x, y - v2.y, z - v2.z };
    }

    /// Dot product
    float operator*(const Vector3 v2) const {
        return x * v2.x + y * v2.y + z * v2.z;
    }

    /// Scalar product
    Vector3 operator*=(const float s) const {
        return { x * s, y * s, z * s };
    }
    
    /// Scalar division
    Vector3 operator/=(const float s) const {
        return { x / s, y / s, z / s };
    }

    /// Cross product
    Vector3 operator^(const Vector3 v2) const {
        return { y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x };
    }

    /// Norm
    float operator!() const {
        return sqrt(x * x + y * y + z * z);
    }

    /// Normalization
    Vector3 operator~() const {
        float norm = !(*this);
        return { x / norm, y / norm, z / norm };
    }

    /// Angle
    float operator%(const Vector3 v2) const {
        float norm1 = !(*this);
        float norm2 = !v2;
        return acos(((*this) * v2) / (norm1 * norm2));
    }

    /// Projection
    Vector3 operator>>(const Vector3 v2) const {
        float norm2 = !v2;
        norm2 *= norm2; // Squared the norm
        float dotProd = (*this) * v2;
        return (*this) *= (dotProd / norm2);
    }
} Vector3;

/// Print a Vector2.
inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "{ x: " << v.x << ", y: " << v.y << "}";
    return os;
}

/// Print a Vector3.
inline std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "{ x: " << v.x << ", y: " << v.y << ", z: " << v.z << "}";
    return os;
}

/// Definitions of the types in TailorMade.
#define ECS_Types int, float, std::string, bool, Vector2, Vector3

/// Map of dataName -> {dataType, value}
using dataUnMap = std::unordered_map<std::string, std::pair<std::string, std::variant<ECS_Types>>>;

/// Vector of {dataType, value}.
using dataVector = std::vector<std::pair<std::string, std::variant<ECS_Types>>>;

/**
 * @brief Return a variant<ECS_Types> with the default value of the given type.
 * @details You can give a string of one of the supported types of TailorMade and it will return the associated variant with its default value.
 * @warning If the type is unknown an error is thrown. You can checked the currently available types of this version on the project's github page.
 * @param type The string version of the desired type. (e.g.: "Integer")
 */
std::variant<ECS_Types> strToType(std::string type);

/**
 * @brief Return a variant<ECS_Types> with the given value of the given type.
 * @details Works the same way as TM_Tools::strToType but with the given value.
 * @warning If the type is unknown an error is thrown. You can checked the currently available types of this version on the project's github page.
 * @param value A JSON object with the desired value in it.
 * @param type The string version of the desired type. (e.g.: "Integer")
 */
std::variant<ECS_Types> valueToType(const nlohmann::json& value, std::string type);

/**
 * @brief Serialize the data in an ordered_json.
 * @see dataUnMap
 * @param data The unordered_map with the data from a component in it.
 */
nlohmann::ordered_json serializeType(const dataUnMap& data);

/**
 * @brief Will append in the given stream, the serialized version of the given variant value.
 * @param stream The stream on which the value should be append.
 * @param value The variant you want to serialize and append to the stream.
 */
void valueToStream(std::ostream& stream, std::variant<ECS_Types> value);

/**
 * @brief Return every files from the given directory, includes all subfolders.
 * @param directory The directory in which the research starts.
 */
std::vector<std::string> getAllFilesFromDirectory(const std::string& directory);

inline std::variant<ECS_Types> strToType(std::string type) {
    // This function return the default value for the given type.
    // We authorize the first letter to be an upper or lower case, the others must be lower case exclusively.
    // There is the common variations for most of the types.

    type[0] = tolower(static_cast<unsigned char>(type[0]));

    if (type == "integer" || type == "int") {
        return 0;
    }
    else if (type == "float") {
        return 0.0f;
    }
    else if (type == "string" || type == "str") {
        return "";
    }
    else if (type == "boolean" || type == "bool") {
        return false;
    }
    else if (type == "vector2") {
        return Vector2({ 0.0f, 0.0f });
    }
    else if (type == "vector3") {
        return Vector3({ 0.0f, 0.0f, 0.0f });
    }

    throw std::runtime_error("Error : invalid type \"" + type + "\".");
}

inline std::variant<ECS_Types> valueToType(const nlohmann::json& value, std::string type) {
    // This function return the variant from the given type with the given value.
    type[0] = tolower(static_cast<unsigned char>(type[0]));

    if (type == "integer" || type == "int") {
        return value.get<int>();
    }
    else if (type == "float") {
        return value.get<float>();
    }
    else if (type == "string" || type == "str") {
        return value.get<std::string>();
    }
    else if (type == "boolean" || type == "bool") {
        return value.get<bool>();
    }
    else if (type == "vector2") {
        return Vector2{ value[0].get<float>(), value[1].get<float>() };
    }
    else if (type == "vector3") {
        return Vector3{ value[0].get<float>(), value[1].get<float>(), value[2].get<float>() };
    }

    throw std::runtime_error("Error : invalid type \"" + type + "\".");
}

inline nlohmann::ordered_json serializeType(const dataUnMap& dataMap) {
    nlohmann::ordered_json dict = nlohmann::ordered_json::object();

    for (const auto& [key, valuePair] : dataMap) {
        const auto& data = valuePair.second;

        // Fast way to visit all the possibility.
        std::visit([&](auto&& val) {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, int> ||
                std::is_same_v<T, float> ||
                std::is_same_v<T, std::string> ||
                std::is_same_v<T, bool>) {
                dict[key] = val; // int, float, string, bool
            }
            else if constexpr (std::is_same_v<T, Vector2>) {
                dict[key] = { val.x, val.y };
            }
            else if constexpr (std::is_same_v<T, Vector3>) {
                dict[key] = { val.x, val.y, val.z };
            }
        }, data);
    }
    return dict;
}

inline void valueToStream(std::ostream& stream, std::variant<ECS_Types> value) {
    // Fast way to add to stream.
    std::visit([&](auto&& val) {
            stream << val;
    }, value);
}

inline std::vector<std::string> getAllFilesFromDirectory(const std::string& directory) {
    std::vector<std::string> result;

    try {
        for (const auto& file : std::filesystem::recursive_directory_iterator(directory)) {
            if (std::filesystem::is_regular_file(file.path())) { // Only the files
                result.push_back(file.path().string());
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return result;
}

#endif //_TM_TOOLS_H