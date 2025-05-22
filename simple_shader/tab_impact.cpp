#include "framework.h"
#include "tab_impact.h"
#include "impact.h"
#include "application.h"
#include "app/shader/render.h"
#include "acme/filesystem/filesystem/listing.h"
#include "base/user/menu/list_impact.h"
#include "base/user/user/tab_pane.h"


namespace app_simple_shader
{


   tab_impact::tab_impact()
   {

   }


   tab_impact::~tab_impact()
   {

   }


   //void tab_impact::assert_ok() const
   //{

   //   ::user::impact::assert_ok();

   //}


   //void tab_impact::dump(dump_context & dumpcontext) const
   //{

   //   ::user::impact::dump(dumpcontext);

   //}


   void tab_impact::install_message_routing(::channel * pchannel)
   {

      ::user::tab_impact::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create, pchannel, this, &tab_impact::on_message_create);

   }


   void tab_impact::on_message_create(::message::message * pmessage)
   {

      if(pmessage->previous())
      {

         return;

      }

      //initialize_application_consumer();

      get_app()->m_ptabimpact = this;

      _001SetVertical();

      auto papp = get_app();

      ::file::listing listing;

      papp->get_shader_listing(listing);

      set_tab("Menu", MENU_IMPACT);

      for (auto& path : listing)
      {

         string strTitle = path.title();

         string strPath = path;

         set_tab(strTitle, "shader://" + strPath);

      }

      //set_tab("1", "shader1");
      //set_tab("2", "shader2");
      //set_tab("3", "shader3");
      //set_tab("4", "shader4");
      //set_tab("5", "shader5");
      //set_tab("6", "shader6");
      //set_tab("7", "shader7");
      //set_tab("8", "shader8");
      //set_tab("9", "shader9");
      //set_tab("10", "shader10");
      //set_tab("11", "shader11");
      //set_tab("12", "shader12");
      //set_tab("13", "shader13");
      //set_tab("14", "shader14");

      //set_tab("Font", FONTSEL_IMPACT);
      //set_tab("Color", COLORSEL_IMPACT);
      //set_tab("Open", FILEMANAGER_IMPACT);

      if (listing.has_element())
      {

         auto path = listing.first();

         ::string strPath = path;

         set_current_tab_by_id("shader://" + strPath);

      }

   }


   void tab_impact::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::tab_impact::_001OnNcDraw(pgraphics);

   }


   void tab_impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::tab_impact::_001OnDraw(pgraphics);

   }


   void tab_impact::on_change_cur_sel()
   {

      ::user::tab_impact::on_change_cur_sel();

      string strId = get_impact_id();

      string_array stra;

      stra.explode("->:<-",strId);

//      if (get_impact_id() == MENU_IMPACT)
//      {
//
//         ::pointer<::user::menu_list_impact> pmenuview = get_impact_uie();
//
//         pmenuview->destroy_menu();
//
//       }

      if(strId.case_insensitive_begins_eat("shader://"))
      {

         string strShaderPath = strId;

         auto ptabpaneFileManager = get_tab_by_id(FILEMANAGER_IMPACT);

         if(ptabpaneFileManager != nullptr && ptabpaneFileManager->m_pplaceholder != nullptr)
         {

            ptabpaneFileManager->m_pplaceholder->hide();

         }

         auto ptabpaneFontSel = get_tab_by_id(FONTSEL_IMPACT);

         if (ptabpaneFontSel != nullptr && ptabpaneFontSel->m_pplaceholder != nullptr)
         {

            ptabpaneFontSel->m_pplaceholder->hide();

         }

         m_pviewTopic = m_pimpactdata->m_pplaceholder->get_hold();

      }

   }


   void tab_impact::on_create_impact(::user::impact_data * pimpactdata)
   {

      auto papp = get_app();

      if (papp->has_property("notabs"))
      {

         return;

      }

      switch(pimpactdata->id().as_long_long())
      {
      case MENU_IMPACT:
      {

         ::user::impact::create_impact < ::user::menu_list_impact >(pimpactdata);

      }
      break;
      }

      string strId = pimpactdata->id();

      if(strId.case_insensitive_begins_eat("shader://"))
      {

         string strShaderPath = strId;

         auto pimpact = get_app()->create_simple_shader_impact(this, pimpactdata);

         pimpact->id() = pimpactdata->id();

         pimpact->m_iImpact = (int) pimpactdata->m_iIndex;

         ::pointer<app_simple_shader::impact> pviewShader = pimpact;

         pviewShader->m_prender->m_strShaderPath = strShaderPath;

         pimpactdata->m_eflag.add(::user::e_flag_hidid_on_show);

      }

      ::user::tab_impact::on_create_impact(pimpactdata);

   }


   void tab_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {



   }


//   void tab_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
//   {
//
//
//      ::user::tab_impact::handle(ptopic, phandlercontext);
//
//   }


} // namespace simple_shader



