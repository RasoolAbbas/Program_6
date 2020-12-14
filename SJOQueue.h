#pragma once
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#include "PrinterQueue.h"

using namespace std;

class SJOQueue {
private:
	vector<PrinterQueue*> sjoVector;

public:
	SJOQueue( ) {
		// Reading in the data file
		ifstream inputStream;

		inputStream.open( "Program6Data.txt" );

		while ( inputStream.good( ) && !inputStream.eof( ) )
		{
			int timeQueued, numPages;
			char category;

			inputStream >> timeQueued >> category >> numPages;

			this->sjoVector.push_back( new PrinterQueue( timeQueued, category, numPages ) );
		}

		inputStream.close( );

		// The sort algo based upon the page size
		for ( size_t i = 0; i < sjoVector.size( ); ++i ) {
			for ( size_t c = 0; c < sjoVector.size( ); ++c) {
				
				if ( sjoVector.at( i )->getPageSize( ) < sjoVector.at( c )->getPageSize( ) ) {
					// Switch them around
					PrinterQueue* tempPtr = sjoVector.at( i );

					sjoVector.at( i ) = sjoVector.at( c );
					sjoVector.at( c ) = tempPtr;
				}
			}
		}
	}

	~SJOQueue( ) { }

	vector<PrinterQueue*> getVector( ) { return this->sjoVector; }

	void printQueue( ) {
		for ( size_t i = 0; i < this->sjoVector.size(); ++i )
		{
			this->sjoVector.at(i)->print( );
		}
	}
};
