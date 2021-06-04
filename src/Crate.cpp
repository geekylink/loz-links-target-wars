#include "Crate.h"

#include <Zeni/Model.hxx>
#include <Zeni/Quaternion.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sounds.h>
#include <Zeni/Video.h>

using namespace Zeni;
using namespace Zeni_Collision;

namespace Crate {

  Crate::Crate(const Point3f &corner_,
        const Vector3f &scale_,
        const Quaternion &rotation_)
		:m_source(new Sound_Source(Sounds::get_reference().get_sound("collide")))
  {
	  // incriment  m_instance_count (initially is 0)
	  // if it was 0 at the start, make a "new model"

		m_corner = corner_;
		m_scale = scale_;
		m_rotation = rotation_;

	  if (m_instance_count == 0)
		m_model = new Model("models/crate.3ds");

	  ++m_instance_count;

	  create_body();
	  }

  Crate::Crate(const Crate &/*rhs*/)
  {
	  //don't worry about it for now
  }

  Crate & Crate::operator=(const Crate &/*rhs*/) {
      //no worries
	  
	  //...

	  return *this;
  }

  Crate::~Crate() {
	  //decrement, if 0, delete
	  --m_instance_count;

	  if(m_instance_count)
		  delete m_model;
	  delete m_source;
  }

  void Crate::render() {
	  //official order, scale, rotate, translate
	  m_model->set_translate(m_corner);
	  m_model->set_rotate(m_rotation);
	  m_model->set_scale(m_scale);

	  Video::get_reference().render(*m_model);
  }

  void Crate::collide() {
	  // play the "collide" sound
	  // this isn't a "test"
	  if (!m_source->is_playing())
		m_source->play();
  }

  void Crate::create_body() {
	  m_body = Parallelepiped(m_corner, 
								m_rotation * m_scale.get_i(), 
								m_rotation * m_scale.get_j(),
								m_rotation * m_scale.get_k());

	  m_source->set_position(m_corner + 0.5f * (m_rotation * m_scale));
	  m_source->set_velocity(Vector3f(0.0f,0.0f,0.0f));


  }

  Model * Crate::m_model = 0;
  unsigned long Crate::m_instance_count = 0lu;

}
