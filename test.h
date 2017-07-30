#ifndef __TEST_H__
#define __TEST_H__
#define LIST_NODE_CHAR_LENGTH 7
enum regx_types{
    FIXED=0,LETTER_NO_REPET=1,LETTER_REPET=2,NUMBERS=3,LENGTH=4,NODE_INFO=5
};
struct list_regx_node{
    bool flag;
    int numbers[3];
    std::string str;
    char characters[LIST_NODE_CHAR_LENGTH];
    enum regx_types type;
    // std::vector<char> letters;
    struct list_regx_node *next,*prev;
};
#endif