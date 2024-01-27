#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "json.h"
#include "miniunit.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static void _bst_free(BSTNode *node) {
    if(node == NULL)
    	return;
    _bst_free(node->left);
    _bst_free(node->right);
    free(node->key);
    free_element(node->element);
    free(node);
    return;
}


static int _test_parse_integer_valid() {
  mu_start();
  int result;
  char const* input = "0";
  char const* pos = input;
  bool is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 1);
  mu_check(result == 0);

  input = "1";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 1);
  mu_check(result == 1);

  input = "-1";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(result == -1);

  input = "123";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(result == 123);

  input = "-123";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(result == -123);

  input = "123AB";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(result == 123);

  mu_end();
}

static int _test_parse_integer_invalid() {
  mu_start();
  int result;
  char const* input = "--123";
  char const* pos = input;
  bool is_success = parse_int(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "a";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "-";
  pos = input;
  is_success = parse_int(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  mu_end();
}

static int _test_parse_string_valid() {
  mu_start();
  char* result;
  char const* input = "\"\"";
  char const* pos = input;
  bool is_success = parse_string(&result, &pos);
  mu_check(is_success);
  mu_check_strings_equal("", result);
  mu_check(pos == input + 2);
  free(result);

  input = "\"a\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(is_success);
  mu_check_strings_equal("a", result);
  mu_check(pos == input + 3);
  free(result);

  input = "\"abc\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(is_success);
  mu_check_strings_equal("abc", result);
  mu_check(pos == input + 5);
  free(result);

  input = "\"abc\"123";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(is_success);
  mu_check_strings_equal("abc", result);
  mu_check(pos == input + 5);
  free(result);

  mu_end();
}

static int _test_parse_string_invalid() {
  mu_start();
  char* result;
  char const* input = "";
  char const* pos = input;
  bool is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "\"abc";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 4);

  input = "abc\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "\"a\n\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\a\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\b\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\r\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\f\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\t\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "\"a\v\"";
  pos = input;
  is_success = parse_string(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  mu_end();
}

static int _test_parse_bool_valid() {
  mu_start();
  bool result;
  char const* input = "true";
  char const* pos = input;
  bool is_success = parse_bool(&result, &pos);
  mu_check(is_success);
  mu_check(result == true);
  mu_check(pos == input + 4);

  input = "false";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(is_success);
  mu_check(result == false);
  mu_check(pos == input + 5);

  input = "true wove";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(is_success);
  mu_check(result == true);
  mu_check(pos == input + 4);

  input = "falsehood";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(is_success);
  mu_check(result == false);
  mu_check(pos == input + 5);


  mu_end();
}

static int _test_parse_bool_invalid() {
  mu_start();
  bool result;
  char const* input = "bacon";
  char const* pos = input;
  bool is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "tuth";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "treth";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "truth";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);

  input = "filed";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "failed";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "fales";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);

  input = "falsd";
  pos = input;
  is_success = parse_bool(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 4);

  mu_end();
}

static int _test_parse_null_valid() {
  mu_start();
  bool result;
  char const* input;
  char const* pos;

  input = "null";
  pos = input;
  result = parse_null(&pos);
  mu_check(result);
  mu_check(pos = input + 4);

  input = "nullify";
  pos = input;
  result = parse_null(&pos);
  mu_check(result);
  mu_check(pos = input + 4);

  mu_end();
}

static int _test_parse_null_invalid() {
  mu_start();
  bool result;
  char const* input;
  char const* pos;

  input = "parsing";
  pos = input;
  result = parse_null(&pos);
  mu_check(!result);
  mu_check(pos = input);

  input = "nil";
  pos = input;
  result = parse_null(&pos);
  mu_check(!result);
  mu_check(pos = input + 1);

  input = "nuull";
  pos = input;
  result = parse_null(&pos);
  mu_check(!result);
  mu_check(pos = input + 2);

  input = "nuly";
  pos = input;
  result = parse_null(&pos);
  mu_check(!result);
  mu_check(pos = input + 3);

  mu_end();
}

