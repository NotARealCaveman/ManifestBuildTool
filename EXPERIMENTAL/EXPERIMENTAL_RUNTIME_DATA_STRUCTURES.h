#pragma once
#include <type_traits>
#include <concepts>
#include <atomic>
#include <thread>

#include <ManifestGLPersistence/DatabaseTypes.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Persistence;

namespace Manifest_Experimental
{	
	//loop on exchange while lock is held(1) until released(0)
	constexpr MFbool LOCKED{ 1 };
	constexpr MFbool UNLOCKED{ 0 };
	struct ExchangeLock
	{
	protected:
		std::atomic<MFbool> lock{ UNLOCKED };
	public:
		//attempts to lock, spins on exchange until locked
		void Lock();
		//restores lock to unlocked state
		void Unlock();
	};

	//single reader single writing
	struct SRSWExchangeLock : public ExchangeLock
	{
	private:
		std::atomic_flag reading = ATOMIC_FLAG_INIT;
		std::atomic_flag writing = ATOMIC_FLAG_INIT;
	public:
		//returns true if a reader is not reading
		inline const bool Try_Write();
		//returns true if a writer is not writing
		inline const bool Try_Read();
		//locks for writing, sets writing flag, performs write
		template<typename Function, typename... Params>
		void Write(const Function& writeFunc, const Params&... params)
		{
			//Lock();
			writing.test_and_set(std::memory_order_release);
			writeFunc(params...);
			writing.clear(std::memory_order_release);
			//Unlock();
		};
		//locks for reading, sets reading flag, performs read
		template<typename Function, typename... Params>
		void Read(const Function& readFunc, const Params... params)
		{
			//Lock();
			reading.test_and_set(std::memory_order_release);
			readFunc(params...);
			reading.clear(std::memory_order_release);
			//Unlock();
		}

	};

	//TODO - Multi Reader Single Writer(MRSW) /Lock/
	//not sure if it will be a lock - previous ended up not being a technical lock
	//the previous method is essentially already a MRSW exchange
	//will not be looking into MRMW exchanges - all data production points must be contained in their respective logical spacings. only a single writer(producer) will ever exist for a given piece of data though an infinite amount of readers may exist. therefore, should the program be structured properly, then the production of consumed data will naturally be applicable in only the place in which it may be found.
	/*----
	while realistcally there is only going to be a single reader and a single writer for a given set of data the above locking is overkill but still useful for investigation as i believe the notifcation and messaging system will need to implement a form of RCU as the lifetime of that data cannot have assumptions made about the need to keep it around.
	the event space will also be coming back as more of a philosophical type than anything else but still plays a cruical role of brokering oberseved events. essentially the event space allows observers to know what type of event they observed and when
	the assumption about the production of data appears to hold up - the messaging system is a coupling bypass to handle the specific/variable data of otherwise static interactions. by forming the logical spacings and allowing only one writer per type of event message type(s) this forces the prior assumption the program will be properly formed such that the only place in which this data may be created is that where it is produced*/

	//table types
	template<typename Key, typename Value>
	concept UniqueType = !std::is_same<Key, Value>::value;
	template<typename Key, typename Value>
	concept EqualType = !UniqueType<Key, Value>;
	//table iterators
	template<typename Iterator, typename Key, typename Value>	
	concept KeyIterator = std::is_same<Iterator, Key>::value && UniqueType<Key, Value>;
	template<typename Iterator, typename Key, typename Value>
	concept ValueIterator = std::is_same<Iterator, Value>::value && UniqueType<Key, Value>;

	template<typename Key, typename Value>	
	struct Table
	{
		MFsize tableEntries{0};
		MFsize tableSize;
		Key* keys;//byte 16
		Value* values;//byte 16 + sizeof(Key)	
		///TABLE BEGIN AND END FUNCTIONS
		//default begin for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* begin()
		{	
			return *(&keys+valueSearch);//returns field by using size as offset
		}
		//specialized functions for begin - fewer instructions than defualt
		template<typename KIterator>
		requires KeyIterator<KIterator,Key, Value>
		inline KIterator* begin()
		{
			return keys;
		}
		template<typename VIterator>
		requires ValueIterator<VIterator, Key, Value>
		inline VIterator* begin()
		{
			return values;
		}
		//default end for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* end()
		{						
			return &(*(&keys + valueSearch))[tableEntries];
		}
		//specialized functions for end - fewer instructions than defualt
		template<typename KIterator>
		requires KeyIterator<KIterator, Key, Value>
		inline KIterator* end()
		{
			return &keys[tableEntries];
		}
		template<typename VIterator>
		requires ValueIterator<VIterator, Key, Value>
		inline VIterator* end()
		{
			return &values[tableEntries];
		}
	};	
}