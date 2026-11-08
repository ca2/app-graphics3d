#include "platform.h"
#include "application.h"
#include "acme/application.h"


//app_core_vulken::application g_app_core_vulken;


//int __implement()
//{
//
//   auto papp = ___new app_core_hello_multiverse::application ();
//
//   int iExitCode = papp->__implement();
//
//   return iExitCode;
//
//}
//
//
//




void application_main(::platform::system * psystem)
{

   app_core_vulken::application application;

   application.application_main();

}


