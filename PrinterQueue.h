#pragma once

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class PrinterQueue {
private:
	int timeEntered;
	char printCategory;
	int pageSize;

public:

	bool operator<( const PrinterQueue& rhs )
	{
		return ( this->pageSize < rhs.pageSize );
	}

	// Constructor and destructor
	PrinterQueue(int timeEntered, char printCategory, int pageSize ) {
		this->timeEntered = timeEntered;
		this->printCategory = printCategory;
		this->pageSize = pageSize;
	}

	~PrinterQueue( ) { }

	// Setters and getters
	int getTimeEntered( ) { return this->timeEntered; }
	char getPrintCategory( ) { return this->printCategory; }
	int getPageSize( ) { return this->pageSize; }

	void setTimeEntered( int time ) { this->timeEntered = time; }
	void setPrintCategory( char newCategory ) { this->printCategory = newCategory; }
	void setPageSize( int newPageSize ) { this->pageSize = newPageSize; }

	void print( ) {
		cout << "Time entered: " << this->timeEntered << endl;
		cout << "Type of job: " << this->printCategory << endl;
		cout << "Number of pages: " << this->pageSize << endl;
	}
};