static int _test_parse_list_valid() {
  mu_start();
  bool is_success;
  char const* input;
  char const* pos;
  Element elem;
  elem.type = ELEMENT_LIST;
  Node* node;
  Node* node1;

  input = "[]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = "[1]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(elem.as_list != NULL);
  mu_check(elem.as_list->element.type == ELEMENT_INT);
  mu_check(elem.as_list->element.as_int = 1);
  mu_check(elem.as_list->next == NULL);
  free_element(elem);

  input = "[1,2]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[1,2,3]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 7);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 3);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[1,[2,3]]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 9);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_LIST);
  node1 = node->element.as_list;
  mu_check(node1 != NULL);
  mu_check(node1->element.type == ELEMENT_INT);
  mu_check(node1->element.as_int = 2);
  node1 = node1->next;
  mu_check(node1 != NULL);
  mu_check(node1->element.type == ELEMENT_INT);
  mu_check(node1->element.as_int = 3);
  node1 = node1->next;
  mu_check(node1 == NULL);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[[1],[2]]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 9);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_LIST);
  node1 = node->element.as_list;
  mu_check(node1 != NULL);
  mu_check(node1->element.type == ELEMENT_INT);
  mu_check(node1->element.as_int = 1);
  node1 = node1->next;
  mu_check(node1 == NULL);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_LIST);
  node1 = node->element.as_list;
  mu_check(node1 != NULL);
  mu_check(node1->element.type == ELEMENT_INT);
  mu_check(node1->element.as_int = 2);
  node1 = node1->next;
  mu_check(node1 == NULL);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[[]]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_LIST);
  node1 = node->element.as_list;
  mu_check(node1 == NULL);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[ 1,2]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 6);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[1 ,2]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 6);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[1, 2]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 6);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[1,2 ]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 6);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  input = "[]abc";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = "[ 1, 2, 3 ] Hello";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(is_success);
  mu_check(pos == input + 11);
  node = elem.as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 2);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 3);
  node = node->next;
  mu_check(node == NULL);
  free_element(elem);

  mu_end();
}

static int _test_parse_list_invalid() {
  mu_start();
  bool is_success;
  char const* input;
  char const* pos;
  Element elem;
  elem.type = ELEMENT_LIST;

  input = "[";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);
  mu_check(elem.as_list == NULL);

  input = "]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input);
  mu_check(elem.as_list == NULL);

  input = "[[]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);
  mu_check(elem.as_list == NULL);

  input = ",1,";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input);
  mu_check(elem.as_list == NULL);

  input = ",1]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input);
  mu_check(elem.as_list == NULL);

  input = "[1 1]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);
  mu_check(elem.as_list == NULL);

  input = "[1,]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);
  mu_check(elem.as_list == NULL);

  input = "[1,1,";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 5);
  mu_check(elem.as_list == NULL);

  input = "[a\"]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);
  mu_check(elem.as_list == NULL);

  input = "[\"a]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 4);
  mu_check(elem.as_list == NULL);

  input = "[1\"a\"]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);
  mu_check(elem.as_list == NULL);

  input = "[\"a\"1]";
  pos = input;
  is_success = parse_list(&elem.as_list, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 4);
  mu_check(elem.as_list == NULL);

  mu_end();
}

