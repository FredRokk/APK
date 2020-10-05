// ProgramTransformation.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;

#include "LogFile.h"

int main()
{
  {
    LogFile dummy;
    if( !dummy.write( "Writing from main" ) )
    {
      switch( dummy.getState() )
      {
        case LogFile::es_OK:
          break;
        case LogFile::es_NOT_INITIALIZED:
          cerr << "Dummy not initialized" << endl;
          break;
        case LogFile::es_COULD_NOT_OPEN_FILE:
          cerr << "Could not open Dummy" << endl;
          break;
        case LogFile::es_FILENAME_IS_EMPTY:
          cerr << "Dummy filename is empty" << endl;
          break;
      }
    }
  }

  LogFile logFile( "Exceptions.log" );
  if( logFile.getState() != LogFile::es_OK )
  {
    switch( logFile.getState() )
    {
      case LogFile::es_OK:
        break;
      case LogFile::es_NOT_INITIALIZED:
        cerr << "LogFile not initialized" << endl;
        break;
      case LogFile::es_COULD_NOT_OPEN_FILE:
        cerr << "Could not open LogFile" << endl;
        break;
      case LogFile::es_FILENAME_IS_EMPTY:
        cerr << "LogFile filename is empty" << endl;
        break;
    }
  }
  else if(!logFile.write( "Writing from main" ) )
  {
    switch( logFile.getState() )
    {
      case LogFile::es_OK:
        break;
      case LogFile::es_NOT_INITIALIZED:
        cerr << "LogFile not initialized" << endl;
        break;
      case LogFile::es_COULD_NOT_OPEN_FILE:
        cerr << "Could not open LogFile" << endl;
        break;
      case LogFile::es_FILENAME_IS_EMPTY:
        cerr << "LogFile filename is empty" << endl;
        break;
    }
  }

  if(logFile.getState() == LogFile::es_OK)
    if( !logFile.write( "Writing from main - again" ) )
    {
      switch( logFile.getState() )
      {
        case LogFile::es_OK:
          break;
        case LogFile::es_NOT_INITIALIZED:
          cerr << "LogFile not initialized" << endl;
          break;
        case LogFile::es_COULD_NOT_OPEN_FILE:
          cerr << "Could not open LogFile" << endl;
          break;
        case LogFile::es_FILENAME_IS_EMPTY:
          cerr << "LogFile filename is empty" << endl;
          break;
      }
    }
}
