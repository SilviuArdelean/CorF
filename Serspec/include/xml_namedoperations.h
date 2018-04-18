#pragma once

#include "xml_node.h"
#include "xml_namedop.h"
#include "string_utils.h"
#include <unordered_map>

class xml_namedop;
class xml_namedoperations : public xml_node
{
public:
	xml_namedoperations(xml_node* _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = ROOT;
	}

	~xml_namedoperations()
	{
	}

	void execute()
	{
		for (auto &namedop : list_internal_children)
		{
			namedop->execute();
		}

		// !!! not necesary !!!
		//for (auto &namedop : m_htNamedop)
		//{
		//	namedop.second->execute();
		//}
	}

	void add(xml_node* ptr_child)
	{
		if (!ptr_child)
			return;

		ustring strNamedOp;

		for (auto &attribute : ptr_child->get_attributes_list())
		{
			if (string_utils::compare_nocase_sensitive(attribute.name, "name"))
			{
				strNamedOp = attribute.value;
				break;
			}
		}

		if (strNamedOp.empty())
			return;

		xml_node::add(ptr_child);

		if (ptr_child->get_type() == NAMEDOP)
		{
			m_htNamedop.insert(std::pair<std::string, xml_node*>(strNamedOp, ptr_child));
		}
		
	}

	xml_node* get_namedop(ustring const& namedop) 
	{
		auto itfind = m_htNamedop.find(namedop);

		return (itfind != m_htNamedop.end()) ? itfind->second : nullptr;
	}

	void get_namedop_hashtable(std::unordered_map<ustring, xml_node*>& ht) { ht = m_htNamedop; }

protected:
	std::unordered_map<ustring, xml_node*>	m_htNamedop;
};

