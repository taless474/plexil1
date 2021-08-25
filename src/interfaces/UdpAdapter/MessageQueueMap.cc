/* Copyright (c) 2006-2021, Universities Space Research Association (USRA).
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Universities Space Research Association nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * MessageQueueMap.cc
 *
 *  Created on: Feb 19, 2010
 *      Author: jhogins
 */

#include "MessageQueueMap.hh"

#include "AdapterExecInterface.hh"
#include "CommandHandle.hh"
#include "Debug.hh"

#include <utility>

namespace PLEXIL 
{

  MessageQueueMap::MessageQueueMap(AdapterExecInterface& execInterface, bool allowDuplicateMessages) :
    m_mutex(),
    m_execInterface(execInterface),
    m_allowDuplicateMessages(allowDuplicateMessages) {
  }

  MessageQueueMap::~MessageQueueMap() {
    // Clean up the map
    while (!m_map.empty()) {
      PairingQueue *pq = m_map.begin()->second;
      m_map.erase(m_map.begin());
      delete pq;
    }
  }

  void MessageQueueMap::addRecipient(const std::string& message, Command *cmd) {
    debugMsg("MessageQueueMap:addRecipient", " entered for \"" << message.c_str() << "\"");
    ThreadMutexGuard guard(m_mutex);
    PairingQueue* que = getQueue(message);
    que->m_recipientQueue.push_back(cmd);
    updateQueue(que);
    debugMsg("MessageQueueMap:addRecipient", " recipient for message \"" << que->m_name.c_str() << "\" added");
  }

  /**
   * @brief Removes all instances of the given recipient waiting on the given message string.
   */
  void MessageQueueMap::removeRecipient(const std::string& message, Command const *cmd) {
    ThreadMutexGuard guard(m_mutex);
    PairingQueue* pq = getQueue(message);
    for (RecipientQueue::iterator it = pq->m_recipientQueue.begin();
         it != pq->m_recipientQueue.end();
         it++) {
      if (*it == cmd) {
        debugMsg("MessageQueueMap:removeRecipient", " Removing recipient for \"" << pq->m_name.c_str() << "\"");
        it = pq->m_recipientQueue.erase(it);
        //this increments the iterator, so check for the end immediately
        if (it == pq->m_recipientQueue.end())
          break;
      }
    }
  }

  void MessageQueueMap::clearRecipientsForMessage(const std::string& message) {
    ThreadMutexGuard guard(m_mutex);
    PairingQueue* pq = getQueue(message);
    pq->m_recipientQueue.clear();
  }

  void MessageQueueMap::addMessage(const std::string& message) {
    ThreadMutexGuard guard(m_mutex);
    PairingQueue* pq = getQueue(message);
    if (!pq->m_allowDuplicateMessages)
      pq->m_messageQueue.clear();
    pq->m_messageQueue.push_back(message);
    updateQueue(pq);
    debugMsg("MessageQueueMap:addMessage", " Message \"" << pq->m_name.c_str() << "\" added");
  }

  void MessageQueueMap::addMessage(const std::string& message, const Value& param) {
    ThreadMutexGuard guard(m_mutex);
    PairingQueue* pq = getQueue(message);
    if (!pq->m_allowDuplicateMessages)
      pq->m_messageQueue.clear();
    pq->m_messageQueue.push_back(param);
    updateQueue( pq);
    debugMsg("MessageQueueMap:addMessage",
             " Message \"" << pq->m_name.c_str() << "\" added, value = \"" << param << "\"");
  }

  void MessageQueueMap::setAllowDuplicateMessages(bool flag) {
    ThreadMutexGuard guard(m_mutex);
    for (std::map<std::string, PairingQueue*>::iterator it = m_map.begin(); it != m_map.end(); it++) {
      MessageQueue mq = it->second->m_messageQueue;
      //if setting flag from true to false, ensure all queues have at most one message
      if (m_allowDuplicateMessages && !flag && mq.size() > 1) {
        MessageQueue::iterator begin = mq.begin();
        MessageQueue::iterator end = mq.end();
        end-= 2;
        mq.erase(begin, end);
      }
      it->second->m_allowDuplicateMessages = flag;
    }
    m_allowDuplicateMessages = flag;
  }

  MessageQueueMap::PairingQueue * MessageQueueMap::getQueue(const std::string& message) {
    PairingQueue* result;
    std::map<std::string, PairingQueue*>::iterator it = m_map.find(message);
    if (m_map.end() == it) {
      debugMsg("MessageQueueMap:getQueue", " creating new queue with name \"" << message.c_str() << "\"");
      result = new PairingQueue(message, m_allowDuplicateMessages);
      m_map.insert(it, std::pair<std::string, PairingQueue*> (message, result));
    } else {
      result = it->second;
    }
    return result;
  }

  void MessageQueueMap::updateQueue(PairingQueue* queue) {
    debugMsg("MessageQueueMap:updateQueue", " entered");
    MessageQueue& mq = queue->m_messageQueue;
    RecipientQueue& rq = queue->m_recipientQueue;
    MessageQueue::iterator mqIter = mq.begin();
    RecipientQueue::iterator rqIter = rq.begin();
    bool valChanged = !mq.empty() && !rq.empty();
    while (!(mqIter == mq.end()) && !(rqIter == rq.end())) {
      debugMsg("MessageQueueMap:updateQueue", " returning value");
      m_execInterface.handleCommandReturn(*rqIter, *mqIter);
      m_execInterface.handleCommandAck(*rqIter, COMMAND_SUCCESS);
      rqIter = rq.erase(rqIter);
      mqIter = mq.erase(mqIter);
    }
    if (valChanged) {
      debugMsg("MessageQueueMap:updateQueue", " Message \"" << queue->m_name.c_str() << "\" paired and sent");
      m_execInterface.notifyOfExternalEvent();
    }
  }

}
