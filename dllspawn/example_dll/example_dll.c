#include "ReflectiveLoader.h"
#include "string.h"
#include "stdio.h"

void go(char * args) {
   char *arg  = NULL;
   char *next = NULL;


   printf("Execute dllspawn callback example:\n");

   
   if (args != NULL) {
      char *copy = NULL;
      size_t len = 0;
   
      // Do not destroy the original string
      len = strlen(args);
      copy = malloc(len + 1);

      if (copy != NULL) {
         memcpy(copy, args, len);
         copy[len] = '\0';

         // Write out the arguments
         // Simple parser to tokenize on a ' '.  Does not handle quoted tokens.
         arg = strtok_s(copy, " ", &next);
         while (arg != NULL) { 
            printf("d:%s\n", arg);
            fflush(stdout);

            // Wait before sending the next item.
            Sleep(5000);

            arg = strtok_s(NULL, " ", &next);
         }

         // cleanup the copy
         free(copy);
      }
   }

   printf("done\n");
   fflush(stdout);
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved ) {
   BOOL bReturnValue = TRUE;

   switch( dwReason ) {
      case DLL_QUERY_HMODULE:
         break;

      case DLL_PROCESS_ATTACH:

         go((char *)lpReserved);

         ExitProcess(0);
         break;

      case DLL_PROCESS_DETACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
         break;
   }
   return bReturnValue;
}
