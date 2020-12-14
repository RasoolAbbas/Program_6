#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

#include "FIFOQueue.h"
#include "PrinterQueue.h"
#include "SJOQueue.h"
#include "MultiLevel.h"

using namespace std;

// This function helps to visualize the current time of day
// in a nice format
void printCurrentTime( int timeInMinutes )
{
	int hour = timeInMinutes / 60;
	int minutes = timeInMinutes % 60;
	
	string dayTime = ( hour >= 12 ) ? "PM" : "AM";

	if ( minutes >= 60 ) {
		hour++;
		minutes -= 60;
	}

	if ( hour > 12 ) {
		hour -= 12;
	}

	cout << "Time is " << hour << ":" << setprecision(2) << minutes << " " << dayTime << endl;
}

template <class T>
vector<PrinterQueue*> determineQueuesPending( int curMinute, T customQueue )
{
	// First we want to add all of the printer requests into a temporary vector
	vector<PrinterQueue*> tempPrints;
	vector<PrinterQueue*> toReturn;

	// Adding
	while ( customQueue->size( ) > 0 ) {
		tempPrints.push_back( customQueue->front( ) );

		customQueue->pop( );
	}

	// Doing time calculation 
	for ( size_t i = 0; i < tempPrints.size( ); ++i ) {
		if ( tempPrints.at( i )->getTimeEntered( ) == curMinute ) {
			toReturn.push_back( tempPrints.at( i ) );
		}
	}

	// Returning
	return toReturn;
}

queue<PrinterQueue*> vectorToQueue( vector<PrinterQueue*> vec ) {
	queue<PrinterQueue*> q;

	for ( size_t i = 0; i < vec.size( ); ++i ) {
		q.push( vec.at( i ) );
	}

	return q;
}

