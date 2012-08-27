#ifndef _YON_AUDIO_ISOUND_H_
#define _YON_AUDIO_ISOUND_H_

#include "IReferencable.h"
#include "vector3d.h"
#include "position3d.h"
#include "path.h"

namespace yon{
namespace audio{
	 
	class ISound : public virtual core::IReferencable{
	protected:
		io::SNamedPath m_namePath;
	public:
		ISound(const io::path& p) : m_namePath(p){}
		virtual ~ISound(){}

		const io::path& getPath() const { return m_namePath.getPath(); }

		//播放控制
		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;
		virtual void rewind() = 0;

		//状态查询
		virtual bool isPlaying() = 0;
		virtual bool isPaused() = 0;
		virtual bool isStopped() = 0;

		//循环播放
		virtual void setLooping(bool on = true) = 0;
		virtual bool isLooping() = 0;

		//音量[0.0,1.0]
		virtual void setGain(f32 gain) = 0;
		virtual f32 getGain() = 0;

		virtual void setPitch(f32 pitch) = 0 ;
		virtual f32 getPitch() = 0;

		virtual void setRolloffFactor(f32 factor) = 0;
		virtual f32 getRolloffFactor() = 0;

		virtual void setMinGain(f32 gain) = 0;
		virtual void setMaxGain(f32 gain) = 0;
		virtual f32 getMinGain() = 0;
		virtual f32 getMaxGain() = 0;

		virtual void setMinDistance(f32 d) = 0;
		virtual void setMaxDistance(f32 d) = 0;
		virtual f32 getMinDistance() = 0;
		virtual f32 getMaxDistance() = 0;

		virtual f32 getSecondOffset() = 0;
		virtual void setSecondOffset(const f32 offset) = 0;

		virtual void setPosition(const core::position3df& p) = 0;
		virtual const core::position3df getPosition() = 0;

		virtual void setVelocity(const core::vector3df& v) = 0;
		virtual const core::vector3df getVelocity() = 0;

		virtual void setDirection(const core::vector3df& d) = 0;
		virtual const core::vector3df getDirection() = 0;
	};
}
}
#endif