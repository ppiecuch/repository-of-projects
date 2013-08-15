#ifndef _MUI_IRESOURCE_H_
#define _MUI_IRESOURCE_H_

namespace mui{

	class IResource : public ISerializable{
	private:
		// constructors and operator =, without implementation, just for private
		IResource(const IResource&);
		IResource& operator = (const IResource&);
	protected:
		//std::string mResourceName;
		core::stringc m_name;
	public:

		IResource() {}
		virtual ~IResource() {}

		virtual void deserialization(xml::ElementPtr node)
		{
			node->findAttribute("name", m_name);
		}

		//const std::string& getResourceName() const
		const core::stringc& getName() const
		{
			return m_name;
		}
	};
}
#endif