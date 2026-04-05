#pragma once

#include "Environment.hpp"

namespace Velyra::Core::Test {

    enum class COLOR {
        RED,
        GREEN,
        BLUE,
    };

    template<typename T>
    std::vector<T> createColor(const COLOR color) {
        if constexpr (std::is_same_v<float, T>) {
           switch (color) {
                case COLOR::RED:    return {1.0f, 0.0f, 0.0f, 1.0f};
                case COLOR::GREEN:  return {0.0f, 1.0f, 0.0f, 1.0f};
                case COLOR::BLUE:   return {0.0f, 0.0f, 1.0f, 1.0f};
            }
        }
        else if constexpr (std::is_same_v<U8, T>) {
            switch (color) {
                case COLOR::RED:    return {255, 0, 0, 255};
                case COLOR::GREEN:  return {0, 255, 0, 255};
                case COLOR::BLUE:   return {0, 0, 255, 255};
            }
        }
        return {};
    }

    template<typename T, int CHANNEL_COUNT>
    VL_TEXTURE_FORMAT getFormat() {
        if constexpr (std::is_same_v<float, T>) {
            switch (CHANNEL_COUNT) {
                case 4: return VL_TEXTURE_RGBA_F32;
                case 3: return VL_TEXTURE_RGB_F32;
                case 2: return VL_TEXTURE_RG_F32;
                case 1: return VL_TEXTURE_R_F32;
                default: return VL_TEXTURE_FORMAT_MAX_VALUE;
            }
        }
        else if constexpr (std::is_same_v<U8, T>) {
            switch (CHANNEL_COUNT) {
                case 4: return VL_TEXTURE_RGBA_U8;
                case 3: return VL_TEXTURE_RGB_U8;
                case 2: return VL_TEXTURE_RG_U8;
                case 1: return VL_TEXTURE_R_U8;
                default: return VL_TEXTURE_FORMAT_MAX_VALUE;
            }
        }
        VL_NOT_IMPLEMENTED();
    }

    template<typename T>
    std::vector<T> createSquareRGBA(const Size width, const Size height, const Size channelCount, const COLOR color) {
        std::vector<T> texture(width * height * channelCount);
        auto colorData = createColor<T>(color);
        for (size_t i = 0; i < texture.size(); i += channelCount) {
            for (size_t j = 0; j < channelCount; ++j) {
                texture[i + j] = colorData[j];
            }
        }
        return texture;
    }

}