// PerspXML.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include "string_utils.h"
#include "operations_manager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		cout << "Incomplete or too many parameters. Please add the .XML file path only and try again." << std::endl;
		return -1;
	}

	operations_manager	op_man((ustring)argv[1]);
	if (!op_man.loadXMLdocument())
	{
		cout << "The .XML file was not loaded. Cannot continue..." << std::endl;
		return -1;
	}
		
	op_man.build_tree_xml_based();

	op_man.test_print_tree();

	// Q1 - How many namedops are in the file?
	op_man.run_get_namedoperation_count();

	// Q2 - Which namedop(identified by it’s “name” attribute) is the most complex ? Which one is the least complex ?
	op_man.run_get_most_complex_namedop();

	// Q3 - Which namedop has a return value ?
	op_man.run_find_namedop_with_returnvalue();

	// Q4 - Which is the maximum call depth for each namedop? A namedop that does not call another namedop has a call depth of 1.
	op_man.run_find_maximum_call_depth();

    return 0;
}

