//
// Created by Alexander Dovzhikov on 3/5/20.
//

#ifndef XLC_REMOTE_TRANSPORT_H
#define XLC_REMOTE_TRANSPORT_H

namespace xlc {
  class remote_transport {
  public:
      virtual void open() = 0;
      virtual void close() = 0;
      [[nodiscard]] virtual bool isOpen() const = 0;
  };
}

#endif //XLC_REMOTE_TRANSPORT_H
