 /********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2016 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_CONVEX_MESH_SHAPE_H
#define REACTPHYSICS3D_CONVEX_MESH_SHAPE_H

// Libraries
#include "ConvexPolyhedron.h"
#include "engine/CollisionWorld.h"
#include "mathematics/mathematics.h"
#include "collision/TriangleMesh.h"
#include "collision/PolyhedronMesh.h"
#include "collision/narrowphase/GJK/GJKAlgorithm.h"
#include <vector>
#include <set>
#include <map>

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Declaration
class CollisionWorld;

// Class ConvexMeshShape
/**
 * This class represents a convex mesh shape. In order to create a convex mesh shape, you
 * need to indicate the local-space position of the mesh vertices. You do it either by
 * passing a vertices array to the constructor or using the addVertex() method. Make sure
 * that the set of vertices that you use to create the shape are indeed part of a convex
 * mesh. The center of mass of the shape will be at the origin of the local-space geometry
 * that you use to create the mesh. The method used for collision detection with a convex
 * mesh shape has an O(n) running time with "n" beeing the number of vertices in the mesh.
 * Therefore, you should try not to use too many vertices. However, it is possible to speed
 * up the collision detection by using the edges information of your mesh. The running time
 * of the collision detection that uses the edges is almost O(1) constant time at the cost
 * of additional memory used to store the vertices. You can indicate edges information
 * with the addEdge() method. Then, you must use the setIsEdgesInformationUsed(true) method
 * in order to use the edges information for collision detection.
 */
class ConvexMeshShape : public ConvexPolyhedron {

    protected :

        // -------------------- Attributes -------------------- //

        /// Polyhedron structure of the mesh
        PolyhedronMesh* mPolyhedronMesh;

        /// Mesh minimum bounds in the three local x, y and z directions
        Vector3 mMinBounds;

        /// Mesh maximum bounds in the three local x, y and z directions
        Vector3 mMaxBounds;

        // -------------------- Methods -------------------- //

        /// Recompute the bounds of the mesh
        void recalculateBounds();

        /// Set the scaling vector of the collision shape
        virtual void setLocalScaling(const Vector3& scaling) override;

        /// Return a local support point in a given direction without the object margin.
        virtual Vector3 getLocalSupportPointWithoutMargin(const Vector3& direction,
                                                          void** cachedCollisionData) const override;

        /// Return true if a point is inside the collision shape
        virtual bool testPointInside(const Vector3& localPoint, ProxyShape* proxyShape) const override;

        /// Raycast method with feedback information
        virtual bool raycast(const Ray& ray, RaycastInfo& raycastInfo, ProxyShape* proxyShape) const override;

        /// Return the number of bytes used by the collision shape
        virtual size_t getSizeInBytes() const override;

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        // TODO : Do we really need to use the margin anymore ? Maybe for raycasting ? If not, remove all the
        // comments documentation about margin
        ConvexMeshShape(PolyhedronMesh* polyhedronMesh, decimal margin = OBJECT_MARGIN);

        /// Destructor
        virtual ~ConvexMeshShape() override = default;

        /// Deleted copy-constructor
        ConvexMeshShape(const ConvexMeshShape& shape) = delete;

        /// Deleted assignment operator
        ConvexMeshShape& operator=(const ConvexMeshShape& shape) = delete;

        /// Return the local bounds of the shape in x, y and z directions
        virtual void getLocalBounds(Vector3& min, Vector3& max) const override;

        /// Return the local inertia tensor of the collision shape.
        virtual void computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const override;

        /// Return the number of faces of the polyhedron
        virtual uint getNbFaces() const override;

        /// Return a given face of the polyhedron
        virtual HalfEdgeStructure::Face getFace(uint faceIndex) const override;

        /// Return the number of vertices of the polyhedron
        virtual uint getNbVertices() const override;

        /// Return a given vertex of the polyhedron
        virtual HalfEdgeStructure::Vertex getVertex(uint vertexIndex) const override;

        /// Return the number of half-edges of the polyhedron
        virtual uint getNbHalfEdges() const override;

        /// Return a given half-edge of the polyhedron
        virtual HalfEdgeStructure::Edge getHalfEdge(uint edgeIndex) const override;

        /// Return the position of a given vertex
        virtual Vector3 getVertexPosition(uint vertexIndex) const override;

