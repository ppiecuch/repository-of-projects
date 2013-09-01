#ifndef _MUI_XMLDOCUMENT_H_
#define _MUI_XMLDOCUMENT_H_

#include "MUI_Prepare.h"

namespace mui{
namespace xml{

	struct ElementType
	{
		enum Enum
		{
			Comment,
			Declaration,
			Normal,
			COUNT
		};

		ElementType(Enum value = COUNT) : value(value) {}
		friend bool operator == (ElementType const& a, ElementType const& b)
		{
			return a.value == b.value;
		}
		friend bool operator != (ElementType const& a, ElementType const& b)
		{
			return a.value != b.value;
		}

	private:
		Enum value;
	};

	struct ErrorType
	{
		enum Enum
		{
			OpenFileFail,
			CreateFileFail,
			IncorrectContent,
			NotClosedElements,
			NoXMLDeclaration,
			CloseNotOpenedElement,
			InconsistentOpenCloseElements,
			MoreThanOneXMLDeclaration,
			MoreThanOneRootElement,
			IncorrectAttribute,
			COUNT
		};

		ErrorType(Enum value = COUNT) : value(value) { }

		core::stringc print() const
		{
			return getValueName(value);
		}

	private:
		const char* getValueName(s32 index) const
		{
			static const char* values[COUNT + 1] =
			{
				"Failed to open XML file",
				"Failed to ceate XML file",
				"XML file contain incorrect content",
				"XML file contain not closed elements",
				"XML file without declaration",
				"XML file contain closed but not opened element",
				"XML file contain inconsistent elements",
				"XML file contain more than one declaration",
				"XML file contain more than one root element",
				"XML file contain incorrect attribute",
				""
			};
			return values[(index < COUNT && index >= 0) ? index : COUNT];
		}
	private:
		Enum value;
	};

	class Element;
	class Document;

	//typedef Element* ElementPtr;
	//typedef std::pair<std::string, std::string> PairAttribute;
	typedef struct{
		PairAttribute(const core::stringc& key,const core::stringc& value)
			:Key(key),value(Value){}
		core::stringc Key;
		core::stringc Value;
	} PairAttribute;
	//typedef std::vector<PairAttribute> VectorAttributes;
	//typedef std::vector<ElementPtr> VectorElement;

	//----------------------------------------------------------------------//
	// class ElementEnumerator
	//----------------------------------------------------------------------//
	/*class MYGUI_EXPORT ElementEnumerator
	{
		friend class Element;

	private:
		ElementEnumerator(VectorElement::iterator _begin, VectorElement::iterator _end);

	public:
		bool next();
		bool next(const std::string& _name);

		ElementPtr operator->() const;
		ElementPtr current();

	private:
		bool m_first;
		VectorElement::iterator m_current, m_end;
	};*/

	//----------------------------------------------------------------------//
	// class Element
	//----------------------------------------------------------------------//
	class MUI_API Element
	{
		friend class Document;

	public:
		~Element();

	private:
		//Element(const std::string& _name, ElementPtr _parent, ElementType _type = ElementType::Normal, const std::string& _content = "");
		Element(const core::stringc& name, Element* parent, ElementType type = ElementType::Normal, const core::stringc& content = "");

		//void save(std::ostream& _stream, size_t _level);

	public:
		ElementPtr createChild(const core::stringc& name, const core::stringc& content = "", ElementType type = ElementType::Normal);
		void removeChild(Element* child);

		template <typename T>
		void addAttribute(const core::stringc& key, const T& value)
		{
			//addAttribute(key, utility::toString(value));
			addAttribute(key,core::stringc::formatAuto(value));
		}

		void addAttribute(const core::stringc& key, const core::stringc& value);

		void removeAttribute(const core::stringc& key);

		void setAttribute(const core::stringc& key, const core::stringc& value);

		template <typename T>
		void addContent(const T& content)
		{
			//addContent(utility::toString(content));
			addContent(core::stringc::formatAuto(content));
		}

		void addContent(const core::stringc& content);

		template <typename T>
		void setContent(const T& content)
		{
			//setContent(utility::toString(content));
			setContent(core::stringc::formatAuto(content));
		}

		void setContent(const core::stringc& content);

		void clear();

		bool findAttribute(const core::stringc& name, core::stringc& value);
		core::stringc findAttribute(const core::stringc& name);

		const core::stringc& getName() const;

		const core::stringc& getContent() const;

		const core::array<PairAttribute>& getAttributes() const;

		Element* getParent() const;

		//ElementEnumerator getElementEnumerator();

		ElementType getType() const;

		Element* createCopy();

	private:
		//std::string mName;
		//std::string mContent;
		//VectorAttributes mAttributes;
		//VectorElement mChilds;
		//ElementPtr mParent;
		//ElementType mType;

		core::stringc m_name;
		core::stringc m_content;
		core::array<PairAttribute> m_attributes;
		core::array<Element*> m_children;
		Element* m_pParent;
		ElementType m_type;
	};

	//----------------------------------------------------------------------//
	// class Document
	//----------------------------------------------------------------------//
	class MUI_API Document
	{
	public:
		Document();
		~Document();

		bool open(const core::stringc& filename);

		//bool open(const std::wstring& _filename);

		// §à§ä§Ü§â§í§Ó§Ñ§Ö§ä §à§Ò§í§é§ß§í§Þ §á§à§ä§à§Ü§à§Þ
		//bool open(std::istream& _stream);

		//bool open(const UString& _filename);

		//bool open(IDataStream* data);

		bool save(const core::stringc& filename);

		//bool save(const std::wstring& _filename);

		//bool save(std::ostream& _stream);

		//bool save(const UString& _filename);

		void clear();

		core::stringc getLastError();

		void clearLastError();

		Element* createDeclaration(const core::stringc& version = "1.0", const core::stringc& encoding = "UTF-8");
		Element* createRoot(const core::stringc& name);

		Element* getRoot() const;

		/*obsolete:
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : ElementPtr Document::createDeclaration(const std::string& _version, const std::string& _encoding)")
			ElementPtr createInfo(const std::string& _version = "1.0", const std::string& _encoding = "UTF-8")
		{
			return createDeclaration(_version, _encoding);
		}

#endif // MYGUI_DONT_USE_OBSOLETE*/

	private:
		void setLastFileError(const core::stringc& filename);
		//void setLastFileError(const std::wstring& _filename);

		bool parseTag(Element*& currentNode, core::stringc content);

		bool checkPair(core::stringc& key, core::stringc& value);

		bool parseLine(core::stringc& line, Element* &element);

		//size_t find(const std::string& _text, char _char, size_t _start = 0);
		s32 find(const core::stringc& text, c8 char, u32 start = 0);

		void clearDeclaration();
		void clearRoot();

	private:
		//ElementPtr mRoot;
		//ElementPtr mDeclaration;
		//ErrorType mLastError;
		//std::string mLastErrorFile;
		//size_t mLine;
		//size_t mCol;

		Element* m_pRoot;
		Element* m_pDeclaration;
		ErrorType m_lastError;
		core::stringc m_lastErrorFile;
		u32 m_uLine;
		u32 m_uColumn;

	}; // class Document

	//MYGUI_OBSOLETE("use : class MyGUI::xml::ElementEnumerator")
	//typedef ElementEnumerator xmlNodeIterator;
	//MYGUI_OBSOLETE("use : class MyGUI::xml::ElementPtr")
	//typedef ElementPtr xmlNodePtr;
	//MYGUI_OBSOLETE("use : class MyGUI::xml::Document")
	typedef Document xmlDocument;

}
}
#endif