static int _test_parse_element_valid() {
  mu_start();
  bool is_success;
  char const* input;
  char const* pos;
  Element elem;

  input = "1";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 1);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = 1);
  free_element(elem);

  input = "21";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = 21);
  free_element(elem);

  input = "-1";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = -1);
  free_element(elem);

  input = "-415";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = -415);
  free_element(elem);

  input = "\"a\"";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "a");
  free_element(elem);

  input = "\"\"";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "");
  free_element(elem);

  input = "\"abc\"";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "abc");
  free_element(elem);

  input = "[]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = "[1]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "[1,2,3]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 7);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "[\"Hello\", \"World\"]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 18);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  mu_check(elem.as_list->element.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_list->element.as_string, "Hello");
  mu_check(elem.as_list->next != NULL);
  mu_check(elem.as_list->next->element.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_list->next->element.as_string, "World");
  mu_check(elem.as_list->next->next == NULL);
  free_element(elem);

  input = "[[1],[2]]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 9);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "null";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = "true";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);

  input = "false";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);

  input = " 34";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = 34);
  free_element(elem);

  input = " \"b\"";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "b");
  free_element(elem);

  input = " []";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 3);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = " null";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = " true";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);

  input = "false ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 5);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);

  input = "51 ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = 51);
  free_element(elem);

  input = "\"cd\" ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "cd");
  free_element(elem);

  input = "[] ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = " [ 1 , 2 ] ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 10);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "null ";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = "45AB";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 2);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int = 45);
  free_element(elem);

  input = "\"ef\"CD";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "ef");
  free_element(elem);

  input = "[ 1,2 ]EF";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 7);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "nullify";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = "truely";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(is_success);
  mu_check(pos == input + 4);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);

  mu_end();
}

static int _test_parse_element_invalid() {
  mu_start();
  bool is_success;
  char const* input;
  char const* pos;
  Element elem;

  input = "";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "--123";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "-";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "a";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "a\"";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "\"a";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "[";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = "]";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input);

  input = "nil";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 1);

  input = " noodles";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = "truth";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);

  input = "failed";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = " test";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 2);

  input = " fame";
  pos = input;
  is_success = parse_element(&elem, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 3);

  mu_end();
}

static int _test_read_json_valid() {
  mu_start();
  char* filename = "test_read_json.json";
  FILE* file;
  bool is_success;
  Element elem;
  char* input;

  input = "1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  free_element(elem);

  input = "1 ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  free_element(elem);

  input = "21";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 21);
  free_element(elem);

  input = "-1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -1);
  free_element(elem);

  input = "-415";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -415);
  free_element(elem);

  input = "\"a\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "a");
  free_element(elem);

  input = "\"\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "");
  free_element(elem);

  input = "\"abc\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "abc");
  free_element(elem);

  input = "[]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = "[1,2,3]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "[\"Hello\",\"World\"]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "[[1],[2]]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);

  input = "null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = "true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);

  input = "false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);

  input = " 34";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 34);
  free_element(elem);

  input = " \"b\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "b");
  free_element(elem);

  input = " []";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);

  input = " null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);

  input = " true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);

  input = " false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);

  is_success = read_json("missing file", &elem);
  mu_check(!is_success);

  remove(filename);
  mu_end();
}

static int _test_read_json_invalid() {
  mu_start();
  char* filename = "test_read_json.json";
  FILE* file;
  bool is_success;
  Element elem;
  char* input;

  input = "";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "--123";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "-";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "a";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "a\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "\"a";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "[";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "nil";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = " noodles";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "truth";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "failed";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = " test";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = " fame";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "45AB";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "\"ef\"CD";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "[ 1, 2 ]EF";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "nullify";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  input = "truely";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(!is_success);

  is_success = read_json("missing file", &elem);
  mu_check(!is_success);

  remove(filename);
  mu_end();
}

static int _test_print_element_valid() {
  mu_start();
  char* filename = "test_print_element.json";
  FILE* file;
  bool is_success;
  Element elem;
  char* input;

  input = "1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  print_element(elem);
  free_element(elem);

  input = "1 ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  print_element(elem);
  free_element(elem);

  input = "21";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 21);
  print_element(elem);
  free_element(elem);

  input = "-1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -1);
  print_element(elem);
  free_element(elem);

  input = "-415";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -415);
  print_element(elem);
  free_element(elem);

  input = "\"a\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "a");
  print_element(elem);
  free_element(elem);

  input = "\"\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "");
  print_element(elem);
  free_element(elem);

  input = "\"abc\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "abc");
  print_element(elem);
  free_element(elem);

  input = "[]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  print_element(elem);
  free_element(elem);

  input = "[1,2,3]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  print_element(elem);
  free_element(elem);

  input = "[\"Hello\",\"World\"]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  print_element(elem);
  free_element(elem);

  input = "[[1],[2]]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  print_element(elem);
  free_element(elem);

  input = "null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  print_element(elem);
  free_element(elem);

  input = "true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  print_element(elem);
  free_element(elem);

  input = "false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  print_element(elem);
  free_element(elem);

  input = " 34";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 34);
  print_element(elem);
  free_element(elem);

  input = " \"b\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "b");
  print_element(elem);
  free_element(elem);

  input = " []";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  print_element(elem);
  free_element(elem);

  input = " null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  print_element(elem);
  free_element(elem);

  input = " true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  print_element(elem);
  free_element(elem);

  input = " false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  print_element(elem);
  free_element(elem);

  remove(filename);
  mu_end();
}

