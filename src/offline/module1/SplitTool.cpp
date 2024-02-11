#include "SplitTool.h"

SplitTool::SplitTool() : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORDS_PATH){

}

SplitTool::~SplitTool(){

}

vector<string> SplitTool::cut(const string &sentence){
    vector<string> result;
    _jieba.CutForSearch(sentence, result);
    return result;
}
