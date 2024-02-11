#include "DictProducer.h"
#include "SplitTool.h"
#include "Configuration.h"

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

//创建英文字典的构造函数
DictProducer::DictProducer(const string &enDir) : _dir(enDir){
    string enDictPath = Configuration::getInstance()->getConfigMap()["enDict"];
    string enDictIndexPath = Configuration::getInstance()->getConfigMap()["enDictIndex"];
    string enStopDictPath = Configuration::getInstance()->getConfigMap()["enStop"];

    loadStopWord(enStopDictPath);

    cout << "\nbuilding English dictionary ..." << endl;
    buildEnDict();
    storeDict(enDictPath.c_str());
    cout << "English dictionary building completed!" << endl;
    cout << "\nbuilding English index table ..." << endl;
    buildIndex();
    storeIndex(enDictIndexPath.c_str());
    cout << "English index table building completed!" << endl;

    cout << "English dictionary and index table built OK!" << endl;
}

//创建中文字典的构造函数，需要用到分词器
DictProducer::DictProducer(const string &cnDir, SplitTool *splitTool) : _splitTool(splitTool), _dir(cnDir){
    string cnDictPath = Configuration::getInstance()->getConfigMap()["cnDict"];
    string cnDictIndexPath = Configuration::getInstance()->getConfigMap()["cnDictIndex"];
    string cnStopDictPath = Configuration::getInstance()->getConfigMap()["cnStop"];

    loadStopWord(cnStopDictPath);

    cout << "\nbuilding Chinese dictionary ..." << endl;
    buildCnDict();
    storeDict(cnDictPath.c_str());
    cout << "Chinese dictionary building completed!" << endl;
    cout << "\nbuilding Chinsese index table ..." << endl;
    buildIndex();
    storeIndex(cnDictIndexPath.c_str());
    
    cout << "Chinese index table building completed!" << endl;
    cout << "Chinese dictionary and index table built OK!" << endl;
}

DictProducer::~DictProducer(){
     
}

void DictProducer::loadStopWord(const string &stopDictPath){
    _stopWord.clear();
    ifstream ifs(stopDictPath);
    if(!ifs.good()){
        cout << "fail to open stop word!" << endl;
        return;
    }

    string line, word;
    while(getline(ifs, line)){
        istringstream iss(line);
        while(iss >> word){
            _stopWord.insert(word);
        }
    }

//    for(auto &elem : _stopWord){
//        cout << elem << endl;
//    }
    ifs.close();
}

void DictProducer::buildEnDict(){
    string line, word, in_word;

    _files.clear();
    getFiles(_dir);
    showFiles();

    for(auto &file : _files){
        ifstream ifs(file);
        if(!ifs.good()){
            cout << "fail to open file" << endl;
            return;
        }

        while(getline(ifs, line)){
            in_word.clear();
            istringstream iss(line);

            while(iss >> word){
                in_word.clear();
                for(char c : word){
                    //去大写
                    if(c >= 'A' && c <= 'Z'){
                        c += 32;
                    }
                    //去符号
                    if(isalpha(c)){
                        in_word += c;
                    }else{
                        break;
                    }
                }

                if(in_word[in_word.size() - 1] == '-'){
                    in_word.resize(in_word.size() - 1);
                }
                if(in_word != " " && find(_stopWord.begin(), _stopWord.end(), in_word) == _stopWord.end()){
                    ++_dict[in_word];
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::buildCnDict(){
    getFiles(_dir);
    for(auto &file : _files){
        ifstream ifs(file, std::ios::ate);
        if(!ifs.good()){
            cout << "fail to open art files" << endl;
            return;
        }

        size_t len = ifs.tellg();//整篇文章长度
        ifs.seekg(std::ios_base::beg);
        char *buffer = new char[len + 1];
        ifs.read(buffer, len + 1);
        string txt(buffer);
        delete []buffer;

        vector<string> tmp = _splitTool->cut(txt);

        for(auto &elem : tmp){
            auto exist = _dict.find(elem);
            if(exist != _dict.end()){
                ++_dict[elem];
            }else{
                _dict.insert({elem, 1});
            }
        }
        ifs.close();
    }
}

void DictProducer::buildIndex(){
    int i = 0;
    for(auto &elem : _dict){
        string word = elem.first;
        size_t charNums = word.size() / getByteNum_UTF8(word[0]);//获取字符长度
        for(size_t idx = 0, n = 0; n != charNums; ++idx, ++n){
            size_t charLen = getByteNum_UTF8(word[idx]);
            string subword = word.substr(idx, charLen);
            _index[subword].insert(i);
            idx += (charLen - 1);
        }
        ++i;
    }
}

void DictProducer::storeDict(const char *filePath){
    ofstream ofs(filePath);
    if(!ofs.good()){
        cout << "fail to open file" << endl;
        return;
    }
    for(auto &it : _dict){
        ofs << it.first << " " << it.second << endl;
    }
    ofs.close();
}

void DictProducer::storeIndex(const char *filePath){
    ofstream ofs(filePath);
    if(!ofs.good()){
        cout << "fail to open file" << endl;
        return;
    }
    for(auto &it : _index){
        ofs << it.first << " ";
        for(auto &set_it : it.second){
            ofs << set_it << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

void DictProducer::showFiles() const{
    for(auto &elem : _files){
        cout << elem << endl;
    }
}

void DictProducer::showDict() const{
    for(auto &it : _index){
        cout << it.first << " ";
        for(auto &set_it : it.second){
            cout << set_it << " ";
        }
        cout << endl;
    }
}

void DictProducer::getFiles(const string &dir){//获取文件绝对路径
    DIR *dirp = opendir(dir.c_str());
    if(dirp == nullptr){
        cout << "error in opendir" << endl;
        return;
    }

    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != nullptr){
        if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0){
            continue;
        }

        string filePath;
        filePath = dir + "/" + pdirent->d_name;
        _files.push_back(filePath);
    }
    closedir(dirp);
}

size_t DictProducer::getByteNum_UTF8(const char byte){
    int byteNum = 0;
    for(size_t i = 0; i < 6; ++i){
        if(byte & (1 << (7 - i))){
            ++byteNum;
        }else{
            break;
        }
    }
    return byteNum == 0 ? 1 : byteNum;
}

