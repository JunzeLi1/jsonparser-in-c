#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "clog.h"
#include "json.h"
#include <limits.h>

// check whether is escaped character
static bool is_escape(char c) {
  return c == '\n' || c == '\r' || c == '\a' || c == '\b' ||
         c == '\f' || c == '\t' || c == '\v';
}

// free list
static void free_list(Node* head) {
  // null list
  if (!head) {
    return;
  }

  Node* curr = head;
  // free all nodes
  while (curr) {
    Node* temp = curr;
    curr = curr->next;
    // free element
    free_element(temp->element);
    // free node
    free(temp);
  }
}

// eat all whitespaces
static void eat_whitespaces(char const** a_pos) {
  const char* pos = *a_pos;
  // eat whitespaces
  while (*pos != '\0' && isspace(*pos)) {
    ++pos;
  }
  // done
  *a_pos = pos;
}

//BST implements
static void _bst_insert(BSTNode** a_root, char const* key, Element element){
    if (*a_root == NULL) {
    	BSTNode *node = malloc(sizeof (BSTNode));
   		node->key = malloc(strlen(key) + 1);
    	for (int i = 0; i < strlen(key) + 1; i++)
        	node->key[i] =0x00;
    	strncpy(node->key, key, strlen(key) + 1);
    
    	node->element = element;
    	node->left = NULL;
    	node->right = NULL;
      *a_root = node;
      return;
    }        
   
    if (strcmp(key, (*a_root)->key) < 0)
			_bst_insert(&((*a_root)->left), key, element);
    else if (strcmp(key, (*a_root)->key) > 0)
			_bst_insert(&((*a_root)->right), key, element);  
		else {	//already exist, update the value
			free_element((*a_root)->element);
			(*a_root)->element = element;
		}
}

Element* get_element(BSTNode** root, char const* key){ 
	if (*root == NULL) {
    return NULL; //key not found
  }        
   
  if (strcmp(key, (*root)->key) < 0)
		return get_element(&((*root)->left), key);
  else if (strcmp(key, (*root)->key) > 0)
		return get_element(&((*root)->right), key);
	else {	//found
		return &((*root)->element);
	}	
}


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



bool parse_int(int* a_value, const char** a_pos) {
  // whether is negative
  bool is_negative = false;
  // success or not
  bool ret = false;
  // start pos
  const char* pos = *a_pos;
  // initial value
  int value = 0;

  // invalid
  if (*pos == '\0') {
    return false;
  }

  // negative
  if (*pos == '-') {
    is_negative = true;
    ++pos;
  }

  // process all digits
  while (*pos != '\0' && isdigit(*pos)) {
    // calculate new value
    value = (value * 10) + (*pos - '0');
    ++pos;
    // success
    ret = true;
  }

  // update pos
  *a_pos = pos;
  // set value
  if (ret) {
    *a_value = is_negative ? -value : value;
  }

  return ret;
}

bool parse_string(char** a_string, char const** a_pos) {
  const char* pos = *a_pos;

  // not "
  if (*pos != '"') {
    return false;
  }

  // start parse string
  ++pos;
  // string start
  const char* start = pos;
  // parse until '\0', ", or escaped
  while (*pos != '\0' && *pos != '"' && !is_escape(*pos)) {
    ++pos;
  }

  // invalid string end
  if (*pos != '"') {
    *a_pos = pos;
    return false;
  }

  // success, calculate string length
  size_t n = pos - start;
  // malloc for string
  *a_string = malloc((n + 1) * sizeof(char));
  // copy string
  for (size_t i = 0; i < n; ++i) {
    (*a_string)[i] = start[i];
  }
  // set terminator
  (*a_string)[n] = '\0';

  // update pos
  *a_pos = pos + 1;
  return true;
}

bool parse_null(char const** a_pos) {
  char const* pos = *a_pos;

  // not 'n'
  if (*pos != 'n') {
    return false;
  }

  ++pos;
  // not 'u'
  if (*pos != 'u') {
    *a_pos = pos;
    return false;
  }

  ++pos;
  // not 'l'
  if (*pos != 'l') {
    *a_pos = pos;
    return false;
  }

  ++pos;
  // not 'l'
  if (*pos != 'l') {
    *a_pos = pos;
    return false;
  }

  // success, update pos
  *a_pos = pos + 1;
  return true;
}

