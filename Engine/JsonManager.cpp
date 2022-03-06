#include "pch.h"
#include "JsonManager.h"
#include <fstream>

//void JsonManager::Save(const std::string& path, const std::string& name)
//{
//	using namespace std;
//    string str;
//    Json::Value root;
//    root["name"] = "KKK";
//    root["age"] = 12;
//    root["address"] = "kor";
//    root["gfriend"] = true;
//
//    Json::Value family;
//    family.append("mother");
//    family.append("father");
//    family.append("brother");
//    root["family"] = family;
//
//    Json::StyledWriter writer;
//    str = writer.write(root);
//
//    std::ofstream ost(path + name + ".json");
//    ost << str;
//}
//
//void JsonManager::Load(const std::string& path, const std::string& name)
//{
//    using namespace std;
//    ifstream ist(path + name + ".json");
//    string str;
//    for (char p; ist >> p;)
//        str += p;
//
//    Json::Reader reader;
//    Json::Value root;
//    bool parsingRet = reader.parse(str, root);
//
//    //cout << root["name"] << endl;
//    //cout << root["age"] << endl;
//    //cout << root["address"] << endl;
//    //cout << root["gfriend"] << endl;
//
//    Json::Value family = root["family"];
//    //cout << family[0].asString() << endl;
//    //cout << family[1].asString() << endl;
//    //cout << family[2].asString() << endl;
//
//}
