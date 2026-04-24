#pragma once

#include <VelyraCore/VelyraCore.hpp>
#include <string>

namespace Velyra::Core::Test {

    template<VL_GRAPHICS_API api>
    std::string getSimpleVertexShader() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;
layout (location = 17) out vec3 fNormal;
layout (location = 18) out vec3 fTangent;
layout (location = 19) out vec2 fTexCoord;

void main(){
    gl_Position = vec4(lPosition, 1.0);
    fColor = vec4(lColor, 1.0);
    fNormal = lNormal;
    fTangent = lTangent;
    fTexCoord = lTexCoord;
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getSimpleFragmentShader() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec3 fTangent;
layout (location = 19) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = fColor;
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getFragmentShaderWithError() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec3 fTangent;
layout (location = 19) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = fColor // Missing semicolon here to induce syntax error
)";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getSimpleFragmentShaderInterfaceMisMatch() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec3 fTangent;
layout (location = 20) in vec2 fTexCoord; // location mismatch

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = fColor;
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getScreenVertexShader1CA() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;

layout (location = 16) out vec3 fColor;

void main(){
    gl_Position = vec4(lPosition, 1.0);
    fColor = lColor;
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getScreenFragmentShader1CA() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 16) in vec3 fColor;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = vec4(fColor, 1.0);
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
std::string getScreenVertexShader2CA() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor1;
layout (location = 2) in vec3 lColor2;

layout (location = 16) out vec3 fColor1;
layout (location = 17) out vec3 fColor2;

void main(){
    gl_Position = vec4(lPosition, 1.0);
    fColor1 = lColor1;
    fColor2 = lColor2;
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }

    template<VL_GRAPHICS_API api>
    std::string getScreenFragmentShader2CA() {
        if constexpr (api == VL_API_OPENGL) {
            return R"(
#version 460

layout (location = 16) in vec3 fColor1;
layout (location = 17) in vec3 fColor2;

layout (location = 0) out vec4 fragColor1;
layout (location = 1) out vec4 fragColor2;

void main(){
    fragColor1 = vec4(fColor1, 1.0);
    fragColor2 = vec4(fColor2, 1.0);
})";
        }
        else {
            VL_NOT_IMPLEMENTED();
        }
    }
}

