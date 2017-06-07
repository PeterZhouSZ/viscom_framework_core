/**
 * @file   FreeCamera.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2017.06.07
 *
 * @brief  Declaration of a free flight camera.
 */

#pragma once

#include "core/camera/CameraBase.h"
#include "core/CameraHelper.h"
#include <glm/gtc/quaternion.hpp>

namespace viscom {

    /**
    * Represents a free moving camera.
    */
    class FreeCamera final : public CameraBase
    {
    public:
        FreeCamera(const glm::vec3& camPos, viscom::CameraHelper& cameraHelper) noexcept;

        virtual bool HandleMouse(int button, int action, float mouseWheelDelta, const ApplicationNodeBase* sender) override;
        virtual void UpdateCamera(const ApplicationNodeBase* sender) override;
    };
}
