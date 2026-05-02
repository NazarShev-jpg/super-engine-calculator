#ifndef UI_HELPDATA_HPP
#define UI_HELPDATA_HPP

#include <string>
#include <map>

struct HelpEntry {
    std::string descEn;
    std::string descRu;
    std::string examplesEn;
    std::string examplesRu;
};

// Возвращает ссылку на статический словарь, инициализируется при первом вызове
const std::map<std::string, HelpEntry>& getDetailedHelp();

#endif