/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef ZENI_SOUND_HXX
#define ZENI_SOUND_HXX

#include <Zeni/Sound.h>

namespace Zeni {

  const ALuint & Sound_Buffer::get_id() const {
    if(m_loader)
      finish_loading();
    
    return m_buffer;
  }

  void Sound_Source::set_buffer(const Sound_Buffer &
#ifndef DISABLE_AL
    buffer
#endif
    ) {
#ifndef DISABLE_AL
    alSourcei(m_source, AL_BUFFER, buffer.get_id());
#endif
  }

  void Sound_Source::set_buffer(const ALuint &
#ifndef DISABLE_AL
    buffer
#endif
    ) {
#ifndef DISABLE_AL
    alSourcei(m_source, AL_BUFFER, buffer);
#endif
  }

  void Sound_Source::set_pitch(const float &
#ifndef DISABLE_AL
    pitch
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_PITCH, pitch);
#endif
  }

  void Sound_Source::set_gain(const float &
#ifndef DISABLE_AL
    gain
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_GAIN, gain);
#endif
  }

  void Sound_Source::set_position(const Point3f &
#ifndef DISABLE_AL
    position
#endif
    ) {
#ifndef DISABLE_AL
    alSourcefv(m_source, AL_POSITION, reinterpret_cast<const float *>(&position));
#endif
  }

  void Sound_Source::set_velocity(const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    ) {
#ifndef DISABLE_AL
    alSourcefv(m_source, AL_VELOCITY, reinterpret_cast<const float *>(&velocity));
#endif
  }

  void Sound_Source::set_looping(const bool &
#ifndef DISABLE_AL
    looping
#endif
    ) {
#ifndef DISABLE_AL
    alSourcei(m_source, AL_LOOPING, looping);
#endif
  }

  void Sound_Source::set_time(const float &
#ifndef DISABLE_AL
    time
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_SEC_OFFSET, time);
#endif
  }
  
  void Sound_Source::set_near_clamp(const float &
#ifndef DISABLE_AL
    near_clamp
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_REFERENCE_DISTANCE, near_clamp);
#endif
  }

  void Sound_Source::set_far_clamp(const float &
#ifndef DISABLE_AL
    far_clamp
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_MAX_DISTANCE, far_clamp);
#endif
  }

  void Sound_Source::set_rolloff(const float &
#ifndef DISABLE_AL
    rolloff
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloff);
#endif
  }

  ALuint Sound_Source::get_buffer() const {
    ALint buffer = 0;
#ifndef DISABLE_AL
    alGetSourcei(m_source, AL_BUFFER, &buffer);
#endif
    return buffer;
  }

  float Sound_Source::get_pitch() const {
    float pitch = 1.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_PITCH, &pitch);
#endif
    return pitch;
  }

  float Sound_Source::get_gain() const {
    float gain = 1.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_GAIN, &gain);
#endif
    return gain;
  }

  Point3f Sound_Source::get_position() const {
    Point3f position;
#ifndef DISABLE_AL
    alGetSourcefv(m_source, AL_POSITION, reinterpret_cast<float *>(&position));
#endif
    return position;
  }

  Vector3f Sound_Source::get_velocity() const {
    Vector3f velocity;
#ifndef DISABLE_AL
    alGetSourcefv(m_source, AL_VELOCITY, reinterpret_cast<float *>(&velocity));
#endif
    return velocity;
  }

  bool Sound_Source::is_looping() const {
    ALint looping = AL_FALSE;
#ifndef DISABLE_AL
    alGetSourcei(m_source, AL_LOOPING, &looping);
#endif
    return looping != AL_FALSE;
  }

  float Sound_Source::get_time() const {
    float time = 0.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_SEC_OFFSET, &time);
#endif
    return time;
  }
  
  float Sound_Source::get_near_clamp() const {
    float near_clamp = 10.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &near_clamp);
#endif
    return near_clamp;
  }

  float Sound_Source::get_far_clamp() const {
    float far_clamp = 1000.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_MAX_DISTANCE, &far_clamp);
