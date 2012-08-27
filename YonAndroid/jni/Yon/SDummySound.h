#ifndef _YON_AUDIO_OAL_SDUMMYSOUND_H_
#define _YON_AUDIO_OAL_SDUMMYSOUND_H_

namespace yon{
namespace audio{
namespace oal{

	struct SDummySound: public ISound{
	public:
		SDummySound(const io::path& p) : ISound(p){}

		void setPath(const io::path& p){m_namePath.setPath(p);}

		virtual void play(){}
		virtual void pause(){}
		virtual void stop(){}
		virtual void rewind(){}

		virtual bool isPlaying(){return false;}
		virtual bool isPaused(){return false;}
		virtual bool isStopped(){return false;}

		virtual void setLooping(bool on){}
		virtual bool isLooping(){return false;}

		virtual void setGain(f32 gain){}
		virtual f32 getGain(){return 0;}

		virtual void setPitch(f32 pitch){}
		virtual f32 getPitch(){return 0;}

		virtual void setRolloffFactor(f32 factor){}
		virtual f32 getRolloffFactor(){return 0;}

		virtual void setMinGain(f32 gain){}
		virtual void setMaxGain(f32 gain){}
		virtual f32 getMinGain(){return 0;}
		virtual f32 getMaxGain(){return 0;}

		virtual void setMinDistance(f32 d){}
		virtual void setMaxDistance(f32 d){}
		virtual f32 getMinDistance(){return 0;}
		virtual f32 getMaxDistance(){return 0;}

		virtual f32 getSecondOffset(){return 0;}
		virtual void setSecondOffset(const f32 offset){}

		virtual void setPosition(const core::position3df& p){}
		virtual const core::position3df getPosition(){return core::ORIGIN_POSITION3DF;}

		virtual void setVelocity(const core::vector3df& v){}
		virtual const core::vector3df getVelocity(){return core::ORIGIN_POSITION3DF;}

		virtual void setDirection(const core::vector3df& d){}
		virtual const core::vector3df getDirection(){return core::ORIGIN_POSITION3DF;}
	};
}
}
}
#endif