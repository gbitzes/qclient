//------------------------------------------------------------------------------
// File: WriterThread.hh
// Author: Georgios Bitzes - CERN
//------------------------------------------------------------------------------

/************************************************************************
 * qclient - A simple redis C++ client with support for redirects       *
 * Copyright (C) 2016 CERN/Switzerland                                  *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#ifndef QCLIENT_WRITER_THREAD_H
#define QCLIENT_WRITER_THREAD_H

#include <hiredis/hiredis.h>
#include "qclient/AssistedThread.hh"
#include "qclient/EventFD.hh"
#include "FutureHandler.hh"
#include "NetworkStream.hh"
#include "BackpressureApplier.hh"
#include "CallbackExecutorThread.hh"
#include "StagedRequest.hh"
#include "RequestStager.hh"
#include "qclient/ThreadSafeQueue.hh"
#include "qclient/Options.hh"
#include "qclient/EncodedRequest.hh"
#include <deque>
#include <future>

namespace qclient {

using redisReplyPtr = std::shared_ptr<redisReply>;

class WriterThread {
public:
  WriterThread(BackpressureStrategy backpressure, EventFD &shutdownFD);
  ~WriterThread();

  void activate(NetworkStream *stream);
  void stageHandshake(EncodedRequest &&req);
  void handshakeCompleted();
  void deactivate();

  void stage(QCallback *callback, EncodedRequest &&req);
  std::future<redisReplyPtr> stage(EncodedRequest &&req, bool bypassBackpressure = false);
#if HAVE_FOLLY == 1
  folly::Future<redisReplyPtr> follyStage(EncodedRequest &&req);
#endif

  void satisfy(redisReplyPtr &&reply);

  void eventLoop(NetworkStream *stream, ThreadAssistant &assistant);
  void clearPending();

private:
  RequestStager requestStager;

  EventFD &shutdownEventFD;
  AssistedThread thread;

  std::atomic<bool> inHandshake { true };
  std::unique_ptr<StagedRequest> handshake;

  std::mutex handshakeMtx;
  std::condition_variable handshakeCV;
};

}

#endif
