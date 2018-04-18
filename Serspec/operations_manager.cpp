#include "operations_manager.h"
#include "string_utils.h"

#include "xml_others.h"
#include "xml_namedop.h"
#include "xml_namedoperations.h"
#include "xml_operation.h"
#include "xml_operationlist.h"
#include "xml_callednamedop.h"
#include "xml_returnvalue.h"

using namespace std;

operations_manager::operations_manager(ustring const& xmlDocPath)
	: m_strXMLFilePath(xmlDocPath)
	, m_ptrRoot(nullptr)
	, m_ptrLatestNamedop(nullptr)
	, m_ptrLatestOperation(nullptr)
	, m_ptrLatestOperationList(nullptr)
{
}

operations_manager::~operations_manager()
{
	xml_tree_cleaner(m_ptrRoot);
}

void operations_manager::xml_tree_cleaner(xml_node *node)
{
	if (!node) return;

	std::list<xml_node*> list_nodes;
	node->get_children_list(list_nodes);
	
	for (auto &ob : list_nodes)
		xml_tree_cleaner(ob);

	delete node;
}

bool operations_manager::loadXMLdocument()
{
	if (!m_docXML.load_file(m_strXMLFilePath.c_str()))
	{
		cout << "XML document load process has failed. Cannot continue..." << std::endl;
		return false;
	}

	pugi::xml_node root = m_docXML.child("namedoperations");

	return true;
}

xml_node* operations_manager::object_factory(ustring const& _tagstr, ustring const& _name, ustring const& _value)
{
	if (string_utils::compare_nocase_sensitive(_tagstr, "namedoperations"))
	{
		m_ptrRoot = new xml_namedoperations(nullptr, _name, _value);

		return m_ptrRoot;	// special case - root
	}
	else if (string_utils::compare_nocase_sensitive(_tagstr, "namedoperation"))
	{
		m_ptrLatestNamedop = new xml_namedop(m_ptrRoot, _name, _value);

		return m_ptrLatestNamedop;
	}
	else if (string_utils::compare_nocase_sensitive(_tagstr, "operation"))
	{
		if (m_ptrLatestOperationList && string_utils::compare_nocase_sensitive(m_strLatestParentName, "operationlist"))
		{
			m_ptrLatestOperation = new xml_operation(m_ptrLatestOperationList, _name, _value);
			xml_operationlist* ptr_op_list = dynamic_cast<xml_operationlist *>(m_ptrLatestOperationList);
			if (!ptr_op_list) return nullptr;
			ptr_op_list->add_ops(m_ptrLatestOperation);
		}
		else
		{
			m_ptrLatestOperation = new xml_operation(m_ptrLatestNamedop, _name, _value);
		}
	
		return m_ptrLatestOperation;
	}
	else if (string_utils::compare_nocase_sensitive(_tagstr, "returnvalue"))
	{
		return new xml_returnvalue(m_ptrLatestNamedop, _name, _value);
	}
	else if (string_utils::compare_nocase_sensitive(_tagstr, "callnamedop"))
	{
		return new xml_callednamedop(m_ptrLatestOperation, _name, _value);;
	}
	else if (string_utils::compare_nocase_sensitive(_tagstr, "operationlist"))	
	{
		m_ptrLatestOperationList = new xml_operationlist(m_ptrLatestOperation, _name, _value);
		return m_ptrLatestOperationList;
	}
	
	// OTHERS param - withparam - localvar, etc -> others
	{
		return new xml_others(m_ptrLatestNamedop, _name, _value);
	}
};

