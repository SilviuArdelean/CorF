#pragma once
#include "string_utils.h"
#include "jsonIOManager.h"
#include "fixed_queue.h"
#include <memory>

template <typename T, typename It>
class CacheManager
{

   struct LessThanByFileSize
   {
      bool operator()(const T& lhs, const T& rhs) const
      {
         return string_utils::getEpochTime(lhs.getLastUpdate()) < string_utils::getEpochTime(rhs.getLastUpdate());
      }
   };

public:

   void SetSaveAsFileName(const std::string& strFile) { assert(m_jsonManager); m_jsonManager->SetSaveAsFileName(strFile); }

   CacheManager(const std::string& consistDataFilePath, size_t cacheSize = 10)
      : m_cacheSize(cacheSize)
   {
      m_cacheQueue = new fixed_queue<T, std::deque<T>, LessThanByFileSize>(m_cacheSize);

      m_jsonManager = new jsonIOManager<T, It>(consistDataFilePath);
   }
   ~CacheManager()
   {
      delete m_jsonManager;
      delete m_cacheQueue;
   }

   bool Initialize()
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      if (!m_jsonManager->LoadFile())
      {
         std::cout << "Cache initialization has fail - invalid source" << std::endl;
         return false;
      }

      // fill the cache queue
      for (auto &ob : m_jsonManager->GetConsistentDataList())
      {
         m_cacheQueue->push(T(ob.getPersonalID(), ob.getName(),
                                    ob.getSurname(), ob.getEmail(), ob.getLastUpdate()));
      }

      return true;
   }

   void Save(const std::string& newFilePath)
   {
      assert(m_jsonManager);

      m_jsonManager->Save2File(newFilePath);
   }

   void AddItem(const T& new_pers)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      if (m_cacheQueue->is_full())
            m_cacheQueue->pop();

      m_cacheQueue->push(new_pers);
      m_jsonManager->Add(new_pers);

      // force write on disk m_jsonManager - reinitialize cache_queue
   }

   void DeleteItem(const std::string& person_id)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      m_cacheQueue->erase(person_id);
      m_jsonManager->Delete(person_id);
   }

   T* SearchItem(const std::string& person_id)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      auto ptr2pers = m_cacheQueue->find(person_id);
      return (nullptr != ptr2pers)
               ? ptr2pers
               : m_jsonManager->Find(person_id);
   }
   
private:
   size_t   m_cacheSize;

   fixed_queue<T, std::deque<T>, LessThanByFileSize>* m_cacheQueue;

   jsonIOManager<T, It>* m_jsonManager;
};
