#pragma once
#include <string>
#include "rapidjson/writer.h"
#include "string_utils.h"

//{
//   "person_id":"1686053076599", 
//   "name" : "Finn", 
//   "surname" : "Nixon",
//   "email" : "luctus.felis@consectetuer.ca", 
//   "last_update" : "2018-01-15 20:04:58"
//}

class Person 
{
public:
   Person() {}

   Person(const std::string& id, const std::string& name, const std::string& surname,
      const std::string& email, const std::string& lastupdate)
      : id_(id),
      name_(name),
      surname_(surname),
      email_(email),
      lastupdate_(lastupdate)
   {
   }

   virtual ~Person() {}

   Person(const Person& rhs)
      : id_(rhs.id_), name_(rhs.name_), surname_(rhs.surname_),
      email_(rhs.email_), lastupdate_(rhs.lastupdate_)
   {
   }   

   Person& operator = (const Person& rhs) 
   {

      if (this != &rhs)
      {
         id_ = rhs.id_;
         name_ = rhs.name_;
         surname_ = rhs.surname_;
         email_ = rhs.email_;
         lastupdate_ = rhs.lastupdate_;
      }

      return *this;
   }

   // add move constructor and move operator
   Person(Person&& rhs)
   {
      id_ = rhs.id_;
      name_ = rhs.name_;
      surname_ = rhs.surname_;
      email_ = rhs.email_;
      lastupdate_ = rhs.lastupdate_;

      rhs.id_ = "";
      rhs.name_ = "";
      rhs.surname_ = "";
      rhs.email_ = "";
      rhs.lastupdate_ = "";
   }

   Person& operator = (Person&& rhs)
   {
      if (this != &rhs)
      {
         id_ = rhs.id_;
         name_ = rhs.name_;
         surname_ = rhs.surname_;
         email_ = rhs.email_;
         lastupdate_ = rhs.lastupdate_;

         rhs.id_ = "";
         rhs.name_ = "";
         rhs.surname_ = "";
         rhs.email_ = "";
         rhs.lastupdate_ = "";
      }

      return *this;
   }

   bool operator > (const Person& rhs) const
   {
      return string_utils::getEpochTime(getLastUpdate()) > string_utils::getEpochTime(rhs.getLastUpdate());
   }

   bool operator < (const Person& rhs) const
   {
      return string_utils::getEpochTime(getLastUpdate()) < string_utils::getEpochTime(rhs.getLastUpdate());
   }
   
   template <typename Writer>
   void Serialize(Writer& writer) const
   {
      // This base class just write out name-value pairs, without wrapping within an object.
      writer.String("person_id");
      writer.String(id_.c_str());
      writer.String("name");
      writer.String(name_.c_str());
      writer.String("surname");
      writer.String(surname_.c_str());
      writer.String("email");
      writer.String(email_.c_str());
      writer.String("last_update");
      writer.String(lastupdate_.c_str());
   }

   std::string getPersonalID() const { return id_; }
   std::string getName() const { return name_; }
   std::string getSurname() const { return surname_; }
   std::string getEmail() const { return email_; }
   std::string getLastUpdate() const { return lastupdate_; }

protected:
   std::string id_;
   std::string name_;
   std::string surname_;
   std::string email_;
   std::string lastupdate_;
};