        /// Return the normal vector of a given face of the polyhedron
        virtual Vector3 getFaceNormal(uint faceIndex) const override;
};

/// Set the scaling vector of the collision shape
inline void ConvexMeshShape::setLocalScaling(const Vector3& scaling) {
    ConvexShape::setLocalScaling(scaling);
    recalculateBounds();
}

// Return the number of bytes used by the collision shape
inline size_t ConvexMeshShape::getSizeInBytes() const {
    return sizeof(ConvexMeshShape);
}

// Return the local bounds of the shape in x, y and z directions
/**
 * @param min The minimum bounds of the shape in local-space coordinates
 * @param max The maximum bounds of the shape in local-space coordinates
 */
inline void ConvexMeshShape::getLocalBounds(Vector3& min, Vector3& max) const {
    min = mMinBounds;
    max = mMaxBounds;
}

// Return the local inertia tensor of the collision shape.
/// The local inertia tensor of the convex mesh is approximated using the inertia tensor
/// of its bounding box.
/**
* @param[out] tensor The 3x3 inertia tensor matrix of the shape in local-space
*                    coordinates
* @param mass Mass to use to compute the inertia tensor of the collision shape
*/
inline void ConvexMeshShape::computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const {
    decimal factor = (decimal(1.0) / decimal(3.0)) * mass;
    Vector3 realExtent = decimal(0.5) * (mMaxBounds - mMinBounds);
    assert(realExtent.x > 0 && realExtent.y > 0 && realExtent.z > 0);
    decimal xSquare = realExtent.x * realExtent.x;
    decimal ySquare = realExtent.y * realExtent.y;
    decimal zSquare = realExtent.z * realExtent.z;
    tensor.setAllValues(factor * (ySquare + zSquare), 0.0, 0.0,
                        0.0, factor * (xSquare + zSquare), 0.0,
                        0.0, 0.0, factor * (xSquare + ySquare));
}

// Return true if a point is inside the collision shape
inline bool ConvexMeshShape::testPointInside(const Vector3& localPoint,
                                             ProxyShape* proxyShape) const {

    // Use the GJK algorithm to test if the point is inside the convex mesh
    return proxyShape->mBody->mWorld.mCollisionDetection.
           mNarrowPhaseGJKAlgorithm.testPointInside(localPoint, proxyShape);
}

// Return the number of faces of the polyhedron
inline uint ConvexMeshShape::getNbFaces() const {
    return mPolyhedronMesh->getHalfEdgeStructure().getNbFaces();
}

// Return a given face of the polyhedron
inline HalfEdgeStructure::Face ConvexMeshShape::getFace(uint faceIndex) const {
    assert(faceIndex < getNbFaces());
    return mPolyhedronMesh->getHalfEdgeStructure().getFace(faceIndex);
}

// Return the number of vertices of the polyhedron
inline uint ConvexMeshShape::getNbVertices() const {
    return mPolyhedronMesh->getHalfEdgeStructure().getNbVertices();
}

// Return a given vertex of the polyhedron
inline HalfEdgeStructure::Vertex ConvexMeshShape::getVertex(uint vertexIndex) const {
    assert(vertexIndex < getNbVertices());
    return mPolyhedronMesh->getHalfEdgeStructure().getVertex(vertexIndex);
}

// Return the number of half-edges of the polyhedron
inline uint ConvexMeshShape::getNbHalfEdges() const {
    return mPolyhedronMesh->getHalfEdgeStructure().getNbHalfEdges();
}

// Return a given half-edge of the polyhedron
inline HalfEdgeStructure::Edge ConvexMeshShape::getHalfEdge(uint edgeIndex) const {
    assert(edgeIndex < getNbHalfEdges());
    return mPolyhedronMesh->getHalfEdgeStructure().getHalfEdge(edgeIndex);
}

// Return the position of a given vertex
inline Vector3 ConvexMeshShape::getVertexPosition(uint vertexIndex) const {
    assert(vertexIndex < getNbVertices());
    return mPolyhedronMesh->getVertex(vertexIndex);
}

// Return the normal vector of a given face of the polyhedron
inline Vector3 ConvexMeshShape::getFaceNormal(uint faceIndex) const {
    assert(faceIndex < getNbFaces());
    return mPolyhedronMesh->getFaceNormal(faceIndex);
}

}

#endif
