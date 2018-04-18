#pragma once

#include "xml_node.h"
#include "xml_namedop.h"
#include "xml_namedoperations.h"

#include <unordered_map>

class xml_namedop;
class xml_callednamedop : public xml_node
{
public:
	xml_callednamedop(xml_node* _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = CALLEDNAMEDOP;
	}

	~xml_callednamedop()
	{
	}

	void execute()
	{
		xml_namedoperations* ptr_namedops = dynamic_cast<xml_namedoperations*>(get_namedops());
		if (!ptr_namedops) return;

		std::unordered_map<ustring, xml_node*> ht_namedops;
		ptr_namedops->get_namedop_hashtable(ht_namedops);
		
		ustring namedp_name2call = get_value();
		
		auto it_effectiveNOP = ht_namedops.find(namedp_name2call);
		if (it_effectiveNOP != ht_namedops.end())
		{		
			it_effectiveNOP->second->execute();
		}

	}

	void add(xml_node* ptr_child)
	{
		xml_node::add(ptr_child);
	}

	void set_ptr2namedop(xml_node* ptr) { ptr_namedop = ptr; }

protected:
	xml_node* ptr_namedop;

	xml_node* get_namedops() 
	{
		auto *crt_parent = get_parent();
		do 
		{
			if (crt_parent->get_type() == ROOT)
			{
				return crt_parent;
			}

			crt_parent = crt_parent->get_parent();

		} while ((crt_parent != nullptr || crt_parent->get_type() != ROOT));

		return nullptr;
	}
};

