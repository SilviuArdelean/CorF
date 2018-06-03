#pragma once

#include "general.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "utils.h"

template<typename T, typename It>
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

   bool LoadFile()
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
            return getEpochTime(lhs.getLastUpdate()) > getEpochTime(rhs.getLastUpdate());
         });

      m_dataList.clear();

      std::move( std::begin(vectData), std::end(vectData), std::back_inserter(m_dataList));
      vectData.clear();

      InitLookupTable();

      return true;
   }

   void InitLookupTable()
   {
      auto it = m_dataList.begin();
      m_lookupTable.reserve(m_dataList.size());

      while (it != m_dataList.end())
      {
         T *item = &*it;
         m_lookupTable[it->getPersonalID()] = it;
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

   std::list<T>& GetConsistentDataList()  { return  m_dataList; }
   void Add(const T& pers) { m_dataList.push_front(pers); }
   
   T* Find(const std::string& pers_id)
   { 
      auto item_iter = m_lookupTable.find(pers_id);

      return (item_iter != m_lookupTable.end()) ? &*item_iter->second : nullptr;
   }

   bool Delete(std::string const& pers_id)
   {
      auto item_iter = m_lookupTable.find(pers_id);

      if (item_iter != m_lookupTable.end())
      {
         m_dataList.erase(item_iter);
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

      for (auto& item : m_dataList)
      {
         writer.StartObject();
         item.Serialize(writer);
         writer.EndObject();
      }

      writer.EndArray();
   }

private:
   std::string          m_jsonFilePath;
   std::string          m_strFileSaveAs;

   rapidjson::Document  m_docJson;
   std::list<T>        m_dataList;

   std::unordered_map< std::string, It >  m_lookupTable;
};