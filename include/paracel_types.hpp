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
#ifndef FILE_149f02d6_0fec_56ac_97f4_a30cea847471_HPP
#define FILE_149f02d6_0fec_56ac_97f4_a30cea847471_HPP

#include <vector>
#include <map>
#include <unordered_map>
#include <type_traits>
#include <string>
#include <functional>
#include <stdexcept>
#include <deque>
#include <tuple>
#include <set>

#include <mpi.h>
#include <msgpack.hpp>
#include <msgpack/type/tr1/unordered_map.hpp>
#include <boost/coroutine/coroutine.hpp>

#include "utils/hash.hpp"

namespace paracel {

const int BLK_SZ = 16;

const int threads_num = 5;

const size_t split_sz = 500;

const std::string seperator = "PARACEL";

const std::string seperator_inner = "paracel";

const std::string default_so_file = "../src/default.so";

const std::string default_update_func_name = "paracel_default_incr";

const std::string default_port = "7773";

const int any_source = MPI_ANY_SOURCE;
const int any_tag = MPI_ANY_TAG;

template <class T>
struct is_atomic : std::false_type {};

template <class T>
struct is_seqic : std::false_type {};

template <class T>
struct is_comm_builtin : std::false_type {};

template <class T>
struct is_comm_container : std::false_type {};

#define PARACEL_REGISTER_ATOM(T)		\
  template <>					\
  struct is_atomic<T> : std::true_type {	\
  }						\

#define PARACEL_REGISTER_SEQ(T)			\
  template <>					\
  struct is_seqic<T> : std::true_type {	        \
  }						\


#define PARACEL_REGISTER_COMM_BUILTIN(T, Dtype)	\
  template <>					\
  struct is_comm_builtin<T> : std::true_type {	\
    static MPI_Datatype datatype() {		\
      return Dtype;				\
    }						\
  }						\

#define PARACEL_REGISTER_COMM_CONTAINER(T, Dtype)	\
  template <>						\
  struct is_comm_container<T> : std::true_type {	\
    static MPI_Datatype datatype() {			\
      return Dtype;					\
    }							\
  }							\

template <class T>
MPI_Datatype datatype() {
  return is_comm_builtin<T>::datatype();
}

template <class T>
MPI_Datatype container_inner_datatype() {
  return is_comm_container<T>::datatype();
}

// paracel only support a subsets of std::atomic
// used for incr op
PARACEL_REGISTER_ATOM(int);
PARACEL_REGISTER_ATOM(long);
PARACEL_REGISTER_ATOM(short);
PARACEL_REGISTER_ATOM(float);
PARACEL_REGISTER_ATOM(double);
PARACEL_REGISTER_ATOM(unsigned int);
PARACEL_REGISTER_ATOM(unsigned short);
PARACEL_REGISTER_ATOM(unsigned long);

// paracel only supported continuous container
// used for incr op
PARACEL_REGISTER_SEQ(std::vector<int>);
PARACEL_REGISTER_SEQ(std::vector<long>);
PARACEL_REGISTER_SEQ(std::vector<short>);
PARACEL_REGISTER_SEQ(std::vector<float>);
PARACEL_REGISTER_SEQ(std::vector<double>);
PARACEL_REGISTER_SEQ(std::vector<unsigned int>);
PARACEL_REGISTER_SEQ(std::vector<unsigned long>);
PARACEL_REGISTER_SEQ(std::vector<unsigned short>);

PARACEL_REGISTER_COMM_BUILTIN(int, MPI_INT);
PARACEL_REGISTER_COMM_BUILTIN(long, MPI_LONG);
PARACEL_REGISTER_COMM_BUILTIN(char, MPI_CHAR);
PARACEL_REGISTER_COMM_BUILTIN(float, MPI_FLOAT);
PARACEL_REGISTER_COMM_BUILTIN(double, MPI_DOUBLE);
PARACEL_REGISTER_COMM_BUILTIN(unsigned, MPI_UNSIGNED);
PARACEL_REGISTER_COMM_BUILTIN(unsigned long, MPI_UNSIGNED_LONG);

// tricky definition
PARACEL_REGISTER_COMM_CONTAINER(std::string, MPI_CHAR);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<int>, MPI_INT);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<long>, MPI_LONG);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<char>, MPI_CHAR);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<float>, MPI_FLOAT);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<double>, MPI_DOUBLE);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<unsigned>, MPI_UNSIGNED);
PARACEL_REGISTER_COMM_CONTAINER(std::vector<unsigned long>, MPI_UNSIGNED_LONG);

// for alltoall usage
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<int> >, MPI_INT);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<long> >, MPI_LONG);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<char> >, MPI_CHAR);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<float> >, MPI_FLOAT);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<double> >, MPI_DOUBLE);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<unsigned> >, MPI_UNSIGNED);
PARACEL_REGISTER_COMM_CONTAINER(std::vector< std::vector<unsigned long> >, MPI_UNSIGNED_LONG);

using str_type = std::string;

using hash_return_type = size_t;

template <class T>
using list_type = std::vector<T>;

template <class T>
using deque_type = std::deque<T>;

// hash function in paracel not support std::vector<bool> which is supported in std::hash
// but support std::vector<int>...
/*
template <class T>
using hash_type = std::hash<T>;
*/
template <class T>
using hash_type = douban::hash<T>;

template <class K, class V>
using dict_type = std::unordered_map<K, V>;

/*
template <class K, class V>
using dict_type = std::tr1::unordered_map<K, V>;
*/

/*
template <class F = std::string, class S = std::string>
using triple_type = std::tuple<F, S, double>;
*/
using triple_type = std::tuple<std::string, std::string, double>;

template <class T = std::string>
using set_type = std::set<T>;

template <bool Cond, class T = void>
using Enable_if = typename std::enable_if<Cond, T>::type;

template <bool Cond, class T = void>
using Disable_if = typename std::enable_if<!Cond, T>::type;

/*
template <bool Cond, class T = void>
using Enable_if_inner = typename std::enable_if<Cond::value_type, T>::type;

template <bool Cond, class T = void>
using Disable_if_inner = typename std::enable_if<!Cond::value_type, T>::type;
*/

template <class T>
using coroutine = boost::coroutines::coroutine<T()>;

using update_result = std::function<std::string(std::string, std::string)>;
using filter_result = std::function<bool(std::string, std::string)>;
using filter_with_key_result = std::function<bool(std::string)>;

template<class T>
using kernel_type = typename std::remove_cv<
			typename std::remove_reference<T>::type
			>::type;

} // namespace paracel

#endif
