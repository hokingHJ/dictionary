#include "test.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>

#define REGEX_ITEM_MAX_LENGTH 12
#define REGEX_MAX_COUNTS 6
#define PARAMETER_MAX_LENGTH 11
#define LETTER_SIZE_POS 1
#define LETTER_COUNTS_POS 2
#define LETTER_BEGIN_POS 0
#define LETTER_END_POS 1
#define LETTER_HEAD_POS 2

const char PATH = 'p';
const char REGEX = 'x';
const char KEY = 'k';
const char *COLON_DELIMITER = ":";
const char *DASH_DELIMITER = "-";

int keyWords_group_handler(std::vector<std::string>, std::ofstream &);
bool regex_handler(std::vector<std::string>, struct list_regx_node **);
bool regex_exp_checker(std::vector<std::string>);
bool all_exp_checker(std::vector<std::string>, std::ofstream &);
void string_slicer(std::vector<std::string> &, char *, const char *);
bool init_list(struct list_regx_node *);
bool scan(struct list_regx_node *, struct list_regx_node *);
std::string int2string(int number);
struct list_regx_node *find_driver(struct list_regx_node *);
static struct list_regx_node *head = new list_regx_node;

/*
    The entry of application.
*/
int main(int argc, char *argv[])
{
    int i;
    std::vector<std::string> vec;
    std::ofstream ofs;

    if (argc > PARAMETER_MAX_LENGTH)
    {
        std::cout << "The number of parameter is too much" << std::endl;
        std::cout << "Note: You can use the -x option for regex expression" << std::endl;
        return 0;
    }
    for (i = 1; i < argc; i = i + 2)
    {
        std::string param;
        const char *p = argv[i];
        param.append(p);
        if (param.at(0) == '-')
        {
            switch (param.at(1))
            {
            case PATH:
                ofs.open(argv[i + 1], std::ios::app);
                break;
            case REGEX:
                string_slicer(vec, argv[i + 1], COLON_DELIMITER);
                break;
            default:
                std::cout << "No such option for -" << param.at(1) << std::endl;
                return 0;
            }
        }
        else
        {
            std::cout << "Parameters error!!!" << std::endl;
            return 0;
        }
    }
    std::cout << "Receiving parameters successfully!!!" << std::endl;
    std::cout << "Start checking......" << std::endl;

    if (all_exp_checker(vec, ofs) && regex_handler(vec, &head) && init_list(head) && find_driver(head) != NULL)
    {
        std::cout << "Everything is OK!!" << std::endl;
        struct list_regx_node *driver = find_driver(head);
        // int driver_pos = driver->numbers[LETTER_COUNTS_POS] + LETTER_HEAD_POS - 1;
        while (scan(driver, head))
        {
            std::string h, t;
            struct list_regx_node *ptr = head->next;
            while (ptr != driver)
            {
                h.append(ptr->str);
                ptr = ptr->next;
            }
            ptr = driver->next;
            while (ptr != head)
            {
                t.append(ptr->str);
                ptr = ptr->next;
            }
            ofs << h << driver->str << t << std::endl;
            driver->flag = true;
        }
        ofs.close();
    }
    return 0;
}

std::string int2string(int number)
{
    char buf[8];
    sprintf(buf, "%d", number);
    return buf;
}

