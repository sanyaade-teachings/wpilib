/*
 * WriteManager.h
 *
 *  Created on: Sep 25, 2012
 *      Author: Mitchell Wills
 */

#ifndef WRITEMANAGER_H_
#define WRITEMANAGER_H_


class AbstractNetworkTableEntryStore;
class WriteManager;


#include "thread/PeriodicRunnable.h"
#include "OutgoingEntryReceiver.h"
#include "thread/NTThread.h"
#include "thread/NTThreadManager.h"
#include "FlushableOutgoingEntryReceiver.h"
#include "NetworkTableEntry.h"
#include <queue>
#include "Synchronized.h"



/**
 * A write manager is a {@link IncomingEntryReceiver} that buffers transactions and then and then dispatches them to a flushable transaction receiver that is periodically offered all queued transaction and then flushed
 * 
 * @author Mitchell
 *
 */
class WriteManager : public OutgoingEntryReceiver, public PeriodicRunnable{
private:
	const static size_t queueSize = 500;
	
	ReentrantSemaphore transactionsLock;
	
	NTThread* thread;
	
	FlushableOutgoingEntryReceiver& receiver;
	NTThreadManager& threadManager;
	AbstractNetworkTableEntryStore& entryStore;

	unsigned long keepAliveDelay;
	
	volatile std::queue<NetworkTableEntry*>* incomingAssignmentQueue;
	volatile std::queue<NetworkTableEntry*>* incomingUpdateQueue;
	volatile std::queue<NetworkTableEntry*>* outgoingAssignmentQueue;
	volatile std::queue<NetworkTableEntry*>* outgoingUpdateQueue;
	
	unsigned long lastWrite;


public:
	/**
	 * Create a new Write manager
	 * @param receiver
	 * @param threadManager
	 * @param transactionPool
	 * @param entryStore
	 */
	WriteManager(FlushableOutgoingEntryReceiver& receiver, NTThreadManager& threadManager, AbstractNetworkTableEntryStore& entryStore, unsigned long keepAliveDelay);
	virtual ~WriteManager();
	/**
	 * start the write thread
	 */
	void start();
	/**
	 * stop the write thread
	 */
	void stop();


	void offerOutgoingAssignment(NetworkTableEntry* entry);


	void offerOutgoingUpdate(NetworkTableEntry* entry);

	
	/**
	 * the periodic method that sends all buffered transactions
	 */
	void run();

};

#endif /* WRITEMANAGER_H_ */
