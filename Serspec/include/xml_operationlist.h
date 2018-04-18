#pragma once
#include "xml_node.h"

class xml_operationlist : public xml_opsonly
{
public:
	xml_operationlist(xml_node* _parent, const ustring& _name, const ustring& _value)
		: xml_opsonly(_parent, _name, _value)
	{
		type = OPERATIONLIST;
	}

	~xml_operationlist()
	{
	}

	void execute()
	{
		execute2();
	}

	void execute2()
	{
		for (auto &operation : list_opsonly)
		{
			if (operation->get_type() == OPERATION)
				operation->execute();
		}
	}

	void add(xml_node* ptr_child)
	{
		xml_node::add(ptr_child);
	}
};

