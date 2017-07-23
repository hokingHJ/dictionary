#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


const char PATH = 'p';
const char FIXED = 'f';
const char REGEX = 'x';
const char KEY ='k';

struct list_head{
     struct list_head *next,*prev;
}
struct hlist_node{
    struct hlist_node *next , **pprev ;
}

struct hlist_head{
    struct hlist_node *first;
}
int main(int argc,char * argv[]){ 

    int count=0;
    int i;
    int keyWordNum;
    const char * delim = ",";
    
    std::string params[10];
    std::string keyWordTmp;
    std::vector<std::string> vec;
    std::ofstream ofs;

    if(argc>10){
        std::cout<<"The number of parameter is too much"<<std::endl;
        std::cout<<"Note: You can use the -x option for regex expression"<<std::endl;
        return 0;
    } 
    

    for(i=1;i<argc;i=i+2){
        std::string param;
        const char * p = argv[i];
        param.append(p);
        if(param.at(0)=='-'){
            switch(param.at(1)){
                case PATH:
                    ofs.open(argv[i+1],std::ios::app);
                break;
                case KEY:
                    char * ptr;
                    ptr = strtok(argv[i+1],delim);
                    while(ptr){
                        vec.push_back(ptr);
                        ptr = strtok(NULL,delim);
                    };
                break;
                case FIXED:
                break;
                default:
                std::cout<<"No such option for -"<<param.at(1)<<std::endl;
                return 0;
            }
        }else{
            std::cout<<"Parameters error!!!"<<std::endl;
            return 0;
        }
    }
    
    if(!ofs.is_open()){
        std::cout<<"No file for output.Please add an option named -p"<<std::endl;
        return 0;
    }
    std::sort(vec.begin(),vec.end());
    do{
        for(std::vector<std::string>::iterator it = vec.begin();it<vec.end();++it){
            ofs<<*it;
        }
        ofs<<std::endl;
        count++;
    }while(std::next_permutation(vec.begin(),vec.end()));
    ofs.close();
    std::cout<<"Total number is:"<<count<<std::endl;
    return 0;
}