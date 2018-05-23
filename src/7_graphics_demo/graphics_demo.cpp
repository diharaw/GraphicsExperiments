#include <iostream>
#include <fstream>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <application.h>
#include <camera.h>
#include <render_device.h>
#include <utility.h>
#include <scene.h>
#include <material.h>
#include <macros.h>
#include <renderer.h>
#include <memory>

#define CAMERA_ROLL 0.0

class GraphicsDemo : public dw::Application
{
private:
    float m_heading_speed = 0.0f;
    float m_sideways_speed = 0.0f;
    bool m_mouse_look = false;
	bool m_show_debug_window = false;
	float m_camera_sensitivity = 0.02f;
	float m_camera_speed = 0.01f;
    Camera* m_camera;
	dw::Scene* m_scene;
	dw::Renderer* m_renderer;

protected:
	void debug_window()
	{
		ImGui::Begin("Debug");

		ImGui::DragFloat3("Light Direction", &m_renderer->per_scene_uniform()->directionalLight.direction.x);
		ImGui::ColorPicker3("Light Color", &m_renderer->per_scene_uniform()->directionalLight.color.x);
		ImGui::InputFloat("Camera Sensitivity", &m_camera_sensitivity);
		ImGui::InputFloat("Camera Speed", &m_camera_speed);

		ImGui::End();
	}
	
    bool init(int argc, const char* argv[]) override
    {
        m_camera = new Camera(45.0f,
                            0.1f,
                            1000.0f,
                            ((float)m_width)/((float)m_height),
                            glm::vec3(0.0f, 0.0f, 10.0f),
                            glm::vec3(0.0f, 0.0f, -1.0f));
        
		m_renderer = new dw::Renderer(&m_device, m_width, m_height);

		m_scene = dw::Scene::load("scene.json", &m_device, m_renderer);

		if (!m_scene)
			return false;

		m_renderer->set_scene(m_scene);

        return true;
    }

    void update(double delta) override
    {
		if (m_show_debug_window)
			debug_window();

		update_camera();
		m_renderer->render(m_camera, m_width, m_height, nullptr);
		m_device.bind_framebuffer(nullptr);
    }

    void shutdown() override
    {
		delete m_scene;
		delete m_renderer;
		delete m_camera;
    }

    void key_pressed(int code) override
    {
        if(code == GLFW_KEY_W)
            m_heading_speed = m_camera_speed;
        else if(code == GLFW_KEY_S)
            m_heading_speed = -m_camera_speed;
        
        if(code == GLFW_KEY_A)
            m_sideways_speed = -m_camera_speed;
        else if(code == GLFW_KEY_D)
            m_sideways_speed = m_camera_speed;
        
        if(code == GLFW_KEY_E)
            m_mouse_look = true;

		if (code == GLFW_KEY_H)
			m_show_debug_window = !m_show_debug_window;
    }
    
    void key_released(int code) override
    {
        if(code == GLFW_KEY_W || code == GLFW_KEY_S)
            m_heading_speed = 0.0f;
        
        if(code == GLFW_KEY_A || code == GLFW_KEY_D)
            m_sideways_speed = 0.0f;
        
        if(code == GLFW_KEY_E)
            m_mouse_look = false;
    }
    
    void update_camera()
    {
        m_camera->set_translation_delta(m_camera->m_forward, m_heading_speed * m_delta);
        m_camera->set_translation_delta(m_camera->m_right, m_sideways_speed * m_delta);
        
        if(m_mouse_look)
        {
            // Activate Mouse Look
            m_camera->set_rotatation_delta(glm::vec3((float)(m_mouse_delta_y * m_camera_sensitivity * m_delta),
                                            (float)(m_mouse_delta_x * m_camera_sensitivity * m_delta),
                                            (float)(CAMERA_ROLL * m_camera_sensitivity * m_delta)));
        }
        else
        {
            m_camera->set_rotatation_delta(glm::vec3((float)(0),
                                             (float)(0),
                                             (float)(0)));
        }
        
        m_camera->update();		
    }
};

DW_DECLARE_MAIN(GraphicsDemo)
