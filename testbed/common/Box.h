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

#ifndef BOX_H
#define BOX_H

// Libraries
#include "openglframework.h"
#include "reactphysics3d.h"
#include "PhysicsObject.h"

// Class Box
class Box : public PhysicsObject {

	private :

		// -------------------- Attributes -------------------- //

		/// Size of each side of the box
		float mSize[3];

        rp3d::BoxShape* mBoxShape;
        rp3d::ProxyShape* mProxyShape;

        /// Scaling matrix (applied to a cube to obtain the correct box dimensions)
        openglframework::Matrix4 mScalingMatrix;

        /// Vertex Buffer Object for the vertices data
        static openglframework::VertexBufferObject mVBOVertices;

        /// Vertex Buffer Object for the normals data
        static openglframework::VertexBufferObject mVBONormals;

        /// Vertex Buffer Object for the texture coords
        static openglframework::VertexBufferObject mVBOTextureCoords;

        /// Vertex Buffer Object for the indices
        static openglframework::VertexBufferObject mVBOIndices;

        /// Vertex Array Object for the vertex data
        static openglframework::VertexArrayObject mVAO;

		/// Total number of boxes created
		static int totalNbBoxes;

		// -------------------- Methods -------------------- //

		/// Create a the VAO and VBOs to render to box with OpenGL
        void createVBOAndVAO();

	public :

		// -------------------- Methods -------------------- //

		/// Constructor
		Box(const openglframework::Vector3& size, const openglframework::Vector3& position,
                reactphysics3d::CollisionWorld* world, const std::string& meshFolderPath);

		/// Constructor
		Box(const openglframework::Vector3& size, const openglframework::Vector3& position,
                float mass, reactphysics3d::DynamicsWorld *world, const std::string& meshFolderPath);

		/// Destructor
		~Box();

		/// Render the cube at the correct position and with the correct orientation
        void render(openglframework::Shader& shader, const openglframework::Matrix4& worldToCameraMatrix, bool wireframe);

        /// Update the transform matrix of the object
        virtual void updateTransform(float interpolationFactor) override;

        /// Set the scaling of the object
        void setScaling(const openglframework::Vector3& scaling);
};

// Update the transform matrix of the object
inline void Box::updateTransform(float interpolationFactor) {
	mTransformMatrix = computeTransform(interpolationFactor, mScalingMatrix);
}


#endif