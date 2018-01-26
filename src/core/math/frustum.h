/**
 * @file   frustum.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.15
 *
 * @brief  Declaration of some math primitives.
 */

#pragma once

#include "aabb.h"
#include "plane.h"
#include <glm/glm.hpp>

#undef near
#undef far

namespace viscom::math {

    template<typename real> struct Frustum
    {
        using v4type = glm::tvec4<real, glm::highp>;
        using mattype = glm::tmat4x4<real, glm::highp>;

        Frustum() = default;
        Frustum(const std::array<Plane3<real>, 6>& initPlanes) : planes(initPlanes) {}
        Frustum(const mattype& projMatrix);

        bool IntersectFrustumAABB(const AABB3<real>& aabb) const;

        Plane3<real>& left() { return planes_[0]; }
        const Plane3<real>& left() const { return planes_[0]; }
        Plane3<real>& right() { return planes_[1]; }
        const Plane3<real>& right() const { return planes_[1]; }
        Plane3<real>& top() { return planes_[2]; }
        const Plane3<real>& top() const { return planes_[2]; }
        Plane3<real>& bttm() { return planes_[3]; }
        const Plane3<real>& bttm() const { return planes_[3]; }
        Plane3<real>& near() { return planes_[4]; }
        const Plane3<real>& near() const { return planes_[4]; }
        Plane3<real>& far() { return planes_[5]; }
        const Plane3<real>& far() const { return planes_[5]; }

        std::array<Plane3<real>, 6> planes_;
    };

    /**
    * @brief      Constructor of #Frustum, constructs a frustum from a projection matrix
    *
    * @param[in]  projMatrix  Projection matrix to be used
    */
    template<typename real> inline Frustum<real>::Frustum(const mattype& projMatrix)
    {
        auto invProjMatrix = inverse(projMatrix);

        const auto hNbl = invProjMatrix * v4type(-1, -1, -1, 1);
        const auto hNbr = invProjMatrix * v4type(1, -1, -1, 1);
        const auto hNtl = invProjMatrix * v4type(-1, 1, -1, 1);
        const auto hNtr = invProjMatrix * v4type(1, 1, -1, 1);
        const auto hFbl = invProjMatrix * v4type(-1, -1, 1, 1);
        const auto hFbr = invProjMatrix * v4type(1, -1, 1, 1);
        const auto hFtl = invProjMatrix * v4type(-1, 1, 1, 1);
        const auto hFtr = invProjMatrix * v4type(1, 1, 1, 1);

        const auto nbl = v3type(hNbl) / hNbl.w;
        const auto nbr = v3type(hNbr) / hNbr.w;
        const auto ntl = v3type(hNtl) / hNtl.w;
        const auto ntr = v3type(hNtr) / hNtr.w;
        const auto fbl = v3type(hFbl) / hFbl.w;
        const auto fbr = v3type(hFbr) / hFbr.w;
        const auto ftl = v3type(hFtl) / hFtl.w;
        const auto ftr = v3type(hFtr) / hFtr.w;

        left() = Plane3<real>(nbl, fbl, ftl);
        right() = Plane3<real>(nbr, ntr, ftr);
        bttm() = Plane3<real>(nbl, nbr, fbr);
        top() = Plane3<real>(ntl, ftl, ftr);
        near() = Plane3<real>(nbl, ntl, ntr);
        far() = Plane3<real>(fbl, fbr, ftr);
    }

    template<typename real> typename Frustum<real>::v3type GetVertexP(const AABB3<real>& aabb, const typename Frustum<real>::v3type& normal)
    {
        Frustum<real>::v3type p = aabb.GetMin();
        if (normal.x >= 0) p.x = aabb.GetMax().x;
        if (normal.y >= 0) p.y = aabb.GetMax().y;
        if (normal.z >= 0) p.z = aabb.GetMax().z;

        return p;
    }

    template<typename real> typename Frustum<real>::v3type GetVertexN(const AABB3<real>& aabb, typename Frustum<real>::v3type& normal)
    {
        Frustum<real>::v3type n = aabb.GetMax();
        if (normal.x >= 0) n.x = aabb.GetMin().x;
        if (normal.y >= 0) n.y = aabb.GetMin().y;
        if (normal.z >= 0) n.z = aabb.GetMin().z;

        return n;
    }

    template<typename real> inline bool Frustum<real>::IntersectFrustumAABB(const AABB3<real>& aabb) const
    {
        bool result = true;
        for (int i = 0; i < 6; i++) {
            if (planes[i].Distance(GetVertexP(aabb, planes[i].GetNormal())) < 0) return false;
            if (planes[i].Distance(GetVertexN(aabb, planes[i].GetNormal())) < 0) result = true;
        }

        return result;
    }
}
