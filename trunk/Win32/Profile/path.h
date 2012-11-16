#ifndef _YON_IO_PATH_H_
#define _YON_IO_PATH_H_

#include "yonTypes.h"
#include "yonString.h"

namespace yon{
namespace io{
	typedef core::string<fschar> path;

	struct SNamedPath
	{
		//! Constructor
		SNamedPath() {}

		//! Constructor
		SNamedPath(const path& p) : m_path(p), m_internalName( pathToName(p) )
		{
		}

		bool operator==(const fschar* other) const { return m_path==other;}
		bool operator!=(const fschar* other) const { return !(*this==other);}

		//! Set the path.
		void setPath(const path& p)
		{
			m_path = p;
			m_internalName = pathToName(p);
		}

		//! Get the path.
		const path& getPath() const
		{
			return m_path;
		};

		const path& getInternalName() const
		{
			return m_internalName;
		}

		//TODO π”√Util
		const path getFileName() const
		{
			s32 index = m_internalName.findLast('/');
			if(index<0)
				return m_internalName;
			return m_internalName.subString(index);
		}

		//! Implicit cast to io::path
		operator core::stringc() const
		{
			return core::stringc(getPath());
		}
		//! Implicit cast to io::path
		operator core::stringw() const
		{
			return core::stringw(getPath());
		}

	protected:
		// convert the given path string to a name string.
		path pathToName(const path& p) const
		{
			path name(p);
			name.replace( '\\', '/' );
			name.makeLower();
			return name;
		}

	private:
		path m_path;
		path m_internalName;
	};
}
}
#endif