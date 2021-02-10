/////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

//test
#include <iostream>

// TODO
#include <functional>
#include <iostream>

//test
using namespace std;

enum disk_color { DISK_DARK, DISK_LIGHT};

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

    assert(light_count > 0);

    for (size_t i = 0; i < _colors.size(); i += 2) {
      _colors[i] = DISK_LIGHT;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
	  //for loop, i starts at 0, goes up to size - 1
	  //	if index % 2 == 0 then
	  //		if it's light, then
	  //			continue
	  //		else
	  //			return false
	  //		endif
	  //	else
	  //		if it's dark, then
	  //			continue
	  //		else
	  //			return false
	  //		endif
	  //	endif
	  //endfor
	  //return true
	for (unsigned i = 0; i < _colors.size(); i++){
		//check whether the iterator has passed the end of the array
		if (!is_index(i))
			break;
		if (i % 2 == 0)
			if (get(i) == DISK_LIGHT)
				continue;
			else
				return false;
		else
			if (get(i) == DISK_DARK)
				continue;
			else
				return false;
	}
	return true;
  }

  // Return true when this disk_state is fully sorted, with all dark disks
  // on the left (low indices) and all light disks on the right (high
  // indices).
  bool is_sorted() const {
	  //for loop, i starts at 0, goes up to size - 1
	  //	if index < size / 2 then
	  //		if it's dark then
	  //			continue
	  //		else
	  //			return false
	  //		endif
	  //	else
	  //		if it's light then
	  //			continue
	  //		else
	  //			return false
	  //		endif
	  //	endif
	  //endfor
	  //return true
	for (unsigned i = 0; i < _colors.size(); i++) {
		if (!is_index(i))
			break;
		else
			if (i < light_count())
				if (get(i) == DISK_DARK)
					continue;
				else
					return false;
			else
				if (get(i) == DISK_LIGHT)
					continue;
				else
					return false;
	}
	return true; 
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;
  
public:
  
  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }
  
  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }
  
  const disk_state& after() const {
    return _after;
  }
  
  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {
  // Pseudocode
  // even = 0 (1)
  // for int j = 0; j < n; j++ do <-- provided in guideline (n)
  // 	for int i = even; i < before.size-1; i+=2 do (2n/2 = n)
  // 		if left is white and right is black then (2)
  // 			switch left and right (5)
  // 		else
  // 			skip
  // 	endfor
  // 	if even = 0 then (1)
  // 		even = 1 (1)
  // 	else
  // 		even = 0 (1)
  // 	endif
  // endfor
  // endwhile
  // step count = 1 + n * ((n * 7) + 2)
  // 		= 1 + n * (7n + 2)
  // 		= 1 + 7n^2 + 2n
  // 		= 7n^2 + 2n + 1
  // time complexity = O(n^2) 
	int start = 0;
	int swap_count = 0;
	//make a copy of the input because the input cannot be changed
	disk_state after(before.light_count());
	for (unsigned j = 0; j < after.light_count(); j++) {
		//check up to -1 because it checks the element to the right of i
		for (unsigned i = start; i < after.total_count()-1; i+=2) {
			if (!after.is_index(i)) {
				break;
			}
			if (after.get(i) == DISK_LIGHT and after.get(i+1) == DISK_DARK) {
				after.swap(i);
				swap_count += 1;
			}
			else
				continue;
		}
		//alternate between the first disk and the second disk on every run
		if (start == 0)
			start = 1;
		else
			start = 0;
	}
			
	return sorted_disks(after, swap_count);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // Pseudocode
  // position = 0
  // for int j = 0, j < n / 2, j++ do (n/2)
  // 	for position, position < before.size - 1, position++ do (2n - 1)
  // 		if left is white and right is black then (2) //"and" counts as an operator
  // 				switch left and right (5, swap function)
  // 		else
  // 			skip
  // 		endif
  // 	endfor
  // 	for position, position > 0, position-- do (2n - 1)
  //		if left is white and right is black then (2)
  //			switch left and right (5)
  //		else
  //			skip
  //		endif
  //	endfor
  // endfor
  // step count = 1 + (n/2) * ((2n - 1) * 7 + (2n - 1) * 7)
  // 		= 1 + (n/2) * (14n - 7 + 14n - 7)
  // 		= 1 + (n/2) * (28n - 14)
  // 		= 1 + 14n^2 - 7n
  // 		= 14n^2 - 7n + 1
  // time complexity = O(n^2)
	//unsigned because positions should not be negative for data structures
	unsigned position = 0;
	int swap_count = 0;
	//light_count() == n
	//make a copy of the input because it cannot be changed
	disk_state after(before.light_count());
	//repeat n/2 times, <= so that when n/2 has a remainder of 1, it runs one more time
	for (unsigned j = 0; j <= before.light_count()/2; j++) {
		//position is set above; go up to 1 before the last disk
		for (; position < before.total_count() - 1; position++) {
			if (after.get(position) == DISK_LIGHT and after.get(position+1) == DISK_DARK) {
				after.swap(position);
				swap_count++;
			} else {
				continue;
			}
		}
		//position has been updated after being used, so it points to the last disk
		//initialize it to one before the last disk so that it has a disk on the right to compare
		//compare upto position > 0 because when position == 1, --position will set it to 0
		//	and the loop will run
		for (position = position - 1; position > 0; --position) {
			if (after.get(position) == DISK_LIGHT and after.get(position+1) == DISK_DARK) {
				after.swap(position);
				swap_count++;
			} else {
				continue;
			}
		}
	 }
  return sorted_disks(after, swap_count);
}
  
