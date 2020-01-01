#pragma once
#include <memory>
#include <mutex>
#include "fixed_queue.h"
#include "jsonIOManager.h"
#include "string_utils.h"

std::mutex g_i_mutexC;

template <typename T>
class CacheManager {
  struct LessThanLastDate {
    bool operator()(const T& lhs, const T& rhs) const {
      return string_utils::getEpochTime(lhs.getLastUpdate()) <
             string_utils::getEpochTime(rhs.getLastUpdate());
    }
  };

 public:
  void SetSaveAsFileName(const std::string& strFile) {
    assert(json_manager_);
    json_manager_->SetSaveAsFileName(strFile);
  }

  CacheManager(const std::string& consistDataFilePath, size_t cacheSize = 10)
      : cache_size_(cacheSize) {
    cache_queue_ =
        new fixed_queue<T, std::vector<T>, LessThanLastDate>(cache_size_);

    json_manager_ = new jsonIOManager<T>(consistDataFilePath);
  }
  ~CacheManager() {
    delete json_manager_;
    delete cache_queue_;
  }

  bool Initialize() {
    assert(json_manager_);
    assert(cache_queue_);

    std::lock_guard<std::mutex> lock(g_i_mutexC);

    if (!json_manager_->Initialize()) {
      std::cout << "Cache initialization has fail - invalid source"
                << std::endl;
      return false;
    }

    // fill the cache queue
    for (auto& ob : json_manager_->GetDataLookupTable()) {
      cache_queue_->push(T(ob.second.getPersonalID(), ob.second.getName(),
                           ob.second.getSurname(), ob.second.getEmail(),
                           ob.second.getLastUpdate()));
    }

    return true;
  }

  void Save(const std::string& newFilePath) {
    assert(json_manager_);

    std::lock_guard<std::mutex> lock(g_i_mutexC);

    json_manager_->Save2File(newFilePath);
  }

  bool AddItem(const T& new_pers) {
    assert(json_manager_);
    assert(cache_queue_);

    std::lock_guard<std::mutex> lock(g_i_mutexC);

    // make sure will reach the cache
    if (cache_queue_->is_full())
      cache_queue_->pop();

    cache_queue_->push(new_pers);

    return json_manager_->Add(new_pers);
  }

  bool DeleteItem(const std::string& person_id) {
    assert(json_manager_);
    assert(cache_queue_);

    std::lock_guard<std::mutex> lock(g_i_mutexC);

    cache_queue_->erase(
        person_id, [&](std::string const& id, Iter itR) -> bool {
          return (
              string_utils::compare_case_sensitive(id, itR->getPersonalID()));
        });

    return json_manager_->Delete(person_id);
  }

  T* SearchItem(const std::string& person_id) {
    assert(json_manager_);
    assert(cache_queue_);

    std::lock_guard<std::mutex> lock(g_i_mutexC);

    auto comp = [&](std::string const& id, Iter itR) -> bool {
      return (string_utils::compare_case_sensitive(id, itR->getPersonalID()));
    };

    auto ptr2pers = cache_queue_->find(person_id, comp);
    if (nullptr != ptr2pers)
      return ptr2pers;

    auto ptrCrtPerson = json_manager_->Find(person_id);

    if (ptrCrtPerson) {
      cache_queue_->push(
          *ptrCrtPerson);  // add to cache for future faster approach
      return ptrCrtPerson;
    }

    return nullptr;
  }

 private:
  size_t cache_size_;

  typedef typename std::vector<T>::iterator Iter;

  // for skills demo reasons I didn't use a lookup table here (as in
  // jsonIOManager.h) prefered to use a special way to limit items count and way
  // to prioritize, the std::primary_queue
  fixed_queue<T, std::vector<T>, LessThanLastDate>* cache_queue_;

  jsonIOManager<T>* json_manager_;
};