void operations_manager::tree_builder(pugi::xml_document const& doc, pugi::xml_node node, int depth)
{
	if (depth == 1)		// root's children
	{
		pugi::xml_node root = doc.child(node.name());
		
		m_ptrRoot = object_factory("namedoperations", "namedoperations", "");

		// tag::namedoperation[]
		for (pugi::xml_node _node = root.first_child(); _node; _node = _node.next_sibling())
		{
			ustring tagName(_node.name());
			xml_node* xmlnode = object_factory(tagName, _node.name(), _node.value());

			for (pugi::xml_attribute attr = _node.first_attribute(); attr; attr = attr.next_attribute())
			{
				xmlnode->add_attribute(attr.name(), attr.value());
			}

			m_ptrRoot->add(xmlnode);

			tree_builder(doc, _node, ++depth);
		}
		// end::namedoperation[]	
	}
	else
	{
		for (pugi::xml_node _node = node.first_child(); _node; _node = _node.next_sibling())
		{
			ustring tagName(_node.name());
			m_strLatestParentName = _node.parent().name();

			xml_node* xmlnode = object_factory(tagName, _node.name(), _node.value());

			for (pugi::xml_attribute attr = _node.first_attribute(); attr; attr = attr.next_attribute())
			{
				xmlnode->add_attribute(attr.name(), attr.value());
			}

			auto parent = xmlnode->get_parent();
			parent->add(xmlnode);

			tree_builder(doc, _node, ++depth);
		}
	}
}

void operations_manager::build_tree_xml_based(BUILD_TREE_MODE mode)
{
	int depth = 1;
	pugi::xml_node root = m_docXML.child("namedoperations");

	tree_builder(m_docXML, root, depth);
}


void operations_manager::test_print_tree()
{
	print_tree(m_ptrRoot);
}

void operations_manager::print_tree(xml_node* node, bool first_time)
{
	if (!node)
	{
		cout << "Invalid NODE!" << std::endl;
		return;
	}

	if (first_time)
		cout << "#: name='" << node->get_tagname() << "', value='" << node->get_value() << "'\n";	// element

	std::list<xml_node*> list_children;
	node->get_children_list(list_children);

	for (auto &ob : list_children)
	{
		for (int i = 0; i < ob->get_depth(); ++i) cout << "  "; // indentation

		cout << "#: name='" << ob->get_tagname() << "', value='" << ob->get_value() << "'\n";		// element

		for (auto &attrib : ob->get_attributes_list())
		{
			for (int i = 0; i < ob->get_depth(); ++i) cout << "  "; // indentation

			cout << "	--> " << attrib.name << "=" << attrib.value << std::endl;				// attribute
		}

		print_tree(ob, false);
	}
}


	// Q1 - How many namedops are in the file?
size_t operations_manager::run_get_namedoperation_count()
{
	if (!m_ptrRoot)
	{
		cout << "Fatal error. No 'namedoperations' detected" << std::endl;
		return 0;
	}

	cout << "Q1 - How many namedops are in the file?" << std::endl;
	size_t namedops_counter = 0;
	std::string pattern_namedops("namedoperation"); 

	std::list<xml_node*> list_children;
	m_ptrRoot->get_children_list(list_children);

	for (auto &ob : list_children)
	{
		if (string_utils::compare_nocase_sensitive(pattern_namedops, ob->get_tagname()))
		{
			namedops_counter++;
		}
	}

	if (namedops_counter)
		cout << "There are " << namedops_counter << " '" << pattern_namedops << "'" << std::endl;
	else
		cout << "There are no 'namedops' tags" << std::endl;

	return namedops_counter;
}

	//Q2 - Which namedop(identified by it’s “name” attribute) is the most complex ? Which one is the least complex ?
	//	Complexity is measured by the number of operations inside each namedop.
