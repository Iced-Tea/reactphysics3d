/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010 Daniel Chappuis                                            *
*********************************************************************************
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
* THE SOFTWARE.                                                                 *
********************************************************************************/

// Libraries
#include <iostream>
#include "Matrix3x3.h"

// Namespaces
using namespace reactphysics3d;

// Constructor of the class Matrix3x3
Matrix3x3::Matrix3x3() {
    // Initialize all values in the matrix to zero
    setAllValues(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

// Constructor
Matrix3x3::Matrix3x3(double value) {
    setAllValues(value, value, value, value, value, value, value, value, value);
}

// Constructor with arguments
Matrix3x3::Matrix3x3(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3) {
    // Initialize the matrix with the values
    setAllValues(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

// Destructor
Matrix3x3::~Matrix3x3() {

}

// Return the inverse matrix
Matrix3x3 Matrix3x3::getInverse() const {
    // Compute the determinant of the matrix
    double determinant = getDeterminant();

    // Check if the determinant is equal to zero
    assert(determinant != 0.0);
    double invDeterminant = 1.0 / determinant;
    Matrix3x3 tempMatrix;

    // Compute the inverse of the matrix
    tempMatrix.setAllValues((array[1][1]*array[2][2]-array[2][1]*array[1][2]), -(array[1][0]*array[2][2]-array[2][0]*array[1][2]), (array[1][0]*array[2][1]-array[2][0]*array[1][1]),
                            -(array[0][1]*array[2][2]-array[2][1]*array[0][2]), (array[0][0]*array[2][2]-array[2][0]*array[0][2]), -(array[0][0]*array[2][1]-array[2][0]*array[0][1]),
                            (array[0][1]*array[1][2]-array[0][2]*array[1][1]), -(array[0][0]*array[1][2]-array[1][0]*array[0][2]), (array[0][0]*array[1][1]-array[0][1]*array[1][0]));

    // Return the inverse matrix
    return (invDeterminant * tempMatrix.getTranspose());
}

// Overloaded operator for multiplication with a matrix
Matrix3x3 Matrix3x3::operator*(const Matrix3x3& matrix2) const {
    // Compute and return the multiplication of the matrices
    return Matrix3x3(array[0][0]*matrix2.array[0][0] + array[0][1]*matrix2.array[1][0] + array[0][2]*matrix2.array[2][0],
                     array[0][0]*matrix2.array[0][1] + array[0][1]*matrix2.array[1][1] + array[0][2]*matrix2.array[2][1],
                     array[0][0]*matrix2.array[0][2] + array[0][1]*matrix2.array[1][2] + array[0][2]*matrix2.array[2][2],
                     array[1][0]*matrix2.array[0][0] + array[1][1]*matrix2.array[1][0] + array[1][2]*matrix2.array[2][0],
                     array[1][0]*matrix2.array[0][1] + array[1][1]*matrix2.array[1][1] + array[1][2]*matrix2.array[2][1],
                     array[1][0]*matrix2.array[0][2] + array[1][1]*matrix2.array[1][2] + array[1][2]*matrix2.array[2][2],
                     array[2][0]*matrix2.array[0][0] + array[2][1]*matrix2.array[1][0] + array[2][2]*matrix2.array[2][0],
                     array[2][0]*matrix2.array[0][1] + array[2][1]*matrix2.array[1][1] + array[2][2]*matrix2.array[2][1],
                     array[2][0]*matrix2.array[0][2] + array[2][1]*matrix2.array[1][2] + array[2][2]*matrix2.array[2][2]);
}

// Overloaded operator for assignment
Matrix3x3& Matrix3x3::operator=(const Matrix3x3& matrix2) {
    // Check for self-assignment
    if (this != &matrix2) {
        setAllValues(matrix2.array[0][0], matrix2.array[0][1], matrix2.array[0][2],
                 matrix2.array[1][0], matrix2.array[1][1], matrix2.array[1][2],
                 matrix2.array[2][0], matrix2.array[2][1], matrix2.array[2][2]);
    }

    // Return a reference to the matrix
    return *this;
}


