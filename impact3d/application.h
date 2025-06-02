#pragma once


#include "core/platform/application.h"


namespace graphics3d_impact3d
{


   class CLASS_DECL_APP_COMPLEX_IMPACT3D application :
      virtual public ::core::application
   {
   public:


      bool                                   m_bMultiverseChat;

      ::user::document *                     m_pdocMenu;
      ::user::plain_edit_impact *              m_prollfps;
      //::user::single_document_template *     m_ptemplateMain;
      //::user::single_document_template *     m_ptemplateImpact;
      pane_impact *                          m_ppaneimpact;

      string                                 m_strMainTitle;


      application();
      ~application() override;
      
      __DECLARE_APPLICATION_RELEASE_TIME();


      virtual string preferred_experience() override;
      void init_instance() override;
      virtual void term_application() override;

      virtual void on_request(::request * prequest) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

      virtual ::pointer<impact> create_simple_shader_impact(::user::impact * pimpactParent, ::user::impact_data * pimpactdata);


      //virtual string get_main_title() const;
      //virtual string get_hover_font() const;


      //DECLARE_MESSAGE_HANDLER(_001OnSimpleCheckbox);

      // slowly phase out this idle/poll approach?
      // is it good for later consolidated evaluation?
      // is this "consolidated" evaluation the only advantage?
      // can it be replaced by custom-on-demand delayed consolidated active update when it is proper to do so?
      // isn't it taking this "_001OnUpdate" approach (and assuming this single advantage) builtin
      // early optimization with the cost of idle update? (is it really a optimization if the update is
      // done through idle/polled evaluation)?
      //DECLARE_MESSAGE_HANDLER(_001OnUpdateSimpleCheckbox);


      //virtual ::identification translate_property_id(const ::identification& identification) override;


   };


} // namespace graphics3d_impact3d



