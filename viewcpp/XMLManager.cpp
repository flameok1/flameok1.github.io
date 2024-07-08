#include "XMLManager.h"


XMLManager::XMLManager()
{
    Reset();
}

XMLManager::~XMLManager()
{
    Reset();
}

void XMLManager::Reset()
{
    for (int i = 0; i < _vAllNodes.size(); ++i)
    {
        ReleaseXMLNode(_vAllNodes[i]);
    }

    _vAllNodes.clear();

    _bBeginNode = false;
    _bBeginScope = false;
    _bCheckPrev = false;
    _bUpdateName = false;
    _bCheckName = false;
    _bLoadString = false;

    _pNewNode = nullptr;

    _sCacheName.clear();
    _sCacheValue.clear();
    _sNowLoad.clear();
}

void XMLManager::ReleaseXMLNode(std::shared_ptr<XMLNode> pXMLNode)
{
    pXMLNode->vAttrs.clear();

    for (int i = 0; i < pXMLNode->vChilds.size(); ++i)
    {
        ReleaseXMLNode(pXMLNode->vChilds[i]);
    }

    pXMLNode->vChilds.clear();
}

void XMLManager::LoadXml(FILE* fp)
{

    int c = 0;//當前字元

    int prevc = 0; //上一個字元


    while (true)
    {
        c = fgetc(fp);
        if (feof(fp))
        {
            break;
        }

        if (_bCheckPrev)
        {
            _bCheckPrev = false;

            if (prevc == '?')
            {
                //不是?> 補回字元
                if (c != '>')
                {
                    //沒特殊事件 補回?
                    _sNowLoad += prevc;
                }
            }
            else if (prevc == '<')
            {
                if (c != '/') //不是</
                {
                    //開始讀取一個<...>
                    _bBeginNode = true;
                    _bBeginScope = true;
                    _sNowLoad.clear();
                    
                    std::shared_ptr<XMLNode> tmpNew = std::make_shared<XMLNode>();
                    if (_pNewNode != nullptr) //產生子物件 將自己存到stack
                    {
                        _pNewNode->vChilds.push_back(tmpNew);
                        tmpNew->pParent = _pNewNode;
                    }
                    
                    _pNewNode = tmpNew;
                    _bUpdateName = true;
                }
                else //是</ 結束一個<...>
                {
                    _bBeginNode = false;
                    _bBeginScope = true;
                    _sNowLoad.clear();
                    _bUpdateName = false;
                    _bCheckName = true;
                }
            }
            else if (prevc == '/')
            {
                if (c == '>') //可能是單標籤結束 />
                {
                    _bBeginNode = false;
                }
                else //屬性中有該字元吧
                {
                    //沒特殊事件 補回/
                    _sNowLoad += prevc;
                }
            }
        }

        if (c == '<' && !_bLoadString)
        {
            //先保留檢查下一個 => 有可能是 </ 結束用符號 中間不能空格
            _bCheckPrev = true;

            if (_sNowLoad.size() > 0)
            {
                //有字串資料 應該是data
                if (_pNewNode != nullptr)
                {
                    _pNewNode->sData = _sNowLoad;
                }
                else
                    printf("Error pNewNode = nullptr\n");
                _sNowLoad.clear();
            }
        }
        else if (c == '>' && !_bLoadString)
        {
            UpdateCacheValue();

            if (_bBeginNode)
            {
                //stackNodes.push_back(pNewNode);
            }
            else
            {
                //可能是單標籤結束
                if (_pNewNode->pParent == nullptr) //不是子物件
                {
                    _vAllNodes.push_back(_pNewNode);
                    _pNewNode = nullptr;
                }
                else
                {
                    _pNewNode = _pNewNode->pParent;
                }
            }
            _bBeginNode = false;
            _bBeginScope = false;
        }
        else if (c == '/' && _bBeginScope && !_bLoadString)
        {
            if (prevc == '<') //<?
            {
                //do notiong上面已經處理
            }
            else
            {
                _bCheckPrev = true;
            }
        }
        else if (c == '=' && _bBeginScope && !_bLoadString)
        {
            if (_sNowLoad.size() > 0)
            {
                _sCacheName = _sNowLoad;
            }
            _sNowLoad.clear();
        }
        else if (c == '?' && _bBeginScope && !_bLoadString)
        {
            if (prevc == '<') //<?
            {
                _bBeginNode = false;
                _sNowLoad.clear();
            }
            else
            {
                //不是<? do nothing
                _bCheckPrev = true;
            }
        }
        else if (c == ' ' && _bBeginScope && !_bLoadString)
        {
            UpdateCacheValue();
        }
        else if (c == '\n')
        {
            //換行 do nothing
        }
        else if (c == '\"')
        {
            _bLoadString = !_bLoadString;
        }
        else
        {
            _sNowLoad += c;
        }

        prevc = c;
    }
}

