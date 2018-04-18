#pragma once

#include "xml_node.h"
#include "xml_operation.h"
#include <unordered_map>

class xml_namedop : public xml_node
{
public:
	xml_namedop(xml_node* const _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
		, exec_depth(0)
		, ops_count(0)
	{
		type = NAMEDOP;
	}

	~xml_namedop()
	{
	}

	void execute()
	{
		cout << "[execute-namedop] " << value << std::endl;
		exec_depth++;

		for (auto & operation : list_internal_children)
		{
			if (operation->get_type() == OPERATION)
			{
				operation->execute();
			}
		}
	}

	bool operator > (const xml_namedop& rhs) const
	{
		return ops_count > rhs.ops_count;
	}

	bool operator < (const xml_namedop& rhs) const
	{
		return ops_count < rhs.ops_count;
	}


	void	set_exec_depth(int depth) { exec_depth = depth; }
	int		get_exec_depth() const { return exec_depth; }	
	void	set_operations_count(int icounter) { ops_count; }
	size_t	get_operations_count() const { return ops_count; }
	
	void add(xml_node* ptr_child)
	{
		if (!ptr_child)
			return;

		if (OPERATION == ptr_child->get_type())
		{
			ops_count++;

			for (auto &attribute : ptr_child->get_parent()->get_attributes_list())
			{
				if (string_utils::compare_nocase_sensitive(attribute.name, "name"))
				{
					ptr_child->get_parent()->set_value(attribute.value);
					break;
				}
			}
		}

		xml_node::add(ptr_child);
	}

protected:
	short			exec_depth;
	size_t			ops_count;
};

struct LessThanByCount
{
	bool operator()(const xml_node* lhs, const xml_node* rhs) const
	{
		return lhs->get_depth() < rhs->get_depth();
	}
};
