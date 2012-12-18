#ifndef _YON_IO_CXMLREADERIMPL_H_
#define _YON_IO_CXMLREADERIMPL_H_

#include "yonXML.h"
#include "IStream.h"
#include "yonString.h"
#include "yonArray.h"
#include "fastatof.h"

#include "ILogger.h"

namespace yon{
namespace io{

	template<class char_type, class superclass>
	class CXMLReaderImpl : public IXMLReader<char_type, superclass>
	{
	private:
		struct SAttribute
		{
			core::string<char_type> Name;
			core::string<char_type> Value;
		};

		IReadStream* m_pStream;
		ENUM_ENCODING m_encoding;									// encoding of the xml file
		s32 m_iSizeOfCharType;										// byte size of the char_type

		ENUM_XML_NODE m_currentNodeType;							// type of the currently parsed node
		core::string<char_type> m_nodeName;							// name of the node currently in
		core::string<char_type> m_emptyString;						// empty string to be returned by getSafe() methods

		//When you declare your vector, 
		//typedef vector<pair<T,T>> stacktype;
		//some compilers will choke with the <pair<T,T>> part because it wants it the have a space between the two >. 
		//For example:
		//typedef vector<pair<T,T> > stacktype;
		//instead of 
		//typedef vector<pair<T,T>> stacktype;
		//reference: http://www.velocityreviews.com/forums/t602903-problem-with-vector-pair-type-type.html
		core::array<core::string<char_type> > m_specialCharacters;	// see createSpecialCharacterList()
		core::array<SAttribute> m_attributes;						// attributes of current element

		bool m_bIsEmptyElement;       // is the currently parsed node empty?

		// finds a current attribute by name, returns 0 if not found
		const SAttribute* getAttributeByName(const char_type* name) const
		{
			if (!name)
				return 0;

			core::string<char_type> n = name;

			for (int i=0; i<(int)m_attributes.size(); ++i)
				if (m_attributes[i].Name == n)
					return &m_attributes[i];

			return 0;
		}


		inline char_type readNext()
		{
#if 1
			switch(m_iSizeOfCharType)
			{
			case 2:
				return (char_type)m_pStream->readChar16();
			case 4:
				return (char_type)m_pStream->readChar32();
			default:
				return (char_type)m_pStream->readChar();
			}
#else
			if(m_iSizeOfCharType==1)
				return (char_type)m_pStream->readChar();
			else
				return (char_type)m_pStream->readWChar();
#endif
		}

		void readString(core::string<char_type>& str,u32 start,u32 end)
		{
			u32 temp=m_pStream->getPos();
			m_pStream->seek(start);
			YON_DEBUG_BREAK_IF(end-start>65535);
			static u8 buffer[65535];
			m_pStream->read(buffer,end-start-1);
			str.build(buffer,end-start-1);
			m_pStream->seek(temp);
		}

		inline bool reachEnd()
		{
			YON_DEBUG_BREAK_IF(m_pStream->getPos()>m_pStream->getSize());
			return m_pStream->getPos()==m_pStream->getSize();
		}

		inline u32 trim(u32 start,u32 end)
		{
			YON_DEBUG_BREAK_IF(end<=start);
			u32 origin=m_pStream->getPos();
			m_pStream->seek(start);
			core::string<char_type> s;
			readString(s,start,end);
			u32 oldLen=s.length();
			s=s.trim();
			u32 newLen=s.length();
			if(newLen>0)
			{
				// set current text to the parsed text, and replace xml special characters
				m_nodeName = replaceSpecialCharacters(s);
				// current XML node type is text
				m_currentNodeType=ENUM_XML_NODE_TEXT;
				m_pStream->seek(end-1);
				return newLen;
			}
			if(origin>0)
				m_pStream->seek(origin-1);
			return newLen;
		}

		/*
		//trim the stream from start to end,if the result is not null return true,or return false
		inline bool trim(u32 start,u32 end)
		{
			YON_DEBUG_BREAK_IF(end<=start);
			u32 origin=m_pStream->getPos();
			m_pStream->seek(start);
			char_type c;
			do{
				c=readNext();
			}while(isWhiteSpace(c));
			if(m_pStream->getPos()==end)
				return false;

			// set current text to the parsed text, and replace xml special characters
			core::string<char_type> s;
			readString(s,start-1,end);
			m_nodeName = replaceSpecialCharacters(s);
			// current XML node type is text
			m_currentNodeType=ENUM_XML_NODE_TEXT;

			m_pStream->seek(origin-1);
			return true;
		}
		*/