void XMLManager::UpdateCacheValue()
{
    if (_sNowLoad.size() > 0)
    {
        if (_bCheckName)
        {
            if (_pNewNode != nullptr)
            {
                if (_pNewNode->sName != _sNowLoad)
                {
                    printf("Name is not match\n");
                }
            }

            _bCheckName = false;
        }
        else if (_bUpdateName)
        {
            if (_pNewNode != nullptr)
                _pNewNode->sName = _sNowLoad;

            _bUpdateName = false;
        }
        else
        {
            _sCacheValue = _sNowLoad;

            if (_pNewNode != nullptr)
            {
                std::shared_ptr<AttributePair> newAttributePair = std::make_shared<AttributePair>();
                newAttributePair->sName = _sCacheName;
                newAttributePair->sValue = _sCacheValue;

                _pNewNode->vAttrs.push_back(newAttributePair);
            }
        }
    }
    _sCacheName.clear();
    _sCacheValue.clear();
    _sNowLoad.clear();
}

void PrintfTab(int tab)
{
    for (int i = 0; i < tab; ++i) printf("\t");
}

void XMLManager::PrintXMLNode(std::shared_ptr<XMLNode> pXMLNode, int tab)
{
    PrintfTab(tab);
    printf("Node Name %s\n", pXMLNode->sName.c_str());

    PrintfTab(tab);
    printf("Node Data %s\n", pXMLNode->sData.c_str());

    for (int i = 0; i < pXMLNode->vAttrs.size(); ++i)
    {
        PrintfTab(tab);
        printf("Attrs %s %s\n", pXMLNode->vAttrs[i]->sName.c_str(), pXMLNode->vAttrs[i]->sValue.c_str());
    }

    for (int i = 0; i < pXMLNode->vChilds.size(); ++i)
    {
        PrintXMLNode(pXMLNode->vChilds[i], tab+1);

        printf("\n");
    }
}

void XMLManager::PrintAllXMLNode()
{
    for (int i = 0; i < _vAllNodes.size(); ++i)
    {
        PrintXMLNode(_vAllNodes[i]);

        printf("\n --------------------------------------------------- \n");
    }
}

std::shared_ptr<XMLNode> XMLManager::GetNodeByName(std::shared_ptr<XMLNode> pXMLNode, std::string NodeName)
{
    if (pXMLNode->sName == NodeName)
        return pXMLNode;

    std::shared_ptr<XMLNode> tmpHR = nullptr;
    for (int i = 0; i < pXMLNode->vChilds.size(); ++i)
    {
        tmpHR = GetNodeByName(pXMLNode->vChilds[i], NodeName);

        if(tmpHR != nullptr)
            return tmpHR;
    }

    return nullptr;
}

std::shared_ptr<XMLNode> XMLManager::Search(std::string NodeName)
{
    std::shared_ptr<XMLNode> tmpHR = nullptr;
    for (int i = 0; i < _vAllNodes.size(); ++i)
    {
        tmpHR = GetNodeByName(_vAllNodes[i] , NodeName);

        if (tmpHR != nullptr)
            return tmpHR;
    }

    return nullptr;
}