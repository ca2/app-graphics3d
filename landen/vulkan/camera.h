#pragma once 
// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace vkc {
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class VkcCamera {
	public:
		VkcCamera() {};
		VkcCamera(floating_sequence3 position, float yaw, float pitch);

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setViewDirection(floating_sequence3 position, floating_sequence3 direction, floating_sequence3 up = floating_sequence3{ 0.f, -1.f, 0.f });
		void setViewTarget(floating_sequence3 position, floating_sequence3 target, floating_sequence3 up = floating_sequence3{ 0.f, -1.f, 0.f });

		void setViewYXZ(floating_sequence3 position, floating_sequence3 rotation);

		void setPerspectiveProjection(float fovy, float aspect, float near, float far);
		const floating_matrix4& getProjection() const { return projectionMatrix; }
		const floating_matrix4& getView() const { return viewMatrix; }
		const floating_matrix4& getInverseView() const { return inverseViewMatrix; }

		// Get zoom (field of view)
		float GetZoom() const;
		// Get the view matrix
		floating_matrix4 GetViewMatrix() const;


		floating_sequence3 GetPosition() const;
		// Set movement speed
		void SetMovementSpeed(float speed);
		// new shit
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessKeyboardInput(int direction, float deltaTime);

		//void calculateMouseOffset(float xoffset, float yoffset);
	private:
		floating_matrix4 projectionMatrix{ 1.f };
		floating_matrix4 viewMatrix{ 1.f };
		floating_matrix4 inverseViewMatrix{ 1.f };

		//float m_LastX; // Last mouse x position
		//float m_LastY; // Last mouse y position
		float m_MouseSensitivity = 0.1f; // Sensitivity factor for mouse input
		// Deadzone
		float m_MouseDeadZone;
		// Inertia
		floating_sequence3 m_Velocity;
		// Camera attributes
		floating_sequence3 m_Position;
		floating_sequence3 m_Front{ 0.0f, 0.0f, -1.0f };  // Camera direction (forward vector)
		floating_sequence3 m_Up{ 0.0f, 1.0f, 0.0f };     // Up vector
		floating_sequence3 m_Right{ 1.0f, 0.0f, 0.0f };   // Right vector (cross product)
		floating_sequence3 m_WorldUp;

		// Euler angles
		float m_Yaw = 0.0f;
		float m_Pitch = -90.0f;

		// Camera options
		float m_Zoom;
		float m_MovementSpeed;  // Added movement speed

		// Update camera vectors
		void UpdateCameraVectors();
	};
}