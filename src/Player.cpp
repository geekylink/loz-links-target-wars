#include "Player.h"

#include <Zeni/Camera.hxx>
#include <Zeni/Sound.hxx>

using namespace Zeni;
using namespace Zeni_Collision;

namespace Crate {

  Player::Player(const Camera &camera_,
         const Vector3f &end_point_b_,
         const float radius_)
  {
	m_camera = camera_;
	m_end_point_b = end_point_b_;
	m_radius = radius_;

	//sets initial velocity, and sets player on ground
	set_velocity(Vector3f(0.0f,0.0f,0.0f));
	m_is_on_ground = true;

	create_body();
  }

  // Level 2
  void Player::set_position(const Point3f &position) {
	  m_camera.set_position(position);
  }

  void Player::adjust_pitch(const float &phi) {
	  //make sure it doesn't flip upside down
	  m_camera.adjust_pitch(phi);
  }

  void Player::turn_left_xy(const float &theta) {
	  m_camera.turn_left_xy(theta);
  }

  void Player::set_on_ground(const bool &is_on_ground_) {
	  m_is_on_ground = is_on_ground_;
  }

  void Player::jump() {
	//didn't use

  }

  void Player::step(const float &time_step) {
	  m_camera.move_forward_xy(m_velocity.i*time_step);
	  m_camera.move_left_xy(m_velocity.j*time_step);
	  m_camera.set_position(m_camera.get_position().x, m_camera.get_position().y, m_camera.get_position().z + get_velocity().k*time_step);
	  create_body();
  }

  void Player::create_body() {
	  m_body = Capsule(m_camera.get_position(), m_camera.get_position() + m_end_point_b, m_radius);

	  Sound &sr = Sound::get_reference();

	  //sr.set_listener_position(m_camera.get_position());
	  //sr.set_listener_velocity(m_velocity);
	  //sr.set_listener_forward_and_up(m_camera.get_forward(), m_camera.get_up());
  }

}
