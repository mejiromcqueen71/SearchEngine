#ifndef __Configuration_H__
#define __Configuration_H__

#include <unordered_map>
using std::string;
using std::unordered_map;

/*********************************
 *
 * 配置文件类，单例模式
 *
 * *******************************/

class Configuration{
public:
    static Configuration *getInstance();
    unordered_map<string, string> &getConfigMap();

private:
    Configuration(const string &);
    ~Configuration();

    //加载配置文件到_configMap
    void loadConfig();

    //销毁实例
    static void destroy();

private:
    string _filePath;
    unordered_map<string, string> _configMap;
    static Configuration *_pInstance;
};

#endif

