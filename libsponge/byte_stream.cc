#include "byte_stream.hh"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in
// `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t cap)
    : buffer(),
      capacity(cap),
      size(0),
      num_bytes_written(0),
      num_bytes_read(0),
      ended(false),
      eof_ind(false) {}

size_t ByteStream::write(const string &data) {
  int num_written = 0;
  // First check that input is not ended
  // and buffer is not full
  if (!(input_ended() || remaining_capacity() == 0)) {
    // Write to the buffer until end of string
    // or until buffer is full
    std::string::size_type i;
    for (i = 0; remaining_capacity() != 0 && i < data.size(); i++) {
      buffer.push_back(data[i]);
      num_bytes_written++;
    }
    num_written = i;
  } else {
    num_written = 0;
  }
  eof_ind = buffer_empty() && ended;
  return num_written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  string temp;
  for (size_t i = 0; i < min(buffer.size(), len); i++) {
    temp += buffer[i];
  }
  return temp;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  for (size_t i = 0; i < len; i++) {
    buffer.pop_front();
    num_bytes_read++;
  }
  eof_ind = buffer_empty() && ended;
}

void ByteStream::end_input() {
  eof_ind = buffer_empty();
  ended = true;
}

bool ByteStream::input_ended() const { return ended; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return eof_ind; }

size_t ByteStream::bytes_written() const { return num_bytes_written; }

size_t ByteStream::bytes_read() const { return num_bytes_read; }

size_t ByteStream::remaining_capacity() const {
  return capacity - buffer_size();
}
