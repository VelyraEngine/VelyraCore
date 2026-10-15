#include "../../TestPch.hpp"

#include <VelyraCore/Context/Texture2D.hpp>
#include <VelyraCore/Context/Sampler.hpp>
#include <VelyraCore/Context/DepthStencilState.hpp>
#include <VelyraCore/Context/ConstantBuffer.hpp>
#include <VelyraCore/Context/StructuredBuffer.hpp>
#include <VelyraCore/Context/VertexLayout.hpp>
#include <VelyraCore/Context/VertexBuffer.hpp>
#include <VelyraCore/Context/MeshBinding.hpp>
#include <VelyraCore/Context/FrameBuffer.hpp>
#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include <VelyraCore/Context/ColorAttachment.hpp>
#include <VelyraCore/Context/DepthStencilAttachment.hpp>
#include <VelyraCore/Context/Device.hpp>
#include "../Environment.hpp"

#include <array>
#include <bit>
#include <numeric>

using namespace Velyra;
using namespace Velyra::Core;

/**
 * Checks the actual OpenGL state/errors produced by the wrapper. The fixture fails every test that leaves a GL error
 * behind, so each test implicitly asserts that the calls it makes are valid.
 */
class TestGLBehavior : public ::testing::Test {
protected:
    using Env = Environment<ApiWrapper<VL_API_OPENGL>>;

    void SetUp() override {
        Env::setup();
        // Other suites create their own windows and contexts
        context()->makeCurrent();
        while (glGetError() != GL_NO_ERROR) {}
    }

    void TearDown() override {
        EXPECT_EQ(glGetError(), static_cast<GLenum>(GL_NO_ERROR));
    }

    static const UP<Context>& context() {
        return Env::m_Window->getContext();
    }

    static View<Texture2D> createTexture(const Size width, const Size height, const VL_TEXTURE_FORMAT format,
                                         const void* data, const bool generateMipmap = false) {
        Texture2DDesc desc;
        desc.width = width;
        desc.height = height;
        desc.format = format;
        desc.data = data;
        desc.generateMipmap = generateMipmap;
        return context()->createTexture2D(desc);
    }

    template<typename T>
    static std::vector<T> toVector(const void* data, const Size count) {
        const auto* typed = static_cast<const T*>(data);
        return std::vector<T>(typed, typed + count);
    }

    template<typename T>
    static std::vector<T> readTexture(const U64 id, const int level, const GLenum format, const GLenum type, const Size count) {
        std::vector<T> data(count);
        glGetTextureImage(static_cast<GLuint>(id), level, format, type, static_cast<GLsizei>(count * sizeof(T)), data.data());
        return data;
    }

    static GLint intState(const GLenum name) {
        GLint value = 0;
        glGetIntegerv(name, &value);
        return value;
    }

    static GLint samplerParam(const U64 id, const GLenum name) {
        GLint value = 0;
        glGetSamplerParameteriv(static_cast<GLuint>(id), name, &value);
        return value;
    }

    static View<FrameBuffer> createFrameBuffer(const Size width, const Size height,
                                               const std::optional<FrameBufferColorAttachmentDesc>& colorAttachment,
                                               const std::optional<FrameBufferDepthStencilAttachmentDesc>& depthStencil) {
        auto layout = context()->createFrameBufferLayout();
        layout->setDimensions(width, height);
        if (colorAttachment.has_value()) {
            layout->addColorAttachment(colorAttachment.value());
        }
        if (depthStencil.has_value()) {
            layout->setDepthStencilAttachment(depthStencil.value());
        }
        return context()->createFrameBuffer(layout);
    }
};

/////////////////// Textures ///////////////////

