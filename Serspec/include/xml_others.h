
#pragma once
#include "xml_node.h"

// generic class for less important tags - unrelated for current task
// param - withparam - localvar - etc

class xml_others : public xml_node
{
public:
	xml_others(xml_node* _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = OTHERS;
	}
	
	~xml_others()
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

