//------------------------------------------------------------------------------
// File: BaseSubscriber.hh
// Author: Georgios Bitzes - CERN
//------------------------------------------------------------------------------

/************************************************************************
 * qclient - A simple redis C++ client with support for redirects       *
 * Copyright (C) 2018 CERN/Switzerland                                  *
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

#ifndef QCLIENT_BASE_SUBSCRIBER_HH
#define QCLIENT_BASE_SUBSCRIBER_HH

#include "Members.hh"
#include "Options.hh"

namespace qclient {

class MessageListener;

//------------------------------------------------------------------------------
//! This is a low-level class, which models closely a redis connection in
//! subscription mode - don't expect a comfortable API.
//!
//! This means we can subscribe into channels and such, while all incoming
//! messages go through a single listener object. We make no effort to filter
//! out the messages according to channel and dispatch accordingly, that's a
//! job for a higher level class.
//------------------------------------------------------------------------------
class BaseSubscriber {
public:
  //----------------------------------------------------------------------------
  //! Constructor taking a list of members for the cluster, the listener, and
  //! options object.
  //!
  //! If you construct a BaseSubscriber with a nullptr listener, we're calling
  //! std::abort. :)
  //----------------------------------------------------------------------------
  BaseSubscriber(const Members &members,
    std::shared_ptr<MessageListener> listener,
    SubscriptionOptions &&options);

private:
  Members members;
  std::shared_ptr<MessageListener> listener;
  SubscriptionOptions options;
};

}

#endif