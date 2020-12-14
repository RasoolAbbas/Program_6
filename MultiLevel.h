#pragma once
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#include "PrinterQueue.h"

using namespace std;

class MultiLevelQueue {
private:
	vector<PrinterQueue*> mlVector;
	queue<PrinterQueue*> mlQueue;

	vector<PrinterQueue*> studentSubset;
	vector<PrinterQueue*> facultySubset;
	vector<PrinterQueue*> adminSubset;

public:
	MultiLevelQueue( ) {
		// Reading in the data file
		ifstream inputStream;

		inputStream.open( "Program6Data.txt" );

		while ( inputStream.good( ) && !inputStream.eof( ) )
		{
			int timeQueued, numPages;
			char category;

			inputStream >> timeQueued >> category >> numPages;

			this->mlVector.push_back( new PrinterQueue( timeQueued, category, numPages ) );
		}

		inputStream.close( );

		// Begin sorting of the multi level queue
		// We are going to first extract subsets of each category..

		vector<PrinterQueue*> facultySubset;
		vector<PrinterQueue*> adminSubset;
		vector<PrinterQueue*> studentSubset;

		// Creating faculty subset
		for ( size_t i = 0; i < this->mlVector.size(); ++i ) 
		{
			if ( this->mlVector.at( i )->getPrintCategory( ) == 'F' ) {
				facultySubset.push_back( this->mlVector.at( i ) );
			}
		}

		// Sorting faculty subset with FIFO
		for ( size_t i = 0; i < facultySubset.size( ); ++i )
		{
			for ( size_t c = 0; c < facultySubset.size( ); ++c )
			{
				if ( facultySubset.at( i )->getTimeEntered( ) < facultySubset.at( c )->getTimeEntered() ) {
					// Switch them around
					PrinterQueue* tempPtr = facultySubset.at( i );

					facultySubset.at( i ) = facultySubset.at( c );
					facultySubset.at( c ) = tempPtr;
				}
			}
		}

		// Creating administration subset
		for ( size_t i = 0; i < this->mlVector.size( ); ++i )
		{
			if ( this->mlVector.at( i )->getPrintCategory( ) == 'A' ) {
				adminSubset.push_back( this->mlVector.at( i ) );
			}
		}

		// Sorting administration subset with FIFO
		for ( size_t i = 0; i < adminSubset.size( ); ++i )
		{
			for ( size_t c = 0; c < adminSubset.size( ); ++c )
			{
				if ( adminSubset.at( i )->getTimeEntered( ) < adminSubset.at( c )->getTimeEntered( ) ) {
					// Switch them around
					PrinterQueue* tempPtr = adminSubset.at( i );

					adminSubset.at( i ) = adminSubset.at( c );
					adminSubset.at( c ) = tempPtr;
				}
			}
		}

		// Creating student subset
		for ( size_t i = 0; i < this->mlVector.size( ); ++i )
		{
			if ( this->mlVector.at( i )->getPrintCategory( ) == 'S' ) {
				studentSubset.push_back( this->mlVector.at( i ) );
			}
		}

		// Sorting student subset with FIFO
		for ( size_t i = 0; i < studentSubset.size( ); ++i )
		{
			for ( size_t c = 0; c < studentSubset.size( ); ++c )
			{
				if ( studentSubset.at( i )->getTimeEntered( ) < studentSubset.at( c )->getTimeEntered( ) ) {
					// Switch them around
					PrinterQueue* tempPtr = studentSubset.at( i );

					studentSubset.at( i ) = studentSubset.at( c );
					studentSubset.at( c ) = tempPtr;
				}
			}
		}

		this->facultySubset = facultySubset;
		this->studentSubset = studentSubset;
		this->adminSubset = adminSubset;

	}

	~MultiLevelQueue( ) { }

	vector<PrinterQueue*> getVector( )		  { return this->mlVector;		}

	vector<PrinterQueue*>& getAdminSubset( )   { return this->adminSubset;   }
	vector<PrinterQueue*>& getStudentSubset( ) { return this->studentSubset; }
	vector<PrinterQueue*>& getFacultySubset( ) { return this->facultySubset; }


	void printVector( ) {
		for ( size_t i = 0; i < this->mlVector.size( ); ++i ) {
			this->mlVector.at( i )->print( );
		}
	}
};
