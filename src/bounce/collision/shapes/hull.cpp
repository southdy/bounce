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

#include <bounce/collision/shapes/hull.h>

void b3Hull::Validate() const 
{
	for (u32 i = 0; i < faceCount; ++i) 
	{
		Validate(faces + i);
	}
}

void b3Hull::Validate(const b3Face* face) const 
{
	bool ok = false;
	const b3HalfEdge* begin = GetEdge(face->edge);
	const b3HalfEdge* edge = begin;
	do
	{
		if (GetFace(edge->face) == face)
		{
			ok = true;
			break;
		}
		edge = edges + edge->next;
	} while (edge != begin);

	B3_ASSERT(ok);

	Validate(edges + face->edge);
}

void b3Hull::Validate(const b3HalfEdge* e) const 
{
	u8 edgeIndex = (u8)(e - edges);
	
	const b3HalfEdge* twin = edges + e->twin;

	// Each edge must be followed by its twin.
	B3_ASSERT(b3Abs(twin - e) == 1);
	B3_ASSERT(twin->twin == edgeIndex);

	u32 count = 0;
	const b3HalfEdge* begin = e;
	do 
	{
		const b3HalfEdge* next = edges + e->next;
		const b3HalfEdge* twin = edges + next->twin;
		e = twin;
		B3_ASSERT(count < edgeCount);
		++count;
	} while (e != begin);
}