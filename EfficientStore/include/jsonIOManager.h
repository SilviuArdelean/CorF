#pragma once

#include <mutex>
#include "general.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"  // for stringify JSON
#include "rapidjson/writer.h"
#include "string_utils.h"

std::mutex g_i_mutexJ;

template <typename T>
class jsonIOManager {
 public:
  jsonIOManager() = delete;

  jsonIOManager(const std::string& jsonFilePath)
      : json_file_path_(jsonFilePath), file_path_save_as_("") {}

  ~jsonIOManager() {
    // Force saving data to file - RTTI based stuff
    Save2File(file_path_save_as_.empty() ? json_file_path_ : file_path_save_as_);
  }

  void SetSaveAsFileName(const std::string& strFile) {
    file_path_save_as_ = strFile;
  }

  bool Initialize() {
    std::lock_guard<std::mutex> lock(g_i_mutexJ);

    std::ifstream ifs(json_file_path_);

    if (!ifs.is_open()) {
      std::cout << "Error opening file: " << json_file_path_ << std::endl;
      return false;
    }

    rapidjson::IStreamWrapper isw(ifs);

    doc_json_.ParseStream<0>(isw);

    lookup_table_.reserve(doc_json_.Size());

    for (rapidjson::SizeType i = 0; i < doc_json_.Size(); i++) {
      auto pers_id = doc_json_[i]["person_id"].GetString();

      lookup_table_.insert(std::pair<std::string, T>(
          pers_id, T(pers_id, doc_json_[i]["name"].GetString(),
                     doc_json_[i]["surname"].GetString(),
                     doc_json_[i]["email"].GetString(),
                     doc_json_[i]["last_update"].GetString())));
    }

    return true;
  }

  void Save2File(const std::string& newFilePath) {
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

  std::unordered_map<std::string, T>& GetDataLookupTable() {
    return lookup_table_;
  }

  bool Add(const T& pers) {
    std::lock_guard<std::mutex> lock(g_i_mutexJ);

    auto it = lookup_table_.find(pers.getPersonalID());
    if (it != lookup_table_.end()) {
      std::cout << "Person ID " << pers.getPersonalID() << " already exists"
                << std::endl;
      return false;
    }

    lookup_table_[pers.getPersonalID()] = pers;

    return true;
  }

  T* Find(const std::string& pers_id) {
    std::lock_guard<std::mutex> lock(g_i_mutexJ);

    auto item_iter = lookup_table_.find(pers_id);

    return (item_iter != lookup_table_.end())
               ? dynamic_cast<T*>(&(item_iter->second))
               : nullptr;
  }

  bool Delete(const std::string& pers_id) {
    std::lock_guard<std::mutex> lock(g_i_mutexJ);

    auto item_iter = lookup_table_.find(pers_id);

    if (item_iter != lookup_table_.end()) {
      lookup_table_.erase(item_iter);
      return true;
    }

    return false;
  }

 protected:
  void Serialize(rapidjson::StringBuffer& sb) {
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartArray();

    for (auto& item : lookup_table_) {
      writer.StartObject();
      item.second.Serialize(writer);
      writer.EndObject();
    }

    writer.EndArray();
  }

 private:
  std::string json_file_path_;
  std::string file_path_save_as_;

  rapidjson::Document doc_json_;

  std::unordered_map<std::string, T> lookup_table_;
};