TEST_F(TestGLBehavior, TextureWithUnalignedRowsRoundTrips) {
    // 5 bytes (R8) and 9 bytes (RGB8) per row are no multiples of GL's default pixel store alignment of 4
    std::vector<U8> singleChannel(5 * 5);
    std::iota(singleChannel.begin(), singleChannel.end(), static_cast<U8>(1));
    const auto r8 = createTexture(5, 5, VL_TEXTURE_R_U8, singleChannel.data());
    const auto r8Image = r8->getData();
    ASSERT_NE(r8Image, nullptr);
    EXPECT_EQ(toVector<U8>(r8Image->getData(), singleChannel.size()), singleChannel);

    std::vector<U8> rgb(3 * 3 * 3);
    std::iota(rgb.begin(), rgb.end(), static_cast<U8>(1));
    const auto rgb8 = createTexture(3, 3, VL_TEXTURE_RGB_U8, rgb.data());
    const auto rgb8Image = rgb8->getData();
    ASSERT_NE(rgb8Image, nullptr);
    EXPECT_EQ(toVector<U8>(rgb8Image->getData(), rgb.size()), rgb);
}

TEST_F(TestGLBehavior, IntegerTexturesAcceptAndReturnIntegerData) {
    std::vector<U32> unsignedPixels(4 * 4);
    std::iota(unsignedPixels.begin(), unsignedPixels.end(), 100000u);
    const auto unsignedTexture = createTexture(4, 4, VL_TEXTURE_R_U32, unsignedPixels.data());
    EXPECT_EQ(readTexture<U32>(unsignedTexture->getIdentifier(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, unsignedPixels.size()), unsignedPixels);

    const std::vector<U32> updatedPixels(4 * 4, 42u);
    unsignedTexture->setData(updatedPixels.data(), 0, 0, 4, 4);
    EXPECT_EQ(readTexture<U32>(unsignedTexture->getIdentifier(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, updatedPixels.size()), updatedPixels);

    const std::vector<I16> signedPixels = {
        -1, 2, -300, 400, -5, 6,
        7, -8, 9, -10, 11, -12,
        13, -14, 15, -16, 17, -18
    };
    const auto signedTexture = createTexture(3, 3, VL_TEXTURE_RG_I16, signedPixels.data());
    EXPECT_EQ(readTexture<I16>(signedTexture->getIdentifier(), 0, GL_RG_INTEGER, GL_SHORT, signedPixels.size()), signedPixels);
}

TEST_F(TestGLBehavior, MipmapChainIsAllocatedWhenRequested) {
    struct Case {
        Size width;
        Size height;
        bool generateMipmap;
        GLint expectedLevels;
    };
    for (const auto& [width, height, generateMipmap, expectedLevels] : {
        Case{64, 64, true, 7}, Case{100, 30, true, 7}, Case{1, 1, true, 1}, Case{64, 64, false, 1}
    }) {
        const auto texture = createTexture(width, height, VL_TEXTURE_RGBA_U8, nullptr, generateMipmap);
        GLint levels = 0;
        glGetTextureParameteriv(static_cast<GLuint>(texture->getIdentifier()), GL_TEXTURE_IMMUTABLE_LEVELS, &levels);
        EXPECT_EQ(levels, expectedLevels) << width << "x" << height << " mipmaps=" << generateMipmap;
    }
}

TEST_F(TestGLBehavior, SetDataRegeneratesMipmaps) {
    const std::vector<U8> black(4 * 4 * 4, 0);
    const auto texture = createTexture(4, 4, VL_TEXTURE_RGBA_U8, black.data(), true);

    const std::vector<U8> white(4 * 4 * 4, 255);
    texture->setData(white.data(), 0, 0, 4, 4);

    const auto level1 = readTexture<U8>(texture->getIdentifier(), 1, GL_RGBA, GL_UNSIGNED_BYTE, 2 * 2 * 4);
    EXPECT_EQ(level1, std::vector<U8>(2 * 2 * 4, 255));
}

TEST_F(TestGLBehavior, SetDataKeepsTheUsersTextureBinding) {
    const std::vector<U8> pixels(4 * 4 * 4, 1);
    const auto texture = createTexture(4, 4, VL_TEXTURE_RGBA_U8, pixels.data());

    GLuint userTexture = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &userTexture);
    glBindTexture(GL_TEXTURE_2D, userTexture);

    texture->setData(pixels.data(), 0, 0, 4, 4);

    EXPECT_EQ(static_cast<GLuint>(intState(GL_TEXTURE_BINDING_2D)), userTexture);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &userTexture);
}