void operations_manager::run_get_most_complex_namedop()
{
	cout << "---------------------------------------------------------------" << std::endl;
	cout << "Q2 - Which namedop identified by it’s 'name' attribute is the most complex ? \n Which one is the least complex ?" << std::endl;
	const std::string strNamedOperation("namedoperation");

	std::list<xml_namedop*> list4top;
//	build_nameop_complexities(m_ptrRoot, strNamedOperation, list4top);

	std::list<xml_node*> list_children;
	m_ptrRoot->get_children_list(list_children);

	for (auto &ob_namedop : list_children)
	{
		if (string_utils::compare_nocase_sensitive(strNamedOperation, ob_namedop->get_tagname()))
		{
			int		ops_count = 0;
			count_operations(ob_namedop, ops_count);

			if (ops_count > 0)
			{
				auto ptr_ob = dynamic_cast<xml_namedop*>(ob_namedop);
				if (!ptr_ob) continue;

				list4top.push_back(dynamic_cast<xml_namedop*>(ob_namedop));
			}
		}
	}

	if (list4top.empty())
	{
		cout << "ERROR - Top list is empty" << std::endl;
		return;
	}

	list4top.sort([](const xml_namedop* lhs, const xml_namedop* rhs)
	{
		return lhs->get_operations_count() > rhs->get_operations_count();
	});

	auto it_most = *list4top.begin();
	auto most_counts_val = it_most->get_operations_count();
	auto it_less = *list4top.rbegin();
	auto less_counts_val = it_less->get_operations_count();

	while (true)
	{
		cout << "The most complex 'namedop' is '" << it_most->get_node_valuename() << "' (count " << most_counts_val << ")" << std::endl;
		++it_most;

		if (it_most->get_operations_count() != most_counts_val)
			break;
	}

	while (true)
	{
		cout << "The less complex 'namedop' is '" << it_less->get_node_valuename() << "' (count " << less_counts_val << ")"  << std::endl;

		++it_less;

		if (it_less->get_operations_count() != less_counts_val)
			break;
	}
}

void operations_manager::build_nameop_complexities(xml_node* node,
													const ustring& namedop_pattern,
													std::list<xml_namedop*>& list_results)
{
	if (!node)
	{
		cout << "Invalid NODE!" << std::endl;
		return;
	}

	// add the complexities info within a priority_queue to easy extract max and min comlexity
	const std::string strNamedOperation("namedoperation");
	
	std::list<xml_node*> list_children;
	node->get_children_list(list_children);

	for (auto &ob_namedop : list_children)
	{
		if (string_utils::compare_nocase_sensitive(strNamedOperation, ob_namedop->get_tagname()))
		{
			int		ops_count = 0;

			count_operations(ob_namedop, ops_count);

			if (ops_count)
			{
				auto ptr_ob = dynamic_cast<xml_namedop*>(ob_namedop);
				if (!ptr_ob) continue;

				list_results.push_back(dynamic_cast<xml_namedop*>(ob_namedop));
			}
		}
	}
}

void  operations_manager::count_operations(xml_node* node, int &ops_count)
{
	if (!node)
	{
		cout << "Invalid NODE or DATA!" << std::endl;
		return;
	}

	const std::string strOperation("operation");
	const std::string strOperationList("operationlist");

	std::list<xml_node*> list_children;
	node->get_children_list(list_children);

	for (auto &ob : list_children)
	{
		if (string_utils::compare_nocase_sensitive(strOperation, ob->get_tagname()))
		{
			ops_count++;

			std::list<xml_node*> ob_list_children;
			ob->get_children_list(ob_list_children);

			for (auto &sub_op : ob_list_children)
			{
				if (string_utils::compare_nocase_sensitive(sub_op->get_tagname(), strOperationList))
				{
					std::list<xml_node*> subob_list_children;
					sub_op->get_children_list(subob_list_children);

					for (auto &op_list : subob_list_children) // /operationlist/operation
					{
						
						if (string_utils::compare_nocase_sensitive(strOperation, op_list->get_tagname()))
						{
							ops_count++;
						}
							
					}
				}
			}
		}
	}
}

	// Q3 - Which namedop has a return value ?
void operations_manager::run_find_namedop_with_returnvalue()
{
	cout << "---------------------------------------------------------------" << std::endl;
	cout << "Q3 - Which namedop has a return value ?" << std::endl;
	std::list<xml_node*> list_namedop_retvalue;
	find_namedop_returnvalue(m_ptrRoot, list_namedop_retvalue);

	cout << "The 'namedoperation' with 'returnedvalue' are: " << endl;
	for (auto &ob_named : list_namedop_retvalue)
	{
		cout << "	" << ob_named->get_value() << std::endl;
	}
}

