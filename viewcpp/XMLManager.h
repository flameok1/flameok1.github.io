#pragma once

#include <iostream>
#include <list>
#include <vector>


struct AttributePair
{
    std::string sName;
    std::string sValue;
};

struct XMLNode
{
    std::string sName;
    std::string sData;
    
    std::vector<std::shared_ptr<AttributePair>> vAttrs;

    std::shared_ptr<XMLNode> pParent;
    std::vector <std::shared_ptr<XMLNode>> vChilds;
};

class XMLManager
{
public:
    XMLManager();
    ~XMLManager();
private:
    bool _bBeginNode; //開始node
    bool _bBeginScope; //開始<...>
    bool _bCheckPrev; //保留不做事，等待下字元先檢查flag
    bool _bUpdateName; //更新標籤
    bool _bCheckName; //結束時 檢查對應標籤
    bool _bLoadString;

    std::vector<std::shared_ptr<XMLNode>> _vAllNodes; //所有node

    std::shared_ptr<XMLNode> _pNewNode; //當前更新Node

    std::string _sNowLoad; //當前保存字串

    std::string _sCacheName;
    std::string _sCacheValue;

    void UpdateCacheValue();

    std::shared_ptr<XMLNode> GetNodeByName(std::shared_ptr<XMLNode> pXMLNode, std::string NodeName);

    void ReleaseXMLNode(std::shared_ptr<XMLNode> pXMLNode);
public:
    void Reset();

    void LoadXml(FILE* fp);

    static void PrintXMLNode(std::shared_ptr<XMLNode> pXMLNode, int tab = 0);

    void PrintAllXMLNode();

    std::shared_ptr<XMLNode> Search(std::string NodeName);
};