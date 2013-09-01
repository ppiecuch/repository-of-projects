#include "MUI_XMLDocument.h"

namespace mui{
namespace xml{

	//----------------------------------------------------------------------//
	// class Element
	//----------------------------------------------------------------------//
Element::Element(const core::stringc& name, Element* parent, ElementType type, const core::stringc& content) :
	m_name(name),
	m_content(content),
	m_pParent(parent),
	m_type(type)
{
}

Element::~Element()
{
	/*for (VectorElement::iterator iter = mChilds.begin(); iter != mChilds.end(); ++iter)
	{
		delete *iter;
	}
	mChilds.clear();*/
	clear();
}

/*void Element::save(std::ostream& _stream, size_t _level)
{
	// сначала табуляции намутим
	for (size_t tab = 0; tab < _level; ++tab)
		_stream  << "    ";

	// теперь заголовок тега
	if (mType == ElementType::Declaration)
		_stream << "<?";
	else if (mType == ElementType::Comment)
		_stream << "<!--";
	else
		_stream << "<";

	_stream << mName;

	for (VectorAttributes::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		_stream << " " << iter->first << "=\"" << utility::convert_to_xml(iter->second) << "\"";
	}

	bool empty = mChilds.empty();
	// если детей нет то закрываем
	if (empty && mContent.empty())
	{
		if (mType == ElementType::Declaration)
			_stream << "?>\n";
		else if (mType == ElementType::Comment)
			_stream << "-->\n";
		else
			_stream << "/>\n";
	}
	else
	{
		_stream << ">";
		if (!empty)
			_stream << "\n";
		// если есть тело то сначало оно
		if (!mContent.empty())
		{
			if (!empty)
			{
				for (size_t tab = 0; tab <= _level; ++tab) _stream  << "    ";
			}
			_stream << utility::convert_to_xml(mContent);

			if (!empty)
				_stream << "\n";
		}
		// если есть детишки путь сохранятся
		for (size_t child = 0; child < mChilds.size(); child++)
		{
			mChilds[child]->save(_stream, _level + 1);
		}

		if (!empty)
		{
			for (size_t tab = 0; tab < _level; ++tab)
				_stream  << "    ";
		}
		_stream << "</" << mName << ">\n";
	}
}*/

Element* Element::createChild(const core::stringc& name, const core::stringc& content, ElementType type)
{
	Element* node = new Element(name, this, type, content);
	m_children.push_back(node);
	return node;
}

void Element::removeChild(Element* child)
{
	/*VectorElement::iterator item = std::find(mChilds.begin(), mChilds.end(), _child);
	if (item != mChilds.end())
	{
		delete (*item);
		mChilds.erase(item);
	}*/
	for(u32 i=0;i<m_children.size();++i)
	{
		if(m_children[i]==child)
		{
			delete m_children[i];
			m_children.erase(i);
		}
	}
}

void Element::clear()
{
	/*for (VectorElement::iterator iter = mChilds.begin(); iter != mChilds.end(); ++iter) delete *iter;
	mChilds.clear();
	mContent.clear();
	mAttributes.clear();*/
	for(u32 i=0;i<m_children.size();++i)
	{
		delete m_children[i];
	}
	m_children.clear();
	m_content="";
	m_attributes.clear()
}

bool Element::findAttribute(const core::stringc& name, core::stringc& value)
{
	/*for (VectorAttributes::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		if ( (*iter).first == _name)
		{
			_value = (*iter).second;
			return true;
		}
	}
	return false;*/
	for(u32 i=0;i<m_attributes.size();++i)
	{
		if(m_attributes[i].Key==name)
		{
			value=m_attributes[i].Value;
			return true;
		}
	}
	return false;
}

core::stringc Element::findAttribute(const core::stringc& name)
{
	/*for (VectorAttributes::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		if ((*iter).first == _name)
			return (*iter).second;
	}
	return "";*/

	core::stringc value;
	findAttribute(name,value);
	return value;
}

void Element::addAttribute(const core::stringc& key, const core::stringc& value)
{
	m_attributes.push_back(PairAttribute(key, value));
}

void Element::removeAttribute(const core::stringc& key)
{
	for(u32 index = 0; index < m_attributes.size(); ++index)
	{
		if (m_attributes[index].Key == key)
		{
			m_attributes.erase(index);
			return;
		}
	}
}

Element* Element::createCopy()
{
	Element* elem = new Element(m_name, NULL, m_type, m_content);
	elem->m_attributes = m_attributes;

	/*for (VectorElement::iterator iter = mChilds.begin(); iter != mChilds.end(); ++iter)
	{
		Element* child = (*iter)->createCopy();
		child->mParent = elem;
		elem->mChilds.push_back(child);
	}*/
	for(u32 i=0;i<m_children.size();++i)
	{
		Element* child = *m_children[i]->createCopy();
		child->m_pParent = elem;
		elem->m_children.push_back(child);
	}
	return elem;
}

void Element::setAttribute(const core::stringc& key, const core::stringc& value)
{
	for (u32 index = 0; index < m_attributes.size(); ++index)
	{
		if (m_attributes[index].Key == key)
		{
			m_attributes[index].Value = value;
			return;
		}
	}
	m_attributes.push_back(PairAttribute(key, value));
}

void Element::addContent(const core::stringc& content)
{
	if (m_content.empty())
	{
		m_content = content;
	}
	else
	{
		m_content += " ";
		m_content += content;
	}
}

void Element::setContent(const core::stringc& content)
{
	m_content = content;
}

const core::stringc& Element::getName() const
{
	return m_name;
}

const core::stringc& Element::getContent() const
{
	return m_content;
}

const core::array<PairAttribute>& Element::getAttributes() const
{
	return m_attributes;
}

Element* Element::getParent() const
{
	return m_pParent;
}

/*ElementEnumerator Element::getElementEnumerator()
{
	return ElementEnumerator(mChilds.begin(), mChilds.end());
}*/

Element* Element::getType() const
{
	return m_type;
}

/*
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && !defined(STLPORT)
inline void open_stream(std::ofstream& _stream, const std::wstring& _wide)
{
	_stream.open(_wide.c_str());
}
inline void open_stream(std::ifstream& _stream, const std::wstring& _wide)
{
	_stream.open(_wide.c_str());
}
#else
inline void open_stream(std::ofstream& _stream, const std::wstring& _wide)
{
	_stream.open(UString(_wide).asUTF8_c_str());
}
inline void open_stream(std::ifstream& _stream, const std::wstring& _wide)
{
	_stream.open(UString(_wide).asUTF8_c_str());
}
#endif
*/

//----------------------------------------------------------------------//
// class Document
//----------------------------------------------------------------------//
Document::Document():
m_pRoot(NULL),
m_pDeclaration(NULL),
m_lastErrorFile(),
m_uLine(0),
m_uColumn(0)
{
}

Document::~Document()
{
	clear();
}

bool Document::open(const core::stringc& filename)
{
	/*std::ifstream stream;
	stream.open(_filename.c_str());

	if (!stream.is_open())
	{
		mLastError = ErrorType::OpenFileFail;
		setLastFileError(_filename);
		return false;
	}

	bool result = open(stream);

	stream.close();
	return result;*/

	//TODO
}

bool Document::save(const core::stringc& filename)
{
	/*std::ofstream stream;
	stream.open(_filename.c_str());

	if (!stream.is_open())
	{
		mLastError = ErrorType::CreateFileFail;
		setLastFileError(_filename);
		return false;
	}

	bool result = save(stream);

	if (!result)
	{
		setLastFileError(_filename);
	}

	stream.close();
	return result;*/

	//TODO
}

/*
bool Document::open(const std::wstring& _filename)
{
	std::ifstream stream;
	open_stream(stream, _filename);

	if (!stream.is_open())
	{
		mLastError = ErrorType::OpenFileFail;
		setLastFileError(_filename);
		return false;
	}

	bool result = open(stream);

	stream.close();
	return result;
}


bool Document::open(const std::wstring& _filename)
{
	std::ifstream stream;
	open_stream(stream, _filename);

	if (!stream.is_open())
	{
		mLastError = ErrorType::OpenFileFail;
		setLastFileError(_filename);
		return false;
	}

	bool result = open(stream);

	stream.close();
	return result;
}
bool Document::open(std::istream& _stream)
{
	DataStream* data = new DataStream(&_stream);

	bool result = open(data);
	delete data;

	return result;
}

// сохраняет файл, имя файла в кодировке utf16 или utf32
bool Document::save(const std::wstring& _filename)
{
	std::ofstream stream;
	open_stream(stream, _filename);

	if (!stream.is_open())
	{
		mLastError = ErrorType::CreateFileFail;
		setLastFileError(_filename);
		return false;
	}

	bool result = save(stream);

	if (!result)
	{
		setLastFileError(_filename);
	}

	stream.close();
	return result;
}

// открывает обычным потоком
bool Document::open(IDataStream* _stream)
{
	clear();

	// это текущая строка для разбора
	std::string line;
	// это строка из файла
	std::string read;
	// текущий узел для разбора
	ElementPtr currentNode = 0;

	while (!_stream->eof())
	{
		// берем новую строку
		_stream->readline(read, '\n');
		if (read.empty())
			continue;
		if (read[read.size() - 1] == '\r')
			read.erase(read.size() - 1, 1);
		if (read.empty())
			continue;

		mLine ++;
		mCol = 0; // потом проверить на многострочных тэгах
		if (read.empty())
			continue;
		// текущая строка для разбора и то что еще прочитали
		line += read;

		if (!parseLine(line, currentNode))
		{
			return false;
		}

	} // while (!stream.eof())

	if (currentNode)
	{
		mLastError = ErrorType::NotClosedElements;
		return false;
	}

	return true;
}

bool Document::save(std::ostream& _stream)
{
	if (!mDeclaration)
	{
		mLastError = ErrorType::NoXMLDeclaration;
		return false;
	}

	// заголовок utf8
	_stream << (char)0xEFu;
	_stream << (char)0xBBu;
	_stream << (char)0xBFu;

	mDeclaration->save(_stream, 0);
	if (mRoot)
		mRoot->save(_stream, 0);

	return true;
}
*/
void Document::clear()
{
	clearDeclaration();
	clearRoot();
	m_uLine = 0;
	m_uColumn = 0;
}

bool Document::parseTag(Element*& currentNode, core::stringc content)
{
	/*
	// убераем лишнее
	MyGUI::utility::trim(_content);

	if (_content.empty())
	{
		// создаем пустой тег
		if (_currentNode)
		{
			_currentNode = _currentNode->createChild("");
		}
		else
		{
			_currentNode = new Element("", 0);
			// если это первый то запоминаем
			if (!mRoot)
				mRoot = _currentNode;
		}
		return true;
	}

	char simbol = _content[0];
	bool tagDeclaration = false;

	// проверяем на коментарии
	if (simbol == '!')
	{
		if (_currentNode != 0)
		{
			//_currentNode->createChild("", _content, ElementType::Comment);
		}
		return true;
	}
	// проверяем на информационный тег
	else if (simbol == '?')
	{
		tagDeclaration = true;
		_content.erase(0, 1); // удаляем первый символ
	}

	size_t start = 0;
	size_t end = 0;
	// проверяем на закрытие тега
	if (simbol == '/')
	{
		if (_currentNode == 0)
		{
			// чета мы закрывам а ниче даже и не открыто
			if (!mRoot)
			{
				mLastError = ErrorType::CloseNotOpenedElement;
				return false;
			}
		}
		// обрезаем имя тэга
		start = _content.find_first_not_of(" \t", 1);
		if (start == _content.npos)
		{
			// тег пустой
			_content.clear();
		}
		else
		{
			end = _content.find_last_not_of(" \t");
			_content = _content.substr(start, end - start + 1);
		}
		// проверяем соответствие открывающего и закрывающего тегов
		if (_currentNode->getName() != _content)
		{
			mLastError = ErrorType::InconsistentOpenCloseElements;
			return false;
		}
		// а теперь снижаем текущий узел вниз
		_currentNode = _currentNode->getParent();
	}
	else
	{
		// выделяем имя до первого пробела или закрывающего тега
		std::string cut = _content;
		start = _content.find_first_of(" \t/?", 1); // << превед
		if (start != _content.npos)
		{
			cut = _content.substr(0, start);
			_content = _content.substr(start);
		}
		else
		{
			_content.clear();
		}

		if (_currentNode)
		{
			_currentNode = _currentNode->createChild(cut);
		}
		else
		{
			if (tagDeclaration)
			{
				// информационный тег
				if (mDeclaration)
				{
					mLastError = ErrorType::MoreThanOneXMLDeclaration;
					return false;
				}
				_currentNode = new Element(cut, 0, ElementType::Declaration);
				mDeclaration = _currentNode;
			}
			else
			{
				// рутовый тег
				if (mRoot)
				{
					mLastError = ErrorType::MoreThanOneRootElement;
					return false;
				}
				_currentNode = new Element(cut, 0, ElementType::Normal);
				mRoot = _currentNode;
			}
		}

		// проверим на пустоту
		start = _content.find_last_not_of(" \t");
		if (start == _content.npos)
			return true;

		// сразу отделим закрывающийся тэг
		bool close = false;
		if ((_content[start] == '/') || (_content[start] == '?'))
		{
			close = true;
			// не будем резать строку, просто поставим пробел
			_content[start] = ' ';
			// проверим на пустоту
			start = _content.find_last_not_of(" \t");
			if (start == _content.npos)
			{
				// возвращаем все назад и уходим
				_currentNode = _currentNode->getParent();
				return true;
			}
		}

		// а вот здесь уже в цикле разбиваем на атрибуты
		while (true)
		{
			// ищем равно
			start = _content.find('=');
			if (start == _content.npos)
			{
				mLastError = ErrorType::IncorrectAttribute;
				return false;
			}
			// ищем вторые ковычки
			end = _content.find_first_of("\"\'", start + 1);
			if (end == _content.npos)
			{
				mLastError = ErrorType::IncorrectAttribute;
				return false;
			}
			end = _content.find_first_of("\"\'", end + 1);
			if (end == _content.npos)
			{
				mLastError = ErrorType::IncorrectAttribute;
				return false;
			}

			std::string key = _content.substr(0, start);
			std::string value = _content.substr(start + 1, end - start);

			// проверка на валидность
			if (! checkPair(key, value))
			{
				mLastError = ErrorType::IncorrectAttribute;
				return false;
			}

			// добавляем пару в узел
			_currentNode->addAttribute(key, value);

			// следующий кусок
			_content = _content.substr(end + 1);

			// в строке не осталось символов
			start = _content.find_first_not_of(" \t");
			if (start == _content.npos)
				break;

			mCol += start;
		}

		// был закрывающий тег для текущего тега
		if (close)
		{
			// не проверяем имена, потому что это наш тэг
			_currentNode = _currentNode->getParent();
		}

	}
	return true;
	*/

	//MyGUI::utility::trim(_content);
	content.trim();

	if (content.empty())
	{
		if (currentNode)
		{
			currentNode = currentNode->createChild("");
		}
		else
		{
			currentNode = new Element("", 0);
			if (!m_pRoot)
				m_pRoot = currentNode;
		}
		return true;
	}

	c8 simbol = content[0];
	bool tagDeclaration = false;

	if (simbol == '!')
	{
		if (currentNode != 0)
		{
			//_currentNode->createChild("", _content, ElementType::Comment);
		}
		return true;
	}
	else if (simbol == '?')
	{
		tagDeclaration = true;
		content.erase(0, 1);
	}

	size_t start = 0;
	size_t end = 0;
	if (simbol == '/')
	{
		if (currentNode == 0)
		{
			if (!mRoot)
			{
				m_lastError = ErrorType::CloseNotOpenedElement;
				return false;
			}
		}
		start = content.find_first_not_of(" \t", 1);
		if (start == content.npos)
		{
			content.clear();
		}
		else
		{
			end = content.find_last_not_of(" \t");
			content = content.substr(start, end - start + 1);
		}
		if (currentNode->getName() != content)
		{
			m_lastError = ErrorType::InconsistentOpenCloseElements;
			return false;
		}
		currentNode = currentNode->getParent();
	}
	else
	{
		core::stringc cut = content;
		start = _content.find_first_of(" \t/?", 1);
		if (start != _content.npos)
		{
			cut = content.substr(0, start);
			content = content.substr(start);
		}
		else
		{
			content.clear();
		}

		if (currentNode)
		{
			currentNode = currentNode->createChild(cut);
		}
		else
		{
			if (tagDeclaration)
			{
				if (mDeclaration)
				{
					m_lastError = ErrorType::MoreThanOneXMLDeclaration;
					return false;
				}
				currentNode = new Element(cut, 0, ElementType::Declaration);
				m_pDeclaration = currentNode;
			}
			else
			{
				if (m_pRoot)
				{
					m_lastError = ErrorType::MoreThanOneRootElement;
					return false;
				}
				currentNode = new Element(cut, 0, ElementType::Normal);
				m_pRoot = currentNode;
			}
		}

		start = content.findFirstCharNotInList(" \t",3);
		if (start == content.npos)
			return true;

		bool close = false;
		if ((content[start] == '/') || (content[start] == '?'))
		{
			close = true;
			content[start] = ' ';
			start = _content.find_last_not_of(" \t");
			if (start == _content.npos)
			{
				currentNode = currentNode->getParent();
				return true;
			}
		}

		while (true)
		{
			start = _content.find('=');
			if (start == _content.npos)
			{
				m_lastError = ErrorType::IncorrectAttribute;
				return false;
			}
			end = content.find("\"\'", start + 1);
			if (end == _content.npos)
			{
				m_lastError = ErrorType::IncorrectAttribute;
				return false;
			}
			end = content.find("\"\'", end + 1);
			if (end == _content.npos)
			{
				m_lastError = ErrorType::IncorrectAttribute;
				return false;
			}

			core::stringc key = content.subString(0, start);
			core::stringc value = content.subString(start + 1, end - start);

			if (! checkPair(key, value))
			{
				m_lastError = ErrorType::IncorrectAttribute;
				return false;
			}

			currentNode->addAttribute(key, value);

			content = content.substr(end + 1);

			start = content.findFirstCharNotInList(" \t");
			if (start == _content.npos)
				break;

			mCol += start;
		}

		if (close)
		{
			currentNode = currentNode->getParent();
		}

	}
	return true;
}

bool Document::checkPair(core::string& key, core::string& value)
{
	// в ключе не должно быть ковычек и пробелов
	MyGUI::utility::trim(_key);
	if (_key.empty())
		return false;
	size_t start = _key.find_first_of(" \t\"\'&");
	if (start != _key.npos)
		return false;

	// в значении, ковычки по бокам
	MyGUI::utility::trim(_value);
	if (_value.size() < 2)
		return false;
	if (((_value[0] != '"') || (_value[_value.length() - 1] != '"')) &&
		((_value[0] != '\'') || (_value[_value.length() - 1] != '\'')))
		return false;
	bool ok = true;
	_value = utility::convert_from_xml(_value.substr(1, _value.length() - 2), ok);
	return ok;
}

s32 Document::find(const core::stringc& text, c8 c, u32 start)
{
	//kind of value
	bool kov = false;

	c8 buff[16] = "\"_\0";
	buff[1] = c;

	//size_t pos = _start;
	s32 pos = (s32)start;

	while (true)
	{
		//pos = _text.find_first_of(buff, pos);
		pos=text.findFirstChar(buff,(u32)pos);

		//if (pos == _text.npos)
		if(pos==-1)
		{
			break;
		}
		//TODO 如此判断，如果value中使用了双引号是否有问题？
		else if (text[pos] == '"')
		{
			kov = !kov;
			++pos;
		}
		//搜到c参数指定的字符
		//应该是指结束双引号后调过一个空格
		else if (kov)
		{
			++pos;
		}
		else
		{
			break;
		}
	}

	return pos;
}

void Document::clearDeclaration()
{
	if (m_pDeclaration)
	{
		delete m_pDeclaration;
		m_pDeclaration = NULL;
	}
}

void Document::clearRoot()
{
	if (m_pRoot)
	{
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

Element* Document::createDeclaration(const core::stringc& version, const core::stringc& encoding)
{
	clearDeclaration();
	m_pDeclaration = new Element("xml", NULL, ElementType::Declaration);
	m_pDeclaration->addAttribute("version", version);
	m_pDeclaration->addAttribute("encoding", encoding);
	return m_pDeclaration;
}

Element* Document::createRoot(const core::stringc& name)
{
	clearRoot();
	m_pRoot = new Element(name, 0, ElementType::Normal);
	return m_pRoot;
}

bool Document::parseLine(core::stringc& line, Element*& element)
{
	while (true)
	{
		/*
		size_t start = find(_line, '<');
		if (start == _line.npos)
			break;
		size_t end = _line.npos;

		if ((start + 3 < _line.size()) && (_line[start + 1] == '!') && (_line[start + 2] == '-') && (_line[start + 3] == '-'))
		{
			end = _line.find("-->", start + 4);
			if (end == _line.npos)
				break;
			end += 2;
		}
		else
		{
			end = find(_line, '>', start + 1);
			if (end == _line.npos)
				break;
		}
		size_t body = _line.find_first_not_of(" \t<");
		if (body < start)
		{
			std::string body_str = _line.substr(0, start);
			mCol = 0;

			if (_element != 0)
			{
				bool ok = true;
				_element->setContent(utility::convert_from_xml(body_str, ok));
				if (!ok)
				{
					mLastError = ErrorType::IncorrectContent;
					return false;
				}
			}
		}
		if (!parseTag(_element, _line.substr(start + 1, end - start - 1)))
		{
			return false;
		}
		_line = _line.substr(end + 1);
		*/

		s32 start=find(line, '<');
		if(start==-1)
			break;
		s32 end=-1;
		//有可能是注释
		if ((start + 3 < line.length()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-'))
		{
			end = line.find("-->", start + 4);
			if (end == -1)
				break;
			end += 2;
		}
		else
		{
			end = find(line, '>', start + 1);
			if (end == -1)
				break;
		}

		s32 body = line.findFirstCharNotInList(" \t<");
		if (body < start)
		{
			core::stringc body_str = line.subString(0, start);
			m_uColumn = 0;

			if (element != 0)
			{
				bool ok = true;
				element->setContent(utility::convert_from_xml(body_str, ok));
				if (!ok)
				{
					m_lastError = ErrorType::IncorrectContent;
					return false;
				}
			}
		}
		if (!parseTag(element, line.subString(start + 1, end - start - 1)))
		{
			return false;
		}
		line = line.subString(end + 1);
	}
	return true;
}

core::stringc Document::getLastError()
{
	const core::stringc& error = m_lastError.print();
	if (error.empty())
		return error;
	//return MyGUI::utility::toString("'", error, "' ,  file='", mLastErrorFile, "' ,  line=", mLine, " ,  col=", mCol);
	return core::stringc("Error:%s in file:%s(%d,%d)",error.c_str(),m_lastErrorFile.c_str(),m_uLine,m_uColumn);
}

/*bool Document::open(const UString& _filename)
{
	return open(_filename.asWStr());
}

bool Document::save(const UString& _filename)
{
	return save(_filename.asWStr());
}*/

void Document::clearLastError()
{
	m_lastError = ErrorType::COUNT;
}

Element* Document::getRoot() const
{
	return m_pRoot;
}

void Document::setLastFileError(const core::stringc& filename)
{
	m_lastErrorFile = filename;
}

/*void Document::setLastFileError(const std::wstring& _filename)
{
	mLastErrorFile = UString(_filename).asUTF8();
}*/

}
}