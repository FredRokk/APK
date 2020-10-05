
#include <iostream>
#include <fstream>
using namespace std;

#include "LogFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogFile::LogFile()
{
	state_ = es_NOT_INITIALIZED;
	cout << "Creating LogFile Object" << endl;
}

LogFile::LogFile( const string& logfilename )
{
	init( logfilename );
}

LogFile::~LogFile()
{
	cout << "Deleting LogFile Object" << endl;
}

bool LogFile::init( const string& logfilename )
{
	logfilename_ = logfilename;
	return internalWrite( "" );
}

bool LogFile::write( const string& data )
{
	return internalWrite( data+"\n" );
}

bool LogFile::internalWrite( const string& data )
{
	if( logfilename_ == "" )
	{
		state_ = es_FILENAME_IS_EMPTY;
		return false;
	}
	std::ofstream out;
	out.open( logfilename_.c_str(), std::ofstream::out | std::ofstream::app);
	if( out )
		out << data;
	else
	{
		state_ = es_COULD_NOT_OPEN_FILE;
		cerr << "Cannot open file " << logfilename_ << endl;
		return false;
	}
	state_ = es_OK;
	return true;
}

