#pragma once
#include "string_utils.h"
#include "jsonIOManager.h"
#include "fixed_queue.h"
#include <memory>

template <typename T>
class CacheManager
{
   struct LessThanLastDate
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
      m_cacheQueue = new fixed_queue<T, std::vector<T>, LessThanLastDate>(m_cacheSize);

      m_jsonManager = new jsonIOManager<T>(consistDataFilePath);
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

      if (!m_jsonManager->Initialize())
      {
         std::cout << "Cache initialization has fail - invalid source" << std::endl;
         return false;
      }

      // fill the cache queue
      for (auto &ob : m_jsonManager->GetDataLookupTable())
      {
         m_cacheQueue->push(T(ob.second.getPersonalID(), ob.second.getName(),
                              ob.second.getSurname(), ob.second.getEmail(), ob.second.getLastUpdate()));
      }
       
      return true;
   }

   void Save(const std::string& newFilePath)
   {
      assert(m_jsonManager);

      m_jsonManager->Save2File(newFilePath);
   }

   bool AddItem(const T& new_pers)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      if (m_cacheQueue->is_full())
            m_cacheQueue->pop();

      m_cacheQueue->push(new_pers);

      return m_jsonManager->Add(new_pers);
   }

   bool DeleteItem(const std::string& person_id)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      m_cacheQueue->erase(person_id, 
                             [&](std::string const& id, Iter itR)-> bool {
                                     return (string_utils::compare_case_sensitive(id, itR->getPersonalID()));
                             });

      return m_jsonManager->Delete(person_id);
   }

   T* SearchItem(const std::string& person_id)
   {
      assert(m_jsonManager);
      assert(m_cacheQueue);

      auto comp = [&](std::string const& id, Iter itR)-> bool {
         return (string_utils::compare_case_sensitive(id, itR->getPersonalID()));
      };

      auto ptr2pers = m_cacheQueue->find(person_id, comp);
      if (nullptr != ptr2pers)
         return ptr2pers;

      auto ptrCrtPerson = m_jsonManager->Find(person_id);

      m_cacheQueue->push(*ptrCrtPerson); // add to cache for future faster approach
      
      return ptrCrtPerson;

   }
   
private:
   size_t   m_cacheSize;

   typedef typename std::vector<T>::iterator  Iter;
   fixed_queue<T, std::vector<T>, LessThanLastDate>* m_cacheQueue;
   
   jsonIOManager<T>* m_jsonManager;
};