/////////////////// Samplers ///////////////////

TEST_F(TestGLBehavior, SamplerWithoutFilteringOrWrappingIsValid) {
    SamplerDesc desc;
    desc.minFilter = VL_TEXTURE_FILTER_NONE;
    desc.magFilter = VL_TEXTURE_FILTER_NONE;
    desc.mipFilter = VL_TEXTURE_FILTER_NONE;
    desc.wrapU = VL_TEXTURE_WRAP_NONE;
    desc.wrapV = VL_TEXTURE_WRAP_NONE;
    desc.wrapW = VL_TEXTURE_WRAP_NONE;
    const auto sampler = context()->createSampler(desc);
    ASSERT_NE(sampler, nullptr);

    const U64 id = sampler->getIdentifier();
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MIN_FILTER), GL_NEAREST);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MAG_FILTER), GL_NEAREST);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_WRAP_R), GL_CLAMP_TO_EDGE);
}

TEST_F(TestGLBehavior, SamplerMipFilterSelectsMipmapMinFilter) {
    const auto sampler = context()->createSampler(SamplerDesc{});
    const U64 id = sampler->getIdentifier();
    EXPECT_EQ(sampler->getMipFilter(), VL_TEXTURE_FILTER_LINEAR);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MIN_FILTER), GL_LINEAR_MIPMAP_LINEAR);

    sampler->setMipFilter(VL_TEXTURE_FILTER_NONE);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MIN_FILTER), GL_LINEAR);

    // Changing the min/mag filter must keep honouring the mip filter
    sampler->setFilter(VL_TEXTURE_FILTER_POINT, VL_TEXTURE_FILTER_LINEAR);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MIN_FILTER), GL_NEAREST);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MAG_FILTER), GL_LINEAR);

    sampler->setMipFilter(VL_TEXTURE_FILTER_POINT);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_MIN_FILTER), GL_NEAREST_MIPMAP_NEAREST);
}

TEST_F(TestGLBehavior, SamplerComparisonFunctionEnablesCompareMode) {
    const auto sampler = context()->createSampler(SamplerDesc{});
    const U64 id = sampler->getIdentifier();
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_COMPARE_MODE), GL_NONE);

    sampler->setComparisonFunc(VL_COMPARISON_FUNC_LESS_EQUAL);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_COMPARE_MODE), GL_COMPARE_REF_TO_TEXTURE);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_COMPARE_FUNC), GL_LEQUAL);

    sampler->setComparisonFunc(VL_COMPARISON_FUNC_NEVER);
    EXPECT_EQ(samplerParam(id, GL_TEXTURE_COMPARE_MODE), GL_NONE);
}

/////////////////// Depth stencil state ///////////////////

TEST_F(TestGLBehavior, DepthMaskDoesNotLeakIntoStatesWithoutDepthTest) {
    DepthStencilStateDesc readOnlyDepth;
    readOnlyDepth.enableDepthTest = true;
    readOnlyDepth.depthMask = VL_DEPTH_MASK_DISABLED;
    DepthStencilStateDesc noDepthTest;
    noDepthTest.enableDepthTest = false;

    const auto first = context()->createDepthStencilState(readOnlyDepth);
    const auto second = context()->createDepthStencilState(noDepthTest);

    first->bind();
    EXPECT_TRUE(glIsEnabled(GL_DEPTH_TEST));
    EXPECT_EQ(intState(GL_DEPTH_WRITEMASK), GL_FALSE);

    // A stale mask would make glClear skip the depth buffer
    second->bind();
    EXPECT_FALSE(glIsEnabled(GL_DEPTH_TEST));
    EXPECT_EQ(intState(GL_DEPTH_WRITEMASK), GL_TRUE);

    second->unbind();
}

