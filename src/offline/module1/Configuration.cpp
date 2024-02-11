#include "Configuration.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

Configuration *Configuration::_pInstance = nullptr;

Configuration::Configuration(const string &filePath) : _filePath(filePath){
    cout << _filePath << endl;
    loadConfig();
}

Configuration::~Configuration(){

}

//加载配置文件，并将配置信息存入_configMap
void Configuration::loadConfig(){
    ifstream ifs(_filePath);
    if(!ifs.good()){
        cout << "fail to open file" << endl;
        return;
    }

    string line;
    string first, second;
    while(getline(ifs, line)){
        istringstream iss(line);
        iss >> first >> second;
        _configMap[first] = second;
    }

    cout << "[_configMap is loading...]" << endl;
    for(auto &item : _configMap){
        cout << item.first << " " << item.second << endl;
    }

    cout << "[_configMap finish loading]" << endl;
    ifs.close();
}

Configuration *Configuration::getInstance(){
    if(_pInstance == nullptr){
        _pInstance = new Configuration("conf/config.conf");
        //注册destroy函数，在程序结束时调用destroy自动释放
        atexit(destroy);
    }
    return _pInstance;
}

void Configuration::destroy(){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
}

unordered_map<string, string> &Configuration::getConfigMap(){
    return _configMap;
}

