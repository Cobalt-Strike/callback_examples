#include <windows.h> 
#include "../beacon.h" 

WINBASEAPI int __cdecl MSVCRT$atoi(const char *_Str);
WINBASEAPI int __cdecl MSVCRT$strcmp(const char *_Str1,const char *_Str2);

#define atoi MSVCRT$atoi
#define strcmp MSVCRT$strcmp

void test_beacon_output() {
   formatp buffer;
   char *output;
   int multipleOutputs = 2;
   int outputLength = 0;

   /* Allocate space for formatted output buffer */
   BeaconFormatAlloc(&buffer, 1024);

   BeaconFormatPrintf(&buffer, "TEST - 1 : Test BeaconOutput(CALLBACK_OUTPUT, ...)\n");
   output = BeaconFormatToString(&buffer, &outputLength);
   BeaconOutput(CALLBACK_OUTPUT, output, outputLength);
      
   // Send multiple message via a loop 
   while (multipleOutputs--) {
      BeaconFormatReset(&buffer);
      BeaconFormatPrintf(&buffer, "This is a countdown loop %d\n", multipleOutputs);
      output = BeaconFormatToString(&buffer, &outputLength);
      BeaconOutput(CALLBACK_OUTPUT, output, outputLength);
   }

   BeaconFormatReset(&buffer);
   BeaconFormatPrintf(&buffer, "Done with countdown loop\n");
   output = BeaconFormatToString(&buffer, &outputLength);
   BeaconOutput(CALLBACK_OUTPUT, output, outputLength);

   /* Cleanup */
   BeaconFormatFree(&buffer);
}

void test_beacon_printf() {
   int multipleOutputs = 2;
   int outputLength = 0;

   BeaconPrintf(CALLBACK_OUTPUT, "TEST - 2 : Test BeaconPrintf(CALLBACK_OUTPUT, ...)\n");
      
   // Send multiple message via a loop 
   while (multipleOutputs--) {
      BeaconPrintf(CALLBACK_OUTPUT, "This is a countdown loop %d\n", multipleOutputs);
   }

   BeaconPrintf(CALLBACK_OUTPUT, "Done with countdown loop\n");
}

void test_beacon_output_encoding_utf8() {
   BeaconPrintf(CALLBACK_OUTPUT_UTF8, "TEST - 3 : Test BeaconOutput(CALLBACK_OUTPUT_UTF8, ...)\n");

   /* Hello world string in Japanese */
   BeaconPrintf(CALLBACK_OUTPUT_UTF8, "Next output represent 'Hello world' in Japanese\n");
   BeaconOutput(CALLBACK_OUTPUT_UTF8, "こんにちは世界\n", 23);
}

void test_beacon_printf_encoding_utf8() {
   BeaconPrintf(CALLBACK_OUTPUT_UTF8, "TEST - 4 : Test BeaconPrintf(CALLBACK_OUTPUT_UTF8, ...)\n");

   /* Hello world string in Japanese, CALLBACK_OUTPUT_UTF8 is the correct ouput type */
   BeaconPrintf(CALLBACK_OUTPUT_UTF8, "Next output represent 'Hello world' in Japanese\n");
   BeaconPrintf(CALLBACK_OUTPUT_UTF8, "こんにちは世界\n");
}

void test_beacon_output_error() {
   formatp buffer;
   char *output;
   int outputLength = 0;
   int status = 5;

   /* Allocate space for formatted output buffer */
   BeaconFormatAlloc(&buffer, 1024);

   BeaconFormatPrintf(&buffer, "TEST - 5 : Test BeaconOutput(CALLBACK_ERROR, ...)\n");
   output = BeaconFormatToString(&buffer, &outputLength);
   BeaconOutput(CALLBACK_OUTPUT, output, outputLength);

   BeaconFormatReset(&buffer);
   BeaconFormatPrintf(&buffer, "This is a test error: %d\n", status);
   output = BeaconFormatToString(&buffer, &outputLength);
   BeaconOutput(CALLBACK_ERROR, output, outputLength);

   /* Cleanup */
   BeaconFormatFree(&buffer);
}

void test_beacon_printf_error() {
   int status = 6;

   BeaconPrintf(CALLBACK_OUTPUT, "TEST - 6 : Test BeaconPrintf(CALLBACK_ERROR, ...)\n");
   BeaconPrintf(CALLBACK_ERROR, "This is a test error: %d\n", status);
}


/* entry point */
void go(char * args, int alen) { 
   char * test_string = NULL;
   int test_number = 1;

   /* check for arguments for the binline_execute aggressor function */
   if (alen > 0) {
      test_string = args;
   }

   /* for binline_execute if no arguments are passed the args is set the string 'null' */
   if (test_string != NULL && 0 != strcmp((const char *)test_string, "null")) {
      test_number = atoi((const char *)test_string);
   }
    

   if (test_number == 1) {
      test_beacon_output();
   }
   else if (test_number == 2) {
      test_beacon_printf();
   }
   else if (test_number == 3) {
      test_beacon_output_encoding_utf8();
   }
   else if (test_number == 4) {
      test_beacon_printf_encoding_utf8();
   }
   else if (test_number == 5) {
      test_beacon_output_error();
   }
   else if (test_number == 6) {
      test_beacon_printf_error();
   }
   else {
      BeaconPrintf(CALLBACK_ERROR, "Unknown test_number %d\n", test_number);
   }

} 

