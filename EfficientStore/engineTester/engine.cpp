// engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "jsonIOManager.h"
#include "CacheManager.h"
#include "Person.h"

using namespace std;

int main()
{
   std::string defaultFile(".\\data\\persons_new.json");
//   jsonIOManager manager(defaultFile);
//   manager.LoadFile();
//   manager.Save2File(".\\data\\persons_data2.json");

   CacheManager<Person> cache(defaultFile, 20);
   if (!cache.Initialize())
   {
      std::cout << "Failed to initialize the cache memory" << std::endl;
   }

   cache.SetSaveAsFileName(".\\data\\persons_new2.json");

   /*
       {
        "person_id": "1698080198",
        "name": "Silviu",
        "surname": "Marius",
        "email": "silviu@silviuardelean.ro",
        "last_update": "2019-05-30 20:30:45"
    },
   */

   Person p1( "1698080198", "Silviu", "Marius", "silviu@silviuardelean.ro", "2011-05-30 20:30:45");
   cache.AddItem(p1);

   auto find1 = cache.SearchItem("1660042025399"); // founded in cache
   auto find2 = cache.SearchItem("1637111483499"); // founded in big data

   auto find3 = cache.SearchItem("1643061702799");    // 1643061702799 - cache
   auto find4 = cache.SearchItem("1677031469399");     // 1677031469399 - big


   //cache.DeleteItem("1607090439499");
   //auto find_new1 = cache.SearchItem("1698080198");
   //cache.DeleteItem("1698080198");
   //auto find_new2 = cache.SearchItem("1698080198");


   return 0;
}