static int _test_write_json_valid() {
  mu_start();
  char* filename = "test_read_json.json";
  char* filename1 = "test_write_json.json";
  FILE* file;
//  FILE* file1;
  bool is_success;
  Element elem, elem1;
  char* input;

  input = "1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  free_element(elem);
  free_element(elem1);

  input = "1 ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 1);
  free_element(elem);
  free_element(elem1);

  input = "21";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 21);
  free_element(elem);
  free_element(elem1);

  input = "-1";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -1);
  free_element(elem);
  free_element(elem1);

  input = "-415";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == -415);
  free_element(elem);
  free_element(elem1);

  input = "\"a\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "a");
  free_element(elem);
  free_element(elem1);

  input = "\"\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "");
  free_element(elem);
  free_element(elem1);

  input = "\"abc\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "abc");
  free_element(elem);
  free_element(elem1);

  input = "[]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);
  free_element(elem1);

  input = "[1,2,3]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);
  free_element(elem1);

  input = "[\"Hello\",\"World\"]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);
  free_element(elem1);

  input = "[[1],[2]]";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list != NULL);
  free_element(elem);
  free_element(elem1);

  input = "null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);
  free_element(elem1);

  input = "true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);
  free_element(elem1);

  input = "false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);
  free_element(elem1);

  input = " 34";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_INT);
  mu_check(elem.as_int == 34);
  free_element(elem);
  free_element(elem1);

  input = " \"b\"";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_STRING);
  mu_check_strings_equal(elem.as_string, "b");
  free_element(elem);
  free_element(elem1);

  input = " []";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_LIST);
  mu_check(elem.as_list == NULL);
  free_element(elem);
  free_element(elem1);

  input = " null";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_NULL);
  mu_check(elem.as_null == NULL);
  free_element(elem);
  free_element(elem1);

  input = " true";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == true);
  free_element(elem);
  free_element(elem1);

  input = " false";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_BOOL);
  mu_check(elem.as_bool == false);
  free_element(elem);
  free_element(elem1);

  Element *psubelem1;
 // Node* node;
  
  
  input = "{ \"key1\": \"cloud\", \"key2\": \"rain\", \"key3\": \"sunny\"} ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_OBJECT);
  psubelem1 = get_element(&(elem.as_object), "key3");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "sunny");
  psubelem1 = get_element(&(elem.as_object), "key2");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "rain");
  psubelem1 = get_element(&(elem.as_object), "key1");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "cloud");
  free_element(elem);	
  free_element(elem1);
	
	input = "{ \"key1\": \"cloud\", \"key2\": \"rain\", \"key3\": \"sunny\", \"key1\": -1} ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_OBJECT);
  psubelem1 = get_element(&(elem.as_object), "key3");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "sunny");
  psubelem1 = get_element(&(elem.as_object), "key2");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "rain");
  psubelem1 = get_element(&(elem.as_object), "key1");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_INT);
  mu_check(psubelem1->as_int = -1);
  free_element(elem);
  free_element(elem1);
  
  input = "{\"key3\": \"sunny\", \"key1\": \"cloud\", \"key2\": {\"inner1\": 1, \"inner2\": \"istring2\"}} ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_OBJECT);
  psubelem1 = get_element(&(elem.as_object), "key3");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "sunny");
  psubelem1 = get_element(&(elem.as_object), "key1");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem1->as_string, "cloud");
  psubelem1 = get_element(&(elem.as_object), "key2");
  mu_check(psubelem1 != NULL);
  mu_check(psubelem1->type == ELEMENT_OBJECT);
  Element* pinnerelem;
  pinnerelem = get_element(&(psubelem1->as_object), "inner1");
 	mu_check(pinnerelem->type == ELEMENT_INT);
 	mu_check(pinnerelem->as_int == 1);
   
  pinnerelem = get_element(&(psubelem1->as_object), "inner2");
 	mu_check(pinnerelem->type == ELEMENT_STRING);
 	mu_check_strings_equal(pinnerelem->as_string, "istring2");
  free_element(elem);
  free_element(elem1);
  
  
  remove(filename);
  remove(filename1);
  mu_end();
}

