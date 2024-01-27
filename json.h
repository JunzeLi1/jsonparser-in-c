#ifndef __JSON_H__
#define __JSON_H__

#include <stdbool.h>

typedef struct _BSTNode BSTNode;

typedef struct {
  enum {
    ELEMENT_INT, ELEMENT_STRING, ELEMENT_LIST, ELEMENT_NULL, ELEMENT_BOOL, ELEMENT_OBJECT
  } type;
  union {
    bool as_bool;
    int as_int;
    char* as_string;
    void* as_null;
    struct _Node* as_list;
    BSTNode* as_object;
    
  }; // ANONYMOUS union (C11).
} Element;


typedef struct _Node {
  struct _Node* next;
  Element element;
} Node;

typedef struct _BSTNode {
    char* key;
    Element element;
    BSTNode* left;
    BSTNode* right;
} BSTNode;

bool parse_int(int* a_value, char const** a_pos);

bool parse_string(char** a_s, char const** a_pos);

bool parse_list(Node** a_head, char const** a_pos);

bool parse_bool(bool* a_value, char const** a_pos);

bool parse_null(char const** a_pos);

bool parse_element(Element* a_element, char const** a_pos);

bool parse_object(BSTNode** a_root, char const**a_pos);

Element* get_element(BSTNode** root, char const* key);

void print_element_to_file(Element element, FILE* file);

void print_element(Element element);

void free_element(Element element);

void write_json(char const* filename, Element element);

bool read_json(char const* filename, Element* a_element);

#endif

#define JSON_H_VERSION_2
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */