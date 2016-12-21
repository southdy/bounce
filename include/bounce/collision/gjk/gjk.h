/*
* Copyright (c) 2016-2016 Irlan Robson http://www.irlan.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B3_GJK_H
#define B3_GJK_H

#include <bounce/common/geometry.h>

class b3GJKProxy;
struct b3SimplexCache;

struct b3SimplexVertex
{
	b3Vec3 pointA; // support vertex on proxy A
	b3Vec3 pointB; // support vertex on proxy B
	b3Vec3 point; // minkowski vertex
	float32 weight; // barycentric coordinate for point
	u32 indexA; // support A vertex index
	u32 indexB; // support B vertex index
};

struct b3Simplex
{
	b3SimplexVertex m_vertices[4];
	u32 m_count;

	b3Vec3 GetSearchDirection(const b3Vec3& Q) const;
	b3Vec3 GetClosestPoint() const;
	void GetClosestPoints(b3Vec3* pA, b3Vec3* pB) const;

	void Solve2(const b3Vec3& Q);
	void Solve3(const b3Vec3& Q);
	void Solve4(const b3Vec3& Q);

	// Cache
	void ReadCache(const b3SimplexCache* cache,
		const b3Transform& xfA, const b3GJKProxy& proxyA,
		const b3Transform& xfB, const b3GJKProxy& proxyB);
	void WriteCache(b3SimplexCache* cache) const;
	float32 GetMetric() const;
};

// The output of the GJK algorithm.
// It contains the closest points between two proxies 
// and their euclidean distance.
// If the distance is zero then the proxies are overlapping.
struct b3GJKOutput
{
	b3Vec3 pointA; // closest point on proxy A
	b3Vec3 pointB; // closest point on proxy B
	float32 distance; // euclidean distance between the closest points
	u32 iterations; // number of GJK iterations
};

// Find the closest points and distance between two proxies.
b3GJKOutput b3GJK(const b3Transform& xfA, const b3GJKProxy& proxyA, 
	const b3Transform& xfB, const b3GJKProxy& proxyB);

#endif