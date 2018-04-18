#pragma once
#include "general.h"
#include <list>
#include "string_utils.h"

#ifdef _UNICODE
#define ustring			std::wstring
#else
#define ustring			std::string
#endif


enum NODE_TYPE {
	ABSTRACT = 900,
	ROOT = 999,
	NAMEDOP,
	OPERATION,
	CALLEDNAMEDOP,
	OPERATIONLIST,
	PARAM,
	WITHPARAM,
	LOCALVAR,
	RETURNVALUE,
	OTHERS,			// param - withparam - localvar 
	OPSONLY_ABSTRACT,
};

struct attribute
{
	attribute(const ustring& _name, const ustring& _value)
		: name(_name)
		, value(_value)
	{
	}

	ustring		name;
	ustring		value;
};

class xml_node
{
  public:
	xml_node(xml_node* const _parent, const ustring& _name, const ustring& _value)
		: tag_name(_name)
		, value(_value)
		, parent(_parent)
		, depth(1)
	{
		type = ABSTRACT;
		depth = (parent) ? parent->depth + 1 : 1;
	}

	xml_node(const xml_node& other)
		: parent(other.parent)
		, tag_name(other.tag_name)
		, value(other.value)
		, type(other.type)
		, depth(other.depth)
		, list_internal_children(other.list_internal_children)
		, list_attributes(other.list_attributes)
	{
	}

	xml_node& operator = (const xml_node& rhs)
	{
		if (this != &rhs)
		{
			parent				= rhs.parent;
			tag_name			= rhs.tag_name;
			value				= rhs.value;
			type				= rhs.type;
			depth				= rhs.depth;
			list_internal_children	= rhs.list_internal_children;
			list_attributes			= rhs.list_attributes;
		}

		return *this;
	}

	xml_node(xml_node&& rhs)
	{
		parent = rhs.parent;
		tag_name = rhs.tag_name;
		value = rhs.value;
		type = rhs.type;
		depth = rhs.depth;
		list_internal_children = std::move(rhs.list_internal_children);
		list_attributes = std::move(rhs.list_attributes);

		rhs.parent = nullptr;
		rhs.tag_name = "";
		rhs.value = "";
		rhs.type = ABSTRACT;
		rhs.depth = 1;
		rhs.list_internal_children.clear();
		rhs.list_attributes.clear();
	}

	xml_node& operator=(xml_node&& rhs)
	{
		if (this != &rhs)
		{
			parent = rhs.parent;
			tag_name = rhs.tag_name;
			value = rhs.value;
			type = rhs.type;
			depth = rhs.depth;
			list_internal_children = std::move(rhs.list_internal_children);
			list_attributes = std::move(rhs.list_attributes);

			rhs.parent = nullptr;
			rhs.tag_name = "";
			rhs.value = "";
			rhs.type = ABSTRACT;
			rhs.depth = 1;
			rhs.list_internal_children.clear();
			rhs.list_attributes.clear();
		}

		return *this;
	}

	virtual ~xml_node()
	{
	}

	virtual void execute() = 0;

	void add_attribute(const ustring& _name, const ustring& _value)
	{
		list_attributes.push_back(attribute(_name, _value));
	}

	void	set_tagname(ustring const& _tagname) { tag_name = _tagname; }
	void	set_value(ustring const& _value) { value = _value; }
	void    set_depth(int _dep) { depth = _dep; }

	ustring		get_tagname() const { return tag_name; }
	ustring		get_value() const { return value; }
	NODE_TYPE	get_type() const { return	type; }
	short		get_depth() const { return depth;  }
	xml_node*	get_parent() const { return	parent; }

	void	get_children_list(std::list<xml_node*>& _list) { _list = list_internal_children; }
	std::list<attribute>& get_attributes_list() { return list_attributes; }

	ustring get_node_valuename()
	{
		ustring main_value;
		for (auto &at : list_attributes)
		{
			if (string_utils::compare_nocase_sensitive(at.name, "name"))
			{
				main_value = at.value;
				break;
			}
		}

		return main_value;
	}

	virtual void add(xml_node* ptr_child)
	{
		if (!ptr_child)
			return;

		ptr_child->depth = (ptr_child->get_parent()) ? ptr_child->get_parent()->get_depth() + 1 : 1;

		list_internal_children.push_back(ptr_child);
	}


  protected:
	ustring			tag_name;
	ustring			value;
	NODE_TYPE		type;
	short			depth;

	xml_node*		parent;

	std::list<xml_node*>		list_internal_children;
	std::list<attribute>		list_attributes;
};

class xml_opsonly : public xml_node
{
  public:
	xml_opsonly(xml_node* const _parent, const ustring& _name, const ustring& _value)
		: xml_node(_parent, _name, _value)
	{
		type = OPSONLY_ABSTRACT;
	}

	virtual void execute2() = 0;

	void add_ops(xml_node* ops)
	{
		if (!ops)
			return;

		list_opsonly.push_back(ops);
	}

	std::list<xml_node*>& get_opsonly_list() { return list_opsonly; }

  protected:
	std::list<xml_node*>	list_opsonly;
};