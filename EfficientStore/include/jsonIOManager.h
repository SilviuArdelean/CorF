#pragma once

#include "general.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "string_utils.h"

template<typename T>
class jsonIOManager
{
public:
   jsonIOManager() = delete;

   jsonIOManager(const std::string& jsonFilePath)
      : m_jsonFilePath(jsonFilePath)
      , m_strFileSaveAs("")
   {
   }

   ~jsonIOManager()
   {
      // Force saving data to file - RTTI based stuff
      Save2File(m_strFileSaveAs.empty() ? m_jsonFilePath : m_strFileSaveAs);
   }

   void SetSaveAsFileName(const std::string& strFile) { m_strFileSaveAs = strFile; }

   bool Initialize()
   {
      std::ifstream ifs(m_jsonFilePath);

      if (!ifs.is_open())
      {
         std::cout << "Error opening file: " << m_jsonFilePath << std::endl;
         return false;
      }

      rapidjson::IStreamWrapper isw(ifs);
      
      m_docJson.ParseStream<0>(isw);

      std::vector<T>  vectData;
      vectData.reserve(m_docJson.Size());

      for (rapidjson::SizeType i = 0; i < m_docJson.Size(); i++)
      {
         vectData.push_back(T(m_docJson[i]["person_id"].GetString(),
                                 m_docJson[i]["name"].GetString(),
                                 m_docJson[i]["surname"].GetString(),
                                 m_docJson[i]["email"].GetString(),
                                 m_docJson[i]["last_update"].GetString()));
      }

      std::sort(vectData.begin(), vectData.end(),
         [](const T& lhs, const T& rhs)
         {
            return string_utils::getEpochTime(lhs.getLastUpdate()) > string_utils::getEpochTime(rhs.getLastUpdate());
         });

      InitLookupTable(vectData);

      return true;
   }

   void InitLookupTable(std::vector<T>&  vectData)
   {
      auto it = vectData.begin();
      m_lookupTable.reserve(vectData.size());

      while (it != vectData.end())
      {
         m_lookupTable[it->getPersonalID()] = *it;
         ++it;
      }
   }

   void Save2File(const std::string& newFilePath)
   {
      rapidjson::StringBuffer sb;
      Serialize(sb);

#ifdef _DEBUG
      puts(sb.GetString());
#endif

      FILE* file;
      file = fopen(newFilePath.c_str(), "w");
      fprintf(file, "%s", sb.GetString());
      fclose(file);
   }

   std::unordered_map< std::string, T > & GetDataLookupTable()  { return  m_lookupTable; }
   
   bool Add(const T& pers) 
   { 
      auto it = m_lookupTable.find(pers.getPersonalID());
      if (it != m_lookupTable.end())
      {
         std::cout << "Person ID " << pers.getPersonalID() << " already exists" << std::endl;
         return false;
      }

      m_lookupTable[pers.getPersonalID()] = pers;

      return true;
   }
   
   T* Find(const std::string& pers_id)
   { 
      auto item_iter = m_lookupTable.find(pers_id);

      return (item_iter != m_lookupTable.end())
               ? dynamic_cast<T*>(&(item_iter->second))
               : nullptr;
   }

   bool Delete(const std::string& pers_id)
   {
      auto item_iter = m_lookupTable.find(pers_id);

      if (item_iter != m_lookupTable.end())
      {
         m_lookupTable.erase(item_iter);
         return true;
      }

      return false;
   }


protected:

   void Serialize(rapidjson::StringBuffer& sb)
   {      
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

      writer.StartArray();

      for (auto& item : m_lookupTable)
      {
         writer.StartObject();
         item.second.Serialize(writer);
         writer.EndObject();
      }

      writer.EndArray();
   }

private:
   std::string          m_jsonFilePath;
   std::string          m_strFileSaveAs;

   rapidjson::Document  m_docJson;

   std::unordered_map< std::string, T >  m_lookupTable;
};