TEST_F(TestGLBehavior, StencilStateIsApplied) {
    DepthStencilStateDesc desc;
    desc.enableStencilTest = true;
    desc.stencilComparisonFunc = VL_COMPARISON_FUNC_LESS;
    desc.stencilReference = 5;
    desc.stencilMask = 0x0F;
    desc.stencilFail = VL_STENCIL_FUNC_REPLACE;
    desc.stencilPassDepthFail = VL_STENCIL_FUNC_INCR;
    desc.stencilPass = VL_STENCIL_FUNC_DECR;
    const auto state = context()->createDepthStencilState(desc);

    state->bind();
    EXPECT_TRUE(glIsEnabled(GL_STENCIL_TEST));
    EXPECT_EQ(intState(GL_STENCIL_FUNC), GL_LESS);
    EXPECT_EQ(intState(GL_STENCIL_REF), 5);
    EXPECT_EQ(intState(GL_STENCIL_VALUE_MASK), 0x0F);
    EXPECT_EQ(intState(GL_STENCIL_WRITEMASK), 0x0F);
    EXPECT_EQ(intState(GL_STENCIL_FAIL), GL_REPLACE);
    EXPECT_EQ(intState(GL_STENCIL_PASS_DEPTH_FAIL), GL_INCR);
    EXPECT_EQ(intState(GL_STENCIL_PASS_DEPTH_PASS), GL_DECR);

    state->setStencilReference(7);
    EXPECT_EQ(state->getStencilReference(), 7u);
    state->bind();
    EXPECT_EQ(intState(GL_STENCIL_REF), 7);

    state->unbind();
    EXPECT_FALSE(glIsEnabled(GL_STENCIL_TEST));
    EXPECT_EQ(intState(GL_STENCIL_WRITEMASK) & 0xFF, 0xFF);
    EXPECT_EQ(intState(GL_DEPTH_WRITEMASK), GL_TRUE);
}

/////////////////// Buffers & meshes ///////////////////

TEST_F(TestGLBehavior, ConstantBufferRejectsOutOfRangeSlot) {
    const std::array<float, 4> data{};
    ConstantBufferDesc desc;
    desc.name = "Bounds";
    desc.size = sizeof(data);
    desc.data = data.data();
    desc.shaderStage = VL_SHADER_VERTEX;
    const auto buffer = context()->createConstantBuffer(desc);
    ASSERT_NE(buffer, nullptr);

    const U32 maxSlots = context()->getDevice().getMaxConstantBufferSlots();
    buffer->bindShaderResource(maxSlots); // first invalid slot, must not reach GL
    buffer->bindShaderResource(maxSlots - 1);

    GLint bound = 0;
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, maxSlots - 1, &bound);
    EXPECT_EQ(static_cast<U64>(bound), buffer->getIdentifier());
    glBindBufferBase(GL_UNIFORM_BUFFER, maxSlots - 1, 0);
}

TEST_F(TestGLBehavior, StructuredBufferRejectsOutOfRangeSlot) {
    const std::array<float, 4> data{};
    StructuredBufferDesc desc;
    desc.size = sizeof(data);
    desc.data = data.data();
    desc.shaderStage = VL_SHADER_VERTEX;
    const auto buffer = context()->createStructuredBuffer(desc);
    ASSERT_NE(buffer, nullptr);

    const U32 maxSlots = context()->getDevice().getMaxStructuredBufferSlots();
    buffer->bindShaderResource(maxSlots); // first invalid slot, must not reach GL
    buffer->bindShaderResource(maxSlots - 1);

    GLint bound = 0;
    glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_BINDING, maxSlots - 1, &bound);
    EXPECT_EQ(static_cast<U64>(bound), buffer->getIdentifier());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, maxSlots - 1, 0);
}

