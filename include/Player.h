#ifndef PLAYER_H
#define PLAYER_H

#include <Zeni/Camera.h>
#include <Zeni/Collision.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Vector3f.h>

#define PLAYER_SPEED 350.0f
#define GROUND 50.0f
#define GRAVITY 1.3f
#define MAX_FALL_SPEED 700.0f

namespace Crate {

  class Player {
  public:
    Player(const Zeni::Camera &camera_,
           const Zeni::Vector3f &end_point_b_,
           const float radius_);

    // Level 1
    const Zeni::Camera & get_camera() const {return m_camera;}

    // Level 2
    void set_position(const Zeni::Point3f &position);

    void adjust_pitch(const float &phi);
    void turn_left_xy(const float &theta);

    // Level 3
    const Zeni_Collision::Capsule & get_body() const {return m_body;}
    bool is_on_ground() const {return m_is_on_ground;}

    const Zeni::Vector3f & get_velocity() const {return m_velocity;}
    void set_velocity(const Zeni::Vector3f &velocity_) {m_velocity = velocity_;}

    void set_on_ground(const bool &is_on_ground_);
    void jump();

    void step(const float &time_step);

  private:
    void create_body();

		 // Level 1/2
    Zeni::Camera m_camera;

    // Level 2
    Zeni::Vector3f m_end_point_b;
    float m_radius;

    // Level 3
    Zeni_Collision::Capsule m_body; // collision
    Zeni::Vector3f m_velocity;
    bool m_is_on_ground;

    // Level 4
    // Controls are external to Player
  };

}

#endif
