#pragma once
#include "XFrameWork/XBase.hpp"
#include "Third/TinyXML/tinyxml.h"

class IXMLParsable : public IXRef
{
public:
	virtual void ParseXml(TiXmlElement* element) = 0;
};