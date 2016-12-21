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

#include <bounce/dynamics/joints/mouse_joint.h>
#include <bounce/dynamics/body.h>
#include <bounce/common/draw.h>

b3MouseJoint::b3MouseJoint(const b3MouseJointDef* def) 
{
	m_type = e_mouseJoint;
	m_worldTargetA = def->target;
	m_localAnchorB = def->bodyB->GetLocalPoint(def->target);
	m_maxForce = def->maxForce;
	m_impulse.SetZero();
}

void b3MouseJoint::InitializeConstraints(const b3SolverData* data) 
{
	b3Body* m_bodyB = GetBodyB();

	m_indexB = m_bodyB->m_islandID;
	m_mB = m_bodyB->m_invMass;
	m_iB = m_bodyB->m_worldInvI;

	b3Vec3 xB = data->positions[m_indexB].x;
	b3Quat qB = data->positions[m_indexB].q;

	b3Vec3 worldAnchorB = b3Mul(qB, m_localAnchorB) + xB;
	
	m_C = worldAnchorB - m_worldTargetA;	
	m_rB = worldAnchorB - xB;

	b3Mat33 M = b3Diagonal(m_mB);
	b3Mat33 RB = b3Skew(m_rB);
	b3Mat33 RBT = b3Transpose(RB);
	m_mass = M + RB * m_iB * RBT;
}

void b3MouseJoint::WarmStart(const b3SolverData* data) 
{
	data->velocities[m_indexB].v += m_mB * m_impulse;
	data->velocities[m_indexB].w += m_iB * b3Cross(m_rB, m_impulse);
}

void b3MouseJoint::SolveVelocityConstraints(const b3SolverData* data) 
{
	b3Vec3 vB = data->velocities[m_indexB].v;
	b3Vec3 wB = data->velocities[m_indexB].w;

	b3Vec3 Cdot = vB + b3Cross(wB, m_rB);

	b3Vec3 impulse = m_mass.Solve(-(Cdot + data->invdt * B3_BAUMGARTE * m_C));
	b3Vec3 oldImpulse = m_impulse;
	m_impulse += impulse;

	// Prevent large reaction impulses.
	float32 maxImpulse = data->dt * m_maxForce;
	float32 sqrImpulse = b3Dot(m_impulse, m_impulse);
	if (sqrImpulse > maxImpulse * maxImpulse)
	{
		float32 ratio = maxImpulse / b3Sqrt(sqrImpulse);
		m_impulse *= ratio;
	}
	
	impulse = m_impulse - oldImpulse;

	vB += m_mB * impulse;
	wB += m_iB * b3Cross(m_rB, impulse);
	
	data->velocities[m_indexB].v = vB;
	data->velocities[m_indexB].w = wB;
}

bool b3MouseJoint::SolvePositionConstraints(const b3SolverData* data) 
{
	// There is no position correction for this constraint.
	// todo Implement Buda spring?
	return true;
}

b3Vec3 b3MouseJoint::GetAnchorA() const 
{
	return m_worldTargetA;
}

b3Vec3 b3MouseJoint::GetAnchorB() const
{
	return GetBodyB()->GetWorldPoint(m_localAnchorB);
}

const b3Vec3& b3MouseJoint::GetTarget() const
{
	return m_worldTargetA;
}

void b3MouseJoint::SetTarget(const b3Vec3& target)
{
	m_worldTargetA = target;
}

void b3MouseJoint::Draw(b3Draw* draw) const 
{
	b3Color red(1.0f, 0.0f, 0.0f);
	b3Color green(0.0f, 1.0f, 0.0f);
	b3Color yellow(1.0f, 1.0f, 0.0f);
	
	b3Vec3 a = GetAnchorA();
	b3Vec3 b = GetAnchorB();

	draw->DrawPoint(a, green);
	draw->DrawPoint(b, red);
	draw->DrawSegment(a, b, yellow);
}