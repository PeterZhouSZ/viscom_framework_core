/**
 * @file   ray.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of a ray.
 */

#pragma once

#include <array>
#include <glm/glm.hpp>

namespace viscom::math {

    template<typename real> using Line2 = std::array<glm::tvec2<real, glm::highp>, 2>;
    template<typename real> using Line3 = std::array<glm::tvec3<real, glm::highp>, 2>;

    ///
    /// Structure of a simple ray
    ///
    template<typename real> struct Ray3
    {
        using v3type = glm::tvec3<real, glm::highp>;
        v3type orig_;
        v3type dir_;
        v3type inv_dir_;

        Ray3(const v3type& o = v3type(static_cast<real>(0.0)), const v3type& d = v3type(static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)))
            : orig_(o), dir_(d), inv_dir_(1.f / d)
        {
        }
        Ray3(const Line3<real>& line)
            : orig_(line[0]), dir_(glm::normalize(line[1] - line[0])), inv_dir_(1.f / dir_)
        {
        }

        v3type GetPoint(float t) const { return orig_ + dir_ * t; }
    };
}
