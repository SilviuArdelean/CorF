#pragma once
#include "xml_node.h"
#include "xml_callednamedop.h"

class xml_operation : public xml_node
{
public:
	xml_operation(xml_node* const _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = OPERATION;
	}

	~xml_operation()
	{
	}

	void execute()
	{
		cout << "[execute-operation]" << std::endl;
		// search for callednamedop and operationslist
		for (auto &ob : list_internal_children)
		{
			if (ob->get_type() == CALLEDNAMEDOP)
			{
				cout << "[execute-callednamed-operation] -> " << ob->get_node_valuename() << std::endl;

				// circullar calls test

				ob->execute();
			}
			else if (ob->get_type() == OPERATIONLIST)
			{
				ob->execute();
			}
		}
	}

	void add(xml_node* ptr_child)
	{
		xml_node::add(ptr_child);
	}
};