int main()
{
	int timeInMinutes = 540; // 9 AM

	int totalWaitTime = 0;
	int numOfJobsHandeled = 0;
	int serviceTime = 0;
	int pagesLeftToPrint = 0;
	int timePrinterNextAvailable = 0;
	int longestWaitTime = 0;

	int studentWait = 0, facultyWait = 0, adminWait = 0;
	int studentJobsCompleted = 0, facultyJobsCompleted = 0, adminJobsCompleted = 0;
	double studentAvgWait = 0, facultyAvgWait = 0, adminAvgWait = 0;

	double averageWait = 0;

	bool printerAvailable = true;

	FIFOQueue fifoQueue;
	queue<PrinterQueue*> curQueue = fifoQueue.getQueue( );
	queue<PrinterQueue*> jobsPendingQueue;

	cout << "Beginning First In First Out (FIFO) queue simulation..." << endl;
	
	cout << endl;
	cout << endl;

	// Go until 5 PM
	while ( timeInMinutes < 1020 ) {
		
		// Determining whether the printer is available
		printerAvailable = ( timePrinterNextAvailable <= ( timeInMinutes - 540 ) ) ? true : false;
		
		// Determining whether we can add to the jobs pending queue
		// This is determined by looking at if the jobs are arriving at this current
		// minute and if so, add each one to the job pending queue in order
		if ( curQueue.size( ) > 0 && curQueue.front( )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
			while ( curQueue.size( ) > 0 && curQueue.front( )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
				jobsPendingQueue.push( curQueue.front( ) );

				curQueue.pop( );
			}
		}

		// This is checking if we have any current
		// jobs pending, if we do then we can
		// check if the printer is busy or not
		// and go ahead and print while taking down
		// some stats
		if ( jobsPendingQueue.size( ) > 0 ) {
			if ( printerAvailable ) {
				PrinterQueue* curJob = jobsPendingQueue.front( );

				cout << "Current job has been waiting for " << ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) ) << " minutes." << endl;

				// Calculating the total wait time
				totalWaitTime += ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );

				// Calculating the longest wait time
				if ( longestWaitTime < ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) ) ) {
					longestWaitTime = ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );
				}

				// Calculating when the printer is next available
				serviceTime = (static_cast< double >( curJob->getPageSize( ) ) / 150.0) + 1;

				// Calculating when the printer will be next available 
				timePrinterNextAvailable = ( timeInMinutes - 540 ) + serviceTime;

				// Counting whos served
				if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'F' ) {
					facultyWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					facultyJobsCompleted++;
				} else if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'S' ) {
					studentWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					studentJobsCompleted++;
				} else {
					adminWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					adminJobsCompleted++;
				}

				jobsPendingQueue.pop( );
				numOfJobsHandeled++;
			}
		}

		// Printing the time of day
		printCurrentTime( timeInMinutes );

		// Incrementing the minutes
		timeInMinutes++; 
	}

	averageWait = static_cast< double >( totalWaitTime ) / static_cast< double >( numOfJobsHandeled );

	studentAvgWait = static_cast< double >( studentWait ) / static_cast< double >( studentJobsCompleted );
	facultyAvgWait = static_cast< double >( facultyWait ) / static_cast< double >( facultyJobsCompleted );
	adminAvgWait   = static_cast< double >( adminWait ) / static_cast< double >( adminJobsCompleted );

	// Printing out the statistics
	cout << endl;
	cout << "=============( FIFO Statistics )=================" << endl;
	cout << endl;

	cout << "Number of jobs handeled: " << numOfJobsHandeled << endl;
	
	cout << endl;

	cout << "Total wait time: " << totalWaitTime << endl;

	cout << endl;

	cout << "Average wait time: " << fixed << setprecision(3) << averageWait << endl;

	cout << endl;

	cout << "Longest wait time: " << longestWaitTime << endl;

	cout << endl;

	cout << "Average student wait time: " << studentAvgWait << endl;

	cout << endl;

	cout << "Average faculty wait time: " << facultyAvgWait << endl;

	cout << endl;

	cout << "Average administrator wait time: " << adminAvgWait << endl;

	cout << endl;
	cout << "=============( FIFO Statistics )=================" << endl;
	cout << endl;

	cout << endl;

	timeInMinutes = 540; // 9 AM

	totalWaitTime = 0;
	numOfJobsHandeled = 0;
	serviceTime = 0;
	pagesLeftToPrint = 0;
	timePrinterNextAvailable = 0;
	longestWaitTime = 0;

	studentWait = 0, facultyWait = 0, adminWait = 0;
	studentJobsCompleted = 0, facultyJobsCompleted = 0, adminJobsCompleted = 0;
	studentAvgWait = 0, facultyAvgWait = 0, adminAvgWait = 0;

	averageWait = 0;

	printerAvailable = true;

	SJOQueue sjoQueue;
	vector<PrinterQueue*> curVector = sjoQueue.getVector( );

	jobsPendingQueue = {};

	cout << "Beginning Shortest Job Out (SJO) queue simulation..." << endl;

	cout << endl;
	cout << endl;

	// Go until 5 PM
	while ( timeInMinutes < 1020 ) {

		// Determining whether the printer is available
		printerAvailable = ( timePrinterNextAvailable <= ( timeInMinutes - 540 ) ) ? true : false;

		// Determining whether we can add to the jobs pending queue
		// This determination is different based upon the queue type,
		// for SJO we just add everything to the jobs pending queue
		if ( curVector.size( ) > 0 ) {

			// Iteration to be able to delete elements from 
			// vectors as we need
			vector<PrinterQueue*>::iterator iter;
			for ( iter = curVector.begin(); iter != curVector.end(); ) {
				if ( ( *iter )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
					jobsPendingQueue.push( *iter );
					
					iter = curVector.erase( iter );
				} else {
					iter++;
				}
			}

		}
		

		// This is checking if we have any current
		// jobs pending, if we do then we can
		// check if the printer is busy or not
		// and go ahead and print while taking down
		// some stats
		if ( jobsPendingQueue.size( ) > 0 ) {
			if ( printerAvailable ) {
				PrinterQueue* curJob = jobsPendingQueue.front( );

				// Calculating the total wait time
				totalWaitTime += ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );

				// Calculating the longest wait time
				if ( longestWaitTime < ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) ) ) {
					longestWaitTime = ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );
				}

				// Calculating when the printer is next available
				serviceTime = ( static_cast< double >( curJob->getPageSize( ) ) / 150.0 ) + 1;

				// Calculating when the printer will be next available 
				timePrinterNextAvailable = ( timeInMinutes - 540 ) + serviceTime;

				// Counting whos served
				if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'F' ) {
					facultyWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					facultyJobsCompleted++;
				}
				else if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'S' ) {
					studentWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					studentJobsCompleted++;
				}
				else {
					adminWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					adminJobsCompleted++;
				}

				jobsPendingQueue.pop( );
				numOfJobsHandeled++;
			}
		}

		// Printing the time of day
		printCurrentTime( timeInMinutes );

		// Incrementing the minutes
		timeInMinutes++;
	}

	averageWait = static_cast< double >( totalWaitTime ) / static_cast< double >( numOfJobsHandeled );

	studentAvgWait = static_cast< double >( studentWait ) / static_cast< double >( studentJobsCompleted );
	facultyAvgWait = static_cast< double >( facultyWait ) / static_cast< double >( facultyJobsCompleted );
	adminAvgWait = static_cast< double >( adminWait ) / static_cast< double >( adminJobsCompleted );

	// Printing out the statistics
	cout << endl;
	cout << "=============( SJO Statistics )=================" << endl;
	cout << endl;

	cout << "Number of jobs handeled: " << numOfJobsHandeled << endl;

	cout << endl;

	cout << "Total wait time: " << totalWaitTime << endl;

	cout << endl;

	cout << "Average wait time: " << averageWait << endl;

	cout << endl;

	cout << "Longest wait time: " << longestWaitTime << endl;

	cout << endl;

	cout << "Average student wait time: " << studentAvgWait << endl;

	cout << endl;

	cout << "Average faculty wait time: " << facultyAvgWait << endl;

	cout << endl;

	cout << "Average administrator wait time: " << adminAvgWait << endl;

	cout << endl;
	cout << "=============( SJO Statistics )=================" << endl;
	cout << endl;

	cout << endl;
	
	timeInMinutes = 540; // 9 AM

	totalWaitTime = 0;
	numOfJobsHandeled = 0;
	serviceTime = 0;
	pagesLeftToPrint = 0;
	timePrinterNextAvailable = 0;
	longestWaitTime = 0;

	studentWait = 0, facultyWait = 0, adminWait = 0;
	studentJobsCompleted = 0, facultyJobsCompleted = 0, adminJobsCompleted = 0;
	studentAvgWait = 0, facultyAvgWait = 0, adminAvgWait = 0;

	averageWait = 0;

	printerAvailable = true;

	MultiLevelQueue mlQueue;

	jobsPendingQueue = {};

	cout << "Beginning Multi Level (ML) queue simulation..." << endl;

	cout << endl;
	cout << endl;

	// Go until 5 PM
	while ( timeInMinutes < 1020 ) {

		// Determining whether the printer is available
		printerAvailable = ( timePrinterNextAvailable <= ( timeInMinutes - 540 ) ) ? true : false;

		// For the multi level queue,
		// we first want to check if the overall queue
		// has anything in them
		if ( ( mlQueue.getStudentSubset( ).size( ) + mlQueue.getAdminSubset( ).size( ) + mlQueue.getFacultySubset( ).size( ) ) > 0 ) {

			// Getting administrators' jobs
			vector<PrinterQueue*>::iterator iter;
			for ( iter = mlQueue.getAdminSubset( ).begin( ); iter != mlQueue.getAdminSubset( ).end( ); ) {
				if ( ( *iter )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
					jobsPendingQueue.push( *iter );

					iter = mlQueue.getAdminSubset( ).erase( iter );
				}
				else {
					iter++;
				}
			}

			// Getting faculty jobs
			vector<PrinterQueue*>::iterator iter_;
			for ( iter_ = mlQueue.getFacultySubset( ).begin( ); iter_ != mlQueue.getFacultySubset( ).end( ); ) {
				if ( ( *iter_ )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
					jobsPendingQueue.push( *iter_ );

					iter_ = mlQueue.getFacultySubset( ).erase( iter_ );
				}
				else {
					iter_++;
				}
			}
			
			// Finally getting student jobs even though we pay 
			// the salaries of the persons above us lol
			vector<PrinterQueue*>::iterator iter__;
			for ( iter__ = mlQueue.getStudentSubset( ).begin( ); iter__ != mlQueue.getStudentSubset( ).end( ); ) {
				if ( ( *iter__ )->getTimeEntered( ) == ( timeInMinutes - 540 ) ) {
					jobsPendingQueue.push( *iter__ );

					iter__ = mlQueue.getStudentSubset( ).erase( iter__ );
				}
				else {
					iter__++;
				}
			}
		}


		// This is checking if we have any current
		// jobs pending, if we do then we can
		// check if the printer is busy or not
		// and go ahead and print while taking down
		// some stats
		if ( jobsPendingQueue.size( ) > 0 ) {
			if ( printerAvailable ) {
				PrinterQueue* curJob = jobsPendingQueue.front( );

				// Calculating the total wait time
				totalWaitTime += ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );

				// Calculating the longest wait time
				if ( longestWaitTime < ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) ) ) {
					longestWaitTime = ( ( timeInMinutes - 540 ) - curJob->getTimeEntered( ) );
				}

				// Calculating when the printer is next available
				serviceTime = ( static_cast< double >( curJob->getPageSize( ) ) / 150.0 ) + 1;

				// Calculating when the printer will be next available 
				timePrinterNextAvailable = ( timeInMinutes - 540 ) + serviceTime;

				// Counting whos served
				if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'F' ) {
					facultyWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					facultyJobsCompleted++;
				}
				else if ( jobsPendingQueue.front( )->getPrintCategory( ) == 'S' ) {
					studentWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					studentJobsCompleted++;
				}
				else {
					adminWait += ( timeInMinutes - 540 ) - curJob->getTimeEntered( );
					adminJobsCompleted++;
				}

				jobsPendingQueue.pop( );
				numOfJobsHandeled++;
			}
		}

		// Printing the time of day
		printCurrentTime( timeInMinutes );

		// Incrementing the minutes
		timeInMinutes++;
	}

	averageWait = static_cast< double >( totalWaitTime ) / static_cast< double >( numOfJobsHandeled );

	studentAvgWait = static_cast< double >( studentWait ) / static_cast< double >( studentJobsCompleted );
	facultyAvgWait = static_cast< double >( facultyWait ) / static_cast< double >( facultyJobsCompleted );
	adminAvgWait = static_cast< double >( adminWait ) / static_cast< double >( adminJobsCompleted );

	// Printing out the statistics
	cout << endl;
	cout << "=============( ML Statistics )=================" << endl;
	cout << endl;

	cout << "Number of jobs handeled: " << numOfJobsHandeled << endl;

	cout << endl;

	cout << "Total wait time: " << totalWaitTime << endl;

	cout << endl;

	cout << "Average wait time: " << averageWait << endl;

	cout << endl;

	cout << "Longest wait time: " << longestWaitTime << endl;

	cout << endl;

	cout << "Average student wait time: " << studentAvgWait << endl;

	cout << endl;

	cout << "Average faculty wait time: " << facultyAvgWait << endl;

	cout << endl;

	cout << "Average administrator wait time: " << adminAvgWait << endl;

	cout << endl;
	cout << "=============( ML Statistics )=================" << endl;
	cout << endl;

	system( "pause" );
}