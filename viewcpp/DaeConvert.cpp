// DaeConvert.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "Windows.h"
#include "list"
#include <vector>
#include "XMLManager.h"
#include "LibraryAll.h"

char gszInputFile[_MAX_PATH];            // File name to import

XMLManager *gpXMLManager;

void GetInputFileName();

void runDaeConvert()
{
    gpXMLManager = new XMLManager();
    
    GetInputFileName();


    //system("PAUSE");

    FILE* fp;

    fopen_s(&fp, gszInputFile, "r");

    if (fp == nullptr || feof(fp))
        return;

    
    gpXMLManager->LoadXml(fp);

    fclose(fp);
    
    //gpXMLManager->PrintAllXMLNode();

    //system("PAUSE");
    
    std::shared_ptr<XMLNode> pXMLNode = gpXMLManager->Search("library_nodes");
    if (pXMLNode != nullptr)
    {
        //gpXMLManager->PrintXMLNode(pXMLNode);
    }

    
    LibraryNodes libNodes;

    libNodes.InitByXmlNode(pXMLNode);

    
    libNodes.PrintfOut();

    //gpXMLManager->PrintAllXMLNode();
    
    pXMLNode = gpXMLManager->Search("library_geometries");
    if (pXMLNode != nullptr)
    {
        //gpXMLManager->PrintXMLNode(pXMLNode);
    }
    
    LibraryGeometry libGeometry;

    libGeometry.InitByXmlNode(pXMLNode);

    libGeometry.PrintfOut();
    
    pXMLNode = gpXMLManager->Search("library_controllers");
    if (pXMLNode != nullptr)
    {
        //gpXMLManager->PrintXMLNode(pXMLNode);
    }
   
    LibraryControllers LibrControllers;

    LibrControllers.InitByXmlNode(pXMLNode);

    //LibrControllers.PrintfOut();
    
    pXMLNode = gpXMLManager->Search("library_visual_scenes");

    LibraryVisualScenes LibVisualScenes;

    LibVisualScenes.InitByXmlNode(pXMLNode);

    //LibVisualScenes.PrintfOut();
    
    delete gpXMLManager;
}

int main()
{
    runDaeConvert();

    return 0;
}


// show the <Open file> dialog
void GetInputFileName()
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    char szFile[_MAX_PATH];  // buffer for file name
    ZeroMemory(szFile, sizeof(szFile));

    // Initialize OPENFILENAME
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.lpstrTitle = "Select the file to import from ... (use the file type filter)";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // get a description of all readers registered in the FBX SDK
    ofn.lpstrFilter = "xml\0*.xml\0dae\0*.dae\0";

    // Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == false)
    {
        return;
    }

    // Keep a copy of the file name
    strcpy_s(gszInputFile, _MAX_PATH, szFile);
}