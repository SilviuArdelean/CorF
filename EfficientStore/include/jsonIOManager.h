#pragma once

#include "general.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "string_utils.h"
#include <mutex>

std::mutex           g_i_mutexJ;

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
      std::lock_guard<std::mutex> lock(g_i_mutexJ);

      std::ifstream ifs(m_jsonFilePath);

      if (!ifs.is_open())
      {
         std::cout << "Error opening file: " << m_jsonFilePath << std::endl;
         return false;
      }

      rapidjson::IStreamWrapper isw(ifs);
      
      m_docJson.ParseStream<0>(isw);

      m_lookupTable.reserve(m_docJson.Size());

      for (rapidjson::SizeType i = 0; i < m_docJson.Size(); i++)
      {
         auto pers_id = m_docJson[i]["person_id"].GetString();

         m_lookupTable.insert(
               std::pair<std::string, T>(pers_id, 
                      T(pers_id,
                           m_docJson[i]["name"].GetString(),
                           m_docJson[i]["surname"].GetString(),
                           m_docJson[i]["email"].GetString(),
                           m_docJson[i]["last_update"].GetString())));
      }
      
      return true;
   }

   void Save2File(const std::string& newFilePath)
   {
      std::lock_guard<std::mutex> lock(g_i_mutexJ);

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
      std::lock_guard<std::mutex> lock(g_i_mutexJ);

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
      std::lock_guard<std::mutex> lock(g_i_mutexJ);

      auto item_iter = m_lookupTable.find(pers_id);

      return (item_iter != m_lookupTable.end())
               ? dynamic_cast<T*>(&(item_iter->second))
               : nullptr;
   }

   bool Delete(const std::string& pers_id)
   {
      std::lock_guard<std::mutex> lock(g_i_mutexJ);

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