bool parse_bool(bool* a_value, char const** a_pos) {
  char const* pos = *a_pos;

  // start with 't'
  if (*pos == 't') {
    ++pos;
    // not 'r'
    if (*pos != 'r') {
      *a_pos = pos;
      return false;
    }

    ++pos;
    // not 'u'
    if (*pos != 'u') {
      *a_pos = pos;
      return false;
    }

    ++pos;
    // not 'e'
    if (*pos != 'e') {
      *a_pos = pos;
      return false;
    }

    // success, update pos
    *a_pos = pos + 1;
    // set bool value
    *a_value = true;
    return true;
  }

  // start with 'f'
  if (*pos == 'f') {
    ++pos;
    // not 'a'
    if (*pos != 'a') {
      *a_pos = pos;
      return false;
    }

    ++pos;
    // not 'l'
    if (*pos != 'l') {
      *a_pos = pos;
      return false;
    }

    ++pos;
    // not 's'
    if (*pos != 's') {
      *a_pos = pos;
      return false;
    }

    ++pos;
    // not 'e'
    if (*pos != 'e') {
      *a_pos = pos;
      return false;
    }

    // success, update pos
    *a_pos = pos + 1;
    // set bool
    *a_value = false;
    return true;
  }

  // invalid start
  return false;
}

bool parse_list(Node** a_head, char const** a_pos) {
  *a_head = NULL;
  Node* tail = NULL;

  // not start with '[]
  if (**a_pos != '[') {
    return false;
  }
  // update pos
  *a_pos += 1;

  // eat whitespaces
  eat_whitespaces(a_pos);
  // encounter ']', null list
  if (**a_pos == ']') {
    // update pos
    *a_pos += 1;
    return true;
  }

  // start parse element
  while (true) {
    // create a node
    Node* node = malloc(sizeof(Node));
    node->next = NULL;
    // parse element
    bool ret = parse_element(&node->element, a_pos);
    // parse element failed
    if (!ret) {
      // free node
      free(node);
      // free list
      free_list(*a_head);
      // failed
      *a_head = NULL;
      return false;
    }

    // parse success, put node to list
    if (!tail) {
      *a_head = node;  // node is head
    } else {
      tail->next = node;  // put node to tail
    }
    // update tail
    tail = node;

    // eat whitespaces
    eat_whitespaces(a_pos);
    // found ']', done
    if (**a_pos == ']') {
      // update pos
      *a_pos += 1;
      return true;
    }

    // found ',', parse next element
    if (**a_pos == ',') {
      *a_pos += 1;
      continue;
    }

    // invalid list, free list
    free_list(*a_head);
    // set head to NULL
    *a_head = NULL;
    return false;
  }
}

bool parse_object(BSTNode** a_root, char const**a_pos) {
    const char* pos = *a_pos;
    bool wait_for_next = false;
    bool ret;
    int finish = 0;
    pos++;
    while (!finish) {
    	eat_whitespaces(&pos);
      if (*pos == '\"') {
      	Element element;
        //get key
        int key_len = 0;
        const char* key_pos = pos;
        key_pos++;
        while (*key_pos != '\"') {
        	key_len++;
         	key_pos++;
        }
        char *key = malloc(key_len + 1);
        for (int i = 0; i < key_len + 1; i++)
        	key[i] =0x00;
        int i = 0;
        key_pos = pos;
        key_pos++;
        while (*key_pos != '\"') {
        	if ((*key_pos) == '\0' || is_escape(*key_pos))	{//unexpected END
        		free(key);
        		*a_pos = key_pos;
        		_bst_free(*a_root);
        		return false;
        	}
        	key[i] = *key_pos;
         	key_pos++;
         	i++;
        }
            
        pos = key_pos;
        pos++;  //skip the right \"
        eat_whitespaces(&pos);
            
            
		    if (*pos == ':') {
		    	pos++;
		     	ret = parse_element(&element, &pos);
		     	if (!ret) {
		     		free(key);
		     		*a_pos = pos;
		     		_bst_free(*a_root);
        		return false;
		     	}
		     		
		      //get value
		    } else {
		    	free(key);
		    	*a_pos = pos;
		    	_bst_free(*a_root);
		      return false;   //cannot find value with the key
		   	}    
		  	_bst_insert(a_root, key, element);
		  	free(key);
		  	wait_for_next = false;
      } else  if (*pos == ',') {
      	wait_for_next = true;
      	pos++;
      }
      else if (*pos == '}') {
      	if (wait_for_next) {
      		pos++;
      		*a_pos = pos;
      		_bst_free(*a_root);
      		return false;
      	}
        pos++;
        finish = 1;
      } else {  //unexpected other character
      		*a_pos = pos;
      		_bst_free(*a_root);
        	return false;
      }
            
    }
    
    *a_pos = pos;
    return true;
}