		//! ignores an xml definition like <?xml something />
		void ignoreDefinition()
		{
			m_currentNodeType = ENUM_XML_NODE_UNKNOWN;

			// move until end marked with '>' reached
			char_type c;
			do{
				c=readNext();
			}while(c!= L'>');
		}

		// Reads the current xml node
		// return false if no further node is found
		bool parseCurrentNode()
		{
			u32 start=m_pStream->getPos();
			char_type c=0;
			do{
				c=readNext();
			}while(c!=L'<'&&c);

			if(!c)
				return false;
			u32 end=m_pStream->getPos();
			//因为每次读都包含有"<"符号，所以用2
			//“..><...”这样的排列不进行trim
			if(end-start>=2&&trim(start,end)>0)
			{
				Logger->debug("text:%s\n",m_nodeName.c_str());
				return true;
			}
			/*
			if(end-start>=2&&trim(start,end)){
				Logger->debug("text:%s\n",m_nodeName.c_str());
				return true;
			}*/
			c=readNext();
			//对于Close元素需要读取多一个符号
			char_type tmp=readNext();
			if(tmp==L'/')
				c=tmp;
			else
				m_pStream->seek(-1,true);
			switch(c)
			{
			case L'/':
				parseClosingXMLElement();
				Logger->debug("</%s>\n",m_nodeName.c_str());
				break;
			case L'?':
				Logger->debug("header->ignore\n");
				ignoreDefinition();	
				break;
			case L'!':
				if (!parseCDATA()){
					parseComment();	
					Logger->debug("comment:%s\n",m_nodeName.c_str());
				}
				else
					Logger->debug("cdata:%s\n",m_nodeName.c_str());
				break;
			default:
				{
					parseOpeningXMLElement();
					break;
				}
			}
			return true;
		}

		//! parses a comment
		void parseComment()
		{
			m_currentNodeType = ENUM_XML_NODE_COMMENT;
			u32 commentStart=m_pStream->getPos()+1;

			char_type c;
			s32 count = 1;

			do{
				c=readNext();
				if(c==L'>')
					--count;
				else if(c==L'<')
					++count;
			}while(count);

			u32 commentEnd=m_pStream->getPos()-2;
			readString(m_nodeName,commentStart,commentEnd);
		}

		//! parses a possible CDATA section, returns false if begin was not a CDATA section
		bool parseCDATA()
		{
			char_type c=readNext();
			if(c!=L'[')
				return false;

			m_currentNodeType = ENUM_XML_NODE_CDATA;
			// skip '<![CDATA['
			m_pStream->seek(6,true);
			if(reachEnd())
				return true;

			u32 cdataStart=m_pStream->getPos();
			u32 cdataEnd=0;
			char_type lastc=0;
			char_type lastlastc=0;
			do{
				lastlastc=lastc;
				lastc=c;
				c=readNext();
				if(c==L'>'&&lastc==L']'&&lastlastc==L']')
					cdataEnd=m_pStream->getPos()-2;
			}while(c&&!cdataEnd);

			if(cdataEnd)
				readString(m_nodeName,cdataStart,cdataEnd);
			else
				m_nodeName="";

			return true;
		}

		//! parses an closing xml tag
		void parseClosingXMLElement()
		{
			m_currentNodeType = ENUM_XML_NODE_ELEMENT_END;
			m_bIsEmptyElement = false;
			m_attributes.clear();

			u32 nameStart=m_pStream->getPos();
			char_type c;

			do{
				c=readNext();
			}while(c!=L'>');

			u32 nameEnd=m_pStream->getPos();
			readString(m_nodeName,nameStart,nameEnd);
		}


