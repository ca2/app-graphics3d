//
// Created by camilo on 2025-08-22.
//
#pragma once

class vkresult_exception :
virtual public ::exception
{
public:


      VkResult          m_vkresult;

      vkresult_exception(VkResult vkresult, const ::scoped_string & scopedstr);
   ~vkresult_exception() override;

};

