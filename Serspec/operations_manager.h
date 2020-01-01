#pragma once
#include <memory>
#include <queue>
#include "pugixml.hpp"
#include "xml_namedop.h"
#include "xml_namedoperations.h"
#include "xml_node.h"

enum BUILD_TREE_MODE { manual_TRAVERSE = 1, pugi_TRAVERSE };

class operations_manager {
 public:
  operations_manager(ustring const& xmlDocPath);
  ~operations_manager();

  bool loadXMLdocument();
  void build_tree_xml_based(BUILD_TREE_MODE mode = manual_TRAVERSE);
  size_t run_get_namedoperation_count();
  void run_get_most_complex_namedop();
  void run_find_namedop_with_returnvalue();
  void run_find_maximum_call_depth();

  // testing proposes only
  void test_print_tree();

 protected:
  xml_node* object_factory(ustring const& _tagstr,
                           ustring const& _name,
                           ustring const& _value);

  void tree_builder(pugi::xml_document const& doc,
                    pugi::xml_node node,
                    int depth);

  void print_tree(xml_node* node, bool first_time = true);

  void count_operations(xml_node* node, int& ops_count);

  void build_nameop_complexities(xml_node* node,
                                 const ustring& namedop_pattern,
                                 std::list<xml_namedop*>& list_results);

  void find_namedop_returnvalue(xml_node* node,
                                std::list<xml_node*>& list_results);

  void update_callnamedop();

  void search_update_callnamed(xml_node* node);

  void xml_tree_cleaner(xml_node* node);

 protected:
  xml_node* ptr_root_;
  xml_node* ptr_latest_namedop_;
  xml_node* ptr_latest_operation_;
  xml_node* ptr_latest_operation_list_;
  xml_namedoperations* ptr_named_operations_;
  pugi::xml_document doc_xml_;
  ustring str_latest_parent_name_;
  ustring str_xml_file_path_;
};