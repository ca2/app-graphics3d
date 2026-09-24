//
// Created by camilo on 2025-08-22 05:49 <3ThomasBorregaardSorensen!!
//
#include "platform.h"
#include "vkresult_exception.h"


vkresult_exception::vkresult_exception(VkResult vkresult, const ::scoped_string & scopedstrMessage):
::exception(error_vkresult, scopedstrMessage),
m_vkresult(vkresult)
{


}


vkresult_exception::~vkresult_exception()
{


}