// Q3 - Which namedop has a return value ?
void operations_manager::find_namedop_returnvalue(xml_node* node,
													std::list<xml_node*>& list_results)
{

	if (!node)
	{
		cout << "Invalid NODE!" << std::endl;
		return;
	}

	const std::string strNamedOperation("namedoperation");
	const std::string strReturnValue("returnvalue");
	const std::string strName("name");

	std::list<xml_node*> list_children;
	node->get_children_list(list_children);

	for (auto &ob_namedop : list_children)
	{
		if (string_utils::compare_nocase_sensitive(strNamedOperation, ob_namedop->get_tagname()))
		{
			auto ptr_namedop = dynamic_cast<xml_namedop*>(ob_namedop);
			if (!ptr_namedop)
				break;

			std::list<xml_node*> list_named_children;
			ob_namedop->get_children_list(list_named_children);

			for (auto &ob_retval : list_named_children)
			{
				if (string_utils::compare_nocase_sensitive(ob_retval->get_tagname(), strReturnValue))
				{
					list_results.push_back(ob_namedop);
					break;
				}
			}
		}
	}
}


// Q4 - Which is the maximum call depth for each namedop?
void operations_manager::run_find_maximum_call_depth()
{
	if (!m_ptrRoot)
	{
		cout << "Fatal error. No 'namedoperations' detected" << std::endl;
		return;
	}

	cout << "---------------------------------------------------------------" << std::endl;
	cout << "Q4 - Which is the maximum call depth for each namedop?" << std::endl;

	// update xml_callednamedop with the 'xml_namedop' name = "namedop_to_be_executed" information
	update_callnamedop();

	// run allnamedop - might contains circular calls - infinite loop execution
	m_ptrRoot->execute();

	// extract the maximum call depth
	std::list<xml_node*> list_maxdepth;
	int max_depth = 1;
	std::unordered_map<ustring, xml_node*> ht_namedops;

	xml_namedoperations* namedops = dynamic_cast<xml_namedoperations*>(m_ptrRoot);

	namedops->get_namedop_hashtable(ht_namedops);
	for (auto &namedop : ht_namedops)
	{
		xml_namedop* ptr = dynamic_cast<xml_namedop*>(namedop.second);
		if (ptr->get_exec_depth() > max_depth)
		{
			max_depth = ptr->get_exec_depth();
			list_maxdepth.push_back(ptr);
		}
	}

	cout << "______________________________________" << std::endl;
	cout << "The maximum call depth is executed in" << std::endl;
	for (auto max_namedop : list_maxdepth)
	{
		cout << "namedoperation: " << max_namedop->get_node_valuename() << std::endl;
	}
}

void operations_manager::update_callnamedop()
{
	const std::string strNamedOperation("namedoperation");

	std::list<xml_node*> list_root_children;
	m_ptrRoot->get_children_list(list_root_children);

	for (auto &ob_namedop : list_root_children	)
	{
		if (string_utils::compare_nocase_sensitive(strNamedOperation, ob_namedop->get_tagname()))
		{
			search_update_callnamed(ob_namedop);
		}
	}
}

void operations_manager::search_update_callnamed(xml_node* node)
{
	if (!node)
	{
		cout << "Invalid NODE!" << std::endl;
		return;
	}

	if (string_utils::compare_nocase_sensitive(node->get_tagname(), "callnamedop"))
	{
		xml_callednamedop *ptr_calledop = dynamic_cast<xml_callednamedop *>(node);
		if (!ptr_calledop) return;

		ustring attributeKey;
		for (auto & attrib : ptr_calledop->get_attributes_list())
		{
			if (string_utils::compare_nocase_sensitive(attrib.name, "name"))
			{
				attributeKey = attrib.value;
				break;
			}
		}

		if (attributeKey.empty()) return;

		// update the called 'nameop' information that has to be available while effective executing
		ptr_calledop->set_value(attributeKey);
	}


	std::list<xml_node*> list_children;
	node->get_children_list(list_children);

	for (auto &child_node : list_children)
	{
		search_update_callnamed(child_node);
	}
}