#endif
    return far_clamp;
  }

  float Sound_Source::get_rolloff() const {
    float rolloff = 1.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &rolloff);
#endif
    return rolloff;
  }

  void Sound_Source::play() {
#ifndef DISABLE_AL
    alSourcePlay(m_source);
#endif
  }

  void Sound_Source::pause() {
#ifndef DISABLE_AL
    alSourcePause(m_source);
#endif
  }

  void Sound_Source::stop() {
#ifndef DISABLE_AL
    alSourceStop(m_source);
#endif
  }

  bool Sound_Source::is_playing() {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
#else
    return false;
#endif
  }

  bool Sound_Source::is_paused() {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
#else
    return false;
#endif
  }

  bool Sound_Source::is_stopped() {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
#else
    return true;
#endif
  }

  void Sound::set_listener_position(const Point3f &
#ifndef DISABLE_AL
    position
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_position[3] = {position.x, position.y, position.z};
    alListenerfv(AL_POSITION, listener_position);

    assert_m_bgm();
    m_bgm_source->set_position(position);
#endif
  }

  void Sound::set_listener_velocity(const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_velocity[3] = {velocity.i, velocity.j, velocity.k};
    alListenerfv(AL_VELOCITY, listener_velocity);

    assert_m_bgm();
    m_bgm_source->set_velocity(velocity);
#endif
  }

  void Sound::set_listener_forward_and_up(const Vector3f &
#ifndef DISABLE_AL
    forward
#endif
    , const Vector3f &
#ifndef DISABLE_AL
    up
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_forward_and_up[6] = {forward.i, forward.j, forward.k, up.i, up.j, up.k};
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
#endif
  }

  Point3f Sound::get_listener_position() const {
    return m_bgm_source->get_position();
    ALfloat listener_position[3] = {0.0f, 0.0f, 0.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_POSITION, listener_position);
#endif
    return Point3f(listener_position[0], listener_position[1], listener_position[2]);
  }

  Vector3f Sound::get_listener_velocity() const {
    ALfloat listener_velocity[3] = {0.0f, 0.0f, 0.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_VELOCITY, listener_velocity);
#endif
    return Vector3f(listener_velocity[0], listener_velocity[1], listener_velocity[2]);
  }

  std::pair<Vector3f, Vector3f> Sound::get_listener_forward_and_up() const {
    ALfloat lfau[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_ORIENTATION, lfau);
#endif
    return std::make_pair(Vector3f(lfau[0], lfau[1], lfau[2]), Vector3f(lfau[3], lfau[4], lfau[5]));
  }

  void Sound::set_BGM_pitch(const float &
#ifndef DISABLE_AL
    pitch
#endif
    ) {
#ifndef DISABLE_AL
    m_bgm_source->set_pitch(pitch);
#endif
  }

  void Sound::set_BGM_gain(const float &
#ifndef DISABLE_AL
    gain
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_gain(gain);
#endif
  }

  void Sound::set_BGM_looping(const bool &
#ifndef DISABLE_AL
    looping
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_looping(looping);
#endif
  }

  void Sound::set_BGM_time(const float &
#ifndef DISABLE_AL
    time
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_time(time);
#endif
  }

  float Sound::get_BGM_pitch() {
    assert_m_bgm();
    return m_bgm_source->get_pitch();
  }

  float Sound::get_BGM_gain() {
    assert_m_bgm();
    return m_bgm_source->get_gain();
  }

  bool Sound::is_BGM_looping() {
    assert_m_bgm();
    return m_bgm_source->is_looping();
  }

  float Sound::get_BGM_time() {
    assert_m_bgm();
    return m_bgm_source->get_time();
  }

  bool Sound::playing_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_playing();
  }

  bool Sound::paused_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_paused();
  }

  bool Sound::stopped_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_stopped();
  }

  void Sound::play_BGM() {
    assert_m_bgm();
    return m_bgm_source->play();
  }

  void Sound::pause_BGM() {
    assert_m_bgm();
    return m_bgm_source->pause();
  }
  
  void Sound::stop_BGM() {
    assert_m_bgm();
    return m_bgm_source->stop();
  }

}

#endif
