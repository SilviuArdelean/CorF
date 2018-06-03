#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include "string_utils.h"

template<class _Ty,
	class _Container = std::vector<_Ty>,
	class _Pr = std::less<typename _Container::value_type> >
class fixed_queue : public std::priority_queue<_Ty, _Container, _Pr>
{

public:
   fixed_queue() {}
   fixed_queue(unsigned int size)
      : fixed_size(size)
   {
   }

   typedef typename  std::priority_queue<_Ty, _Container, _Pr> _PQ_specialization;

   void push(const _Ty& x)
   {
      if (fixed_size == _PQ_specialization::size())
      {
         auto min = std::min_element(_PQ_specialization::c.begin(),
            _PQ_specialization::c.end(),
            _Pr());
         if (x > *min)
         {
            *min = x;

            // Re-make the heap, since we may have just invalidated it.
            std::make_heap(_PQ_specialization::c.begin(),
               _PQ_specialization::c.end());
         }
      }
      else
      {
         _PQ_specialization::emplace(x);
      }
   }


   void pop()
   {
      _PQ_specialization::c.pop_front();
   }

   void clear()
   {
      _PQ_specialization::c.clear();
   }

   bool erase(std::string const& pers_id)
   {
      for (auto it = _PQ_specialization::c.begin(); it != _PQ_specialization::c.end(); ++it)
      {
         _Ty *pers = &*it;
         
         if (string_utils::compare_case_sensitive( pers->getPersonalID(), pers_id))
         {
            _PQ_specialization::c.erase(it);
            return true;
         }
      }

      return false;
   }

   _Ty* find(std::string const& pers_id)
   {
      for (auto it = _PQ_specialization::c.begin(); it != _PQ_specialization::c.end(); ++it)
      {
         _Ty *pers = &*it;

         if (string_utils::compare_case_sensitive(pers->getPersonalID(), pers_id))
         {
            return pers;
         }
      }

      return nullptr;
   }


   size_t size() const { return _PQ_specialization::c.size(); }

   bool is_full() {      return fixed_size == _PQ_specialization::c.size();    }

	
private:
	const unsigned int fixed_size;

	// Prevent heap allocation
//	void * operator new(size_t) = delete;
//	void * operator new[](size_t) = delete;
//	void   operator delete   (void *) = delete;
//	void   operator delete[](void*) = delete;
};