		//! parses an opening xml element and reads attributes
		void parseOpeningXMLElement()
		{
			m_currentNodeType = ENUM_XML_NODE_ELEMENT;
			m_bIsEmptyElement=false;
			m_attributes.clear();

			// find name
			u32 nameStart = m_pStream->getPos();

			// find end of element
			char_type c=0;
			char_type lastc=0;
			do{
				lastc=c;
				c=readNext();
			}while(c!=L'>'&&!isWhiteSpace(c));

			u32 nameEnd = m_pStream->getPos();

			// find Attributes
			while(c!=L'>')
			{
				if(isWhiteSpace(c))
					c=readNext();
				else
				{
					if(c!=L'/')
					{
						// we've got an attribute

						// read the attribute names
						u32 attributeNameBegin = m_pStream->getPos()-1;

						do{
							c=readNext();
						}while(c!=L'='&&!isWhiteSpace(c));

						u32 attributeNameEnd =  m_pStream->getPos();

						// read the attribute value
						// check for quotes and single quotes, thx to murphy
						do{
							c=readNext();
						}while(c!=L'\"'&&c!='\''&&c);

						if (!c) // malformatted xml file
							return;

						char_type attributeQuoteChar = c;

						u32 attributeValueBegin = m_pStream->getPos();

						do{
							c=readNext();
						}while(c!=attributeQuoteChar&&c);

						if (!c) // malformatted xml file
							return;

						u32 attributeValueEnd = m_pStream->getPos();

						SAttribute attr;
						//attr.Name = core::string<char_type>(attributeNameBegin, (int)(attributeNameEnd - attributeNameBegin));
						readString(attr.Name,attributeNameBegin,attributeNameEnd);

						//core::string<char_type> s(attributeValueBegin, (int)(attributeValueEnd - attributeValueBegin));
						core::string<char_type> s;
						readString(s,attributeValueBegin,attributeValueEnd);

						attr.Value = replaceSpecialCharacters(s);
						m_attributes.push_back(attr);
					}
					else
					{
						// tag is closed directly
						//++P;
						m_bIsEmptyElement = true;
						readNext();
						break;
					}
					c=readNext();
				}
			}

			// check if this tag is closing directly
			if (nameEnd > nameStart && lastc == L'/')
			{
				// directly closing tag
				m_bIsEmptyElement = true;
				//endName--;
				--nameEnd;
			}

			//NodeName = core::string<char_type>(startName, (int)(endName - startName));
			//++P;
			//readNext();
			readString(m_nodeName,nameStart,nameEnd);

			core::string<char_type> str;
			for(u32 i=0;i<m_attributes.size();++i)
				if(i<m_attributes.size()-1)
					str+=core::string<char_type>("%s:%s,",m_attributes[i].Name.c_str(),m_attributes[i].Value.c_str());
				else
					str+=core::string<char_type>("%s:%s",m_attributes[i].Name.c_str(),m_attributes[i].Value.c_str());
			if(c==L'/')
				Logger->debug("<%s/>(%d):%s\n",m_nodeName.c_str(),m_attributes.size(),str.c_str());
			else
				Logger->debug("<%s>(%d):%s\n",m_nodeName.c_str(),m_attributes.size(),str.c_str());
		}

		// replaces xml special characters in a string and creates a new one
		core::string<char_type> replaceSpecialCharacters(core::string<char_type>& origstr)
		{
			int pos = origstr.findFirst(L'&');
			int oldPos = 0;

			if (pos == -1)
				return origstr;

			core::string<char_type> newstr;

			while(pos != -1 && pos < (int)origstr.length()-2)
			{
				// check if it is one of the special characters

				int specialChar = -1;
				for (int i=0; i<(int)m_specialCharacters.size(); ++i)
				{
					const char_type* p = &origstr.c_str()[pos]+1;

					if (equalsn(&m_specialCharacters[i][1], p, m_specialCharacters[i].length()-1))
					{
						specialChar = i;
						break;
					}
				}

				if (specialChar != -1)
				{
					newstr.append(origstr.subString(oldPos, pos - oldPos));
					newstr.append(m_specialCharacters[specialChar][0]);
					pos += m_specialCharacters[specialChar].length();
				}
				else
				{
					newstr.append(origstr.subString(oldPos, pos - oldPos + 1));
					pos += 1;
				}

				// find next &
				oldPos = pos;
				pos = origstr.findNext(L'&', pos);		
			}

			if (oldPos < (int)origstr.length()-1)
				newstr.append(origstr.subString(oldPos, origstr.length()-oldPos));

			return newstr;
		}

		//! returns true if a character is whitespace
		inline bool isWhiteSpace(char_type c)
		{
			return (c==' ' || c=='\t' || c=='\n' || c=='\r');
		}

