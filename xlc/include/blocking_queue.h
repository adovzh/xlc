//
// Created by Alexander Dovzhikov on 13/4/20.
//

#ifndef XLC_BLOCKING_QUEUE_H
#define XLC_BLOCKING_QUEUE_H

#include <cstdlib>
#include <number_utils.h>

namespace xlc {

  template <typename T>
  class blocking_queue
  {
      enum { DefaultCapacity = 16 };
  public:
      explicit blocking_queue(std::size_t capacity = DefaultCapacity);
      blocking_queue(const blocking_queue<T>&) = delete;
      blocking_queue(blocking_queue<T>&& that) = delete;
      ~blocking_queue();

      blocking_queue<T>& operator=(const blocking_queue<T>&) = delete;
      blocking_queue<T>& operator=(blocking_queue<T>&& that) = delete;

      [[nodiscard]] std::size_t capacity() const { return capacity_; }

      bool poll(T& elem, long timeout);
      bool offer(const T& elem, long timeout);
      bool offer(T&& elem, long timeout);
  private:
      T* buffer_;
      std::size_t capacity_;
      std::size_t mask;
      std::size_t head_, tail_;
      std::mutex mx_head_, mx_tail_;
      std::condition_variable cv_head_, cv_tail_;
  };

  template<typename T>
  blocking_queue<T>::blocking_queue(std::size_t capacity) :
          capacity_(number::npo2(capacity)),
          head_(0), tail_(0)
  {
      buffer_ = new T[capacity_];
      mask = capacity_ - 1;
  }

  template<typename T>
  blocking_queue<T>::~blocking_queue()
  {
      delete [] buffer_;
  }

  template<typename T>
  bool blocking_queue<T>::poll(T& elem, long timeout)
  {
      // condition head <> tail
      bool result;
      std::unique_lock<std::mutex> head_lock(mx_head_);

      if ((result = cv_head_.wait_for(head_lock, std::chrono::milliseconds(timeout),
              [this] { return head_ != tail_; }))) {
          elem = std::move(buffer_[head_]);
          head_ = head_ + 1 & mask;
          cv_tail_.notify_one();
      }

      return result;
  }

  template<typename T>
  bool blocking_queue<T>::offer(const T& elem, long timeout)
  {
      bool result;
      std::unique_lock<std::mutex> tail_lock(mx_tail_);

      if ((result = cv_tail_.wait_for(tail_lock, std::chrono::milliseconds(timeout),
              [this] { return tail_ != (head_ - 1 & mask); }))) {
          buffer_[tail_] = elem;
          tail_ = tail_ + 1 & mask;
          cv_head_.notify_one();
      }

      return result;
  }

  template<typename T>
  bool blocking_queue<T>::offer(T&& elem, long timeout)
  {
      bool result;
      std::unique_lock<std::mutex> tail_lock(mx_tail_);

      if ((result = cv_tail_.wait_for(tail_lock, std::chrono::milliseconds(timeout),
              [this] { return tail_ != (head_ - 1 & mask); }))) {
          buffer_[tail_] = std::move(elem);
          tail_ = tail_ + 1 & mask;
          cv_head_.notify_one();
      }

      return result;
  }

} // namespace xlc

#endif //XLC_BLOCKING_QUEUE_H