bool parse_element(Element* a_element, char const** a_pos) {
  // eat whitespaces
  eat_whitespaces(a_pos);
  // set start pos
  const char* pos = *a_pos;

  // may be int
  if (isdigit(*pos) || *pos == '-') {
    a_element->type = ELEMENT_INT;
    return parse_int(&a_element->as_int, a_pos);
  }

  // may be string
  if (*pos == '"') {
    a_element->type = ELEMENT_STRING;
    return parse_string(&a_element->as_string, a_pos);
  }

  // may be list
  if (*pos == '[') {
    a_element->type = ELEMENT_LIST;
    return parse_list(&a_element->as_list, a_pos);
  }

  // may be bool
  if (*pos == 't' || *pos == 'f') {
    a_element->type = ELEMENT_BOOL;
    return parse_bool(&a_element->as_bool, a_pos);
  }

  // may be null
  if (*pos == 'n') {
    a_element->type = ELEMENT_NULL;
    a_element->as_null = NULL;
    return parse_null(a_pos);
  }
  // may be object
  if (*pos == '{') {
    a_element->type = ELEMENT_OBJECT;
    a_element->as_object = NULL;
    return parse_object(&a_element->as_object, a_pos);
  }

  // invalid jSON
  return false;
}

void write_json(char const* filename, Element element) {
  // open file
  FILE* file = fopen(filename, "w");
  // print element to file
  print_element_to_file(element, file);
  // close file
  fclose(file);
}

bool read_json(char const* filename, Element* a_element) {
  // open file
  FILE* file = fopen(filename, "r");
  // open file failed
  if (!file) {
    fprintf(stderr, "The file does not exist.\n");
    return false;
  }

  // get file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // read all content to buffer
  char* buffer = malloc((size + 1) * sizeof(char));
  fread(buffer, sizeof(char), size, file);
  // set string terminator
  buffer[size] = '\0';
  fclose(file);

  // set start position
  char const* pos = buffer;
  // trail ending whitespaces
  char* end = buffer + size - 1;
  while (end >= pos && isspace(*end)) {
    *end = '\0';
    --end;
  }
  // set the end of string (without tail whitespace)
  ++end;

  // parse buffer
  bool is_success = parse_element(a_element, &pos);
  // parse failed
  if (!is_success) {
    free(buffer);
    fprintf(stderr, "The file does not contain a valid JSON element.\n");
    return false;
  }

  // has trailing characters
  if (pos != end) {
    free(buffer);
    // need to free element
    free_element(*a_element);
    fprintf(stderr, "The file starts with a valid JSON element but has trailing characters.\n");
    return false;
  }

  // success, free buffer
  free(buffer);
  return true;
}

void print_element(Element element) {
  // call print to file
  print_element_to_file(element, stdout);
}

// print list to file
static void print_list_to_file(Node* head, FILE* file) {
  Node* curr = head;
  // empty list
  if (!curr) {
    return;
  }

  // print first node to file
  print_element_to_file(curr->element, file);
  curr = curr->next;
  // print left nodes
  while (curr) {
    // print ','
    fprintf(file, ",");
    // print element
    print_element_to_file(curr->element, file);
    // process next
    curr = curr->next;
  }
}

static void print_object_to_file(BSTNode* a_root, FILE* file, bool is_first_element) {
	if (a_root == NULL) {
		return;
	}        
	if (!is_first_element)
		fprintf(file, ",");
	fprintf(file, "\"%s\" : ", a_root->key);
	print_element_to_file(a_root->element, file);
	print_object_to_file(a_root->left, file, false);
	print_object_to_file(a_root->right, file, false);
}

void print_element_to_file(Element element, FILE* file) {
  switch (element.type) {
    // int
    case ELEMENT_INT:
      fprintf(file, "%d", element.as_int);
      break;
    // string
    case ELEMENT_STRING:
      // need to print '"'
      fprintf(file, "\"%s\"", element.as_string);
      break;
    // list
    case ELEMENT_LIST:
      // print '['
      fprintf(file, "[");
      // print list
      print_list_to_file(element.as_list, file);
      // print ']'
      fprintf(file, "]");
      break;
    // null
    case ELEMENT_NULL:
      fprintf(file, "null");
      break;
    // bool
    case ELEMENT_BOOL: {
      char* str = element.as_bool ? "true" : "false";
      fprintf(file, "%s", str);
      break;
    }
    case ELEMENT_OBJECT: {
    	fprintf(file, "{");
    	print_object_to_file(element.as_object, file, true);    	
    	fprintf(file, "}");
    	break;
    }
  }
}

void free_element(Element element) {
  switch (element.type) {
    // string, need to free string
    case ELEMENT_STRING:
      free(element.as_string);
      break;
    // list, need to free list
    case ELEMENT_LIST:
      free_list(element.as_list);
      break;
    case ELEMENT_OBJECT:
    	_bst_free(element.as_object);
    // other case, nothing to do
    default:
      break;
  }
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */