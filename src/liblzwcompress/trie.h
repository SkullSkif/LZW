#pragma once

#include <stdint.h>
#include <stdbool.h>

#define N 256 // Размер ASCII

typedef struct TrieNode TrieNode;

struct TrieNode
{
    uint32_t value;
    TrieNode *children[N];
    bool is_leaf;
};

TrieNode *make_trie_node();

void free_trie_node(TrieNode *node);

TrieNode *insert_char(TrieNode *root, unsigned char _char, uint32_t value);

bool search_char(TrieNode *root, unsigned char _char, TrieNode **node_found);