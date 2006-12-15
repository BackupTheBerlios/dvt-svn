/** @file
 * 
 * Dvt Exceptions - Part of the portable Dictionary and Vocabulary Trainer.
 * 
 * Copyright (C) 2006  Denis Martin (http://www.delta-my.de/)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 */

#ifndef DVTEXCEPTIONS_H_
#define DVTEXCEPTIONS_H_

#include <string>

namespace Dvt
{

/** @name Exceptions *********************************************************/
//@{

// TODO: unmake exceptions inline...

typedef enum {
	ecNone,
	ecFileError,
	ecFileWriteError,
	ecXmlParsingError,
	ecXmlTreeInvalid,
	ecXmlRootInvalid,
	ecXmlUnsupportedFormatVersion,
	ecXmlMissingAttribute,
	ecXmlMissingNode,
	ecMissingAttribute,
	ecMissingElement,
	ecWordClassOutOfRange
} ExceptionCode;

class Exception
{
public:
	ExceptionCode code;
	std::string msg;
	
	Exception(const ExceptionCode code);
	Exception(const ExceptionCode code, const std::string& msg);
	virtual ~Exception();
};

class EFileException : public Exception
{
public:
	std::string fileName;
	
	EFileException(const ExceptionCode code, const std::string& msg):Exception(code, msg)
	{
		this->fileName = "<unknown/stream>";
	}
};

class EFileError : public EFileException
{
public:
	std::string fileName;
	
	EFileError(const std::string& fileName):EFileException(
		ecFileError, 
		"Error while opening file or directory: " + fileName)
	{
		this->fileName = fileName;
	}
};

class EFileWriteError : public EFileException
{
public:
	std::string fileName;
	
	EFileWriteError(const std::string& fileName):EFileException(
		ecFileWriteError, 
		"Error while writing to file \"" + fileName + "\".")
	{
		this->fileName = fileName;
	}
};

class EXmlParsingError : public EFileException
{
public:
	EXmlParsingError(const std::string& fileName):EFileException(
		ecXmlParsingError, 
		"Error while parsing XML file " + fileName)
	{
		this->fileName = fileName;
	}
};

class EXmlTreeInvalid : public EFileException
{
public:
	std::string nodeName;
	
	EXmlTreeInvalid(const std::string& fileName, const std::string& nodeName):EFileException(
		ecXmlTreeInvalid, 
		"XML node \"" + nodeName + "\" (or a sub-node) is invalid in file " + fileName)
	{
		this->nodeName = nodeName;
		this->fileName = fileName;
	}
};

class EXmlRootInvalid : public EFileException
{
public:
	std::string tagName;
	
	EXmlRootInvalid(const std::string& fileName, const std::string& tagName):EFileException(
		ecXmlRootInvalid, 
		"The file \"" + fileName + "\" does not contain the expected XML root tag \"" + tagName + "\".")
	{
		this->tagName = tagName;
		this->fileName = fileName;
	}
};

class EXmlUnsupportedFormatVersion : public EFileException
{
public:
	std::string version;
	
	EXmlUnsupportedFormatVersion(const std::string& fileName, const std::string& version):EFileException(
		ecXmlUnsupportedFormatVersion, 
		"XML file's format version is not supported (version " + version + ", file " + fileName + ")")
	{
		this->version = version;
		this->fileName = fileName;
	}
};

class EXmlMissingAttribute : public EFileException
{
public:
	std::string nodeName;
	std::string attrName;
	
	EXmlMissingAttribute(const std::string& fileName, const std::string& nodeName, const std::string& attrName):EFileException(
		ecXmlMissingAttribute, 
		"Missing attribute \"" + attrName + "\" of node \"" + nodeName + "\" in file \"" + fileName + "\".")
	{
		this->fileName = fileName;
		this->nodeName = nodeName;
		this->attrName = attrName;
	}
};

/**
 * Divotra training lesson (.dtl) has no title
 */
class EXmlMissingNode : public EFileException
{
public:
	std::string nodeName;
	
	EXmlMissingNode(const std::string& fileName, const std::string& nodeName):EFileException(
		ecXmlMissingNode, 
		"Missing XML node \"" + nodeName + "\" in file \"" + fileName + "\".")
	{
		this->fileName = fileName;
		this->nodeName = nodeName;
	}
};

class EMissingAttribute : public Exception
{
public:
	std::string attrName;
	
	EMissingAttribute(const std::string& attrName):Exception(
		ecMissingAttribute, 
		"Missing attribute \"" + attrName + "\".")
	{
		this->attrName = attrName;
	}
};

class EMissingElement : public Exception
{
public:
	std::string elemName;
	
	EMissingElement(const std::string& elemName):Exception(
		ecMissingElement, 
		"Missing attribute \"" + elemName + "\".")
	{
		this->elemName = elemName;
	}
};

class EWordClassOutOfRange : public Exception
{
public:	
	EWordClassOutOfRange():Exception(
		ecWordClassOutOfRange, 
		"WordClass index out of range.")
	{
	}
};

//@}

} // namespace

#endif /*DVTEXCEPTIONS_H_*/