bool scan(struct list_regx_node *driver, struct list_regx_node *head)
{
    struct list_regx_node *ptr = driver;
    while (ptr != head)
    {
        switch (ptr->type)
        {
        case NUMBERS:
            if (ptr->flag)
            {
                ptr->flag = false;
                ptr->numbers[0]++;
                if (ptr->numbers[0] > ptr->numbers[1])
                {
                    if (ptr->prev != head)
                    {
                        ptr->numbers[0] = ptr->numbers[2];
                        ptr->prev->flag = true;
                    }
                    else
                    {
                        return false; //Terminate
                    }
                }
                else
                {
                    ptr->str = int2string(ptr->numbers[0]);
                    return true;
                }
            }
            else
            {
                return true;
            }
            break;
        case LETTER_REPET:
            if (ptr->flag)
            {
                int i, driver_pos;
                ptr->flag = false;
                driver_pos = ptr->numbers[LETTER_COUNTS_POS] + LETTER_HEAD_POS - 1;
                for (i = driver_pos; i >= LETTER_HEAD_POS; i--)
                {
                    ptr->characters[i]++;
                    if (ptr->characters[i] > ptr->characters[LETTER_END_POS])
                    {
                        ptr->characters[i] = ptr->characters[LETTER_BEGIN_POS];
                    }
                    else
                    {
                        const char *tmp;
                        std::string s;
                        tmp = &(ptr->characters[LETTER_HEAD_POS]);
                        ptr->str = s.append(tmp, ptr->numbers[LETTER_COUNTS_POS]);
                        std::cout << "@:" << ptr->str << std::endl;
                        return true;
                    }
                }
                if (ptr->prev != head)
                {
                    ptr->prev->flag = true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
            break;
        case FIXED:
            if (ptr->flag)
            {
                if (ptr->prev != head)
                {
                    ptr->flag = false;
                    ptr->prev->flag = true;
                }
                else
                {
                    return false; //Terminate!!!
                }
            }
            else
            {
                return true;
            }
            break;
            return false;
        }
        ptr = ptr->prev;
    }
    return false;
}
struct list_regx_node *find_driver(struct list_regx_node *head)
{
    struct list_regx_node *ptr;
    ptr = head->prev;
    while (ptr != head)
    {
        if (ptr->type != FIXED)
        {
            break;
        }
        ptr = ptr->prev;
    };
    if (ptr != head)
    {
        std::cout << "Find a driver!!!" << std::endl;
        return ptr;
    }
    else
    {
        std::string temp;
        ptr = ptr->next;
        while (ptr != head)
        {
            temp.append(ptr->str);
            ptr = ptr->next;
        }
        std::cout << "This is a fixed expression:" << temp << std::endl;
        return NULL;
    }
}

bool init_list(struct list_regx_node *head)
{
    struct list_regx_node *ptr;
    ptr = head->next;
    while (ptr != head)
    {
        switch (ptr->type)
        {
        case LETTER_REPET:
            int i;
            const char *tmp;
            if (ptr->characters[LETTER_BEGIN_POS] > ptr->characters[LETTER_END_POS])
            {
                char tmp = ptr->characters[LETTER_END_POS];
                ptr->characters[LETTER_END_POS] = ptr->characters[LETTER_BEGIN_POS];
                ptr->characters[LETTER_BEGIN_POS] = tmp;
            }
            ptr->flag = false;
            // ptr->numbers[LETTER_SIZE_POS] = ptr->characters[LETTER_END_POS] -
            // ptr->characters[LETTER_BEGIN_POS] + 1;
            for (i = LETTER_HEAD_POS; i < ptr->numbers[LETTER_COUNTS_POS] + LETTER_HEAD_POS; i++)
            {
                ptr->characters[i] = ptr->characters[LETTER_BEGIN_POS];
            }
            tmp = &(ptr->characters[LETTER_HEAD_POS]);
            ptr->str.append(tmp, ptr->numbers[LETTER_COUNTS_POS]);
            std::cout << "LOOP"
                      << "times:" << ptr->str << std::endl;
            break;
        case LETTER_NO_REPET:
            break;
        case FIXED:
            ptr->flag = false;
            break;
        case NUMBERS:
            if (ptr->numbers[0] > ptr->numbers[1])
            {
                int tmp = ptr->numbers[0];
                ptr->numbers[0] = ptr->numbers[1];
                ptr->numbers[1] = tmp;
            }
            ptr->numbers[2] = ptr->numbers[0];
            ptr->str = int2string(ptr->numbers[2]);
            ptr->flag = false;
            break;
        }
        ptr = ptr->next;
    }
    if (head->next == head ? true : false)
    {
        std::cout << "No valid parameters!!!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "init_list() successfully!!!" << std::endl;
        return true;
    }
}

void string_slicer(std::vector<std::string> &vec, char *argv, const char *delimiter)
{
    char *ptr;
    ptr = strtok(argv, delimiter);
    while (ptr)
    {
        vec.push_back(ptr);
        ptr = strtok(NULL, delimiter);
    }
}

/*
*/
bool regex_handler(std::vector<std::string> vec, struct list_regx_node **head)
{
    struct list_regx_node *ptr = (*head);
    ptr->next = (*head);
    ptr->next = (*head);
    // std::cout<<head->regx_node_value<<std::endl;
    for (std::vector<std::string>::iterator it = vec.begin(); it < vec.end(); ++it)
    {
        std::string raw_exp;
        raw_exp.append(*it);
        int strlen = raw_exp.size();
        if (strlen < 3)
            continue;
        struct list_regx_node *current = new list_regx_node;
        if (raw_exp.at(0) == '[' && raw_exp.at(strlen - 1) == ']')
        {
            if (raw_exp.at(1) == '#' && strlen > 5)
            {
                unsigned int index = raw_exp.find('-', 2);
                if (index != -1 && index != strlen - 2 && index != 2)
                {
                    std::string str = raw_exp.substr(2, index - 2);
                    char const *tmp = (char const *)(str.c_str());
                    int begin = atoi(tmp);
                    str = raw_exp.substr(index + 1, strlen - index - 2);
                    tmp = (char const *)(str.c_str());
                    int end = atoi(tmp);
                    current->numbers[0] = begin;
                    current->numbers[1] = end;
                    current->type = NUMBERS;
                    current->next = (*head);
                    current->prev = ptr;
                    (*head)->prev = current;
                    ptr->next = current;
                    ptr = current;
                    current = NULL;
                    std::cout << "[#" << begin << "-" << end << "]"
                              << ">>>>>>Done!!!" << std::endl;
                }
                else
                {
                    std::cout << "Parameter ERROR!!!" << std::endl;
                    return false;
                }
            }
            else if ((raw_exp.at(1) == '$' ? true : raw_exp.at(1) == '!') && strlen == 8)
            {
                if (raw_exp.at(3) == '-' && raw_exp.at(5) == '%')
                {
                    char const *tmp = &raw_exp.at(6);
                    current->characters[LETTER_BEGIN_POS] = raw_exp.at(2);
                    current->characters[LETTER_END_POS] = raw_exp.at(4);
                    int x = atoi(tmp);
                    if (x > LIST_NODE_CHAR_LENGTH - 3 || x < 1)
                    {
                        std::cout << "Number is invalide!" << std::endl;
                        return false;
                    }
                    current->numbers[LETTER_COUNTS_POS] = x;
                    current->next = (*head);
                    current->prev = ptr;
                    if (raw_exp.at(1) == '$')
                    {
                        current->type = LETTER_REPET;
                    }
                    else
                    {
                        current->type = LETTER_NO_REPET;
                    }
                    ptr->next = current;
                    (*head)->prev = current;
                    ptr = current;
                    current = NULL;
                    std::cout << "[" << ptr->characters[LETTER_BEGIN_POS] << "-" << ptr->characters[LETTER_END_POS] << "%" << ptr->numbers[LETTER_COUNTS_POS] << "]"
                              << ">>>>>>Done!!!" << std::endl;
                }
                else
                {
                    std::cout << "Parameter ERROR!!!" << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Parameter ERROR!!!" << std::endl;
                return false;
            }
            // std::string new_exp = raw_exp.substr(1,raw_exp.size()-2);
            // char new_exp[];
        }
        else if (raw_exp.at(0) == '@' && raw_exp.at(strlen - 1) == '@')
        {
            current->str = raw_exp.substr(1, strlen - 2);
            // *(current->str) = new_exp;
            current->type = FIXED;
            current->next = (*head);
            current->prev = ptr;
            (*head)->prev = current;
            ptr->next = current;
            ptr = current;
            current = NULL;
            std::cout << raw_exp << "***" << ptr->str << "@Done!!!" << ptr->type << std::endl;
        }
        else
        {
            std::cout << "Unknown regex expression character:" << raw_exp.at(0) << std::endl;
            return false;
        }
    }
    std::cout << "regex_handler() is OK!!!" << std::endl;
    return true;
}
/*
    Just for some simple combination of the keywords user's inputing.
    @vec: the container packaging the kewwords.
    @ofs: the outputStream of the file.
*/
int keyWords_group_handler(std::vector<std::string> vec, std::ofstream &ofs)
{
    int count = 0;
    std::sort(vec.begin(), vec.end());
    do
    {
        for (std::vector<std::string>::iterator it = vec.begin(); it < vec.end(); ++it)
        {
            ofs << *it;
        }
        ofs << std::endl;
        count++;
    } while (std::next_permutation(vec.begin(), vec.end()));
    return count;
}
/*
    Some simple check for regex expression
*/
bool regex_exp_checker(std::vector<std::string> regx_vec)
{
    for (std::vector<std::string>::iterator it = regx_vec.begin(); it < regx_vec.end(); it++)
    {
        std::string tmp(*it);
        if (tmp.size() > REGEX_ITEM_MAX_LENGTH)
        {
            std::cout << "Error:" << tmp << "-->is too long!!!,it can not superb " << REGEX_ITEM_MAX_LENGTH << "!!!" << std::endl;
            return false;
        }
    }
    std::cout << "regex_exp_checker() is ok!!!" << std::endl;
    return true;
}

bool all_exp_checker(std::vector<std::string> vec, std::ofstream &ofs)
{
    if (!ofs.is_open())
    {
        std::cout << "No file for output.Please add an option named -p" << std::endl;
        return false;
    }
    if (vec.size() > REGEX_MAX_COUNTS)
    {
        std::cout << "The regex expression has too much item!!!" << std::endl;
        return false;
    }
    if (!regex_exp_checker(vec))
    {
        return false;
    }
    std::cout << "all_exp_checker() is OK!!!" << std::endl;
    return true;
}