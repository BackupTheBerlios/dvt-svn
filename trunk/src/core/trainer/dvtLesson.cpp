/** @file
 * 
 * Dvt::Lesson - Part of the portable Dictionary and Vocabulary Trainer.
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

#include "dvtLesson.h"

#include "dvtCore.h"
#include "dvtExceptions.h"
#include "dvtLanguageProfile.h"

#include "debug.h"
#include "xmlNode.h"

namespace Dvt
{

using namespace std;

Lesson::Lesson(int number, LanguageProfile* orig, LanguageProfile* trans)
{
	core = Core::getInstance();
	p_langProfile_o = orig;
	p_langProfile_t = trans;
	p_number = number;
}

Lesson::~Lesson()
{
	unloadData();
}

LanguageProfile* Lesson::langProfile_o() {return p_langProfile_o;}
LanguageProfile* Lesson::langProfile_t() {return p_langProfile_t;}

void Lesson::setLangProfile_o(LanguageProfile* lp) {p_langProfile_o = lp;}
void Lesson::setLangProfile_t(LanguageProfile* lp) {p_langProfile_t = lp;}

int Lesson::number() {return p_number;}
MlString& Lesson::title() {return p_title;}

std::vector<TrainingEntry*>& Lesson::entries() {return p_entries;}

TrainingEntry* Lesson::addEntry()
{
	Entry* o = new Entry(p_langProfile_o);
	Entry* t = new Entry(p_langProfile_t);
	TrainingEntry* te = new TrainingEntry(o, t);
	p_entries.push_back(te);
	return te;
}

void Lesson::deleteEntry(TrainingEntry* te)
{
	TrainingEntry* my_te = NULL;
	vector<TrainingEntry*>::iterator it;
	for (it = p_entries.begin(); it != p_entries.end(); it++) {
		my_te = *it;
		if (my_te == te) break;
		
	}
	
	assert(it != p_entries.end());
	p_entries.erase(it);
	delete te;
}

void Lesson::setFromXmlNode(sxml::XmlNode* lessonNode)
{
	assert(lessonNode != NULL);
	
	sxml::XmlNode* node;
	
	// get title
	node = lessonNode->findFirst("title");
	if (node == NULL) 	
		throw EXmlMissingNode("<stream>", "title");
	p_title.setFromXmlNode(node);
	
	unloadData();
	
	// pairs
	sxml::XmlNode* entriesNode = lessonNode->findFirst("entries");
	if (entriesNode != NULL) {
		sxml::NodeSearch* ns = entriesNode->findInit("e");
		for (node = entriesNode->findNext(ns); node != NULL; node = entriesNode->findNext(ns)) {
			WordClass::Type wc = WordClass::Undefined;
			Entry* orig = NULL;
			Entry* trans = NULL;
			TrainingEntry* te = NULL;
			
			// word class
			sxml::NodeAttributesIterator ait;
			ait = node->attributes.find("t");
			if (ait != node->attributes.end()) {
				wc = WordClass::getType(ait->second);
			}
			
			try {
				sxml::XmlNode* tmpNode;
				
				// original
				tmpNode = node->findFirst("o");
				if (tmpNode == NULL) throw EXmlMissingNode("<stream>", "o");
				
				if (tmpNode->children.size() != 1 ||
					(tmpNode->children.size() == 1 && tmpNode->children[0]->type != sxml::ntTextNode))
				{
					throw EXmlTreeInvalid("", tmpNode->name);
				}
				
				orig = new Entry(p_langProfile_o, tmpNode->children[0]->name, wc);
				
				// translation
				tmpNode = node->findFirst("t");
				if (tmpNode == NULL) throw EXmlMissingNode("<stream>", "t");
				
				if (tmpNode->children.size() != 1 ||
					(tmpNode->children.size() == 1 && tmpNode->children[0]->type != sxml::ntTextNode))
				{
					throw EXmlTreeInvalid("", tmpNode->name);
				}
				
				trans = new Entry(p_langProfile_t, tmpNode->children[0]->name, wc);
				
				te = new TrainingEntry(orig, trans);
				
				// meaning
				tmpNode = node->findFirst("m");
				if (tmpNode != NULL) {
					te->meaning().setFromXmlNode(tmpNode);
				}
				
				// extended info for originating language
				tmpNode = node->findFirst("oe");
				if (tmpNode != NULL)
					te->orig()->setFromXmlNode(tmpNode);
				
				// extended info for translation
				tmpNode = node->findFirst("te");
				if (tmpNode != NULL)
					te->trans()->setFromXmlNode(tmpNode);
				
				p_entries.push_back(te);
				
			} catch (Exception e) {
				if (te != NULL)  
					delete te;
				else {
					if (orig != NULL) delete orig;
					if (trans != NULL) delete trans;
				}
				entriesNode->findFree(ns);
				throw e;
			}
		}
		entriesNode->findFree(ns);
	}
}

void Lesson::setToXmlNode(sxml::XmlNode* lessonNode)
{
	if (p_title.empty()) throw EMissingElement("title");
	
	sxml::XmlNode* node;
	sxml::XmlNode* entriesNode;
	sxml::XmlNode* entryNode;
	
	node = new sxml::XmlNode(sxml::ntElementNode, "title");
	p_title.setToXmlNode(node);
	lessonNode->children.push_back(node);
	
	if (p_entries.size() > 0) {
		entriesNode = new sxml::XmlNode(sxml::ntElementNode, "entries");
		
		try {
			vector<TrainingEntry*>::iterator entries_it;
			for (entries_it = p_entries.begin(); entries_it != p_entries.end(); entries_it++) {
				TrainingEntry* te = *entries_it;
				assert(te != NULL);
				
				// entry
				entryNode = new sxml::XmlNode(sxml::ntElementNode, "e");
				entriesNode->children.push_back(entryNode);
				if (te->wordClass() != WordClass::Undefined) {
					assert(te->wordClass() < WordClass::High);
					entryNode->attributes["t"] = WordClass::getXmlAttr(te->wordClass());
				}
				
				// original
				if (te->orig() == NULL || te->orig()->baseName().empty()) 
					throw EMissingElement("original");
				node = new sxml::XmlNode(sxml::ntElementNode, "o");
				entryNode->children.push_back(node);
				node->children.push_back(new sxml::XmlNode(sxml::ntTextNode, te->orig()->baseName()));
				
				// translation
				if (te->trans() == NULL || te->trans()->baseName().empty()) 
					throw EMissingElement("translation");
				node = new sxml::XmlNode(sxml::ntElementNode, "t");
				entryNode->children.push_back(node);
				node->children.push_back(new sxml::XmlNode(sxml::ntTextNode, te->trans()->baseName()));
				
				// meaning
				if (!te->meaning().empty()) {
					node = new sxml::XmlNode(sxml::ntElementNode, "m");
					te->meaning().setToXmlNode(node);
					entryNode->children.push_back(node);
				}
				
				// extended info for origin
				node = new sxml::XmlNode(sxml::ntElementNode, "oe");
				te->orig()->setToXmlNode(node);
				if (node->children.size() > 0) {
					entryNode->children.push_back(node);
				} else {
					delete node;
				}
				
				// extended info for translation
				node = new sxml::XmlNode(sxml::ntElementNode, "te");
				te->trans()->setToXmlNode(node);
				if (node->children.size() > 0) {
					entryNode->children.push_back(node);
				} else {
					delete node;
				}
				
			}
			
		} catch (Exception e) {
			delete entriesNode;
			throw e;
		}
		
		lessonNode->children.push_back(entriesNode);
		
	}
}

void Lesson::unloadData()
{
	vector<TrainingEntry*>::iterator it;
	for (it = p_entries.begin(); it != p_entries.end(); it++) {
		if (*it != NULL) delete ((TrainingEntry*) *it);
	}
	p_entries.clear();
}

} // namespace
