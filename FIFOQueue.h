#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#include "PrinterQueue.h"

using namespace std;

class FIFOQueue {
private:
	queue<PrinterQueue*> fifoQueue;

public:
	FIFOQueue( ) {
		// Reading in the data file
		ifstream inputStream;

		inputStream.open( "Program6Data.txt" );

		while ( inputStream.good( ) && !inputStream.eof( ) )
		{
			int timeQueued, numPages;
			char category;

			inputStream >> timeQueued >> category >> numPages;

			this->fifoQueue.push( new PrinterQueue( timeQueued, category, numPages ) );
		}

		inputStream.close( );
	}

	~FIFOQueue( ) { }

	queue<PrinterQueue*> getQueue( ) { return this->fifoQueue; }

	void printQueue( ) {
		auto queueCopy = this->fifoQueue;

		while ( queueCopy.size( ) > 0 )
		{
			queueCopy.front( )->print( );

			queueCopy.pop( );
		}
	}

	PrinterQueue* getNext( ) { return this->fifoQueue.front( ); }
};