		//! generates a list with xml special characters
		void createSpecialCharacterList()
		{
			// list of strings containing special symbols, 
			// the first character is the special character,
			// the following is the symbol string without trailing &.

			m_specialCharacters.push_back("&amp;");
			m_specialCharacters.push_back("<lt;");
			m_specialCharacters.push_back(">gt;");
			m_specialCharacters.push_back("\"quot;");
			m_specialCharacters.push_back("'apos;");

		}


		//! compares the first n characters of the strings
		bool equalsn(const char_type* str1, const char_type* str2, int len)
		{
			int i;
			for(i=0; str1[i] && str2[i] && i < len; ++i)
				if (str1[i] != str2[i])
					return false;

			// if one (or both) of the strings was smaller then they
			// are only equal if they have the same length
			return (i == len) || (str1[i] == 0 && str2[i] == 0);
		}

	public:
		CXMLReaderImpl(IReadStream* stream)
			: m_pStream(stream), //TextData(0), P(0), TextBegin(0), TextSize(0),SourceFormat(ENUM_ENCODING_ASCII),
			m_currentNodeType(ENUM_XML_NODE_NONE),m_iSizeOfCharType(sizeof(char_type))
		{
			if(!stream||stream->pointer()==NULL){
				m_pStream=NULL;
				Logger->debug(YON_LOG_WARN_FORMAT,"Instance empty CXMLReaderImpl");
				return;
			}

			// create list with special characters
			createSpecialCharacterList();

			stream->grab();

			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CXMLReaderImpl");
		}

		virtual ~CXMLReaderImpl()
		{
			if(m_pStream)
				m_pStream->drop();
			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CXMLReaderImpl");
		}


		//! Reads forward to the next xml node. 
		//! \return Returns false, if there was no further node. 
		virtual bool read()
		{
			if(!m_pStream||reachEnd())
				return false;
			return parseCurrentNode();
		}

		//! Returns the type of the current XML node.
		virtual ENUM_XML_NODE getNodeType() const
		{
			return m_currentNodeType;
		}

		//! Returns attribute count of the current XML node.
		virtual unsigned int getAttributeCount() const
		{
			return m_attributes.size();
		}

		//! Returns name of an attribute.
		virtual const char_type* getAttributeName(int idx) const
		{
			if ((u32)idx >= m_attributes.size())
				return m_emptyString.c_str();

			return m_attributes[idx].Name.c_str();
		}

		//! Returns the value of an attribute. 
		virtual const char_type* getAttributeValue(int idx) const
		{
			if ((unsigned int)idx >= m_attributes.size())
				return m_emptyString.c_str();

			return m_attributes[idx].Value.c_str();
		}

		//! Returns the value of an attribute. 
		virtual const char_type* getAttributeValue(const char_type* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return m_emptyString.c_str();

			return attr->Value.c_str();
		}

		//! Returns the value of an attribute
		virtual const char_type* getAttributeValueSafe(const char_type* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
			{
				YON_DEBUG("empty\r\n");
				return m_emptyString.c_str();
			}

			return attr->Value.c_str();
		}

		//! Returns the value of an attribute as integer. 
		int getAttributeValueAsInt(const char_type* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			core::stringc c(attr->Value.c_str());
			return core::strtol10(c.c_str());
		}

		//! Returns the value of an attribute as integer. 
		int getAttributeValueAsInt(int idx) const
		{
			const char_type* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			core::stringc c(attrvalue);
			return core::strtol10(c.c_str());
		}

		//! Returns the value of an attribute as float. 
		float getAttributeValueAsFloat(const char_type* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			core::stringc c = attr->Value.c_str();
			return core::fastatof(c.c_str());
		}

		//! Returns the value of an attribute as float. 
		float getAttributeValueAsFloat(int idx) const
		{
			const char_type* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			core::stringc c = attrvalue;
			return core::fastatof(c.c_str());
		}

		//! Returns the name of the current node.
		virtual const char_type* getNodeName() const
		{
			return m_nodeName.c_str();
		}

		//TODO为什么跟getNodeName一样的实现?
		//! Returns data of the current node.
		virtual const char_type* getNodeData() const
		{
			return m_nodeName.c_str();
		}

		//! Returns if an element is an empty element, like <foo />
 		virtual bool isEmptyElement() const
 		{
 			return m_bIsEmptyElement;
 		}

		virtual ENUM_ENCODING getEncoding() const
		{
			return m_encoding;
		}

	};
}
}
#endif