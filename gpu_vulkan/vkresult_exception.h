//
// Created by camilo on 2025-08-22.
//
#pragma once


class CLASS_DECL_GPU_VULKAN vkresult_exception :
virtual public ::exception
{
public:


      VkResult          m_vkresult;

      vkresult_exception(VkResult vkresult, const ::scoped_string & scopedstr);
   ~vkresult_exception() override;

};

