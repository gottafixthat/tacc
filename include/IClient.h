/*
** File:       IClient.h
** Contents:   Declarations for The Blarg Internet Server Client
**
**             Requires the Mime++ libraries.
*/

#ifndef _ICLIENT_H
#define _ICLIENT_H

// Requires the Mime++ Libraries.

#include <mimepp/protocol.h>
#include <mimepp/string.h>
#include <stdio.h>
#include <qstrlist.h>


/*
** Name IClient -- Class for handling the client side of an internet
**                 connection.  Should be inherited by any applications
**                 that want to use it.
*/


class IClient : public DwProtocolClient {

public:

    IClient();
    virtual ~IClient();

    virtual int Open(const char* aServer, DwUint16 aPort=9003);

    int StatusCode() const;
    //. Returns the status code received from the server in response to the
    //. last client command. The status codes are '+', indicating
    //. success, and '-', indicating failure. If no response was received,
    //. {\tt StatusCode()} returns zero.

    const DwString& SingleLineResponse() const;
    //. Returns the single line status response last received from the server.
    //. If no response was received, perhaps because of a communications
    //. failure, {\tt SingleLineResponse()} returns an empty string.

    const DwString& MultiLineResponse() const;
    //. If no observer is set for this object, {\tt MultiLineResponse()}
    //. returns a string that comprises the entire sequence of lines
    //. received from the server.  Otherwise, if an observer {\it is} set
    //. for this object, {\tt MultiLineResponse()} returns only the most
    //. recent line received.

    char*       mSendBuffer;
    int         mStatusCode;
    char*       mRecvBuffer;
    int         mNumRecvBufferChars;
    int         mRecvBufferPos;
    DwString    mSingleLineResponse;
    DwString    mMultiLineResponse;



    int Send(const char *aBuf, int aBufLen);

    int PGetLine(char** aPtr, int* aLen);
    // Tries to get one complete line of input from the socket.  On success,
    // the function sets {\tt *aPtr} to point to the beginning of the line in
    // the object's internal buffer, sets {\tt *aLen} to the length of the
    // line, including the CR LF, and returns 0.  On failure, the function
    // returns -1.

    void PGetSingleLineResponse();
    // Gets a single line of input, assigns that line {\tt mSingleLineResponse}, and
    // sets {\tt mStatusCode}.  On failure, clears {\tt mSingleLineResonse}
    // and sets {\tt mStatusCode} to -1.

    void PGetMultiLineResponse();
    // Gets a complete multiline response and assigns it to {\tt mMultiLineResponse},
    // or interacts with the {\tt DwObserver} object to deliver a multiline response
    // one line at a time.
    // If an error occurs, its sets {\tt mStatusCode} to -1.

    void sendData(const char *data);
    void endData(void);

private:
    QStrList    *bufferedData;
    

};

#endif
