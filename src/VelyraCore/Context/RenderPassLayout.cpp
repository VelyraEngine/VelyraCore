#include "../Pch.hpp"

#include <VelyraCore/Context/RenderPassLayout.hpp>
#include "../Logging/LoggerNames.hpp"

namespace Velyra::Core {

    RenderPassLayout::RenderPassLayout(const Device& device):
    m_Logger(Utils::getLogger(VL_LOGGER_CTX)),
    m_Device(device){}

    RenderPassLayout::~RenderPassLayout() = default;

    void RenderPassLayout::setDimensions(const Size width, const Size height) {
        m_Width = width;
        m_Height = height;
    }

    void RenderPassLayout::addColorAttachment(const RenderPassColorAttachmentDesc &desc) {
        m_ColorAttachments.emplace_back(desc);
    }

    void RenderPassLayout::setDepthStencilAttachment(const RenderPassDepthStencilDesc &desc) {
        m_DepthStencilAttachment = desc;
    }

    bool RenderPassLayout::isValid() const {
        return dimensionsValid() && colorAttachmentsValid() && depthStencilAttachmentValid();
    }

    const std::vector<RenderPassColorAttachmentDesc> &RenderPassLayout::getColorAttachments() const {
        return m_ColorAttachments;
    }

    const std::optional<RenderPassDepthStencilDesc> &RenderPassLayout::getDepthStencilAttachment() const {
        return m_DepthStencilAttachment;
    }

    bool RenderPassLayout::dimensionsValid() const {
        if (m_Width == 0 || m_Height == 0) {
            SPDLOG_LOGGER_WARN(m_Logger, "RenderPassLayout has invalid dimensions: width and height must be greater than 0 (current: {}x{})", m_Width, m_Height);
            return false;
        }
        if (m_Width > m_Device.getMaxFramebufferWidth() || m_Height > m_Device.getMaxFramebufferHeight()) {
            SPDLOG_LOGGER_WARN(m_Logger, "RenderPassLayout has invalid dimensions: width and height must be less than or equal to the device limits (current: {}x{}, max: {}x{})",
                m_Width, m_Height, m_Device.getMaxFramebufferWidth(), m_Device.getMaxFramebufferHeight());
            return false;
        }
        return true;
    }

    bool RenderPassLayout::colorAttachmentsValid() const {
        if (m_ColorAttachments.size() > m_Device.getMaxFramebufferColorAttachments()) {
            SPDLOG_LOGGER_WARN(m_Logger, "RenderPassLayout has too many color attachments: the number of color attachments must be less than or equal to the device limit (current: {}, max: {})",
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

    bool RenderPassLayout::depthStencilAttachmentValid() const {
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
