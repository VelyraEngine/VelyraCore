#include "../Pch.hpp"

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    VL_TEXTURE_FORMAT getTextureFormat(const View<Image::IImage> &image) {
        const VL_TYPE dataType = image->getDataType();
        const VL_CHANNEL_FORMAT channelFormat = image->getChannelFormat();
        
        // The upper 4 bits of VL_TEXTURE_FORMAT represent the channel count,
        // the lower 4 bits represent the data type (upper nibble of VL_TYPE)
        const U8 channelBits = static_cast<U8>(channelFormat) << 4;
        const U8 typeBits = static_cast<U8>(dataType) >> 4;
        
        return static_cast<VL_TEXTURE_FORMAT>(channelBits | typeBits);
    }

    VL_TYPE getTextureDataType(const VL_TEXTURE_FORMAT format) {
        U8 typeBits = static_cast<U8>(format) & 0x0F; // lower 4 bits
        typeBits <<= 4; // shift back to upper nibble
        return static_cast<VL_TYPE>(typeBits);
    }

    VL_CHANNEL_FORMAT getTextureChannelFormat(const VL_TEXTURE_FORMAT format) {
        U8 channelBits = static_cast<U8>(format) & 0xF0; // upper 4 bits
        channelBits >>= 4; // shift down to lower nibble
        return static_cast<VL_CHANNEL_FORMAT>(channelBits);
    }
}
