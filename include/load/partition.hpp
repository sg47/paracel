/**
 * Copyright (c) 2014, Douban Inc. 
 *   All rights reserved. 
 *
 * Distributed under the BSD License. Check out the LICENSE file for full text.
 *
 * Paracel - A distributed optimization framework with parameter server.
 *
 * Downloading
 *   git clone http://code.dapps.douban.com/paracel.git
 *
 * Authors: Hong Wu <xunzhangthu@gmail.com>
 *
 */
#ifndef FILE_275e6247_9a21_93f3_3d3c_51a539d1c8d6_HPP
#define FILE_275e6247_9a21_93f3_3d3c_51a539d1c8d6_HPP

#include <sys/stat.h>    

#include <fstream>
#include <iostream>
#include <functional>
#include <boost/coroutine/coroutine.hpp>

#include "paracel_types.hpp"
//#define BLK_SZ 8

namespace paracel {

void file_load_lines_impl(paracel::coroutine<paracel::str_type>::caller_type & yield, 
                          const paracel::str_type & fname, 
			  long st, 
			  long en) {
  
  std::ifstream f(fname, std::ios::binary);
  if(!f) { throw std::runtime_error("paracel error in file_load_lines_impl: loader reading failed."); }

  auto offset = st;
  if(offset) {
    f.seekg(offset - 1);
    paracel::str_type l;
    std::getline(f, l);
    // add edge offset. if no egde, add 1:'\n'
    offset += l.size();
  }
  while(offset < en) {
    paracel::str_type l;
    std::getline(f, l);
    offset += l.size() + 1;
    yield(l);
  }
  f.close();
}

paracel::deque_type< paracel::coroutine<paracel::str_type> >
file_partition(const paracel::str_type & fname, int np) {
  
  paracel::deque_type< paracel::coroutine<paracel::str_type> > func_loaders;
  
  long s, e;
  std::ifstream f(fname, std::ios::binary | std::ios::ate);
  long sz = f.tellg();
  int nbk = np;
  long bk_sz = sz / (long)nbk;
  for(int i = 0; i < nbk; ++i) {
    s = i * bk_sz;
    if(i == nbk - 1) { 
      e = sz; 
    } else {
      e = (i + 1) * bk_sz;
    }
    paracel::coroutine<paracel::str_type> file_load_lines(std::bind(paracel::file_load_lines_impl, std::placeholders::_1, fname, s, e));
    func_loaders.push_back(std::move(file_load_lines));
  }
  return func_loaders; 
}

void files_load_lines_impl(paracel::coroutine<paracel::str_type>::caller_type & yield, 
                           const paracel::list_type<paracel::str_type> & name_list,
			   const paracel::list_type<long> & displs,
			   long st, 
			   long en) {
  if(en < st) { throw std::runtime_error("paracel error in files_load_lines_impl: en < st"); }
  // to locate files index to load from
  int fst = 0;
  int fen = 0;
  int offset;
  for(size_t i = 0; i < name_list.size(); ++i) {
    if(st >= displs[i]) fst = i;
    if(en > displs[i + 1]) fen = i + 1;
  }
  bool flag = false;
  // load from files
  for(auto fi = fst; fi < fen + 1; ++fi) {
    if(flag) { 
      offset = 0;
    } else { 
      offset = st - displs[fi];
    }

    std::ifstream f(name_list[fi], std::ios::binary);
    if(!f) { 
      throw std::runtime_error("paracel error in files_load_lines_impl: loader reading failed."); 
    }
    
    if(offset) {
      f.seekg(offset - 1);
      paracel::str_type l;
      std::getline(f, l);
      offset += l.size();
    }

    if(fi == fen) {
      while(offset + displs[fi] < en) {
        paracel::str_type l;
	    std::getline(f, l);
	    offset += l.size() + 1;
	    yield(l);
      }
    } else {
      flag = true;
      while(1) {
        paracel::str_type l;
	    std::getline(f, l);
	    if(l.size() == 0) break;
	    yield(l);
      }
    }
	f.close();
  } // end of for
}

paracel::deque_type< paracel::coroutine<paracel::str_type> > 
files_partition(paracel::list_type<paracel::str_type> & name_list, 
				int np, const paracel::str_type & pattern = "",
				int blk_sz = BLK_SZ) {

  paracel::deque_type< paracel::coroutine<paracel::str_type> > func_loaders;
  if(pattern == "linesplit" || pattern == "fvec") blk_sz = 1; 
  np = np * blk_sz;
  paracel::list_type<long> displs(name_list.size() + 1, 0);
  for(size_t i = 0; i < displs.size() - 1; ++i) {
    std::ifstream f(name_list[i], std::ios::binary | std::ios::ate);
    long tmp = f.tellg();
    displs[i + 1] = displs[i] + tmp;
  }
  long sz = displs[displs.size() - 1];
  int nbk = np;
  long bk_sz = sz / (long)nbk;
  long e;
  for(int i = 0; i < nbk; ++i) {
    long s = (long)i * bk_sz;
    if(i == nbk -1) {
      e = sz;
    } else {
      e = (i + 1) * bk_sz;
    }
    paracel::coroutine<paracel::str_type> files_load_lines(std::bind(paracel::files_load_lines_impl, std::placeholders::_1, name_list, displs, s, e));
    func_loaders.push_back(std::move(files_load_lines));
  }
  return func_loaders;
}

} // namespace paracel
#endif
