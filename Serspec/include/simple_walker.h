#pragma once
#include "general.h"
#include "pugixml.hpp"
#include "xml_node.h"

const wchar_t* node_types[] =
{
	L"null", L"document", L"element", L"pcdata", L"cdata", L"comment", L"pi", L"declaration"
};

struct simple_walker : pugi::xml_tree_walker
{

	virtual bool for_each(pugi::xml_node& node)
	{
		const ustring str_element(L"element");
		const ustring str_namedOPS(L"namedoperations");
		const ustring str_namedop(L"namedoperation");

		for (int i = 0; i < depth(); ++i) cout << "  "; // indentation

		cout << "#: name='" << node.name() << "', value='" << node.value() << "'\n";

		xml_node_data pcrt_node(node.name(), node.value(), NODE_TYPE::ELEMENT);

//		static generic_node<xml_node_data>* root = m_ptrTree->get_root();

		
		if (!ustrncmp(node_types[node.type()], str_element.c_str(), str_element.size()))	// element
		{
			if (!ustrncmp(node.name(), str_namedOPS.c_str(), str_namedOPS.size()))		// namedoperations - ROOT
			{
				m_pCrtNode = m_ptrTree->get_root();
			}
		}

		for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())	// attributes
		{
			if (!ustrncmp(node.name(), str_namedOPS.c_str(), str_namedOPS.size()))		// namedoperations
			{
				// don't enumerate the root element - the root already extracted
				// don't need to extract for current purposes attributes such:
				//	xmlns:xsi = http://www.w3.org/2001/XMLSchema-instance
				//  xsi:noNamespaceSchemaLocation = http ://serspectis.com/pxml/stable/po/PO2NamedOperations.xsd
				return true;
			}

			for (int i = 0; i < depth(); ++i) cout << "  "; // indentation
			cout << "	--> " << attr.name() << "=" << attr.value() << std::endl;

			pcrt_node.add_attribute(attr.name(), attr.value());
		}

		m_pCrtNode = m_ptrTree->add(m_pCrtNode, pcrt_node);

		return true; // continue traversal
	}

	generic_node<xml_node_data>*		m_pCrtNode = nullptr;
};