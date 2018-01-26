/**
 * @file   plane.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of a plane primitive.
 */

#pragma once

#include <glm/glm.hpp>

namespace viscom::math {


    ///
    /// Plane with the form p.x*x + p.y*y + p.z*z + p.w = 0
    ///
    template<typename real> struct Plane3
    {
        using ptype = glm::tvec4<real, glm::highp>;
        using v3type = glm::tvec3<real, glm::highp>;
        using v4type = glm::tvec4<real, glm::highp>;
        using mattype = glm::tmat4x4<real, glm::highp>;
        typename ptype p_;

        Plane3(const v4type& v = v4type(static_cast<real>(0.0)));
        Plane3(const v3type& normal, const v3type& point);
        Plane3(const v3type& v0, const v3type& v1, const v3type& v2);

        operator const ptype&() const;

        float Distance(const v3type& point) const;

        const v3type GetNormal() const;
        v3type GetPoint() const;

        Plane3 Transform(const mattype& matrix) const;
    };

    ///
    /// Constructor of #Plane: Given as vec4
    /// @param[in] v vec4 containing (a,b,c,d)
    ///
    template<typename real> inline Plane3<real>::Plane3(const v4type& v) : p_{ v } {}

    ///
    /// Constructor of #Plane: Given normal and point
    /// @param[in] normal normal of plane
    /// @param[in] point point in plane
    ///
    template<typename real> inline Plane3<real>::Plane3(const v3type& normal, const v3type& point)
    {
        auto n = glm::normalize(normal);
        p_ = ptype(n, -glm::dot(n, point));
    }

    ///
    /// Constructor of #Plane: Given three vertices or points
    /// @param[in]  v0    vertex 0
    /// @param[in]  v1    vertex 1
    /// @param[in]  v2    vertex 2
    ///
    template<typename real> inline Plane3<real>::Plane3(const v3type& v0, const v3type& v1, const v3type& v2)
        : Plane3{ glm::cross(v1 - v0, v2 - v0), v0 }
    {
    }

    template<typename real> inline Plane3<real>::operator const ptype&() const { return p_; }

    ///
    /// Computed distance between #Plane and point
    /// @param[in]  point  point in space
    /// @return     Closest distance between plane and point
    ///
    template<typename real> inline float Plane3<real>::Distance(const v3type& point) const { return glm::dot(p_, v4type(point, static_cast<real>(1.0))); }

    ///
    /// Gets normal from object vec3(a,b,c)
    /// @return     normal of plane
    ///
    template<typename real> inline const typename Plane3<real>::v3type Plane3<real>::GetNormal() const { return v3type(p_); }

    ///
    /// Gets a point in #Plane
    /// @return     point given as vec3
    ///
    template<typename real> inline typename Plane3<real>::v3type Plane3<real>::GetPoint() const { return GetNormal() * -p_.w; }

    ///
    /// Transforms a Plane with a matrix
    /// @param[in]  matrix  transformation matrix
    /// @return     Transformed plane
    ///
    template<typename real> inline Plane3<real> Plane3<real>::Transform(const mattype& matrix) const
    {
        auto result = glm::inverse(glm::transpose(matrix)) * p_;
        return Plane3<real>(result);
    }
}
