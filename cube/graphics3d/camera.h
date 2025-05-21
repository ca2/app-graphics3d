// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 20:12 <3ThomasBorregaardSorensen!!
#pragma once 


#include "app-cube/cube/graphics3d/types.h"


namespace graphics3d
{


	enum CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


	struct camera_t
	{

		//void calculateMouseOffset(float xoffset, float yoffset);
//private:
		quad_matrix		m_matrixProjection{ 1.f };
		quad_matrix		m_matrixImpact{ 1.f };
		quad_matrix		m_matrixAntImpact{ 1.f };

		//float m_LastX; // Last mouse x position
		//float m_LastY; // Last mouse y position
		float				m_MouseSensitivity = 0.1f; // Sensitivity factor for mouse input
		// Deadzone
		float				m_MouseDeadZone;
		// Inertia
		pole				m_poleVelocity;
		// Camera attributes
		location			m_locationPosition;
		pole				m_poleFront{ 0.0f, 0.0f, -1.0f };  // Camera direction (forward vector)
		pole				m_poleUp{ 0.0f, 1.0f, 0.0f };     // Up vector
		pole				m_poleRight{ 1.0f, 0.0f, 0.0f };   // Right vector (cross product)
		pole				m_poleWorldUp;

		// Euler angles
		float				m_fYaw = 0.0f;
		float				m_fPitch = glm::radians(-90.0f);

		// Camera options
		float				m_fZoom;
		float				m_fMovementSpeed;  // Added movement speed

		::block as_block()
		{

			return as_memory_block(*this);

		}

	};

	
	class CLASS_DECL_CUBE camera :
		virtual public camera_t,
		virtual public ::particle
	{
	public:



		camera() {};
		//camera(location position, float yaw, float pitch);
		//camera(location position, location to, location from);

		virtual void initialize_camera(location position, float yaw, float pitch);
		virtual void initialize_camera(location position, location to, location from);

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setViewDirection(location position, pole direction, pole up = pole{ 0.f, -1.f, 0.f });
		void setViewTarget(location position, location target, pole up = pole{ 0.f, -1.f, 0.f });

		void setViewYXZ(location position, pole rotation);

		virtual void setPerspectiveProjection(float fovy, float aspect, float near, float far);
		const quad_matrix& getProjection() const { return m_matrixProjection; }
		const quad_matrix& getView() const { return m_matrixImpact; }
		const quad_matrix& getInverseView() const { return m_matrixAntImpact; }

		// Get zoom (field of view)
		float GetZoom() const;
		// Get the view matrix
		quad_matrix GetViewMatrix() const;


		location GetPosition() const;
		// Set movement speed
		void SetMovementSpeed(float speed);
		// new shit
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessKeyboardInput(int direction, float deltaTime);

	
		// Update camera vectors
		void UpdateCameraVectors();


		// from MyPlace2025@V0idsEmbrace@Twitch
      void UpdateIdleMovement(float deltaTime); // Add this line
      //// Get zoom (field of view)
      //float GetZoom() const;
      //// Set movement speed
      //void SetMovementSpeed(float speed);
      void Jump(float jumpHeight);
      void TeleportDownward(float distance); // Moves the camera down
      void TeleportInDirection(int direction);
		// END from MyPlace2025@V0idsEmbrace@Twitch


	};


} // namespace graphics3d



