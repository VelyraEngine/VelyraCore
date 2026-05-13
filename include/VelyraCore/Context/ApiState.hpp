#pragma once

#include <VelyraCore/Context/Definitions.hpp>

namespace Velyra::Core {

    /**
     * @brief Handy for debugging. This state will capture the current state of the graphics API by querying the API
     *        itself for the current state. This circumvents all wrappers in place from our side and allows us to
     *        compare the state we think we have with the actual state of the API. This is especially useful for
     *        debugging state leaks and ensuring that our state management is working correctly.
     */
    class ApiState {
    public:
        explicit ApiState(const VL_GRAPHICS_API api) : m_Api(api) {}

        virtual ~ApiState() = default;

        [[nodiscard]] VL_GRAPHICS_API getApi() const { return m_Api; }

        /**
         * @brief Compares this state with another state.
         * @param other
         * @return
         */
        virtual bool isEqual(const ApiState& other) const = 0;

        /**
         * @brief Logs the current state to the console.
         */
        virtual void log() const = 0;

    private:
        const VL_GRAPHICS_API m_Api;

    };

}
