//
// Created by phil on 01.10.20.
//

#ifndef ECS_FAMILYTYPEID_H
#define ECS_FAMILYTYPEID_H

#include "../Platform.h"

namespace ECS {
    template<class T>
	class FamilyTypeID
	{
		static TypeID s_count;

	public:

		template<class U>
		static const TypeID Get()
		{
			static const TypeID STATIC_TYPE_ID { s_count++ };
			return STATIC_TYPE_ID;
		}

		static const TypeID Get()
		{
			return s_count;
		}
	};
}
#endif //ECS_FAMILYTYPEID_H
