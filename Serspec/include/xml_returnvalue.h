#pragma once
#include "xml_node.h"

class xml_returnvalue : public xml_node
{
public:
	xml_returnvalue(xml_node* _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = RETURNVALUE;
	}

	~xml_returnvalue()
	{
	}

	void execute()
	{
	}

	void add(xml_node* ptr_child)
	{
		xml_node::add(ptr_child);
	}
};

