/**
 * @file   primitives.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of some math primitives.
 */

#pragma once

#include "aabb.h"
#include "plane.h"
#include "frustum.h"
#include "ray.h"
#include <array>
#include <glm/glm.hpp>

namespace viscom::math {

    template<typename real> using Tri2 = std::array<glm::tvec2<real, glm::highp>, 3>;
    template<typename real> using Tri3 = std::array<glm::tvec3<real, glm::highp>, 3>;
}
