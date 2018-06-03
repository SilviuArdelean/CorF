#pragma once

#include "general.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "utils.h"

//#ifdef _WIN32
//#include "Shlwapi.h"
//#include <windows.h>
//#endif

template<class T>
class jsonIOManager
{
public:
   jsonIOManager() = delete;

   jsonIOManager(const std::string& jsonFilePath)
      : m_jsonFilePath(jsonFilePath)
      , m_strFileSaveAs("")
   {
//#ifdef _WIN32
//      if (1 != PathFileExistsA(jsonFilePath.c_str()))
//      {
//         std::cout << "Incorrect file path: " << jsonFilePath << std::endl;
//      }
//#endif
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

      m_dataQueue.clear();

      std::move( std::begin(vectData), std::end(vectData), std::back_inserter(m_dataQueue));
      vectData.clear();

      return true;
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

   std::deque<T>& GetConsistentDataQueue()  { return  m_dataQueue; }
   void Add(const T& pers) { m_dataQueue.push_front(pers); }
   
   T* Find(const std::string& pers_id)
   { 
      auto it = m_dataQueue.begin();
      while (it != m_dataQueue.end())
      {
         T *item = &*it;
         if (string_utils::compare_case_sensitive(item->getPersonalID(), pers_id))
         {
            return item;
         }

         ++it;
      }

      return nullptr;
   }

   bool Delete(std::string const& pers_id)
   {
      auto it = m_dataQueue.begin();
      while (it != m_dataQueue.end())
      {
         T *item = &*it;
         if (string_utils::compare_case_sensitive(item->getPersonalID(), pers_id))
         {
            m_dataQueue.erase(it);
            return true;
         }

         ++it;
      }

      false;
   }


protected:

   void Serialize(rapidjson::StringBuffer& sb)
   {      
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

      writer.StartArray();

      for (auto& person : m_dataQueue)
      {
         writer.StartObject();
         person.Serialize(writer);
         writer.EndObject();
      }

      writer.EndArray();
   }

private:
   std::string          m_jsonFilePath;
   std::string          m_strFileSaveAs;

   rapidjson::Document  m_docJson;
   std::deque<T>        m_dataQueue;
};

