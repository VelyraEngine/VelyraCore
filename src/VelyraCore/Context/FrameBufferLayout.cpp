#include "../Pch.hpp"

#include <VelyraCore/Context/FrameBufferLayout.hpp>
#include "../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    FrameBufferLayout::FrameBufferLayout(const Device& device):
    m_Logger(Utils::getLogger(VL_LOGGER_CTX)),
    m_Device(device){}

    FrameBufferLayout::~FrameBufferLayout() = default;

    void FrameBufferLayout::setDimensions(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
    }

    void FrameBufferLayout::addColorAttachment(const FrameBufferColorAttachmentDesc &desc) {
        m_ColorAttachments.emplace_back(desc);
    }

    void FrameBufferLayout::setDepthStencilAttachment(const FrameBufferDepthStencilDesc &desc) {
        m_DepthStencilAttachment = desc;
    }

    bool FrameBufferLayout::isValid() const {
        return dimensionsValid() && colorAttachmentsValid() && depthStencilAttachmentValid();
    }

    const std::vector<FrameBufferColorAttachmentDesc> &FrameBufferLayout::getColorAttachments() const {
        return m_ColorAttachments;
    }

    const std::optional<FrameBufferDepthStencilDesc> &FrameBufferLayout::getDepthStencilAttachment() const {
        return m_DepthStencilAttachment;
    }

    Size FrameBufferLayout::getWidth() const {
        return m_Width;
    }

    Size FrameBufferLayout::getHeight() const {
        return m_Height;
    }

    bool FrameBufferLayout::dimensionsValid() const {
        if (m_Width == 0 || m_Height == 0) {
            SPDLOG_LOGGER_WARN(m_Logger, "FrameBufferLayout has invalid dimensions: width and height must be greater than 0 (current: {}x{})", m_Width, m_Height);
            return false;
        }
        if (m_Width > m_Device.getMaxFramebufferWidth() || m_Height > m_Device.getMaxFramebufferHeight()) {
            SPDLOG_LOGGER_WARN(m_Logger, "FrameBufferLayout has invalid dimensions: width and height must be less than or equal to the device limits (current: {}x{}, max: {}x{})",
                m_Width, m_Height, m_Device.getMaxFramebufferWidth(), m_Device.getMaxFramebufferHeight());
            return false;
        }
        return true;
    }

    bool FrameBufferLayout::colorAttachmentsValid() const {
        if (m_ColorAttachments.size() > m_Device.getMaxFramebufferColorAttachments()) {
            SPDLOG_LOGGER_WARN(m_Logger, "FrameBufferLayout has too many color attachments: the number of color attachments must be less than or equal to the device limit (current: {}, max: {})",
                m_ColorAttachments.size(), m_Device.getMaxFramebufferColorAttachments());
            return false;
        }
        for (const auto& desc: m_ColorAttachments) {
            if (desc.enableShaderAccess) {
                if (!m_Device.isTextureFormatSupported(desc.format)) {
                    SPDLOG_LOGGER_WARN(m_Logger, "The device does not support format: {} as a shader resource format for color attachments", desc.format);
                    return false;
                }
            }
            else {
                if (!m_Device.isColorAttachmentFormatSupported(desc.format)) {
                    SPDLOG_LOGGER_WARN(m_Logger, "The device does not support format: {} as a color attachment format", desc.format);
                    return false;
                }
            }
        }
        return true;
    }

    bool FrameBufferLayout::depthStencilAttachmentValid() const {
        if (!m_DepthStencilAttachment.has_value()) {
            return true; // depth stencil attachment is optional
        }
        const auto& desc = m_DepthStencilAttachment.value();
        if (desc.resource.enableShaderAccess) {
            if (!m_Device.isTextureFormatSupported(desc.resource.format)) {
                SPDLOG_LOGGER_WARN(m_Logger, "The device does not support format: {} as a shader resource format for depth stencil attachments", desc.resource.format);
                return false;
            }
        }
        else {
            if (!m_Device.isDepthStencilAttachmentFormatSupported(desc.resource.format)) {
                SPDLOG_LOGGER_WARN(m_Logger, "The device does not support format: {} as a depth stencil attachment format", desc.resource.format);
                return false;
            }
        }
        return true;
    }
}
