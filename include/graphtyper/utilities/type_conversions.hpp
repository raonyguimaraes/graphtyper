#pragma once

// functions to split a string by a specific delimiter
#include <stdio.h>
#include <iostream>
#include <stdint.h> // For uint64_t

#include <seqan/basic.h>
#include <seqan/sequence.h>

#include <graphtyper/constants.hpp>


namespace gyper
{

static const uint64_t A_VALUE = 0x0000000000000000ULL; /** \brief 'A' is represented by '00'. */
static const uint64_t C_VALUE = 0x0000000000000001ULL; /** \brief 'C' is represented by '01'. */
static const uint64_t G_VALUE = 0x0000000000000002ULL; /** \brief 'G' is represented by '10'. */
static const uint64_t T_VALUE = 0x0000000000000003ULL; /** \brief 'T' is represented by '11'. */

static const uint64_t A_LAST_VALUE = 0x0000000000000000ULL; /** \brief 0 is hex for 0000. */
static const uint64_t C_LAST_VALUE = 0x4000000000000000ULL; /** \brief 4 is hex for 0100. */
static const uint64_t G_LAST_VALUE = 0x8000000000000000ULL; /** \brief 8 is hex for 1000. */
static const uint64_t T_LAST_VALUE = 0xC000000000000000ULL; /** \brief C is hex for 1100. */

/**
 * @brief Converts a string/list of DNA bases to a unsigned 64 bit integer.
 *
 * @param s String of DNA bases.
 * @return The new unsigned 64 bit integer.
 */
uint64_t to_uint64(char const c);
uint64_t to_uint64(std::vector<char> const & s);
uint64_t to_uint64(seqan::DnaString const & s, std::size_t i);

template <typename TSeq>
std::vector<uint64_t> to_uint64_vec(TSeq const & s, std::size_t i);
std::array<uint64_t, 96> to_uint64_vec_hamming_distance_1(uint64_t const key);
// std::array<uint64_t, 2160> to_uint64_vec_hamming_distance_2(uint64_t const key);

seqan::String<seqan::Dna> to_dna(uint64_t const & d, uint8_t k = K);
std::array<uint64_t, 3> get_mismatches_of_last_base(uint64_t const d);
std::array<uint64_t, 3> get_mismatches_of_first_base(uint64_t const d);

/**
 * @brief Inserts all elements from one map to another and optionally deletes the elements from the old map.
 */
template <typename Map1, typename Map2>
void map_swap(Map1 & map1, Map2 & map2);
template <typename Map1, typename Map2>
void map_swap(Map1 & map1, Map2 & map2, bool const & delete_as_i_go);
template <class TListItems>
std::vector<TListItems> to_vector(std::forward_list<TListItems> const & q);

/**
 * @brief Takes a rvalue reference of a std::string and returns the contents of the std::string as a vector of 'char'.
 */
std::vector<char> to_vec(std::string && str);

} // namespace gyper
