#pragma once

#include "../IProcedure.hpp"
#include "../Utils.hpp"

VL_ENUM(MeshType, int,
    Cube = 0,
    Sphere = 1,
    Cone = 2,
    Cylinder = 3,
    Torus = 4,
    Plane = 5
);

namespace Velyra::SandBox {

    class MeshBindingProcedure: public IProcedure {
    public:
        MeshBindingProcedure() = default;

        ~MeshBindingProcedure() override = default;

        void onAttach(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onUpdate(Duration deltaTime, const UP<Core::Context> &context, const UP<Core::Window> &window) override;

        void onImGui(const UP<Core::Context> &context, const UP<Core::Window> &window) override;

    private:

        void createTransformBuffer(const UP<Core::Context> &context);

        void createMeshBindings(const UP<Core::Context> &context);

        void imGuiDrawModeSelector();

        void imGuiMeshSelector();

        void imGuiDrawTransform();

        void setTransformData();

    private:
        View<Core::ConstantBuffer> m_TransformBuffer = nullptr;
        std::unordered_map<MeshType, View<Core::MeshBinding>> m_MeshBindings;

        MeshType m_SelectedMeshType = Cube;
        VL_DRAW_MODE m_SelectedDrawMode = VL_DRAW_TRIANGLES;
        glm::vec3 m_InputTranslate = glm::vec3(0.0f);
        glm::vec3 m_InputRotate = glm::vec3(0.0f);
        glm::vec3 m_InputScale = glm::vec3(1.0f);
    };

    VL_DECLARE_PROCEDURE_FACTORY(MeshBindingProcedure, VL_SBX_PROCEDURE_TYPE::MeshBinding, {});

}