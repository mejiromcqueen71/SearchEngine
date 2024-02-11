#include "Configuration.h"
#include "SplitTool.h"
#include "DictProducer.h"

#include <iostream>
#include <string>
#include <vector>

int main(){
    string cnPath = Configuration::getInstance()->getConfigMap()["cnDir"];
    string enPath = Configuration::getInstance()->getConfigMap()["enDir"];

    SplitTool tool;
    DictProducer enDict(enPath);
    DictProducer cnDict(cnPath, &tool);
    return 0;
}

