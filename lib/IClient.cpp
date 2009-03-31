/*
** IClient    - A generic class for connecting to an IServer.
*/


#include <string.h>
#include <stdlib.h>
#ifdef DW_USE_ANSI_IOSTREAM
#  include <iostream>
#else
#  include <iostream>
#endif

#include "IClient.h"
#include <QtCore/QString>
#include <Qt3Support/Q3StrList>

#define RECV_BUFFER_SIZE  65536
#define SEND_BUFFER_SIZE  65536

IClient::IClient()
{
    mSendBuffer = new char[SEND_BUFFER_SIZE];
    mRecvBuffer = new char[RECV_BUFFER_SIZE];
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mStatusCode = 0;
    bufferedData    = new Q3StrList(TRUE);
}


IClient::~IClient()
{
    if (mRecvBuffer) {
        delete [] mRecvBuffer;
        mRecvBuffer = 0;
    }
    if (mSendBuffer) {
        delete [] mSendBuffer;
        mSendBuffer = 0;
    }
}


int IClient::Open(const char* aServer, DwUint16 aPort)
{
    mStatusCode = 0;
    mSingleLineResponse = mMultiLineResponse = "";
    int err = DwProtocolClient::Open(aServer, aPort);
    if (!err) {
        PGetSingleLineResponse();
        // fprintf(stderr, "Open:  Received '%s'\n", mSingleLineResponse.data());
    } else mStatusCode = -1;
    return mStatusCode;
}



int IClient::StatusCode() const
{
    return mStatusCode;
}


const DwString& IClient::SingleLineResponse() const
{
    return mSingleLineResponse;
}


const DwString& IClient::MultiLineResponse() const
{
    return mMultiLineResponse;
}



void IClient::PGetSingleLineResponse()
{
    mStatusCode = 0;
    mSingleLineResponse = "";
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mStatusCode = ptr[0];
        mSingleLineResponse.assign(ptr, len);
    }
}


void IClient::PGetMultiLineResponse()
{
    mMultiLineResponse = "";
    
    DwString tmpResponse = "";
    // The first line will contain the + or - and the status.  Get that first.
    mStatusCode = 0;
    mSingleLineResponse = "";
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mStatusCode = ptr[0];
        mSingleLineResponse.assign(ptr, len);
    }

    // Get a line at a time until we get CR LF . CR LF

    while (1) {
        char* ptr;
        int len;
        int err = PGetLine(&ptr, &len);

        // Check for an error

        if (err) {
            mStatusCode = 0;
            return;
        }

        // Check for '.' on a line by itself, which indicates end of multiline
        // response

        if (len >= 3 && ptr[0] == '.' && ptr[1] == '\r' && ptr[2] == '\n') {
            break;
        }

        // Remove '.' at beginning of line

        if (*ptr == '.') ++ptr;

        mMultiLineResponse.append(ptr, len);
    }
    
    
    
}


int IClient::PGetLine(char** aPtr, int* aLen)
{
    // Restore the saved state

    int startPos = mRecvBufferPos;
    int pos = mRecvBufferPos;
    int lastChar = -1;

    // Keep trying until we get a complete line, detect an error, or
    // determine that the connection has been closed

    int isEndOfLineFound = 0;
    while (1) {

        // Search buffer for end of line chars. Stop when we find them or when
        // we exhaust the buffer.

        while (pos < mNumRecvBufferChars) {
            if (lastChar == '\r' && mRecvBuffer[pos] == '\n') {
                isEndOfLineFound = 1;
                ++pos;
                break;
            }
            lastChar = mRecvBuffer[pos];
            ++pos;
        }
        if (isEndOfLineFound) {
            *aPtr = &mRecvBuffer[startPos];
            *aLen = pos - startPos;
            mRecvBufferPos = pos;
            return 0;
        }

        // If the buffer has no room, return without an error; otherwise,
        // replenish the buffer.

        // Implementation note: The standard does not allow long lines,
        // however, that does not mean that they won't occur.  The way
        // this function deals with long lines is to return a full buffer's
        // worth of characters as a line.  The next call to this function
        // will start where this call left off.  In essence, we have
        // *forced* a line break, but without putting in CR LF characters.

        if (startPos == 0 && pos == RECV_BUFFER_SIZE) {
            *aPtr = mRecvBuffer;
            *aLen = RECV_BUFFER_SIZE;
            mRecvBufferPos = pos;
            return 0;
        }
        memmove(mRecvBuffer, &mRecvBuffer[startPos],
            mNumRecvBufferChars-startPos);
        mNumRecvBufferChars -= startPos;
        mRecvBufferPos = mNumRecvBufferChars;
        int bufFreeSpace = RECV_BUFFER_SIZE - mRecvBufferPos;
        int n = PReceive(&mRecvBuffer[mRecvBufferPos], bufFreeSpace);
        if (n == 0) {
            // The connection has been closed or an error occurred
            return -1;
        }
        mNumRecvBufferChars += n;
        startPos = 0;
        pos = mRecvBufferPos;
    }
}


int IClient::Send(const char* aBuf, int aBufLen)
{
    int           tmpLen = strlen(aBuf);
    int           tmpRet;

    tmpRet = PSend(aBuf, tmpLen);
    if (tmpRet == tmpLen) {
        // We sent all of the data.  Return the amount that the program
        // thinks we sent.
        tmpRet = aBufLen;
    }
    
    return(tmpRet);
}

/*
** sendData    - Takes a bit of data and buffers it.  It doesn't really
**               send it until the endData() command is called.
*/

void IClient::sendData(const char *data)
{
    char    *tmpStr = new char[strlen(data) + 8];
    sprintf(tmpStr, "%s\r\n", data);
    bufferedData->append(tmpStr);
    delete tmpStr;
}

/*
** endData    - Takes all of the buffered data and sends it out to
**              the client, terminated with a "\n\r.\n\r".
*/

void IClient::endData(void)
{
    int tmpRet  = 0;
    if (bufferedData->count()) {
        for (unsigned int i = 0; i < bufferedData->count(); i++) {
            if (strlen(bufferedData->at(i))) {
                //fprintf(stderr, "Sending: '%s'\n", (const char *) bufferedData->at(i));
                tmpRet = PSend((const char *) bufferedData->at(i), strlen((const char *) bufferedData->at(i)));
            }
        }
    }
    
    tmpRet = PSend(".\r\n", strlen(".\r\n"));

    bufferedData->clear();    
}