static int _test_empty_object() {
	mu_start();
  char* filename = "test_read_json.json";
  char* filename1 = "test_write_json.json";
  FILE* file;
 // FILE* file1;
  bool is_success;
  Element elem, elem1;
  char* input;
  
  input = " { } ";
  file = fopen(filename, "w");
  fwrite(input, sizeof(char), strlen(input), file);
  fclose(file);
  is_success = read_json(filename, &elem1);
  mu_check(is_success);
  write_json(filename1, elem1);
  is_success = read_json(filename1, &elem);
  mu_check(is_success);
  mu_check(elem.type == ELEMENT_OBJECT);
  mu_check(elem.as_object == NULL);
  free_element(elem);
  free_element(elem1);
  
  remove(filename);
  remove(filename1);
  mu_end();
}

static int _test_single_object_valid() {
	
	Node* node = NULL;
  BSTNode* result = NULL;
  char const* input = 0;
  char const* pos = input;
  Element* psubelem;
  
  mu_start();
  input = "{ \"key1\": \"cloud\" } ";
  pos = input;
  result = NULL;  
  bool is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 19);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "cloud");
  _bst_free(result);
  
  input = "{ \"key1\": -135 } ";
  pos = input;
  result = NULL;  
 	is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 16);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_INT);
  mu_check(psubelem->as_int = -135);
  _bst_free(result);
  
  input = "{ \"key1\": [1, 3, 5] }   ";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 21);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_LIST);
  node = psubelem->as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 3);
  node = node->next;
 	mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 5);
  node = node->next;
  mu_check(node == NULL);
  _bst_free(result);
  
  mu_end();
}

static int _test_multi_object_valid() {
	Node* node = NULL;
  BSTNode* result = NULL;
  char const* input = 0;
  char const* pos = input;
  Element* psubelem;
  
  mu_start();
  input = "{ \"key1\": \"cloud\", \"key2\": \"rain\", \"key3\": \"sunny\"} ";
  pos = input;
  result = NULL;
  bool is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 51);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "cloud");
  psubelem = get_element(&result, "key2");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "rain");
  psubelem = get_element(&result, "key4");
  mu_check(psubelem == NULL);
  psubelem = get_element(&result, "key3");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "sunny");
  _bst_free(result);
  
  
  input = "{ \"key1\": \"cloud\", \"key2\": \"rain\", \"key3\": \"sunny\", \"key1\": -1} ";
  pos = input;
  result = NULL;
 	is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 63);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_INT);
  mu_check(psubelem->as_int = -1);
  psubelem = get_element(&result, "key2");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "rain");
  psubelem = get_element(&result, "key3");
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "sunny");
  _bst_free(result);
  
  input = "{ \"key1\": [1, 3, 5], \"longkey2\": \"longint\", \"sh\": 4, \"bool\": true }";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 67);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem->type == ELEMENT_LIST);
  node = psubelem->as_list;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 1);
  node = node->next;
  mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 3);
  node = node->next;
 	mu_check(node != NULL);
  mu_check(node->element.type == ELEMENT_INT);
  mu_check(node->element.as_int = 5);
  node = node->next;
  mu_check(node == NULL);
  _bst_free(result);
  
  mu_end();
}

