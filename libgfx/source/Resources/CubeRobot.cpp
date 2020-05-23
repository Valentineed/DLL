#include "Resources/CubeRobot.h"
#include "Physics/Transform.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
namespace Resources
{
	LowRenderer::Model* CubeRobot::s_cube = nullptr;

	CubeRobot::CubeRobot(void)
	{

		Graph* body = new Graph(s_cube, vec4{ 1.f, 0.f, 0.f, 1.f }); // Red !
		body->SetModelScale(vec3{ 10, 15, 5 });
		body->SetTransform(Transform::Translate(mat4::Identity(), vec3(0.f, 0.f, -6.f)));
		AddChild(body);
		
		//m_head = new Graph(s_cube, vec4(0.f, 1.f, 0.f, 1.f)); // Green !
		//m_head->SetModelScale(vec3(5, 5, 5));
		//m_head->SetTransform(Transform::Translate(mat4::Identity(), vec3(0.f, 10.f, 0.f)));
		//body->AddChild(m_head);
		
		m_leftArm = new Graph(s_cube, vec4(0.f, 0.f, 1.f, 1.f)); // Blue !
		m_leftArm-> SetModelScale(vec3(3.f, 18.f, 3.f));
		m_leftArm-> SetTransform(Transform::Translate(mat4::Identity(), vec3(-7.f, 0.f, -1.f)));
		body->AddChild(m_leftArm);
		
		m_rightArm = new Graph(s_cube, vec4(0.f, 0.f, 1.f, 1.f)); // Blue !
		m_rightArm-> SetModelScale(vec3(3.f, 18.f, 3.f));
		m_rightArm-> SetTransform(Transform::Translate(mat4::Identity(), vec3(7.f, 0.f, -1.f)));
		body->AddChild(m_rightArm);
		
		Graph* leftLeg = new Graph(s_cube, vec4(0.f, 0.f, 1.f, 1.f)); // Blue !
		leftLeg->SetModelScale(vec3(3.f, 17.5f, 3.f));
		leftLeg->SetTransform(Transform::Translate(mat4::Identity(), vec3(-3.f, -17.f, 0.f)));
		body->AddChild(leftLeg);
		
		Graph* rightLeg = new Graph(s_cube, vec4(0.f, 0.f, 1.f, 1.f)); // Blue !
		rightLeg->SetModelScale(vec3(3.f, 17.5f, 3.f));
		rightLeg->SetTransform((Transform::Translate(mat4::Identity(), vec3(3.f, -17.f, 0.f))));
		body->AddChild(rightLeg);

	}

	void CubeRobot::Update(float delta)
	{

		//m_angle += msec / 10.0f;
		m_transform = Transform::Rotate(m_transform, delta / 10.0f, vec3(0.f, 1.f, 0.f));

		//m_head->SetTransform(Transform::Rotate(m_head->GetTransform(),-msec / 10.0f, vec3(0.f, 1.f, 0.f)));

		//TODO implemmentation matrix arbitrary point
		m_leftArm->m_angle += delta / 5.0f;
		mat4 leftArmTransfo = Transform::Translate(m_leftArm->GetTransform(), { 0.f, 9.f, 0.f });
		leftArmTransfo = Transform::Rotate(leftArmTransfo, sinf(m_leftArm->m_angle * (float)(M_PI / 180)), vec3(1.f, 0.f, 0.f));
		leftArmTransfo = Transform::Translate(leftArmTransfo, { 0.f, -9.f, 0.f });
		m_leftArm->SetTransform(leftArmTransfo);
		
		m_rightArm->m_angle -= delta / 5.0f;
		mat4 rightArmTransfo = Transform::Translate(m_rightArm->GetTransform(), { 0.f, 9.f, 0.f });
		rightArmTransfo = Transform::Rotate(rightArmTransfo, sinf(m_rightArm->m_angle * (float)(M_PI / 180)), vec3(1.f, 0.f, 0.f));
		rightArmTransfo = Transform::Translate(rightArmTransfo, { 0.f, -9.f, 0.f });
		m_rightArm->SetTransform(rightArmTransfo); 
	
		
		Graph::Update(delta);
	}
}
