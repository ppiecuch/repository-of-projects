#ifndef _YON_IO_YONXML_H_
#define _YON_IO_YONXML_H_

namespace yon{
namespace io{

	//XML节点类型
	enum ENUM_XML_NODE{
		//! No xml node. This is usually the node if you did not read anything yet.
		ENUM_XML_NODE_NONE,

		//! An xml element such as &lt;foo&gt;
		ENUM_XML_NODE_ELEMENT,

		//! End of an xml element such as &lt;/foo&gt;
		ENUM_XML_NODE_ELEMENT_END,

		//! Text within an xml element: &lt;foo&gt; this is the text. &lt;foo&gt;
		ENUM_XML_NODE_TEXT,

		//! An xml comment like &lt;!-- I am a comment --&gt; or a DTD definition.
		ENUM_XML_NODE_COMMENT,

		//! An xml cdata section like &lt;![CDATA[ this is some CDATA ]]&gt;
		ENUM_XML_NODE_CDATA,

		//! Unknown element.
		ENUM_XML_NODE_UNKNOWN
	};

	template<class char_type, class super_class>
	class IXMLReader : public super_class
	{
	public:

		//! Destructor
		virtual ~IXMLReader() {}

		//! Reads forward to the next xml node.
		/** \return Returns false, if there was no further node. */
		virtual bool read() = 0;

		//! Returns the type of the current XML node.
		virtual ENUM_XML_NODE getNodeType() const = 0;

		//! Returns attribute count of the current XML node.
		/** This is usually
		non null if the current node is ENUM_XML_NODE_ELEMENT, and the element has attributes.
		\return Returns amount of attributes of this xml node. */
		virtual unsigned int getAttributeCount() const = 0;

		//! Returns name of an attribute.
		/** \param idx: Zero based index, should be something between 0 and getAttributeCount()-1.
		\return Name of the attribute, 0 if an attribute with this index does not exist. */
		virtual const char_type* getAttributeName(int idx) const = 0;

		//! Returns the value of an attribute.
		/** \param idx: Zero based index, should be something between 0 and getAttributeCount()-1.
		\return Value of the attribute, 0 if an attribute with this index does not exist. */
		virtual const char_type* getAttributeValue(int idx) const = 0;

		//! Returns the value of an attribute.
		/** \param name: Name of the attribute.
		\return Value of the attribute, 0 if an attribute with this name does not exist. */
		virtual const char_type* getAttributeValue(const char_type* name) const = 0;

		//! Returns the value of an attribute in a safe way.
		/** Like getAttributeValue(), but does not
		return 0 if the attribute does not exist. An empty string ("") is returned then.
		\param name: Name of the attribute.
		\return Value of the attribute, and "" if an attribute with this name does not exist */
		virtual const char_type* getAttributeValueSafe(const char_type* name) const = 0;

		//! Returns the value of an attribute as integer.
		/** \param name Name of the attribute.
		\return Value of the attribute as integer, and 0 if an attribute with this name does not exist or
		the value could not be interpreted as integer. */
		virtual int getAttributeValueAsInt(const char_type* name) const = 0;

		//! Returns the value of an attribute as integer.
		/** \param idx: Zero based index, should be something between 0 and getAttributeCount()-1.
		\return Value of the attribute as integer, and 0 if an attribute with this index does not exist or
		the value could not be interpreted as integer. */
		virtual int getAttributeValueAsInt(int idx) const = 0;

		//! Returns the value of an attribute as float.
		/** \param name: Name of the attribute.
		\return Value of the attribute as float, and 0 if an attribute with this name does not exist or
		the value could not be interpreted as float. */
		virtual float getAttributeValueAsFloat(const char_type* name) const = 0;

		//! Returns the value of an attribute as float.
		/** \param idx: Zero based index, should be something between 0 and getAttributeCount()-1.
		\return Value of the attribute as float, and 0 if an attribute with this index does not exist or
		the value could not be interpreted as float. */
		virtual float getAttributeValueAsFloat(int idx) const = 0;

		//! Returns the name of the current node.
		/** Only non null, if the node type is ENUM_XML_NODE_ELEMENT.
		\return Name of the current node or 0 if the node has no name. */
		virtual const char_type* getNodeName() const = 0;

		//! Returns data of the current node.
		/** Only non null if the node has some
		data and it is of type ENUM_XML_NODE_TEXT or ENUM_XML_NODE_UNKNOWN. */
		virtual const char_type* getNodeData() const = 0;

		//! Returns if an element is an empty element, like &lt;foo />
		virtual bool isEmptyElement() const = 0;
	};
}
}
#endif