static int _test_nested_object_valid() {
//	Node* node = NULL;
  BSTNode* result = NULL;
  char const* input = 0;
  char const* pos = input;
  Element* psubelem;
  Element* pinnerelem;
  
  mu_start();
  input = "{ \"key1\": { \"inner1\": [1, 3, 5], \"inner2\":\"ice\", \"inner3\": 4}, \"key2\": {\"inner1\": 1, \"inner2\": 2}, \"key3\": \"sunny\"} ";
  pos = input;
  result = NULL;
  bool is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 115);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_OBJECT);
  
  psubelem = get_element(&result, "key2");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_OBJECT);
  pinnerelem = get_element(&(psubelem->as_object), "inner1");
  mu_check(pinnerelem->type == ELEMENT_INT);
  mu_check(pinnerelem->as_int == 1);
  pinnerelem = get_element(&(psubelem->as_object), "inner2");
  mu_check(pinnerelem->type == ELEMENT_INT);
  mu_check(pinnerelem->as_int == 2);
    
  psubelem = get_element(&result, "key3");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "sunny");	
  _bst_free(result);  
  
  input = "{\"key3\": \"sunny\", \"key1\": \"cloud\", \"key2\": {\"inner1\": 1, \"inner2\": \"istring2\"}} ";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(is_success);
  mu_check(pos == input + 79);
  psubelem = get_element(&result, "key1");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "cloud");
  
  psubelem = get_element(&result, "key2");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_OBJECT);
  pinnerelem = get_element(&(psubelem->as_object), "inner1");
  mu_check(pinnerelem->type == ELEMENT_INT);
  mu_check(pinnerelem->as_int == 1);
  pinnerelem = get_element(&(psubelem->as_object), "inner2");
  mu_check(pinnerelem->type == ELEMENT_STRING);
  mu_check_strings_equal(pinnerelem->as_string, "istring2");
  
  psubelem = get_element(&result, "key3");
  mu_check(psubelem != NULL);
  mu_check(psubelem->type == ELEMENT_STRING);
  mu_check_strings_equal(psubelem->as_string, "sunny");	
  _bst_free(result); 
	
	mu_end();
}
static int _test_object_invalid() {
	
//	Node* node = NULL;
  BSTNode* result = NULL;
  char const* input = 0;
  char const* pos = input;
//  Element* psubelem;
  
  mu_start();
  
  input = "{ \"key1\"  } ";
  pos = input;
  result = NULL;
  bool is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 10);
  
  
  input = "{ \"key1\":  } ";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 11);
  
  input = "{ \"key1\": --135 } ";
  pos = input;
  result = NULL;
 	is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 11);

  input = "{ \"key1\": -135, } ";
  pos = input;
  result = NULL;
 	is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 17);
    
  input = "{ \"  }";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 6);
  
  input = "{ \"key1\": \"cloud\", \"key2\": \"rain\", : \"sunny\"} ";
  pos = input;
  result = NULL;
  is_success = parse_object(&result, &pos);
  mu_check(!is_success);
  mu_check(pos == input + 35);

  mu_end();
}


int main(int argc, char* argv[]) {
  mu_run(_test_parse_integer_valid);
  mu_run(_test_parse_integer_invalid);
  mu_run(_test_parse_string_valid);
  mu_run(_test_parse_string_invalid);
  mu_run(_test_parse_null_valid);
  mu_run(_test_parse_null_invalid);
  mu_run(_test_parse_bool_valid);
  mu_run(_test_parse_bool_invalid);
  mu_run(_test_parse_list_valid);
  mu_run(_test_parse_list_invalid);
  mu_run(_test_parse_element_valid);
  mu_run(_test_parse_element_invalid);
  mu_run(_test_read_json_valid);
  mu_run(_test_read_json_invalid);
  mu_run(_test_print_element_valid);
  mu_run(_test_write_json_valid);

  mu_run(_test_empty_object);
  mu_run(_test_single_object_valid);
  mu_run(_test_multi_object_valid);
  mu_run(_test_nested_object_valid);
  mu_run(_test_object_invalid);
  return EXIT_SUCCESS;
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */