#include "stream_reassembler.hh"

#include <iostream>

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in
// `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity),
      //_capacity(capacity),
      unassembled_segments(),
      last_assembled_byte(-1),
      eof_received(false),
      unassembled_bytes_count(0) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index,
                                       const bool eof) {
  string segmentData;
  size_t bytesAdded = 0;

  const Segment new_segment(index, data);

  // Pop top_segment off unassembled_segments
  // if new_data contains it
  while (!empty() && new_segment.contains(unassembled_segments.top())) {
    unassembled_bytes_count -= unassembled_segments.top().data.size();
    unassembled_segments.pop();
  }
  if (empty() || !unassembled_segments.top().contains(new_segment)) {
    unassembled_segments.push(new_segment);
    unassembled_bytes_count += data.size();
  }

  Segment topSegment = unassembled_segments.top();

  while (topSegment.index <= last_assembled_byte + 1) {
    bytesAdded = 0;

    if (topSegment.index <= last_assembled_byte) {
      size_t nonOverlapStart = last_assembled_byte - topSegment.index + 1;
      size_t nonOverlapLength = topSegment.data.size() - nonOverlapStart;
      if (nonOverlapStart < topSegment.data.size()) {
        segmentData = topSegment.data.substr(nonOverlapStart, nonOverlapLength);
        _output.write(segmentData);
        bytesAdded += segmentData.size();
        unassembled_bytes_count -= topSegment.data.size();
      } else {
        bytesAdded += 0;
        unassembled_bytes_count -= topSegment.data.size();
      }
    } else if (topSegment.index == last_assembled_byte + 1) {
      _output.write(topSegment.data);
      bytesAdded += topSegment.data.size();
      unassembled_bytes_count -= topSegment.data.size();
    }

    last_assembled_byte += bytesAdded;
    unassembled_segments.pop();
    if (!unassembled_segments.empty()) {
      topSegment = unassembled_segments.top();
    } else {
      break;
    }
  }

  eof_received = eof || eof_received;
  if (eof_received && unassembled_segments.size() == 0) {
    _output.end_input();
  }
}

size_t StreamReassembler::unassembled_bytes() const {
  return unassembled_bytes_count;
}

bool StreamReassembler::empty() const { return unassembled_segments.empty(); }

bool operator<(const Segment &A, const Segment &B) {
  if (A.index > B.index)
    return true;
  else if (A.index == B.index)
    return A.data.size() < B.data.size();
  else
    return false;
}

bool Segment::contains(const Segment &segment) const {
  return (segment.index >= index) &&
         (segment.last_index() <= last_index());
}