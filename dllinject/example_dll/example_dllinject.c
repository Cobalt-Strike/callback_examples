#include "ReflectiveLoader.h"
#include "string.h"
#include "stdio.h"

// Set buffer size to default task size (1Mb) to avoid limitations with the bi-directional communication.
#define BUFFER_SIZE (1024*1024)

void go(char * args) {
    /* As this is an example the pipename and names array will be hard coded as it is
     * a little more difficult to pass user information to a DLL designed to work with
     * the bdllinject aggressor function.
     *
     * It is possible to utilize Aggressor Script to "stomp" information into the DLL
     * but that is left as an exercise for the user to implement.
     */
    const char pipename[] = "\\\\.\\pipe\\mynamedpipe";
    const char *names[] = {"Han", "Luke", "Leia"};
    int numberOfNames = 3, index = 0;

    HANDLE hPipe = INVALID_HANDLE_VALUE;
    CHAR chBuf[512];
    int sleepInterval = 2000;
    BOOL   fSuccess = FALSE;
    DWORD cbToWrite = 0, cbWritten = 0;

    hPipe = CreateNamedPipeA(pipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, BUFFER_SIZE, BUFFER_SIZE, 0, NULL);
    if (!hPipe) {
        return;
    }

    if (!ConnectNamedPipe(hPipe, NULL)) {
        return;
    }

    numberOfNames++; // Add one extra name for the 'done' keyword
    while (numberOfNames--) {
        if (numberOfNames == 0) {
            cbToWrite = _snprintf(chBuf, sizeof(chBuf), "done\n");
            sleepInterval = 0;
        }
        else {
            cbToWrite = _snprintf(chBuf, sizeof(chBuf), "d:%s\n", names[index]);
            index++;
        }

        fSuccess = WriteFile(hPipe, chBuf, cbToWrite, &cbWritten, NULL);

        if (!fSuccess) {
            break;
        }

        if (sleepInterval > 0) {
            Sleep(sleepInterval);
        }
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved ) {
   BOOL bReturnValue = TRUE;

   switch( dwReason ) {
      case DLL_QUERY_HMODULE:
         break;

      case DLL_PROCESS_ATTACH:

         go((char *)lpReserved);

         ExitThread(0);
         break;

      case DLL_PROCESS_DETACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
         break;
   }
   return bReturnValue;
}
