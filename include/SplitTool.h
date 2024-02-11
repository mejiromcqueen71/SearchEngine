#ifndef __SplitTool_H__
#define __SplitTool_H__

#include "simhash-cppjieba/Jieba.hpp"
using namespace cppjieba;

#include <vector>
#include <string>

using std::string;
using std::vector;

/*************************************
 *
 * 分词工具类
 *
 *************************************/

class SplitTool{
    const string DICT_PATH = "lib/simhash-cppjieba/dict/jieba.dict.utf8";
    const string HMM_PATH = "lib/simhash-cppjieba/dict/hmm_model.utf8";
    const string USER_DICT_PATH = "lib/simhash-cppjieba/dict/user.dict.utf8";
    const string IDF_PATH = "lib/simhash-cppjieba/dict/idf.utf8";
    const string STOP_WORDS_PATH = "lib/simhash-cppjieba/dict/stop_words.utf8";

public:
    SplitTool();
    ~SplitTool();

    //分词
    vector<string> cut(const string &);

private:
    Jieba _jieba;
};

#endif