TEST_F(TestGLBehavior, MatrixVertexAttributesGetOneLocationPerColumn) {
    const auto layout = context()->createVertexLayout();
    layout->addAttribute("Position", VL_FLOAT32_3); // location 0, offset 0
    layout->addAttribute("Model", VL_FLOAT32_4X4);  // locations 1-4, offset 12
    layout->addAttribute("Normal", VL_FLOAT32_3X3); // locations 5-7, offset 76

    const std::vector<float> vertices(2 * layout->getStride() / sizeof(float), 0.0f);
    VertexBufferDesc vbDesc;
    vbDesc.count = 2;
    vbDesc.data = vertices.data();
    vbDesc.layout = layout;
    MeshBindingDesc mbDesc;
    mbDesc.vertexBuffer = context()->createVertexBuffer(vbDesc);
    const auto binding = context()->createMeshBinding(mbDesc);
    ASSERT_NE(binding, nullptr);

    // MeshBinding does not expose its VAO, so look for the one that uses all 8 locations
    GLuint vao = 0;
    for (GLuint candidate = 1; candidate < 1024 && vao == 0; ++candidate) {
        GLint enabled = 0;
        if (glIsVertexArray(candidate)) {
            glGetVertexArrayIndexediv(candidate, 7, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        }
        if (enabled == GL_TRUE) {
            vao = candidate;
        }
    }
    ASSERT_NE(vao, 0u);

    constexpr std::array<GLint, 8> expectedSizes = {3, 4, 4, 4, 4, 3, 3, 3};
    constexpr std::array<GLint, 8> expectedOffsets = {0, 12, 28, 44, 60, 76, 88, 100};
    for (GLuint location = 0; location < 8; ++location) {
        GLint size = 0;
        GLint offset = 0;
        glGetVertexArrayIndexediv(vao, location, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        glGetVertexArrayIndexediv(vao, location, GL_VERTEX_ATTRIB_RELATIVE_OFFSET, &offset);
        EXPECT_EQ(size, expectedSizes[location]) << "location " << location;
        EXPECT_EQ(offset, expectedOffsets[location]) << "location " << location;
    }
}

/////////////////// Device ///////////////////

TEST_F(TestGLBehavior, FrameBufferLimitsMatchAttachmentLimits) {
    const Device& device = context()->getDevice();
    const GLint maxRenderbufferSize = intState(GL_MAX_RENDERBUFFER_SIZE);

    EXPECT_GT(device.getMaxFramebufferWidth(), 0u);
    EXPECT_GT(device.getMaxFramebufferHeight(), 0u);
    EXPECT_LE(device.getMaxFramebufferWidth(), device.getMaxTextureSize());
    EXPECT_LE(device.getMaxFramebufferHeight(), device.getMaxTextureSize());
    EXPECT_LE(device.getMaxFramebufferWidth(), static_cast<U32>(maxRenderbufferSize));
    EXPECT_LE(device.getMaxFramebufferHeight(), static_cast<U32>(maxRenderbufferSize));
}

/////////////////// Frame buffers ///////////////////

TEST_F(TestGLBehavior, DepthOnlyFrameBufferIsComplete) {
    FrameBufferDepthStencilAttachmentDesc ds;
    ds.format = VL_TEXTURE_DEPTH_32;
    ds.enableShaderAccess = true;
    const auto fb = createFrameBuffer(32, 32, std::nullopt, ds);
    ASSERT_NE(fb, nullptr);

    fb->begin();
    EXPECT_EQ(glCheckFramebufferStatus(GL_FRAMEBUFFER), static_cast<GLenum>(GL_FRAMEBUFFER_COMPLETE));
    // The default read buffer (COLOR_ATTACHMENT0) does not exist on a depth-only framebuffer
    EXPECT_EQ(intState(GL_READ_BUFFER), GL_NONE);
    EXPECT_EQ(intState(GL_DRAW_BUFFER0), GL_NONE);
    fb->end();
}

TEST_F(TestGLBehavior, IntegerColorAttachmentsAreClearedWithTheMatchingCommand) {
    constexpr Size width = 8;
    constexpr Size height = 8;
    constexpr Size pixelCount = width * height;

    FrameBufferColorAttachmentDesc unsignedDesc;
    unsignedDesc.format = VL_TEXTURE_RGBA_U32;
    unsignedDesc.enableShaderAccess = true;
    unsignedDesc.clearColor = Utils::Color(3.0f, 7.0f, 0.0f, 1.0f);
    const auto unsignedFb = createFrameBuffer(width, height, unsignedDesc, std::nullopt);
    ASSERT_NE(unsignedFb, nullptr);
    unsignedFb->clear();
    const auto unsignedPixels = readTexture<U32>(unsignedFb->getColorAttachment(0)->getIdentifier(), 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, pixelCount * 4);
    std::vector<U32> expectedUnsigned;
    for (Size i = 0; i < pixelCount; ++i) {
        expectedUnsigned.insert(expectedUnsigned.end(), {3u, 7u, 0u, 1u});
    }
    EXPECT_EQ(unsignedPixels, expectedUnsigned);

    FrameBufferColorAttachmentDesc signedDesc;
    signedDesc.format = VL_TEXTURE_RGBA_I32;
    signedDesc.enableShaderAccess = true;
    signedDesc.clearColor = Utils::Color(-2.0f, 5.0f, 0.0f, 1.0f);
    const auto signedFb = createFrameBuffer(width, height, signedDesc, std::nullopt);
    ASSERT_NE(signedFb, nullptr);
    signedFb->clear();
    const auto signedPixels = readTexture<I32>(signedFb->getColorAttachment(0)->getIdentifier(), 0, GL_RGBA_INTEGER, GL_INT, pixelCount * 4);
    std::vector<I32> expectedSigned;
    for (Size i = 0; i < pixelCount; ++i) {
        expectedSigned.insert(expectedSigned.end(), {-2, 5, 0, 1});
    }
    EXPECT_EQ(signedPixels, expectedSigned);

    // Renderbuffer backed attachments go through the same clear path
    unsignedDesc.enableShaderAccess = false;
    const auto renderbufferFb = createFrameBuffer(width, height, unsignedDesc, std::nullopt);
    ASSERT_NE(renderbufferFb, nullptr);
    renderbufferFb->clear();
}

TEST_F(TestGLBehavior, InvalidFrameBufferLayoutIsRejected) {
    EXPECT_EQ(createFrameBuffer(0, 0, FrameBufferColorAttachmentDesc{}, std::nullopt), nullptr);
    EXPECT_EQ(context()->createFrameBuffer(nullptr), nullptr);
}

TEST_F(TestGLBehavior, ResizingToZeroKeepsTheCurrentSize) {
    FrameBufferColorAttachmentDesc color;
    color.enableShaderAccess = true;
    FrameBufferDepthStencilAttachmentDesc depth;
    depth.enableShaderAccess = true;
    const auto fb = createFrameBuffer(32, 32, color, depth);
    ASSERT_NE(fb, nullptr);

    fb->onResize(0, 16);
    fb->onResize(16, 0);

    EXPECT_EQ(fb->getColorAttachment(0)->getWidth(), 32u);
    EXPECT_EQ(fb->getColorAttachment(0)->getHeight(), 32u);
    EXPECT_EQ(fb->getDepthStencilAttachment()->getWidth(), 32u);
    EXPECT_EQ(fb->getDepthStencilAttachment()->getHeight(), 32u);
}

TEST_F(TestGLBehavior, DepthStencil32F8CanBeReadBack) {
    for (const bool shaderAccess : {true, false}) {
        FrameBufferDepthStencilAttachmentDesc ds;
        ds.format = VL_TEXTURE_DEPTH_32_STENCIL_8;
        ds.enableShaderAccess = shaderAccess;
        ds.clearDepth = 0.75f;
        ds.clearStencil = 9;
        const auto fb = createFrameBuffer(8, 8, std::nullopt, ds);
        ASSERT_NE(fb, nullptr) << "shader access: " << shaderAccess;

        fb->clear();
        const auto image = fb->getDepthStencilAttachment()->getData();
        ASSERT_NE(image, nullptr) << "shader access: " << shaderAccess;

        // Each pixel is a float depth followed by a word that holds the stencil in its lowest 8 bits
        const auto words = toVector<float>(image->getData(), 8 * 8 * 2);
        EXPECT_EQ(words[0], 0.75f) << "shader access: " << shaderAccess;
        EXPECT_EQ(std::bit_cast<U32>(words[1]) & 0xFFu, 9u) << "shader access: " << shaderAccess;
    }
}
