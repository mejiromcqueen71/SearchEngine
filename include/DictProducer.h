#ifndef __DictProducer_H__
#define __DictProducer_H__

#include <string>
#include <vector>
#include <set>
#include <map>

using std::string;
using std::vector;
using std::set;
using std::map;

class SplitTool;

/**************************************
 *
 * 词典生成类
 *
 **************************************/

class DictProducer{
public:
    DictProducer(const string &);//英文词典库使用这个构造函数
    DictProducer(const string &, SplitTool *);//中文词典库使用这个构造函数
    ~DictProducer();

    //建立英文字典
    void buildEnDict();
    //建立中文字典
    void buildCnDict();
    //建立索引表（单词及所对应的行号）
    void buildIndex();
    
    //存储字典和索引表
    void storeDict(const char *);
    void storeIndex(const char *);

private:
    //获取文件绝对路径
    void getFiles(const string &);

    //获取汉字字节数
    size_t getByteNum_UTF8(const char);
    
    //打印字典和索引表，用于调试
    void showFiles() const;
    void showDict() const;

    //加载截止文件
    void loadStopWord(const string &);

private:
    vector<string> _files;//用于读取的文件，因为读取的文件可能不止一个
    map<string, int> _dict;//单词及其词频
    SplitTool *_splitTool;//分词器，用于获取中文词
    map<string, set<int>> _index;//索引表，单词及其出现的行号
    set<string> _stopWord;//截止单词
    string _dir;//待读取的文件目录
};

#endif

